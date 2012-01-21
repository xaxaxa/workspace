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
#define _USE_MATH_DEFINES
#include <math.h>
#include <fftw3.h>
#include "filter.H"

namespace xaxaxa
{
	template<class NUMTYPE>class FFTFilter: public MultiPassFilter<NUMTYPE>
	{
	public:
		fftw_plan p1, p2;
		double* tmpdouble;
		fftw_complex* tmpcomplex;
		/*virtual void AllocInputBuffer()
		{
			InBuffer = (double*)fftw_malloc(sizeof(double)*BufferSize);
			tmpbuffer = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * ((UInt)(BufferSize / 2) + 1));
		}
		virtual void FreeInputBuffer()
		{
			if(InBuffer!=NULL)fftw_free(InBuffer);
		}*/
		FFTFilter(UInt buffersize, UInt outbuffers): MultiPassFilter<NUMTYPE>(buffersize, outbuffers)
		{
			tmpdouble = (double*)fftw_malloc(sizeof(double)*buffersize);
			tmpcomplex = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * ((UInt)(buffersize / 2) + 1));
			p1 = fftw_plan_dft_r2c_1d(buffersize, tmpdouble, tmpcomplex, 0); //FFTW_UNALIGNED
			p2 = fftw_plan_dft_c2r_1d(buffersize, tmpcomplex, tmpdouble, 0);
		}
		virtual void DoProcess(const NUMTYPE* in, NUMTYPE* out)
		{
			//fftw_one(
			UInt complexsize = (UInt)(this->BufferSize / 2) + 1;
			for(UInt i=0;i<this->BufferSize;i++)
				tmpdouble[i] = (double)in[i];
			//fftw_execute_dft_r2c(p1, tmpdouble, tmpcomplex);
			//fftw_execute_dft_c2r(p2, tmpcomplex, tmpdouble);
			fftw_execute(p1);
			for(UInt i=0;i<complexsize;i++)
				if(i<128)
				{
					tmpcomplex[i][0] *= 20.0;
					tmpcomplex[i][1] *= 20.0;
				}
				//else tmpcomplex[i][0] *= 0.5;
				//tmpcomplex[i][1] /= (((float)i/1024)+1);
			fftw_execute(p2);
			for(UInt i=0;i<this->BufferSize;i++)
				out[i] = (NUMTYPE)(tmpdouble[i] / this->BufferSize);
		}
	};
};


