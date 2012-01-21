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
	template<class NUMTYPE> class MultiPassFilter: public Filter<NUMTYPE>
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
		virtual void AllocInputBuffer()
		{
			InBuffer = (NUMTYPE*)malloc(sizeof(NUMTYPE)*BufferSize);
		}
		virtual void FreeInputBuffer()
		{
			if(InBuffer!=NULL)free(InBuffer);
		}
		MultiPassFilter(UInt buffersize, UInt outbuffers)
			:InBuffer(NULL), BufferSize(buffersize), OutBuffers(outbuffers),
			OutBuffer(outbuffers, buffersize), outbuffer_index(-1), outbuffer_pos(0)
		{
			AllocInputBuffer();
		}
		virtual ~MultiPassFilter(){FreeInputBuffer();}
	};
};


