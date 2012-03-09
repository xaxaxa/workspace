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
		fftw_complex* tmpcomplex2;
		double* coefficients;
		virtual void alloc_buffer(){this->tmpbuffer=(double*)fftw_malloc(sizeof(double)*this->PeriodSize());}
		virtual void free_buffer(){fftw_free(this->tmpbuffer);}
		//struct timespec last_refreshed;
		FFTFilter(UInt buffersize, UInt inbuffers, UInt outbuffers, UInt overlapcount, UInt BuffersPerPeriod): OverlappedFilter2<NUMTYPE, double>(buffersize, inbuffers, outbuffers, overlapcount, BuffersPerPeriod)
		{
			asdf=0;
			//memset(&last_refreshed,0,sizeof(last_refreshed));
			Int l=((UInt)(this->PeriodSize() / 2) + 1);
			//tmpdouble = (double*)fftw_malloc(sizeof(double)*buffersize);
			tmpcomplex = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * l);
			tmpcomplex2 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * l);
			coefficients = new double[l];
			for(Int i=0;i<l;i++)
				coefficients[i] = 1.0;
			//cout << this->PeriodSize() << endl;
			p1 = fftw_plan_dft_r2c_1d(this->PeriodSize(), this->tmpbuffer, tmpcomplex, 0); //FFTW_UNALIGNED
			p2 = fftw_plan_dft_c2r_1d(this->PeriodSize(), tmpcomplex2, this->tmpbuffer, 0);
		}
		~FFTFilter()
		{
			fftw_destroy_plan(p1);
			fftw_destroy_plan(p2);
			//fftw_free(tmpdouble);
			fftw_free(tmpcomplex);
			fftw_free(tmpcomplex2);
			delete[] coefficients;
		}
		
		/*long long get_nsec(const timespec& t)
		{
			return t.tv_sec*1000000000+t.tv_nsec;
		}*/
		int asdf;
		
		virtual void DoProcess()
		{
			UInt complexsize = (UInt)(this->PeriodSize() / 2) + 1;
			//asdf++;
			//if(asdf>1)
			//{
			fftw_execute(p1);
				//int shift=21;
				/*for(UInt i=0;i<complexsize;i++)
				{
					tmpcomplex[i][0] = tmpcomplex[i][0]*coefficients[i];
					tmpcomplex[i][1] = tmpcomplex[i][1]*coefficients[i];
				}*/
				//asdf=0;
			//}
			
			/*for(Int i=complexsize-1-shift;i>=0;i--)
			{
				tmpcomplex[i+shift][0]=tmpcomplex[i][0]*coefficients[i+shift];
				tmpcomplex[i+shift][1]=tmpcomplex[i][1]*coefficients[i+shift];
			}*/
			//tmpcomplex[0][0]=0;
			//tmpcomplex[0][1]=0;
			/*for(Int i=complexsize-1;i>=0;i--)
			{
				int i2=(int)round((double)i*19/20);
				tmpcomplex[i][0] = tmpcomplex[i2][0]*coefficients[i];
				tmpcomplex[i][1] = tmpcomplex[i2][1]*coefficients[i];
			}*/
			
			/*for(UInt i=(complexsize-1)/2+1;i<complexsize;i++)
			{
				tmpcomplex[i][0]=0;
				tmpcomplex[i][1]=0;
			}*/
			memcpy(tmpcomplex2,tmpcomplex,complexsize*sizeof(fftw_complex));
			for(UInt i=0;i<complexsize;i++)
			{
				tmpcomplex2[i][0] = tmpcomplex2[i][0]*coefficients[i];
				tmpcomplex2[i][1] = tmpcomplex2[i][1]*coefficients[i];
			}
			fftw_execute(p2);
			Int ps=this->PeriodSize();
			for(UInt i=0;i<ps;i++)
				this->tmpbuffer[i] /= ps;
		}
	};
	template<class NUMTYPE>class FFTTransform: public OverlappedFilter3<NUMTYPE, double>
	{
	public:
		fftw_plan p1, p2;
		//double* tmpdouble;
		fftw_complex* tmpcomplex;
		//double* coefficients;
		virtual void alloc_buffer(){this->tmpbuffer=(double*)fftw_malloc(sizeof(double)*this->PeriodSize());}
		virtual void free_buffer(){fftw_free(this->tmpbuffer);}
		FFTTransform(UInt buffersize, UInt inbuffers, UInt outbuffers, UInt overlapcount, UInt BuffersPerPeriod): OverlappedFilter3<NUMTYPE, double>(buffersize, (buffersize*BuffersPerPeriod/2+1)*2, inbuffers, outbuffers, overlapcount, BuffersPerPeriod)
		{
			Int l=((UInt)(this->PeriodSize() / 2) + 1);
			//tmpdouble = (double*)fftw_malloc(sizeof(double)*buffersize);
			tmpcomplex = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * l);
			//coefficients = new double[l];
			//cout << this->PeriodSize() << endl;
			p1 = fftw_plan_dft_r2c_1d(this->PeriodSize(), this->tmpbuffer, tmpcomplex, 0); //FFTW_UNALIGNED
			//p2 = fftw_plan_dft_c2r_1d(this->PeriodSize(), tmpcomplex, this->tmpbuffer, 0);
		}
		~FFTTransform()
		{
			fftw_destroy_plan(p1);
			//fftw_destroy_plan(p2);
			//fftw_free(tmpdouble);
			fftw_free(tmpcomplex);
			//delete[] coefficients;
		}
		virtual UInt OutputSize()
		{
			return (UInt)(((this->PeriodSize() / 2) + 1) * 2);
		}
		virtual void DoProcess()
		{
			UInt complexsize = (UInt)(this->PeriodSize() / 2) + 1;
			//apply window
			auto ps=this->PeriodSize();
			int w=ps/this->overlapcount;
			int start=ps/2-w;//ps/2-(w/2)-(w/2);
			int half=w/2;
			int w2=w*2;
			for(int i=0;i<ps;i++)
			{
				double a=0;
				if(i>=start && i<=(start+w))
					a=1.0-(abs(i - half) / (double)w);
				this->tmpbuffer[i]*=a;
			}
			
			fftw_execute(p1);
			int tmp=this->OutBuffer.BeginAppend();
			if(tmp<0)return;
			double* d=&this->OutBuffer.GetPointer(tmp);
			memcpy(d,tmpcomplex,complexsize*sizeof(double)*2);
			this->OutBuffer.EndAppend(tmp);
		}
	};
};


