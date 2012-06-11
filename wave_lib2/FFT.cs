using System;
using System.Collections.Generic;
using System.Text;

namespace wave_lib2
{
	public struct complex
	{
		public double a;//m_re
		public double b;//m_im
		public static complex create(double a, double b)
		{
			complex c;
			c.a = a;
			c.b = b;
			return c;
		}
		public static complex create(double a)
		{
			complex c;
			c.a = a;
			c.b = 0;
			return c;
		}
		public static complex operator +(complex a, complex b)
		{
			complex c;
			c.a = a.a + b.a;
			c.b = a.b + b.b;
			return c;
		}
		public static complex operator -(complex a, complex b)
		{
			complex c;
			c.a = a.a - b.a;
			c.b = a.b - b.b;
			return c;
		}
		public static complex operator *(complex a, complex b)
		{
			complex c;
			c.a = a.a * b.a - a.b * b.b;
			c.b = a.a * b.b + a.b * b.a;
			return c;
		}
		public static complex operator /(complex a, complex b)
		{
			double denominator = b.a * b.a + b.b * b.b;
			complex c;
			c.a = (a.a * b.a + a.b * b.b) / denominator;
			c.b = (a.b * b.a - a.a * b.b) / denominator;
			return c;
		}
		public static implicit operator complex(double d)
		{
			complex c;
			c.a = d;
			c.b = 0;
			return c;
		}
		public static implicit operator double(complex c)
		{
			return c.a;
		}
	}
	public unsafe class FFT
	{
		//   FORWARD FOURIER TRANSFORM
		//     Input  - input data
		//     Output - transform result
		//     N      - length of both input data and result
		public bool Forward(complex* Input, complex* Output, uint N)
		{
			//   Check input parameters
			if (Input == null || Output == null || N < 1 || (N & (N - 1)) != 0)
				return false;
			//   Initialize data
			Rearrange(Input, Output, N);
			//   Call FFT implementation
			Perform(Output, N, false);
			//   Succeeded
			return true;
		}

		//   FORWARD FOURIER TRANSFORM, INPLACE VERSION
		//     Data - both input data and output
		//     N    - length of input data
		public bool Forward(complex* Data, uint N)
		{
			//   Check input parameters
			if (Data == null || N < 1 || (N & (N - 1)) != 0)
				return false;
			//   Rearrange
			Rearrange(Data, N);
			//   Call FFT implementation
			Perform(Data, N, false);
			//   Succeeded
			return true;
		}
		//   Rearrange function
		public void Rearrange(complex* Input, complex* Output, uint N)
		{
			//   Data entry position
			uint Target = 0;
			//   Process all positions of input signal
			for (uint Position = 0; Position < N; ++Position)
			{
				//  Set data entry
				Output[Target] = Input[Position];
				//   Bit mask
				uint Mask = N;
				//   While bit is set
				while ((Target & (Mask >>= 1)) != 0)
					//   Drop bit
					Target &= ~Mask;
				//   The current bit is 0 - set it
				Target |= Mask;
			}
		}

		//   Inplace version of rearrange function
		public void Rearrange(complex* Data, uint N)
		{
			//   Swap position
			uint Target = 0;
			//   Process all positions of input signal
			for (uint Position = 0; Position < N; ++Position)
			{
				//   Only for not yet swapped entries
				if (Target > Position)
				{
					//   Swap entries
					complex Temp = Data[Target];
					Data[Target] = Data[Position];
					Data[Position] = Temp;
				}
				//   Bit mask
				uint Mask = N;
				//   While bit is set
				while ((Target & (Mask >>= 1)) != 0)
					//   Drop bit
					Target &= ~Mask;
				//   The current bit is 0 - set it
				Target |= Mask;
			}
		}
		//   FFT implementation
		public void Perform(complex* Data, uint N, bool Inverse /* = false */)
		{
			double pi = Inverse ? 3.14159265358979323846 : -3.14159265358979323846;
			//   Iteration through dyads, quadruples, octads and so on...
			for (uint Step = 1; Step < N; Step <<= 1)
			{
				//   Jump to the next entry of the same transform factor
				uint Jump = Step << 1;
				//   Angle increment
				double delta = pi / (double)Step;
				//   Auxiliary sin(delta / 2)
				double Sine = System.Math.Sin(delta * .5);
				//   Multiplier for trigonometric recurrence
				complex Multiplier = complex.create(-2.0 * Sine * Sine, System.Math.Sin(delta));
				//   Start value for transform factor, fi = 0
				complex Factor = complex.create(1.0);
				//   Iteration through groups of different transform factor
				for (uint Group = 0; Group < Step; ++Group)
				{
					//   Iteration within group 
					for (uint Pair = Group; Pair < N; Pair += Jump)
					{
						//   Match position
						uint Match = Pair + Step;
						//   Second term of two-point transform
						complex Product = Factor * Data[Match];
						//   Transform for fi + pi
						Data[Match] = Data[Pair] - Product;
						//   Transform for fi
						Data[Pair] += Product;
					}
					//   Successive transform factor via trigonometric recurrence
					Factor = Multiplier * Factor + Factor;
				}
			}
		}
		//   INVERSE FOURIER TRANSFORM
		//     Input  - input data
		//     Output - transform result
		//     N      - length of both input data and result
		//     Scale  - if to scale result
		public bool Inverse(complex* Input, complex* Output, uint N, bool Scale /* = true */)
		{
			//   Check input parameters
			if (Input == null || Output == null || N < 1 || (N & (N - 1)) != 0)
				return false;
			//   Initialize data
			Rearrange(Input, Output, N);
			//   Call FFT implementation
			Perform(Output, N, true);
			//   Scale if necessary
			if (Scale)
				this.Scale(Output, N);
			//   Succeeded
			return true;
		}

		//   INVERSE FOURIER TRANSFORM, INPLACE VERSION
		//     Data  - both input data and output
		//     N     - length of both input data and result
		//     Scale - if to scale result
		public bool Inverse(complex* Data, uint N, bool Scale /* = true */)
		{
			//   Check input parameters
			if (Data == null || N < 1 || (N & (N - 1)) != 0)
				return false;
			//   Rearrange
			Rearrange(Data, N);
			//   Call FFT implementation
			Perform(Data, N, true);
			//   Scale if necessary
			if (Scale)
				this.Scale(Data, N);
			//   Succeeded
			return true;
		}
		//   Scaling of inverse FFT result
		public void Scale(complex* Data, uint N)
		{
			double Factor = 1.0 / (double)N;
			//   Scale all data entries
			for (uint Position = 0; Position < N; ++Position)
				Data[Position] *= Factor;
		}

	}
	public unsafe class VBFFT
	{
		public static complex[] to_complex(short[] src, uint l)
		{
			complex[] c1 = new complex[l];
			fixed (complex* c = c1)
			{
				uint i;
				for (i = 0; i < l; i++)
				{
					c[i].a = (double)src[i];
				}
			}
			return c1;
		}
		public static void to_complex(complex[] c1, short[] src, uint l)
		{
			//complex[] c1 = new complex[l];
			fixed (complex* c = c1)
			{
				uint i;
				for (i = 0; i < l; i++)
				{
					c[i].a = (double)src[i];
					c[i].b = 0D;
				}
			}
			return;
		}
		public static void to_complex(complex[] c1, short* src, uint l)
		{
			//complex[] c1 = new complex[l];
			fixed (complex* c = c1)
			{
				uint i;
				for (i = 0; i < l; i++)
				{
					c[i].a = (double)src[i];
					c[i].b = 0D;
				}
			}
			return;
		}
		public static void to_short(short[] s, complex[] c, uint l)
		{
			uint i;
			for (i = 0; i < l; i++)
			{
				s[i] = (short)(System.Math.Sqrt((c[i].a * c[i].a) + (c[i].b * c[i].b)));
			}
		}
		public static void to_short(short* s, complex[] c, uint l)
		{
			uint i;
			for (i = 0; i < l; i++)
			{
				s[i] = (short)(System.Math.Sqrt((c[i].a * c[i].a) + (c[i].b * c[i].b)));
			}
		}
		public static void to_double(double[] d, complex[] c, uint l)
		{
			uint i;
			for (i = 0; i < l; i++)
			{
				d[i] = System.Math.Sqrt((c[i].a * c[i].a) + (c[i].b * c[i].b));
			}
		}
		public static FFT f = new FFT();
		public static void forward(complex[] src, complex[] dst)
		{
			uint l = (uint)System.Math.Min(src.Length, dst.Length);
			fixed (complex* _src = src)
			fixed (complex* _dst = dst)
			{
				f.Forward(_src, _dst, l);
			}
		}
		public static void inverse(complex[] src, complex[] dst)
		{
			uint l = (uint)System.Math.Min(src.Length, dst.Length);
			FFT f = new FFT();
			fixed (complex* _src = src)
			fixed (complex* _dst = dst)
			{
				f.Inverse(_src, _dst, l, true);
			}
		}
		public static void forward(complex[] srcdst)
		{
			FFT f = new FFT();
			fixed (complex* _srcdst = srcdst)
			{
				f.Forward(_srcdst, (uint)srcdst.Length);
			}
		}
		public static void inverse(complex[] srcdst)
		{
			FFT f = new FFT();
			fixed (complex* _srcdst = srcdst)
			{
				f.Inverse(_srcdst, (uint)srcdst.Length, true);
			}
		}

		/*public static void to_complex(Exocortex.DSP.ComplexF[] c1, short* src, uint l)
		{
			//complex[] c1 = new complex[l];
			fixed (Exocortex.DSP.ComplexF* c = c1)
			{
				uint i;
				for (i = 0; i < l; i++)
				{
					c[i] = new Exocortex.DSP.ComplexF();
					c[i].Im = 0F;
					c[i].Re = (float)src[i];
				}
			}
		}
		public static void to_short(short* s, Exocortex.DSP.ComplexF[] c, uint l)
		{
			uint i;
			for (i = 0; i < l; i++)
			{
				s[i] = (short)(System.Math.Sqrt((c[i].Re * c[i].Re) + (c[i].Im * c[i].Im))/8192);
			}
		}*/
	}
	//UPGRADE_NOTE: Field 'EnclosingInstance' was added to class 'FFT' to access its enclosing instance. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1019'"
	//UPGRADE_NOTE: Inner class 'FFT' is now serializable, and this may become a security issue. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1234'"
	[Serializable]
	public class FFT2
	{
		public double[] wtabf;
		public double[] wtabi;
		public int size;
		public FFT2(int sz)
		{
			size = sz;
			if ((size & (size - 1)) != 0)
				System.Console.Out.WriteLine("size must be power of two!");
			calcWTable();
		}

		public virtual void calcWTable()
		{
			// calculate table of powers of w
			wtabf = new double[size];
			wtabi = new double[size];
			int i;
			for (i = 0; i != size; i += 2)
			{
				double pi = 3.1415926535;
				double th = pi * i / size;
				wtabf[i] = System.Math.Cos(th);
				wtabf[i + 1] = System.Math.Sin(th);
				wtabi[i] = wtabf[i];
				wtabi[i + 1] = -wtabf[i + 1];
			}
		}

		public virtual void transform(double[] data, bool inv)
		{
			int i;
			int j = 0;
			int size2 = size * 2;

			if ((size & (size - 1)) != 0)
				System.Console.Out.WriteLine("size must be power of two!");

			// bit-reversal
			double q;
			int bit;
			for (i = 0; i != size2; i += 2)
			{
				if (i > j)
				{
					q = data[i]; data[i] = data[j]; data[j] = q;
					q = data[i + 1]; data[i + 1] = data[j + 1]; data[j + 1] = q;
				}
				// increment j by one, from the left side (bit-reversed)
				bit = size;
				while ((bit & j) != 0)
				{
					j &= ~bit;
					bit >>= 1;
				}
				j |= bit;
			}

			// amount to skip through w table
			int tabskip = size << 1;
			double[] wtab = (inv) ? wtabi : wtabf;

			int skip1, skip2, ix, j2;
			double wr, wi, d1r, d1i, d2r, d2i, d2wr, d2wi;

			// unroll the first iteration of the main loop
			for (i = 0; i != size2; i += 4)
			{
				d1r = data[i];
				d1i = data[i + 1];
				d2r = data[i + 2];
				d2i = data[i + 3];
				data[i] = d1r + d2r;
				data[i + 1] = d1i + d2i;
				data[i + 2] = d1r - d2r;
				data[i + 3] = d1i - d2i;
			}
			tabskip >>= 1;

			// unroll the second iteration of the main loop
			int imult = (inv) ? -1 : 1;
			for (i = 0; i != size2; i += 8)
			{
				d1r = data[i];
				d1i = data[i + 1];
				d2r = data[i + 4];
				d2i = data[i + 5];
				data[i] = d1r + d2r;
				data[i + 1] = d1i + d2i;
				data[i + 4] = d1r - d2r;
				data[i + 5] = d1i - d2i;
				d1r = data[i + 2];
				d1i = data[i + 3];
				d2r = data[i + 6] * imult;
				d2i = data[i + 7] * imult;
				data[i + 2] = d1r - d2i;
				data[i + 3] = d1i + d2r;
				data[i + 6] = d1r + d2i;
				data[i + 7] = d1i - d2r;
			}
			tabskip >>= 1;

			for (skip1 = 16; skip1 <= size2; skip1 <<= 1)
			{
				// skip2 = length of subarrays we are combining
				// skip1 = length of subarray after combination
				skip2 = skip1 >> 1;
				tabskip >>= 1;
				for (i = 0; i != 1000; i++)
					;
				// for each subarray
				for (i = 0; i < size2; i += skip1)
				{
					ix = 0;
					// for each pair of complex numbers (one in each subarray)
					for (j = i; j != i + skip2; j += 2, ix += tabskip)
					{
						wr = wtab[ix];
						wi = wtab[ix + 1];
						d1r = data[j];
						d1i = data[j + 1];
						j2 = j + skip2;
						d2r = data[j2];
						d2i = data[j2 + 1];
						d2wr = d2r * wr - d2i * wi;
						d2wi = d2r * wi + d2i * wr;
						data[j] = d1r + d2wr;
						data[j + 1] = d1i + d2wi;
						data[j2] = d1r - d2wr;
						data[j2 + 1] = d1i - d2wi;
					}
				}
			}
		}
	}
}
