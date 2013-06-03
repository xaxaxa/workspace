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
			if(tmp<length) WARN(2,"buffer underflow: wanted " << length << " samples, but got " << tmp);
		}
		virtual void Process(const NUMTYPE* in, NUMTYPE* out, int length)
		{
			PutData(in,length);
			int tmp=GetData(out,length);
			if(tmp<length) WARN(2,"buffer underflow: wanted " << length << " samples, but got " << tmp);
		}
		BufferedFilter()
		{
		}
		virtual ~BufferedFilter()
		{
		}
	};
	template<class NUMTYPE> class BufferedFilterHelper: public BufferedFilter<NUMTYPE>
	{
	public:
		NUMTYPE* InBuffer;
		int BufferSize, OutBuffers;
		int InBufferLength;
		CircularQueue<NUMTYPE> OutBuffer;

		//process exactly 1 buffer
		virtual void DoProcess(const NUMTYPE* in, NUMTYPE* out)=0;

		inline void __warn(int length)
		{
			WARN(1, "filter buffer overflow: BufferSize = " << BufferSize << "; OutBuffers = " << OutBuffers << "; inputlength = " << length);
			return;
		}
		virtual void PutData(const NUMTYPE* data, int length)
		{
			asdf: if (InBufferLength <= 0 && length >= BufferSize)
			{ //input buffer is empty and the input data length is larger than BufferSize;
			  //data can be processed directly from the input source without being copied
			  //to the input buffer.
				auto i = OutBuffer.BeginAppend();
				if (i < 0)
				{
					__warn(length);
					return;
				}
				DoProcess(data, &OutBuffer.GetPointer(i));
				OutBuffer.EndAppend(i);
				data += BufferSize;
				length -= BufferSize;
				goto asdf;
			}
			zxcv: if (InBufferLength >= BufferSize)
			{ //input buffer full
				auto i = OutBuffer.BeginAppend();
				if (i < 0)
				{
					__warn(length);
					return;
				}
				DoProcess(InBuffer, &OutBuffer.GetPointer(i));
				OutBuffer.EndAppend(i);
				InBufferLength = 0;
				goto asdf;
			}
			if (length <= 0)
				return;
			auto l = __MIN(length, BufferSize - InBufferLength);
			memcpy(InBuffer + InBufferLength, data, l * sizeof(NUMTYPE));
			data += l;
			length -= l;
			InBufferLength += l;
			goto zxcv;
		}
		Int outbuffer_index;
		int outbuffer_pos;
		virtual int GetData(NUMTYPE* out, int length)
		{
			int br = 0;
			while (length > 0)
			{
				if (outbuffer_index < 0)
				{
					outbuffer_index = OutBuffer.BeginDequeue();
					if (outbuffer_index < 0)
						return br;
				}
				auto l = __MIN(length, BufferSize - outbuffer_pos);
				memcpy(out, &OutBuffer.GetPointer(outbuffer_index) + outbuffer_pos,
						l * sizeof(NUMTYPE));

				outbuffer_pos += l;
				br += l;
				if (outbuffer_pos >= BufferSize)
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
		BufferedFilterHelper(int buffersize, int outbuffers) :
				InBuffer(NULL), BufferSize(buffersize), OutBuffers(outbuffers),
						OutBuffer(outbuffers, buffersize), outbuffer_index(-1), outbuffer_pos(0)
		{
			InBuffer = (NUMTYPE*) malloc(sizeof(NUMTYPE) * BufferSize);
		}
		virtual ~BufferedFilterHelper()
		{
			if (InBuffer != NULL
			)
				free(InBuffer);
		}
	};

	template<class NUMTYPE, class FLOATTYPE = double> class OverlappedFilter: public BufferedFilter<NUMTYPE>
	{
	public:
		CircularQueue<FLOATTYPE> InBuffer, OutBuffer;
		Int curBuffer_in, nextBuffer_in, curBuffer_out, nextBuffer_out;
		int BufferSize, OutBuffers, InBuffers;
		//out[n] = coeff_o*out[n] + coeff_n*f(out[n])

		inline int PeriodSize()
		{
			return BufferSize;
		}
		FLOATTYPE* tmpbuffer;
		virtual void alloc_buffer()
		{
			tmpbuffer = new FLOATTYPE[PeriodSize()];
		}
		virtual void free_buffer()
		{
			delete[] tmpbuffer;
		}

		//virtual void DoProcess(FLOATTYPE* in1, FLOATTYPE* in2, FLOATTYPE* out1, FLOATTYPE* out2, int offset, FLOATTYPE coeff_o, FLOATTYPE coeff_n) = 0;
		virtual void DoProcess() = 0;
		virtual void DoProcess(FLOATTYPE* in1, FLOATTYPE* in2, FLOATTYPE* out1, FLOATTYPE* out2,
				int offset, FLOATTYPE coeff_o, FLOATTYPE coeff_n)
		{
			auto l = BufferSize - offset;
			if (l > 0)
				memcpy(tmpbuffer, in1 + offset, l * sizeof(FLOATTYPE));
			if (offset > 0)
				memcpy(tmpbuffer + l, in2, offset * sizeof(FLOATTYPE));

			DoProcess();

			Int half = BufferSize / 2;
			if (l > 0)
			{
				//memcpy(out1 + offset, tmpdouble, l * sizeof(double));
				for (int i = 0; i < l; i++)
				{
					FLOATTYPE fade = (FLOATTYPE) abs((Int) i - half) / (FLOATTYPE) half;
					//if(fade>1.0)cout << "fade="<<fade<<endl;
					//when fade=1: at the side: coeff=0
					out1[offset + i] = (out1[offset + i] * (coeff_o/*+(fade*coeff_n)*/))
							+ (tmpbuffer[i] * coeff_n * (1.0 - fade));
				}
			}
			if (offset > 0)
			{
				//memcpy(out2, tmpdouble + l, offset * sizeof(double));
				for (int i = l; i < this->BufferSize; i++)
				{
					FLOATTYPE fade = (FLOATTYPE) abs((Int) i - half) / (FLOATTYPE) half;
					out2[i - l] = (out2[i - l] * (coeff_o/*+(fade*coeff_n)*/))
							+ (tmpbuffer[i] * coeff_n * (1.0 - fade));
				}
			}
		}

		Int outbuffer_index, inbuffer_index;
		int outbuffer_pos, inbuffer_pos;
		int overlapcount;
		void proc()
		{
			//while((curBuffer_in = InBuffer.BeginDequeue()) >= 0)
			//	InBuffer.EndDequeue(curBuffer_in);
			//return;
			asdf: if (curBuffer_in < 0 && (curBuffer_in = InBuffer.BeginDequeue()) < 0)
				return;
			if (nextBuffer_in < 0 && (nextBuffer_in = InBuffer.BeginDequeue()) < 0)
				return;
			//WARN(3,"proc() asdfghjkl");
			if (curBuffer_out < 0 && (curBuffer_out = OutBuffer.BeginAppend()) < 0)
				return;
			if (nextBuffer_out < 0 && (nextBuffer_out = OutBuffer.BeginAppend()) < 0)
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
			for (int i = 0; i < overlapcount; i++)
			{
				int offset = BufferSize / overlapcount * i;
				DoProcess(in1, in2, out1, out2, offset, 1.0, coeff);
			}
			//memcpy(out1, in1, sizeof(FLOATTYPE)*BufferSize);
			InBuffer.EndDequeue(curBuffer_in);
			OutBuffer.EndAppend(curBuffer_out);
			curBuffer_in = nextBuffer_in;
			nextBuffer_in = -1;
			curBuffer_out = nextBuffer_out;
			nextBuffer_out = -1;
			goto asdf;
		}
		inline void __warn(int length)
		{
			WARN(1, "filter buffer overflow: BufferSize = " << BufferSize << "; InBuffers = " << InBuffer.Length() << "/" << InBuffers << "; inputlength = " << length);
			return;
		}
		virtual void PutData(const NUMTYPE* data, int length)
		{
			while (length > 0)
			{
				if (inbuffer_index < 0)
				{
					inbuffer_index = InBuffer.BeginAppend();
					if (inbuffer_index < 0)
					{
						__warn(length);
						return;
					}
				}
				auto l = __MIN(length, BufferSize - inbuffer_pos);
				//memcpy(&InBuffer.GetPointer(inbuffer_index) + inbuffer_pos, data, l * sizeof(NUMTYPE));
				FLOATTYPE* dst = &InBuffer.GetPointer(inbuffer_index) + inbuffer_pos;
				for (int i = 0; i < l; i++)
					dst[i] = (FLOATTYPE) data[i];
				inbuffer_pos += l;
				if (inbuffer_pos >= BufferSize)
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
		virtual int GetData(NUMTYPE* out, int length)
		{
			int br = 0;
			while (length > 0)
			{
				if (outbuffer_index < 0)
				{
					outbuffer_index = OutBuffer.BeginDequeue();
					if (outbuffer_index < 0)
						return br;
				}
				auto l = __MIN(length, BufferSize - outbuffer_pos);
				//memcpy(out, &OutBuffer.GetPointer(outbuffer_index) + outbuffer_pos, l * sizeof(NUMTYPE));
				FLOATTYPE* src = &OutBuffer.GetPointer(outbuffer_index) + outbuffer_pos;
				for (int i = 0; i < l; i++)
					out[i] = (NUMTYPE) src[i];

				outbuffer_pos += l;
				br += l;
				if (outbuffer_pos >= BufferSize)
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
		OverlappedFilter(int buffersize, int inbuffers, int outbuffers, int overlapcount) :
				InBuffer(inbuffers, buffersize), OutBuffer(outbuffers, buffersize),
						curBuffer_in(-1), nextBuffer_in(-1), curBuffer_out(-1), nextBuffer_out(-1),
						BufferSize(buffersize), OutBuffers(outbuffers), InBuffers(inbuffers),
						outbuffer_index(-1), inbuffer_index(-1), outbuffer_pos(0), inbuffer_pos(0),
						overlapcount(overlapcount)
		{
			//InBuffer = (NUMTYPE*)malloc(sizeof(NUMTYPE)*BufferSize);
			alloc_buffer();
		}
		~OverlappedFilter()
		{
			free_buffer();
		}
	};
	template<class NUMTYPE, class FLOATTYPE = double> class OverlappedFilter2: public BufferedFilter<NUMTYPE>
	{
	public:
		CircularQueue<FLOATTYPE> InBuffer, OutBuffer;
		int *InBufferPtrs, *OutBufferPtrs;
		Int curBuffer_in, nextBuffer_in, curBuffer_out, nextBuffer_out;
		int BufferSize, OutBuffers, InBuffers;
		Int outbuffer_index, inbuffer_index;
		int outbuffer_pos, inbuffer_pos;
		int overlapcount;
		Int BuffersPerPeriod;
		Int padding1, padding2;
		function<void()> proc;
		//out[n] = coeff_o*out[n] + coeff_n*f(out[n])
		bool didprocess;
		inline int PeriodSize()
		{
			return BufferSize*(BuffersPerPeriod+padding1+padding2);
		}
		FLOATTYPE* tmpbuffer;
		virtual void alloc_buffer()
		{
			tmpbuffer = new FLOATTYPE[PeriodSize()];
		}
		virtual void free_buffer()
		{
			delete[] tmpbuffer;
		}

		//virtual void DoProcess(FLOATTYPE* in1, FLOATTYPE* in2, FLOATTYPE* out1, FLOATTYPE* out2, int offset, FLOATTYPE coeff_o, FLOATTYPE coeff_n) = 0;
		virtual void DoProcess() = 0;
		virtual void DoProcess(FLOATTYPE coeff_o, FLOATTYPE coeff_n)
		{
			Int BuffersPerPeriod=this->BuffersPerPeriod+padding1+padding2;
			Int offset=0;
			Int half = BufferSize*this->BuffersPerPeriod / 2;
			for(Int i1=curBuffer_in;i1!=nextBuffer_in;i1=modulus(i1+1,BuffersPerPeriod+1))
			{
				FLOATTYPE* in1=&InBuffer.GetPointer(InBufferPtrs[i1]);
				memcpy(tmpbuffer + offset, in1, BufferSize * sizeof(FLOATTYPE));
				/*for (Int i = 0; i < BufferSize; i++)
				{
					FLOATTYPE fade;
					if(overlapcount<=1) fade=0;
					else fade = (FLOATTYPE) abs(offset + i - half) / (FLOATTYPE) half;
					tmpbuffer[offset+i] = in1[i] * (1.0 - fade);
				}*/
				offset+=BufferSize;
			}
			//Int trololo,troll;
			//if(padding<=0)goto trolled;
			/*trololo=BufferSize*padding;
			for(Int i=0;i<trololo;i++)
			{
				tmpbuffer[i]*=((double)i)/(double)(trololo-1);
			}
			troll=PeriodSize();
			for(Int i=troll-trololo;i<troll;i++)
			{
				tmpbuffer[i]*=((double)(troll-i-1))/(double)(trololo-1);
			}*/
trolled:
			DoProcess();
			didprocess=true;

			offset=BufferSize*padding1;
			for(Int i1=curBuffer_out;i1!=nextBuffer_out;i1=modulus(i1+1,this->BuffersPerPeriod+1))
			{
				FLOATTYPE* out1=&OutBuffer.GetPointer(OutBufferPtrs[i1]);
				//memcpy(out1, tmpbuffer + offset, BufferSize * sizeof(FLOATTYPE));
				for (Int i = 0; i < (Int)BufferSize; i++)
				{
					FLOATTYPE fade;
					if(overlapcount<=1) fade=0;
					else {
						//Int offset_from_internal=offset-(Int)(BufferSize*padding1);
						fade = (FLOATTYPE) abs(offset - (Int)(BufferSize*padding1) + i - half) / (FLOATTYPE) half;
					}
					//fade=0;
					out1[i] = (out1[i] * (coeff_o)) + (tmpbuffer[offset+i] * coeff_n * (1.0 - fade));
					//out1[i] = (out1[i] * (coeff_o)) + (tmpbuffer[offset+i] * coeff_n);
				}
				offset+=BufferSize;
			}
		}

		void doProc()
		{
			//while((curBuffer_in = InBuffer.BeginDequeue()) >= 0)
			//	InBuffer.EndDequeue(curBuffer_in);
			//return;
			Int skip=this->BuffersPerPeriod/this->overlapcount;
			if(skip<1)skip=1;
			int overlapcount=this->BuffersPerPeriod/skip;
			Int BuffersPerPeriod=this->BuffersPerPeriod+padding1+padding2;
			//WARN(3,"asdf0");
		asdf:
			Int tmp;
			while(modulus(nextBuffer_in-curBuffer_in,BuffersPerPeriod+1) < BuffersPerPeriod)
			{
				if((tmp=InBuffer.BeginDequeue())<0) return;
				InBufferPtrs[nextBuffer_in]=tmp;
				nextBuffer_in=modulus(nextBuffer_in+1,BuffersPerPeriod+1);
				//WARN(3,"got buffers: " << modulus(nextBuffer_in-curBuffer_in,BuffersPerPeriod+1) << " total; nextBuffer_in=" << nextBuffer_in << "; curBuffer_in=" << curBuffer_in);
			}
			//WARN(3,"asdf1");
			while(modulus(nextBuffer_out-curBuffer_out,this->BuffersPerPeriod+1) < this->BuffersPerPeriod)
			{
				if((tmp=OutBuffer.BeginAppend())<0) return;
				OutBufferPtrs[nextBuffer_out]=tmp;
				nextBuffer_out=modulus(nextBuffer_out+1,this->BuffersPerPeriod+1);
				memset(&OutBuffer.GetPointer(tmp), 0, BufferSize * sizeof(FLOATTYPE));
			}
			//WARN(3,"asdf2");
			FLOATTYPE coeff = 1.0 / overlapcount;

			//int offset = BufferSize / overlapcount * i;
			DoProcess(1.0, coeff);
			//memcpy(out1, in1, sizeof(FLOATTYPE)*BufferSize);
			for(Int i=0;i<skip;i++)
			{
				InBuffer.EndDequeue(InBufferPtrs[curBuffer_in]);
				OutBuffer.EndAppend(OutBufferPtrs[curBuffer_out]);
				curBuffer_in=modulus(curBuffer_in+1,BuffersPerPeriod+1);
				curBuffer_out=modulus(curBuffer_out+1,this->BuffersPerPeriod+1);
			}
			goto asdf;
		}
		inline void __warn(int length)
		{
			WARN(1, "filter buffer overflow: BufferSize = " << BufferSize << "; InBuffers = " << InBuffer.Length() << "/" << InBuffers << "; length = " << length);
			return;
		}
		virtual void PutData(const NUMTYPE* data, int length)
		{
			while (length > 0)
			{
				if (inbuffer_index < 0)
				{
					inbuffer_index = InBuffer.BeginAppend();
					if (inbuffer_index < 0)
					{
						__warn(length);
						return;
					}
					//WARN(0,"__BEGINAPPEND");
				}
				auto l = __MIN(length, BufferSize - inbuffer_pos);
				//memcpy(&InBuffer.GetPointer(inbuffer_index) + inbuffer_pos, data, l * sizeof(NUMTYPE));
				FLOATTYPE* dst = &InBuffer.GetPointer(inbuffer_index) + inbuffer_pos;
				for (int i = 0; i < l; i++)
					dst[i] = (FLOATTYPE) data[i];
				inbuffer_pos += l;
				if (inbuffer_pos >= BufferSize)
				{
					InBuffer.EndAppend(inbuffer_index);
					//WARN(0,"__ENDAPPEND");
					//WARN(3, "inbuffer_pos >= BufferSize");
					inbuffer_index = -1;
					inbuffer_pos = 0;
					proc();
				}
				data += l;
				length -= l;
			}
		}
		virtual int GetData(NUMTYPE* out, int length)
		{
			int br = 0;
			while (length > 0)
			{
				if (outbuffer_index < 0)
				{
					outbuffer_index = OutBuffer.BeginDequeue();
					if (outbuffer_index < 0)
						return br;
				}
				auto l = __MIN(length, BufferSize - outbuffer_pos);
				//memcpy(out, &OutBuffer.GetPointer(outbuffer_index) + outbuffer_pos, l * sizeof(NUMTYPE));
				FLOATTYPE* src = &OutBuffer.GetPointer(outbuffer_index) + outbuffer_pos;
				for (int i = 0; i < l; i++)
					out[i] = (NUMTYPE) src[i];

				outbuffer_pos += l;
				br += l;
				if (outbuffer_pos >= BufferSize)
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
		OverlappedFilter2(int buffersize, int inbuffers, int outbuffers,
				int overlapcount, Int BuffersPerPeriod, Int padding1=0, Int padding2=0) :
		InBuffer(inbuffers, buffersize), OutBuffer(outbuffers, buffersize),
		curBuffer_in(0), nextBuffer_in(0), curBuffer_out(0), nextBuffer_out(0),
		BufferSize(buffersize), OutBuffers(outbuffers), InBuffers(inbuffers),
		outbuffer_index(-1), inbuffer_index(-1), outbuffer_pos(0), inbuffer_pos(0),
		overlapcount(overlapcount), BuffersPerPeriod(BuffersPerPeriod), padding1(padding1),
		padding2(padding2), didprocess(false)
		{
			//InBuffer = (NUMTYPE*)malloc(sizeof(NUMTYPE)*BufferSize);
			proc=[this](){this->doProc();};
			alloc_buffer();
			BuffersPerPeriod+=padding1+padding2;
			InBufferPtrs = new int[(BuffersPerPeriod+1)+(this->BuffersPerPeriod+1)];
			OutBufferPtrs = InBufferPtrs+(BuffersPerPeriod+1);
		}
		~OverlappedFilter2()
		{
			free_buffer();
			delete[] InBufferPtrs;
		}
	};
	template<class NUMTYPE, class FLOATTYPE = double> class OverlappedFilter3
	{
	public:
		CircularQueue<FLOATTYPE> InBuffer, OutBuffer;
		int *InBufferPtrs;
		Int curBuffer_in, nextBuffer_in;
		int BufferSize, OutBuffers, InBuffers;
		Int outbuffer_index, inbuffer_index;
		int outbuffer_pos, inbuffer_pos;
		int overlapcount;
		Int BuffersPerPeriod;
		//out[n] = coeff_o*out[n] + coeff_n*f(out[n])

		inline int PeriodSize()
		{
			return BufferSize*BuffersPerPeriod;
		}
		FLOATTYPE* tmpbuffer;
		virtual void alloc_buffer()
		{
			tmpbuffer = new FLOATTYPE[PeriodSize()];
		}
		virtual void free_buffer()
		{
			delete[] tmpbuffer;
		}

		//virtual void DoProcess(FLOATTYPE* in1, FLOATTYPE* in2, FLOATTYPE* out1, FLOATTYPE* out2, int offset, FLOATTYPE coeff_o, FLOATTYPE coeff_n) = 0;
		virtual void DoProcess() = 0;
		void DoProcess1()
		{
			Int offset=0;
			for(Int i1=curBuffer_in;i1!=nextBuffer_in;i1=modulus(i1+1,BuffersPerPeriod+1))
			{
				FLOATTYPE* in1=&InBuffer.GetPointer(InBufferPtrs[i1]);
				memcpy(tmpbuffer + offset, in1, BufferSize * sizeof(FLOATTYPE));
				offset+=BufferSize;
			}
			DoProcess();
		}


		void proc()
		{
			Int skip=BuffersPerPeriod/this->overlapcount;
			if(skip<1)skip=1;
			int overlapcount=BuffersPerPeriod/skip;
			//WARN(3,"asdf0");
		asdf:
			Int tmp;
			while(modulus(nextBuffer_in-curBuffer_in,BuffersPerPeriod+1) < BuffersPerPeriod)
			{
				if((tmp=InBuffer.BeginDequeue())<0) return;
				InBufferPtrs[nextBuffer_in]=tmp;
				nextBuffer_in=modulus(nextBuffer_in+1,BuffersPerPeriod+1);
				//WARN(3,"got buffers: " << modulus(nextBuffer_in-curBuffer_in,BuffersPerPeriod+1) << " total; nextBuffer_in=" << nextBuffer_in << "; curBuffer_in=" << curBuffer_in);
			}

			DoProcess1();
			for(Int i=0;i<skip;i++)
			{
				InBuffer.EndDequeue(InBufferPtrs[curBuffer_in]);
				curBuffer_in=modulus(curBuffer_in+1,BuffersPerPeriod+1);
			}
			goto asdf;
		}
		inline void __warn(int length)
		{
			WARN(1, "filter buffer overflow: BufferSize = " << BufferSize << "; InBuffers = " << InBuffer.Length() << "/" << InBuffers << "; length = " << length);
			return;
		}
		virtual void PutData(const NUMTYPE* data, int length)
		{
			while (length > 0)
			{
				if (inbuffer_index < 0)
				{
					inbuffer_index = InBuffer.BeginAppend();
					if (inbuffer_index < 0)
					{
						__warn(length);
						return;
					}
				}
				auto l = __MIN(length, BufferSize - inbuffer_pos);
				//memcpy(&InBuffer.GetPointer(inbuffer_index) + inbuffer_pos, data, l * sizeof(NUMTYPE));
				FLOATTYPE* dst = &InBuffer.GetPointer(inbuffer_index) + inbuffer_pos;
				for (int i = 0; i < l; i++)
					dst[i] = (FLOATTYPE) data[i];
				inbuffer_pos += l;
				if (inbuffer_pos >= BufferSize)
				{
					InBuffer.EndAppend(inbuffer_index);
					WARN(3, "inbuffer_pos >= BufferSize");
					inbuffer_index = -1;
					inbuffer_pos = 0;
					proc();
				}
				data += l;
				length -= l;
			}
		}
		virtual int OutputSize()=0;
		virtual bool GetData(NUMTYPE* out)
		{
			auto tmp = OutBuffer.BeginDequeue();
			if (tmp < 0)
				return false;
			memcpy(out,&OutBuffer.GetPointer(tmp),OutBuffer.objsize*sizeof(double));
			OutBuffer.EndDequeue(tmp);
			return true;
		}
		OverlappedFilter3(int buffersize_in, int buffersize_out, int inbuffers, int outbuffers,
				int overlapcount, int BuffersPerPeriod) :
		InBuffer(inbuffers, buffersize_in), OutBuffer(outbuffers, buffersize_out),
		curBuffer_in(0), nextBuffer_in(0),
		BufferSize(buffersize_in), OutBuffers(outbuffers), InBuffers(inbuffers),
		outbuffer_index(-1), inbuffer_index(-1), outbuffer_pos(0), inbuffer_pos(0),
		overlapcount(overlapcount), BuffersPerPeriod(BuffersPerPeriod)
		{
			//InBuffer = (NUMTYPE*)malloc(sizeof(NUMTYPE)*BufferSize);
			alloc_buffer();
			InBufferPtrs = new int[(BuffersPerPeriod+1)];
		}
		~OverlappedFilter3()
		{
			free_buffer();
			delete[] InBufferPtrs;
		}
	};
	template<class NUMTYPE>class FilterBuffer
	{
	public:
		NUMTYPE* data;
		int length;
		int size;
		FilterBuffer(NUMTYPE* buf, int size):data(buf),length(0),size(size)
		{

		}
		int Append(NUMTYPE* data, int length)
		{
			int length2=size-this->length;
			if(length2<length)length=length2;
			if(length<=0)return 0;
			memcpy(this->data+this->length,data,length*sizeof(NUMTYPE));
			this->length+=length;
			return length;
		}
		class AppendData
		{
		public:
			FilterBuffer<NUMTYPE>& buf;
			NUMTYPE* data;
			int length;
			AppendData(FilterBuffer<NUMTYPE>& buf, NUMTYPE* data, int length)
				:buf(buf), data(data), length(length)
			{
				int tmp=buf.Append(data, length);
				this->data+=tmp;
				this->length-=tmp;
			}
			~AppendData()
			{
				if(length<=0)return;
				buf.Append(data, length);
			}
			inline NUMTYPE* operator()()
			{
				if(buf.length>=buf.size)
				{
					buf.length=0;
					return buf.data;
				}
				return NULL;
			}
		};
	};
	template<class NUMTYPE>class FilterBuffer2
	{
	public:
		CircularQueue<NUMTYPE> OutBuffer;
		Int BufferSize,index,pos,index2,pos2;
		FilterBuffer2(int size,int buffers):OutBuffer(buffers,size),BufferSize(size),index(-1),pos(0),
			index2(-1),pos2(0)
		{
			//alt_buffer=new NUMTYPE[length];
		}
		~FilterBuffer2()
		{
			//delete[] alt_buffer;
		}
		virtual void PutData(const NUMTYPE* data, Int length)
		{
			while (length > 0)
			{
				if (index < 0)
				{
					index = OutBuffer.BeginAppend();
					if (index < 0)
					{
						//__warn(length);
						return;
					}
				}
				auto l = __MIN(length, BufferSize - pos);
				memcpy(&OutBuffer.GetPointer(index) + pos, data, l * sizeof(NUMTYPE));
				pos += l;
				if (pos >= BufferSize)
				{
					OutBuffer.EndAppend(index);
					//WARN(3, "inbuffer_pos >= BufferSize");
					index = -1;
					pos = 0;
				}
				data += l;
				length -= l;
			}
		}
		//Int outbuffer_index;
		//int outbuffer_pos;
		virtual bool GetData(NUMTYPE* out, Int length)
		{
			int br = 0;
			//cerr << "OutBuffer.Length()=" <<OutBuffer.Length()<<endl;
			Int data_avail=0;
			if(index2>=0)
				data_avail+=(BufferSize-pos2);
			data_avail+=OutBuffer.Length()*BufferSize;
			if(data_avail<length)return false;
			while (length > 0)
			{
				if (index2 < 0)
				{
					index2 = OutBuffer.BeginDequeue();
					if (index2 < 0)
						return true;
				}
				auto l = __MIN(length, BufferSize - pos2);
				memcpy(out, &OutBuffer.GetPointer(index2) + pos2,
						l * sizeof(NUMTYPE));

				pos2 += l;
				br += l;
				if (pos2 >= BufferSize)
				{
					OutBuffer.EndDequeue(index2);
					index2 = -1;
					pos2 = 0;
				}
				out += l;
				length -= l;
			}
			return true;
		}
	};
}

