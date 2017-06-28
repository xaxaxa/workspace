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
#include <string.h>

#ifdef JACKFFT_USE_FFTS
	#include <ffts/ffts.h>
#elif defined JACKFFT_USE_SIMPLEFFT
	#include "../test/simplefft.C"
#else
	#include <fftw3.h>
	#define JACKFFT_USING_FFTW
#endif
#include "filter.H"

#ifdef JACKFFT_USE_FLOAT
	typedef float jackfft_float;
	#ifndef JACKFFT_USING_FFTW
		typedef float jackfft_complex[2];
	#else
		typedef fftwf_complex jackfft_complex;
		#define FFTWFUNC(x) fftwf_ ## x
	#endif
#else
	typedef double jackfft_float;
	#ifndef JACKFFT_USING_FFTW
		typedef double jackfft_complex[2];
	#else
		typedef fftw_complex jackfft_complex;
		#define FFTWFUNC(x) fftw_ ## x
	#endif
#endif

namespace xaxaxa
{
	inline float modulus(float a, float b)
	{
		int tmp=a/b;
		return a-tmp*b;
	}
	inline double modulus(double a, double b)
	{
		int64_t tmp=int64_t(a/b);
		return a-tmp*b;
	}
	void* fft_malloc(int sz) {
		#ifndef JACKFFT_USING_FFTW
			return aligned_alloc(128,sz);
		#else
			return FFTWFUNC(malloc)(sz);
		#endif
	}
	void fft_free(void* v) {
		#ifndef JACKFFT_USING_FFTW
			return free(v);
		#else
			return FFTWFUNC(free)(v);
		#endif
	}
	class FFT
	{
	public:
		int size;
		int size_c;
		jackfft_float* Data;
		jackfft_complex* Data_c;
	#ifdef JACKFFT_USE_FFTS
		ffts_plan_t *p1,*p2;
	#elif defined JACKFFT_USING_FFTW
		FFTWFUNC(plan) p1, p2;
	#endif
		FFT(int size, bool preserveInput=false):size(size),size_c(size/2+1)
		{
			Data=(jackfft_float*)fft_malloc(sizeof(jackfft_float)*size);
			Data_c=(jackfft_complex*)fft_malloc(sizeof(jackfft_complex)*size_c);
			#ifdef JACKFFT_USE_FFTS
				p1 = ffts_init_1d_real(size,-1);	//forward
				p2 = ffts_init_1d_real(size,1);		//inverse
			#elif defined JACKFFT_USING_FFTW
				if(preserveInput) {
					p1 = FFTWFUNC(plan_dft_r2c_1d)(size, Data, Data_c, FFTW_ESTIMATE|FFTW_PRESERVE_INPUT); //FFTW_UNALIGNED
					p2 = FFTWFUNC(plan_dft_c2r_1d)(size, Data_c, Data, FFTW_ESTIMATE|FFTW_PRESERVE_INPUT);
				} else {
					p1 = FFTWFUNC(plan_dft_r2c_1d)(size, Data, Data_c, FFTW_ESTIMATE); //FFTW_UNALIGNED
					p2 = FFTWFUNC(plan_dft_c2r_1d)(size, Data_c, Data, FFTW_ESTIMATE);
				}
			#endif
		}
	#ifdef JACKFFT_USE_FFTS
		void Forward() { ffts_execute(p1,Data,Data_c); }
		void Reverse() { ffts_execute(p2,Data_c,Data); }
	#elif defined JACKFFT_USING_FFTW
		void Forward() { FFTWFUNC(execute)(p1); }
		void Reverse() { FFTWFUNC(execute)(p2); }
	#else
		void Forward() {
			complex* datac=(complex*)Data_c;
			switch(size) {
				case 1024: fftN<1024>(Data,datac); break;
				case 2048: fftN<2048>(Data,datac); break;
				case 4096: fftN<4096>(Data,datac); break;
				case 8192: fftN<8192>(Data,datac); break;
				//case 8192: fftN2(Data,datac,8192); break;
				case 16384: fftN<16384>(Data,datac); break;
				default: throw logic_error("fft size not supported");
			}
		}
		void Reverse() {
			complex* datac=(complex*)Data_c;
			switch(size) {
				case 1024: ifftN<1024>(datac,Data); break;
				case 2048: ifftN<2048>(datac,Data); break;
				case 4096: ifftN<4096>(datac,Data); break;
				case 8192: ifftN<8192>(datac,Data); break;
				case 16384: ifftN<16384>(datac,Data); break;
				default: throw logic_error("fft size not supported");
			}
		}
	#endif
		~FFT() {
			#ifdef JACKFFT_USE_FFTS
				ffts_free(p1);
				ffts_free(p2);
			#elif defined JACKFFT_USING_FFTW
				FFTWFUNC(destroy_plan)(p1);
				FFTWFUNC(destroy_plan)(p2);
			#endif
			fft_free(Data);
			fft_free(Data_c);
		}
	};
	/*
	#ifdef JACKFFT_USE_FFTS
	#else
	#endif
	 */
	class WindowedFFT
	{
	public:
		int size;
		int size_c;
		jackfft_float* Data;
		jackfft_complex* Data_c;
		FFT fft;
		WindowedFFT(int size):size(size),size_c(size/2+1),fft(size)
		{
			Data=fft.Data;
			Data_c=fft.Data_c;
		}
		void Forward(jackfft_float* data, int length)
		{
			if(length>size) throw std::logic_error("WindowedFFT::Forward(): data length exceeds fft size");
			int s=size/2-ceil((double)length/2);
			memset(Data,0,size*sizeof(jackfft_float));
			memcpy(Data+s,data,length*sizeof(jackfft_float));
			fft.Forward();
		}
		jackfft_float* Reverse(int length)
		{
			if(length>size) throw std::logic_error("WindowedFFT::Reverse(): data length exceeds fft size");
			int s=size/2-ceil((double)length/2);
			fft.Reverse();
			return Data+s;
		}
		~WindowedFFT()
		{
		}
	};
	template<class NUMTYPE>class FFTFilter: public OverlappedFilter2_new<NUMTYPE, jackfft_float>
	{
	public:
		//fftw_plan p1, p2;
		//double* tmpdouble;
		jackfft_complex* tmpcomplex;
		jackfft_complex* lastSpectrum;
		//jackfft_complex* tmpcomplex2;
		double* coefficients;
		double* coefficients2;
		double freq_scale;
		virtual void alloc_buffer(){this->tmpbuffer=(jackfft_float*)fft_malloc(sizeof(jackfft_float)*this->PeriodSize());}
		virtual void free_buffer(){fft_free(this->tmpbuffer);}
		//struct timespec last_refreshed;
		WindowedFFT fft;
		inline unsigned ComplexSize()
		{
			return fft.size_c;
		}
		
		FFTFilter(unsigned buffersize, unsigned inbuffers, unsigned outbuffers, unsigned overlapcount, unsigned BuffersPerPeriod, int padding1, int padding2, unsigned FFTSize):
			OverlappedFilter2_new<NUMTYPE, jackfft_float>(buffersize, inbuffers, outbuffers, overlapcount, BuffersPerPeriod, padding1, padding2),freq_scale(1.0),fft(FFTSize)
		{
			asdf=0;
			int l=ComplexSize();//((unsigned)(this->PeriodSize() / 2) + 1);
			coefficients = new double[l];
			
			for(int i=0;i<l;i++)
				coefficients[i] = 1.0;
				
			tmpcomplex=fft.Data_c;
			lastSpectrum=(jackfft_complex*)fft_malloc(sizeof(jackfft_complex)*l);
		}
		~FFTFilter()
		{
			fft_free(lastSpectrum);
			delete[] coefficients;
		}

		int asdf;

		virtual void DoProcess()
		{
			int complexsize = ComplexSize();
			
			fft.Forward(this->tmpbuffer,this->PeriodSize());
			memcpy(lastSpectrum,fft.Data_c,ComplexSize()*sizeof(jackfft_complex));
			if(freq_scale==1.)
			{
				for(int i=0;i<complexsize;i++)
				{
					fft.Data_c[i][0] = fft.Data_c[i][0]*coefficients[i];
					fft.Data_c[i][1] = fft.Data_c[i][1]*coefficients[i];
				}
				goto trolled;
			}
			bool rev;
			rev=freq_scale>1.;
			for(int i=rev?int(complexsize)-1:0;rev?(i>=0):(i<int(complexsize));rev?i--:i++)
			{
				int i2;
				//if(i>50)
				double asdf=((double)i)/freq_scale; //source frequency
				i2=(int)asdf;
				if(i2==0 || i==0)	//prevent division by zero
					continue;
				if(i2+1>=(int)complexsize)
				{
					fft.Data_c[i][0]=fft.Data_c[i][1]=0.;
					continue;
				}
				double trololo=asdf-i2;
				//double freq1=(asdf/(double)(complexsize-1))*0.5;
				double period1=(double)fft.size/(double)asdf;
				//double freq2=(double)i/(double)(complexsize-1)*0.5;
				double period2=(double)fft.size/(double)i;
				double sine=(tmpcomplex[i2][1]*(1.0-trololo) + tmpcomplex[i2+1][1]*trololo);
				double cosine=(tmpcomplex[i2][0]*(1.0-trololo) + tmpcomplex[i2+1][0]*trololo);

				double amplitude=sqrt(sine*sine+cosine*cosine);
				
				
				double phase=atan2(cosine,sine);	//radians; [-pi,pi]
				//double phase_delta1=(skip_samples%period1)/period1*(2*M_PI);
				phase=phase*((double)period1);
				//phase=phase/((double)(M_PI*2));
				//phase=phase*((double)(M_PI*2));
				phase=phase/((double)period2);
				
				amplitude*=coefficients[i];
				fft.Data_c[i][1] = cos(phase)*amplitude;
				fft.Data_c[i][0] = sin(phase)*amplitude;
			}
		trolled:
			jackfft_float* d=fft.Reverse(this->PeriodSize());
			memcpy(this->tmpbuffer,d,sizeof(jackfft_float)*this->PeriodSize());
			int ps=this->PeriodSize();
			for(int i=0;i<ps;i++)
				this->tmpbuffer[i] /= fft.size;
		}
	};
};


