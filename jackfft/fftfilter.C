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
	template<class NUMTYPE>class FFTFilter: public OverlappedFilter<NUMTYPE, double>
	{
	public:
		fftw_plan p1, p2;
		double* tmpdouble;
		fftw_complex* tmpcomplex;
		double* coefficients;
		/*virtual void AllocInputBuffer()
		{
			InBuffer = (double*)fftw_malloc(sizeof(double)*BufferSize);
			tmpbuffer = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * ((UInt)(BufferSize / 2) + 1));
		}
		virtual void FreeInputBuffer()
		{
			if(InBuffer!=NULL)fftw_free(InBuffer);
		}*/
		FFTFilter(UInt buffersize, UInt inbuffers, UInt outbuffers, UInt overlapcount): OverlappedFilter<NUMTYPE, double>(buffersize, inbuffers, outbuffers, overlapcount)
		{
			Int l=((UInt)(buffersize / 2) + 1);
			tmpdouble = (double*)fftw_malloc(sizeof(double)*buffersize);
			tmpcomplex = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * l);
			coefficients = new double[l];
			for(Int i=0;i<l;i++)
				coefficients[i] = 1.0;
			p1 = fftw_plan_dft_r2c_1d(buffersize, tmpdouble, tmpcomplex, 0); //FFTW_UNALIGNED
			p2 = fftw_plan_dft_c2r_1d(buffersize, tmpcomplex, tmpdouble, 0);
		}
		~FFTFilter()
		{
			fftw_destroy_plan(p1);
			fftw_destroy_plan(p2);
			fftw_free(tmpdouble);
			fftw_free(tmpcomplex);
			delete[] coefficients;
		}
		virtual void DoProcess(double* in1, double* in2, double* out1, double* out2, UInt offset, double coeff_o, double coeff_n)
		{
			//fftw_one(
			//cout << "offset=" << offset << endl;
			UInt complexsize = (UInt)(this->BufferSize / 2) + 1;
			//for(UInt i=0;i<this->BufferSize;i++)
			//	tmpdouble[i] = (double)in[i];
			auto l = this->BufferSize - offset;
			if(l > 0) memcpy(tmpdouble, in1 + offset, l * sizeof(double));
			//l = offset;
			if(offset > 0) memcpy(tmpdouble + l, in2, offset * sizeof(double));
			
			//fftw_execute_dft_r2c(p1, tmpdouble, tmpcomplex);
			//fftw_execute_dft_c2r(p2, tmpcomplex, tmpdouble);
			fftw_execute(p1);
			for(UInt i=0;i<complexsize;i++)
			{
				tmpcomplex[i][0] *= coefficients[i];
				tmpcomplex[i][1] *= coefficients[i];
			}
			fftw_execute(p2);
			//for(UInt i=0;i<this->BufferSize;i++)
			//	out[i] = (NUMTYPE)(tmpdouble[i] / this->BufferSize);
			Int half = this->BufferSize / 2;
			if(l > 0)
			{
				//memcpy(out1 + offset, tmpdouble, l * sizeof(double));
				for(UInt i=0;i<l;i++)
				{
					double fade = (double)abs((Int)i - half) / (double)half;
					//if(fade>1.0)cout << "fade="<<fade<<endl;
					//when fade=1: at the side: coeff=0
					out1[offset + i] = (out1[offset + i] * (coeff_o/*+(fade*coeff_n)*/)) + ((tmpdouble[i] / this->BufferSize) * coeff_n * (1.0 - fade));
				}
			}
			if(offset > 0)
			{
				//memcpy(out2, tmpdouble + l, offset * sizeof(double));
				for(UInt i=l;i<this->BufferSize;i++)
				{
					double fade = (double)abs((Int)i - half) / (double)half;
					out2[i - l] = (out2[i - l] * (coeff_o/*+(fade*coeff_n)*/)) + ((tmpdouble[i]  / this->BufferSize) * coeff_n * (1.0 - fade));
				}
			}
		}
	};
};


