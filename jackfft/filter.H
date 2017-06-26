/*
 * fftfilter.C
 *
 * Copyright 2012  <xaxaxa@xaxaxa-mac>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */
#include <stdexcept>

#define __MIN(a,b) (((a)<(b))?(a):(b))
#define JACKFFT_WARN(...) if(jackfftPrintWarnings) fprintf(stderr, __VA_ARGS__)

// please define this variable in the main file; controls whether to print JACKFFT_WARN warnings
extern bool jackfftPrintWarnings;
namespace xaxaxa
{
	template<class NUMTYPE> class Filter
	{
	public:
		virtual void Process(NUMTYPE* data, int length)=0;
		virtual void Process(const NUMTYPE* in, NUMTYPE* out, int length)
		{
			memcpy(out, in, length*sizeof(NUMTYPE));
			Process(out, length);
		}
		Filter()
		{
		}
		virtual ~Filter()
		{
		}
	};
	template<class NUMTYPE> class BufferedFilter: public Filter<NUMTYPE>
	{
	public:
		virtual void PutData(const NUMTYPE* data, int length)=0;
		virtual int GetData(NUMTYPE* out, int length)=0;
		virtual void Process(NUMTYPE* data, int length)
		{
			PutData(data,length);
			int tmp=GetData(data,length);
			if(tmp<length) JACKFFT_WARN("buffer underflow: wanted %d samples, but got %d\n", length, tmp);
		}
		virtual void Process(const NUMTYPE* in, NUMTYPE* out, int length)
		{
			PutData(in,length);
			int tmp=GetData(out,length);
			if(tmp<length) JACKFFT_WARN("buffer underflow: wanted %d samples, but got %d\n", length, tmp);
		}
		BufferedFilter()
		{
		}
		virtual ~BufferedFilter()
		{
		}
	};
	
	template<class T> class RingBuffer {
	public:
		T* buffer;
		int wpos,rpos,size;
		inline int length() {
			// % operator does not do the right thing on negative values,
			// hence the +size
			return (wpos-rpos+size) % size;
		}
		inline int spaceLeft() {
			return (rpos-wpos-1+size) % size;
		}
		// use a new buffer
		void reset(T* buf, int size) {
			buffer = buf;
			this->size = size;
			wpos = rpos = 0;
		}

		// to support appending a different type that can be casted to T,
		// but still use the more efficient memcpy() when possible,
		// two generic _copy() functions are used
		template<class T1, class T2>
		void _copy(T1* dst, const T2* src, int len) {
			for(int i=0;i<len;i++)
				dst[i] = src[i];
		}
		void _copy(T* dst, const T* src, int len) {
			memcpy(dst, src, len*sizeof(T));
		}

		// append data to buffer but do not commit; returns number of items appended
		template<class T2>
		int append(T2* data, int len, int offset=0) {
			if(len > (spaceLeft()-offset)) len = spaceLeft() - offset;
			int wpos2 = (wpos + offset) % size;

			// how much space between wpos and end of buffer
			int len1 = size - wpos2;
			if(len1 > len) len1 = len;
			_copy(buffer+wpos2, data, len1);

			// how much more items to copy
			int len2 = len - len1;
			if(len2 > 0) {
				_copy(buffer, data+len1, len2);
			}
			return len;
		}

		// commit len items, already appended, to buffer
		void commit(int len) {
			wpos = (wpos+len) % size;
		}
		// append & commit len items
		template<class T2>
		int write(const T2* data, int len) {
			int tmp = append(data, len);
			commit(tmp);
			return tmp;
		}
		// peek data from buffer but do not update rpos;
		// returns number of items read into buf
		template<class T2>
		int peek(T2* buf, int len) {
			if(len > length()) len = length();

			// how much space between rpos and end of buffer
			int len1 = size - rpos;
			if(len1 > len) len1 = len;
			_copy(buf, buffer+rpos, len1);

			// how much more items to copy
			int len2 = len - len1;
			if(len2 > 0) {
				_copy(buf+len1, buffer, len2);
			}
			return len;
		}
		// remove len items from the buffer
		void consume(int len) {
			rpos = (rpos + len) % size;
		}
		// read and remove len items into buf;
		// returns number of items read
		template<class T2>
		int read(T2* buf, int len) {
			int tmp = peek(buf, len);
			consume(tmp);
			return tmp;
		}
	};

	template<class NUMTYPE, class FLOATTYPE = double> class OverlappedFilter2_new: public BufferedFilter<NUMTYPE> {
	public:
		/*
		 example with periodLength=4,
		              paddingLeft=5,
		              paddingRight=3,
		              stepLength=2
		 overlap count is periodLength/stepLength = 2

		 ............LLLLLCCCCRRR.......
		 ..............LLLLLCCCCRRR.....
		 ................LLLLLCCCCRRR...
		 L: left padding
		 C: buffer contents (to be outputted after filter is applied)
		 R: right padding

		 note that left padding does not increase latency (time until new data reaches C)
		 but right padding does.
		 */
		int periodLength;	// number of samples of output to generate for each DoProcess() call
		int paddingLeft;	// number of samples of history to include in buffer passed to DoProcess()
		int paddingRight;	// number of samples of post-buffer data to include in buffer passed to DoProcess()
		int stepLength;		// number of samples to advance read/write position by

		// audio data is first queued into inBuffer, a ring buffer, and chunks of it
		// are copied into tmpbuffer for processing
		RingBuffer<FLOATTYPE> inBuffer;
		// processed data from tmpbuffer is mixed in to outBuffer, a ring buffer,
		// and later taken out when GetData() is called
		RingBuffer<FLOATTYPE> outBuffer;

		// the subclass should perform filtering in-place on tmpbuffer when DoProcess() is called
		FLOATTYPE* tmpbuffer;
		bool didprocess;
		virtual void DoProcess() = 0;
		virtual void alloc_buffer()
		{
			tmpbuffer = new FLOATTYPE[PeriodSize()];
		}
		virtual void free_buffer()
		{
			delete[] tmpbuffer;
		}
		// implementation
		OverlappedFilter2_new(int buffersize, int inbuffers, int outbuffers,
				int overlapcount, int BuffersPerPeriod, int padding1=0, int padding2=0) {
			periodLength = buffersize * BuffersPerPeriod;
			paddingLeft = buffersize * padding1;
			paddingRight = buffersize * padding2;
			stepLength = periodLength / overlapcount;

			// output buffer size must be a multiple of the step length
			if(((buffersize * outbuffers) % stepLength) != 0) {
				throw std::logic_error("OverlappedFilter2_new: output buffer size must be a multiple of the step length");
			}

			inBuffer.reset(new FLOATTYPE[buffersize*inbuffers], buffersize*inbuffers);
			outBuffer.reset(new FLOATTYPE[buffersize*outbuffers], buffersize*outbuffers);

			alloc_buffer();
			didprocess = false;
		}
		virtual void PutData(const NUMTYPE* data, int length) {
			// add data to inBuffer
			int bw=inBuffer.write(data, length);
			if(bw!=length) {
				JACKFFT_WARN("OverlappedFilter2_new: inBuffer overrun: attempt to write %d items, but buffer only has %d space left\n",
						length, bw);
			}
			processAvailableData();
		}
		virtual int GetData(NUMTYPE* out, int length) {
			return outBuffer.read(out, length);
		}

		int tmpbufferSize() {
			return paddingLeft+periodLength+paddingRight;
		}
		int PeriodSize() {
			return tmpbufferSize();
		}
		// check if there is enough data in inBuffer to call DoProcess()
		void processAvailableData() {
			int tmpSize = tmpbufferSize();
			while(inBuffer.length() >= tmpSize) {
				// copy data from inBuffer to tmpbuffer and process it
				inBuffer.peek(tmpbuffer, tmpSize);
				inBuffer.consume(stepLength);
				DoProcess();
				didprocess = true;
				// apply a triangular mask to tmpbuffer
				int s=stepLength+1;
				for(int i=0;i<stepLength;i++) {
					tmpbuffer[i] *= double(i+1)/s;
					tmpbuffer[tmpSize - stepLength + i] *= double(stepLength-i)/s;
				}

				// put results in outBuffer
				blend(tmpbuffer, tmpSize - stepLength);
				int off = tmpSize - stepLength;
				outBuffer.append(tmpbuffer + off, stepLength, off);
				outBuffer.commit(stepLength);
			}
		}
		// blend data into outBuffer
		int blend(FLOATTYPE* data, int len) {
			if(len > outBuffer.spaceLeft()) len = outBuffer.spaceLeft();

			// how much space between wpos and end of buffer
			int len1 = outBuffer.size - outBuffer.wpos;
			if(len1 > len) len1 = len;
			_blend(outBuffer.buffer+outBuffer.wpos, data, len1);

			// how much more items to copy
			int len2 = len - len1;
			if(len2 > 0) {
				_blend(outBuffer.buffer, data+len1, len2);
			}
			return len;
		}
		void _blend(FLOATTYPE* dst, FLOATTYPE* src, int len) {
			for(int i=0;i<len;i++)
				dst[i] += src[i];
		}
	};
	
}

