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
#ifdef CEPSTRUM
		FFT fft2;
		inline unsigned ComplexSize2()
		{
			return fft2.size_c;
		}
#endif
		inline unsigned ComplexSize()
		{
			return fft.size_c;
		}
		
		FFTFilter(unsigned buffersize, unsigned inbuffers, unsigned outbuffers, unsigned overlapcount, unsigned BuffersPerPeriod, int padding1, int padding2, unsigned FFTSize):
			OverlappedFilter2_new<NUMTYPE, jackfft_float>(buffersize, inbuffers, outbuffers, overlapcount, BuffersPerPeriod, padding1, padding2),freq_scale(1.0),fft(FFTSize)
#ifdef CEPSTRUM
		,fft2(ComplexSize())
#endif
		{
			asdf=0;
			//memset(&last_refreshed,0,sizeof(last_refreshed));
			int l=ComplexSize();//((unsigned)(this->PeriodSize() / 2) + 1);
			coefficients = new double[l];
			
			for(int i=0;i<l;i++)
				coefficients[i] = 1.0;
				
#ifdef CEPSTRUM
			coefficients2 = new double[ComplexSize2()];
			for(int i=0;i<ComplexSize2();i++)
				coefficients2[i] = 1.0;
#endif
			tmpcomplex=fft.Data_c;
			lastSpectrum=(jackfft_complex*)fft_malloc(sizeof(jackfft_complex)*l);
			//tmpdouble = (double*)fft_malloc(sizeof(double)*buffersize);

			//tmpcomplex = (jackfft_complex*)fft_malloc(sizeof(jackfft_complex) * l);
			/*tmpcomplex2 = (jackfft_complex*)fft_malloc(sizeof(jackfft_complex) * l);

			//cout << this->PeriodSize() << endl;
			p1 = fftw_plan_dft_r2c_1d(this->PeriodSize(), this->tmpbuffer, tmpcomplex, 0); //FFTW_UNALIGNED
			p2 = fftw_plan_dft_c2r_1d(this->PeriodSize(), tmpcomplex2, this->tmpbuffer, 0);
			* */
		}
		~FFTFilter()
		{
			/*fftw_destroy_plan(p1);
			fftw_destroy_plan(p2);
			//fft_free(tmpdouble);*/
			//fft_free(tmpcomplex);
			//fft_free(tmpcomplex2);
			fft_free(lastSpectrum);
			delete[] coefficients;
#ifdef CEPSTRUM
			delete[] coefficients2;
#endif
		}

		/*long long get_nsec(const timespec& t)
		{
			return t.tv_sec*1000000000+t.tv_nsec;
		}*/
		int asdf;

		virtual void DoProcess()
		{
			int complexsize = ComplexSize();//(unsigned)(this->PeriodSize() / 2) + 1;
			//asdf++;
			//if(asdf>1)
			//{
			//fftw_execute(p1);
			//cout << this->PeriodSize() << endl;
			fft.Forward(this->tmpbuffer,this->PeriodSize());
			memcpy(lastSpectrum,fft.Data_c,ComplexSize()*sizeof(jackfft_complex));
#ifdef CEPSTRUM
			for(unsigned i=0;i<complexsize;i++) {
				auto sine=fft.Data_c[i][1];
				auto cosine=fft.Data_c[i][0];
				double amplitude=sqrt(sine*sine+cosine*cosine);
				//double phase=atan2(cosine,sine);
				if(amplitude<0.00001) //10^-5
					fft2.Data[i]=-5;
				else fft2.Data[i]=logf(amplitude)/M_LN10;
				//fft2.Data[i]=amplitude;
			}
			fft2.Forward();
			for(unsigned i=0;i<ComplexSize2();i++)
			{
				fft2.Data_c[i][0] = fft2.Data_c[i][0]*coefficients2[i];
				fft2.Data_c[i][1] = fft2.Data_c[i][1]*coefficients2[i];
			}
			fft2.Reverse();
			for(unsigned i=0;i<complexsize;i++) {
				auto sine=fft.Data_c[i][1];
				auto cosine=fft.Data_c[i][0];
				double amplitude=pow(10,fft2.Data[i]/fft2.size);
				double phase=atan2(cosine,sine);
				fft.Data_c[i][1] = cos(phase)*amplitude;
				fft.Data_c[i][0] = sin(phase)*amplitude;
			}
#endif
			
				//int shift=21;
				/*for(unsigned i=0;i<complexsize;i++)
				{
					tmpcomplex[i][0] = tmpcomplex[i][0]*coefficients[i];
					tmpcomplex[i][1] = tmpcomplex[i][1]*coefficients[i];
				}*/
				//asdf=0;
			//}

			//tmpcomplex[0][0]=0;
			//tmpcomplex[0][1]=0;


			/*for(unsigned i=(complexsize-1)/2+1;i<complexsize;i++)
			{
				tmpcomplex[i][0]=0;
				tmpcomplex[i][1]=0;
			}*/
			//memcpy(tmpcomplex,fft.Data_c,complexsize*sizeof(jackfft_complex));
			if(freq_scale==1.)
			{
				for(unsigned i=0;i<complexsize;i++)
				{
					fft.Data_c[i][0] = fft.Data_c[i][0]*coefficients[i];
					fft.Data_c[i][1] = fft.Data_c[i][1]*coefficients[i];
				}
				goto trolled;
			}
			/*int skip=this->BuffersPerPeriod/this->overlapcount;
			if(skip<1)skip=1;
			unsigned overlapcount=this->BuffersPerPeriod/skip;
			int skip_samples=this->PeriodSize()/overlapcount;*/

			//for(int i=complexsize-1;i>=0;i--)
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
				
				//double phase=atan2(tmpcomplex[i][0],tmpcomplex[i][1]);
				
				//phase*=1.5;
				//if(i==100)cout << phase << endl;
				//else i2=i;
				//fft.Data_c[i][0] = (fft.Data_c[i2][0]*(1.0-trololo) + fft.Data_c[i2+1][0]*trololo)*coefficients[i];
				//fft.Data_c[i][1] = (fft.Data_c[i2][1]*(1.0-trololo) + fft.Data_c[i2+1][1]*trololo)*coefficients[i];
				amplitude*=coefficients[i];
				fft.Data_c[i][1] = cos(phase)*amplitude;
				fft.Data_c[i][0] = sin(phase)*amplitude;
			}
		trolled:
			//fftw_execute(p2);
			/*int shift=50;
			for(int i=complexsize-1-shift;i>=0;i--)
			{
				fft.Data_c[i+shift][0]=fft.Data_c[i][0]*coefficients[i+shift];
				fft.Data_c[i+shift][1]=fft.Data_c[i][1]*coefficients[i+shift];
			}*/
			
			jackfft_float* d=fft.Reverse(this->PeriodSize());
			memcpy(this->tmpbuffer,d,sizeof(jackfft_float)*this->PeriodSize());
			int ps=this->PeriodSize();
			for(int i=0;i<ps;i++)
				this->tmpbuffer[i] /= fft.size;
		}
	};
	/*template<class NUMTYPE>class FFTTransform: public OverlappedFilter3<NUMTYPE, double>
	{
	public:
		fftw_plan p1, p2;
		//double* tmpdouble;
		jackfft_complex* tmpcomplex;
		//double* coefficients;
		virtual void alloc_buffer(){this->tmpbuffer=(double*)fft_malloc(sizeof(double)*this->PeriodSize());}
		virtual void free_buffer(){fft_free(this->tmpbuffer);}
		FFTTransform(unsigned buffersize, unsigned inbuffers, unsigned outbuffers, unsigned overlapcount, unsigned BuffersPerPeriod): OverlappedFilter3<NUMTYPE, double>(buffersize, (buffersize*BuffersPerPeriod/2+1)*2, inbuffers, outbuffers, overlapcount, BuffersPerPeriod)
		{
			int l=((unsigned)(this->PeriodSize() / 2) + 1);
			//tmpdouble = (double*)fft_malloc(sizeof(double)*buffersize);
			tmpcomplex = (jackfft_complex*)fft_malloc(sizeof(jackfft_complex) * l);
			//coefficients = new double[l];
			//cout << this->PeriodSize() << endl;
			p1 = fftw_plan_dft_r2c_1d(this->PeriodSize(), this->tmpbuffer, tmpcomplex, 0); //FFTW_UNALIGNED
			//p2 = fftw_plan_dft_c2r_1d(this->PeriodSize(), tmpcomplex, this->tmpbuffer, 0);
		}
		~FFTTransform()
		{
			fftw_destroy_plan(p1);
			//fftw_destroy_plan(p2);
			//fft_free(tmpdouble);
			fft_free(tmpcomplex);
			//delete[] coefficients;
		}
		virtual unsigned OutputSize()
		{
			return (unsigned)(((this->PeriodSize() / 2) + 1) * 2);
		}
		virtual void DoProcess()
		{
			unsigned complexsize = (unsigned)(this->PeriodSize() / 2) + 1;
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
	class FFTStream_f
	{
	public:
		//double* phase;
		double* buffer;
		jackfft_complex* buffer_c;
		double* buffer_out;
		int size,size_c;
		fftw_plan p1;
		FFTStream_f(int size):size(size),size_c(size/2+1)
		{
			buffer=(double*)fft_malloc(sizeof(double)*size);
			buffer_c=(jackfft_complex*)fft_malloc(sizeof(jackfft_complex)*size_c);
			buffer_out=(double*)fft_malloc(sizeof(double)*size_c);
			p1 = fftw_plan_dft_r2c_1d(size, buffer, buffer_c, 0); //FFTW_UNALIGNED
			//p2 = fftw_plan_dft_c2r_1d(size, buffer_c, buffer, 0);
		}
		~FFTStream_f()
		{
			fftw_destroy_plan(p1);
			fft_free(buffer);
			fft_free(buffer_c);
			fft_free(buffer_out);
		}
		double* Process()
		{
			int w=size/2;
			for(int i=0;i<w;i++)
			{
				buffer[i]*=((double)i)/(w-1);
			}
			for(int i=size-w;i<size;i++)
			{
				buffer[i]*=((double)(i-(size-w)))/(w-1);
			}
			fftw_execute(p1);
			for(int i=0;i<size_c;i++)
			{
				buffer_out[i]=sqrt(pow(buffer_c[i][0],2)+pow(buffer_c[i][1],2));
			}
			return buffer_out;
		}
	};

	class FFTStream_r
	{
	public:
		double* phase;
		double* buffer;
		jackfft_complex* buffer_c;
		double* buffer_out;
		int size,size_c;
		fftw_plan p1;
		FFTStream_r(int size):size(size),size_c(size/2+1)
		{
			buffer=(double*)fft_malloc(sizeof(double)*size);
			buffer_c=(jackfft_complex*)fft_malloc(sizeof(jackfft_complex)*size_c);
			phase=(double*)fft_malloc(sizeof(double)*size_c);
			for(int i=0;i<size_c;i++)
				phase[i]=100;
			buffer_out=(double*)fft_malloc(sizeof(double)*size);
			//p1 = fftw_plan_dft_r2c_1d(size, buffer, buffer_c, 0); //FFTW_UNALIGNED
			p1 = fftw_plan_dft_c2r_1d(size, buffer_c, buffer_out, 0);
		}
		~FFTStream_r()
		{
			fftw_destroy_plan(p1);
			fft_free(buffer);
			fft_free(buffer_c);
			fft_free(phase);
			fft_free(buffer_out);
		}
		double* Process()
		{
			for(int i=0;i<size_c;i++)
			{
				if(i==0||i==size_c-1)
				{
					buffer_c[i][0]=this->buffer[i];
					continue;
				}
				double phase=this->phase[i];
				double amplitude=this->buffer[i];
				//sine
				buffer_c[i][0]=cos(phase)*amplitude;
				//cosine
				buffer_c[i][1]=sin(phase)*amplitude;

				//double period=1/freq;
				//this->phase[i]=phase;
			}
			fftw_execute(p1);
			for(int i=0;i<size;i++)
				buffer_out[i]/=size;
			return buffer_out;
		}
	};*/
};


