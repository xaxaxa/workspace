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
	template<class NUMTYPE>class FFTFilter: public OverlappedFilter2<NUMTYPE, double>
	{
	public:
		fftw_plan p1, p2;
		//double* tmpdouble;
		fftw_complex* tmpcomplex;
		double* coefficients;
		virtual void alloc_buffer(){this->tmpbuffer=(double*)fftw_malloc(sizeof(double)*this->PeriodSize());}
		virtual void free_buffer(){fftw_free(this->tmpbuffer);}
		FFTFilter(UInt buffersize, UInt inbuffers, UInt outbuffers, UInt overlapcount, UInt BuffersPerPeriod): OverlappedFilter2<NUMTYPE, double>(buffersize, inbuffers, outbuffers, overlapcount, BuffersPerPeriod)
		{
			Int l=((UInt)(this->PeriodSize() / 2) + 1);
			//tmpdouble = (double*)fftw_malloc(sizeof(double)*buffersize);
			tmpcomplex = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * l);
			coefficients = new double[l];
			for(Int i=0;i<l;i++)
				coefficients[i] = 1.0;
			cout << this->PeriodSize() << endl;
			p1 = fftw_plan_dft_r2c_1d(this->PeriodSize(), this->tmpbuffer, tmpcomplex, 0); //FFTW_UNALIGNED
			p2 = fftw_plan_dft_c2r_1d(this->PeriodSize(), tmpcomplex, this->tmpbuffer, 0);
		}
		~FFTFilter()
		{
			fftw_destroy_plan(p1);
			fftw_destroy_plan(p2);
			//fftw_free(tmpdouble);
			fftw_free(tmpcomplex);
			delete[] coefficients;
		}
		virtual void DoProcess()
		{
			UInt complexsize = (UInt)(this->PeriodSize() / 2) + 1;
			fftw_execute(p1);
			for(UInt i=0;i<complexsize;i++)
			{
				tmpcomplex[i][0] *= coefficients[i];
				tmpcomplex[i][1] *= coefficients[i];
			}
			fftw_execute(p2);
			Int ps=this->PeriodSize();
			for(UInt i=0;i<ps;i++)
				this->tmpbuffer[i] /= ps;
		}
	};
};


