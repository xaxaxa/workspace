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
				offset+=BufferSize;
			}
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
	
}

