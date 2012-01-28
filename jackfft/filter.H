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

#include <cplib/cplib.hpp>

#define __MIN(a,b) (((a)<(b))?(a):(b))
namespace xaxaxa
{
	template<class NUMTYPE> class Filter
	{
	public:
		virtual void PutData(const NUMTYPE* data, UInt length)=0;
		virtual UInt GetData(NUMTYPE* out, UInt length)=0;
		Filter(){}
		virtual ~Filter(){}
	};
	template<class NUMTYPE> class BufferedFilter: public Filter<NUMTYPE>
	{
	public:
		NUMTYPE* InBuffer;
		UInt BufferSize, OutBuffers;
		UInt InBufferLength;
		CircularQueue<NUMTYPE> OutBuffer;
		
		//process exactly 1 buffer
		virtual void DoProcess(const NUMTYPE* in, NUMTYPE* out)=0;
		
		inline void __warn(UInt length)
		{
			WARN(1, "filter buffer overflow: BufferSize = " << BufferSize << "; OutBuffers = "
					<< OutBuffers << "; length = " << length); return;
		}
		virtual void PutData(const NUMTYPE* data, UInt length)
		{
		asdf:
			if(InBufferLength <= 0 && length >= BufferSize)
			{	//input buffer is empty and the input data length is larger than BufferSize;
				//data can be processed directly from the input source without being copied
				//to the input buffer.
				auto i = OutBuffer.BeginAppend();
				if(i < 0)
				{
					__warn(length); return;
				}
				DoProcess(data, &OutBuffer.GetPointer(i));
				OutBuffer.EndAppend(i);
				data += BufferSize;
				length -= BufferSize;
				goto asdf;
			}
		zxcv:
			if(InBufferLength >= BufferSize)
			{	//input buffer full
				auto i = OutBuffer.BeginAppend();
				if(i < 0)
				{
					__warn(length); return;
				}
				DoProcess(InBuffer, &OutBuffer.GetPointer(i));
				OutBuffer.EndAppend(i);
				InBufferLength = 0;
				goto asdf;
			}
			if(length <= 0)return;
			auto l = __MIN(length, BufferSize - InBufferLength);
			memcpy(InBuffer + InBufferLength, data, l * sizeof(NUMTYPE));
			data += l;
			length -= l;
			InBufferLength += l;
			goto zxcv;
		}
		Int outbuffer_index;
		UInt outbuffer_pos;
		virtual UInt GetData(NUMTYPE* out, UInt length)
		{
			UInt br = 0;
			while(length > 0)
			{
				if(outbuffer_index < 0)
				{
					outbuffer_index = OutBuffer.BeginDequeue();
					if(outbuffer_index < 0) return br;
				}
				auto l = __MIN(length, BufferSize - outbuffer_pos);
				memcpy(out, &OutBuffer.GetPointer(outbuffer_index) + outbuffer_pos, l * sizeof(NUMTYPE));
				
				outbuffer_pos += l;
				br += l;
				if(outbuffer_pos >= BufferSize)
				{
					OutBuffer.EndDequeue(outbuffer_index);
					outbuffer_index = -1;
					outbuffer_pos = 0;
				}
				out += l;
				length -= l;
			}
			return br;
		}
		BufferedFilter(UInt buffersize, UInt outbuffers)
			:InBuffer(NULL), BufferSize(buffersize), OutBuffers(outbuffers),
			OutBuffer(outbuffers, buffersize), outbuffer_index(-1), outbuffer_pos(0)
		{
			InBuffer = (NUMTYPE*)malloc(sizeof(NUMTYPE)*BufferSize);
		}
		virtual ~BufferedFilter(){if(InBuffer!=NULL)free(InBuffer);}
	};

	template<class NUMTYPE, class FLOATTYPE=double> class OverlappedFilter: public Filter<NUMTYPE>
	{
	public:
		CircularQueue<FLOATTYPE> InBuffer, OutBuffer;
		Int curBuffer_in, nextBuffer_in, curBuffer_out, nextBuffer_out;
		UInt BufferSize, OutBuffers, InBuffers;
		//out[n] = coeff_o*out[n] + coeff_n*f(out[n])
		
		FLOATTYPE* tmpbuffer;
		virtual void alloc_buffer(){tmpbuffer=new FLOATTYPE[BufferSize];};
		virtual void free_buffer(){delete[] tmpbuffer;};
		
		//virtual void DoProcess(FLOATTYPE* in1, FLOATTYPE* in2, FLOATTYPE* out1, FLOATTYPE* out2, UInt offset, FLOATTYPE coeff_o, FLOATTYPE coeff_n) = 0;
		virtual void DoProcess() = 0;
		virtual void DoProcess(FLOATTYPE* in1, FLOATTYPE* in2, FLOATTYPE* out1, FLOATTYPE* out2, UInt offset, FLOATTYPE coeff_o, FLOATTYPE coeff_n)
		{
			auto l = BufferSize - offset;
			if(l > 0) memcpy(tmpbuffer, in1 + offset, l * sizeof(FLOATTYPE));
			if(offset > 0) memcpy(tmpbuffer + l, in2, offset * sizeof(FLOATTYPE));
			
			DoProcess();
			
			Int half = BufferSize / 2;
			if(l > 0)
			{
				//memcpy(out1 + offset, tmpdouble, l * sizeof(double));
				for(UInt i=0;i<l;i++)
				{
					FLOATTYPE fade = (FLOATTYPE)abs((Int)i - half) / (FLOATTYPE)half;
					//if(fade>1.0)cout << "fade="<<fade<<endl;
					//when fade=1: at the side: coeff=0
					out1[offset + i] = (out1[offset + i] * (coeff_o/*+(fade*coeff_n)*/)) + ((tmpbuffer[i] / BufferSize) * coeff_n * (1.0 - fade));
				}
			}
			if(offset > 0)
			{
				//memcpy(out2, tmpdouble + l, offset * sizeof(double));
				for(UInt i=l;i<this->BufferSize;i++)
				{
					FLOATTYPE fade = (FLOATTYPE)abs((Int)i - half) / (FLOATTYPE)half;
					out2[i - l] = (out2[i - l] * (coeff_o/*+(fade*coeff_n)*/)) + ((tmpbuffer[i]  / BufferSize) * coeff_n * (1.0 - fade));
				}
			}
		}
		
		Int outbuffer_index, inbuffer_index;
		UInt outbuffer_pos, inbuffer_pos;
		UInt overlapcount;
		void proc()
		{
			//while((curBuffer_in = InBuffer.BeginDequeue()) >= 0)
			//	InBuffer.EndDequeue(curBuffer_in);
			//return;
		asdf:
			if(curBuffer_in < 0 && (curBuffer_in = InBuffer.BeginDequeue()) < 0)
				return;
			if(nextBuffer_in < 0 && (nextBuffer_in = InBuffer.BeginDequeue()) < 0)
				return;
			//WARN(3,"proc() asdfghjkl");
			if(curBuffer_out < 0 && (curBuffer_out = OutBuffer.BeginAppend()) < 0)
				return;
			if(nextBuffer_out < 0 && (nextBuffer_out = OutBuffer.BeginAppend()) < 0)
				return;
			//WARN(3,"proc(): cur_i = " << curBuffer_in << "; next_i = " << curBuffer_out);
			/*memcpy(&OutBuffer.GetPointer(curBuffer_out), &InBuffer.GetPointer(curBuffer_in), sizeof(FLOATTYPE)*BufferSize);
			InBuffer.EndDequeue(curBuffer_in); curBuffer_in = -1;
			OutBuffer.EndAppend(curBuffer_out); curBuffer_out = -1;
			goto asdf;*/
			FLOATTYPE *in1, *in2, *out1, *out2;
			in1 = &InBuffer.GetPointer(curBuffer_in);
			in2 = &InBuffer.GetPointer(nextBuffer_in);
			out1 = &OutBuffer.GetPointer(curBuffer_out);
			out2 = &OutBuffer.GetPointer(nextBuffer_out);
			memset(out2, 0, BufferSize * sizeof(FLOATTYPE));
			FLOATTYPE coeff = 1.0 / overlapcount;
			for(UInt i = 0; i < overlapcount; i++)
			{
				UInt offset = BufferSize / overlapcount * i;
				DoProcess(in1, in2, out1, out2, offset, 1.0, coeff);
			}
			//memcpy(out1, in1, sizeof(FLOATTYPE)*BufferSize);
			InBuffer.EndDequeue(curBuffer_in);
			OutBuffer.EndAppend(curBuffer_out);
			curBuffer_in = nextBuffer_in; nextBuffer_in = -1;
			curBuffer_out = nextBuffer_out; nextBuffer_out = -1;
			goto asdf;
		}
		inline void __warn(UInt length)
		{
			WARN(1, "filter buffer overflow: BufferSize = " << BufferSize << "; InBuffers = "
					<< InBuffers << "; length = " << length); return;
		}
		virtual void PutData(const NUMTYPE* data, UInt length)
		{
			while(length > 0)
			{
				if(inbuffer_index < 0)
				{
					inbuffer_index = InBuffer.BeginAppend();
					if(inbuffer_index < 0)
					{
						__warn(length);
						return;
					}
				}
				auto l = __MIN(length, BufferSize - inbuffer_pos);
				//memcpy(&InBuffer.GetPointer(inbuffer_index) + inbuffer_pos, data, l * sizeof(NUMTYPE));
				FLOATTYPE* dst = &InBuffer.GetPointer(inbuffer_index) + inbuffer_pos;
				for(UInt i = 0; i < l; i++)
					dst[i] = (FLOATTYPE)data[i];
				inbuffer_pos += l;
				if(inbuffer_pos >= BufferSize)
				{
					InBuffer.EndAppend(inbuffer_index);
					//WARN(3, "inbuffer_pos >= BufferSize");
					inbuffer_index = -1;
					inbuffer_pos = 0;
					proc();
				}
				data += l;
				length -= l;
			}
		}
		virtual UInt GetData(NUMTYPE* out, UInt length)
		{
			UInt br = 0;
			while(length > 0)
			{
				if(outbuffer_index < 0)
				{
					outbuffer_index = OutBuffer.BeginDequeue();
					if(outbuffer_index < 0) return br;
				}
				auto l = __MIN(length, BufferSize - outbuffer_pos);
				//memcpy(out, &OutBuffer.GetPointer(outbuffer_index) + outbuffer_pos, l * sizeof(NUMTYPE));
				FLOATTYPE* src = &OutBuffer.GetPointer(outbuffer_index) + outbuffer_pos;
				for(UInt i = 0; i < l; i++)
					out[i] = (NUMTYPE)src[i];
				
				outbuffer_pos += l;
				br += l;
				if(outbuffer_pos >= BufferSize)
				{
					OutBuffer.EndDequeue(outbuffer_index);
					outbuffer_index = -1;
					outbuffer_pos = 0;
				}
				out += l;
				length -= l;
			}
			return br;
		}
		OverlappedFilter(UInt buffersize, UInt inbuffers, UInt outbuffers, UInt overlapcount)
			:InBuffer(inbuffers, buffersize), OutBuffer(outbuffers, buffersize),
			curBuffer_in(-1), nextBuffer_in(-1), curBuffer_out(-1), nextBuffer_out(-1),
			BufferSize(buffersize), OutBuffers(outbuffers), InBuffers(inbuffers),
			outbuffer_index(-1), inbuffer_index(-1),
			outbuffer_pos(0), inbuffer_pos(0), overlapcount(overlapcount)
		{
			//InBuffer = (NUMTYPE*)malloc(sizeof(NUMTYPE)*BufferSize);
			alloc_buffer();
		}
		~OverlappedFilter()
		{
			free_buffer();
		}
	};
};


