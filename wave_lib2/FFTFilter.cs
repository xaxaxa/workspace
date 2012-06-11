using System;
using System.Collections.Generic;
using System.Text;

namespace wave_lib2
{
	public class FFTFilter
	{
		private FFTFilter() { }
		public static void Process(double[] data, IEqualizer eq, uint sampleRate)
		{
			wave_lib2.FFT2 f = new FFT2(data.Length / 2);
			f.transform(data, false);
			int j;
			uint l_freq = 0xFFFFFFFF;
			double _amp = 0;
			uint tmp = 0;
			for (j = 0; j < (int)(data.Length / 2); j++)
			{
				tmp = (uint)(((double)j) / data.Length * sampleRate);
				if (tmp != l_freq)
				{
					l_freq = tmp;
					_amp = eq.GetPoint(tmp);
				}
				data[j] *= _amp;// eq.GetPoint();
			}
			for (j = data.Length / 2; j < data.Length; j++)
			{
				data[j] = 0.0;
			}
			double mult = 1;
			// multiply transforms to get convolution
			for (j = 0; j != data.Length; j += 2)
			{
				double a = data[j] - data[j + 1];
				double b = data[j] + data[j + 1];
				data[j] = a * mult;
				data[j + 1] = b * mult;
				//mult += 0.001;
			}
			f.transform(data, true);
		}
		public static void GetNoise(double[] data, IEqualizer eq, uint sampleRate)
		{
			wave_lib2.FFT2 f = new FFT2(data.Length / 2);
			//f.transform(data, false);
			int j;
			uint l_freq = 0xFFFFFFFF;
			double _amp = 0;
			uint tmp = 0;
			for (j = 0; j < (int)(data.Length / 2); j++)
			{
				tmp = (uint)(((double)j) / data.Length * sampleRate);
				if (tmp != l_freq)
				{
					l_freq = tmp;
					_amp = eq.GetPoint(tmp);
				}
				data[j] = _amp;// eq.GetPoint();
			}
			for (j = data.Length / 2; j < data.Length; j++)
			{
				data[j] = 0.0;
			}
			double mult = 1;
			// multiply transforms to get convolution
			for (j = 0; j != data.Length; j += 2)
			{
				double a = data[j] - data[j + 1];
				double b = data[j] + data[j + 1];
				data[j] = a * mult;
				data[j + 1] = b * mult;
				//mult += 0.001;
			}
			f.transform(data, true);
		}
	}
	public interface IEqualizer
	{
		double GetPoint(uint freq);

	}
	public class NoiseStream : System.IO.Stream
	{
		public IEqualizer equalizer = null;
		public override bool CanRead
		{
			get { return equalizer != null; }
		}

		public override bool CanSeek
		{
			get { return false; }
		}

		public override bool CanWrite
		{
			get { return false; }
		}

		public override void Flush()
		{
			//throw new Exception("The method or operation is not implemented.");
		}

		public override long Length
		{
			get { throw new Exception("The method or operation is not implemented."); }
		}

		public override long Position
		{
			get
			{
				throw new Exception("The method or operation is not implemented.");
			}
			set
			{
				throw new Exception("The method or operation is not implemented.");
			}
		}
		public int sampleRate = 44100;
		private double[] d;
		public unsafe override int Read(byte[] buffer, int offset, int count)
		{
			int length = count / 2;
			if (d == null || d.Length != length) d = new double[length];
			fixed (byte* tmp = buffer)
			{
				short* data = (short*)(tmp + offset);
				int i;
				FFTFilter.GetNoise(d, equalizer, (uint)sampleRate);
				for (i = 0; i < length; i++)
				{
					data[i] = (short)(d[i] * short.MaxValue);
				}
			}
			return count;
		}

		public override long Seek(long offset, System.IO.SeekOrigin origin)
		{
			throw new Exception("The method or operation is not implemented.");
		}

		public override void SetLength(long value)
		{
			throw new Exception("The method or operation is not implemented.");
		}

		public override void Write(byte[] buffer, int offset, int count)
		{
			throw new Exception("The method or operation is not implemented.");
		}
	}
	public struct SineWave
	{
		public double freq;
		public double pos;
	}
	public class SineStream : System.IO.Stream
	{
		public int AddFreq(double freq)
		{
			int i;
			for (i = 0; i < waves.Count; i++)
			{
				if (waves[i].freq == freq)
				{
					return i;
				}
			}
			lock (waves)
			{
				SineWave sw;
				sw.pos = 0.0;
				sw.freq = freq;
				waves.Add(sw);
				return waves.Count - 1;
			}
		}
		public int GetFreq(double freq)
		{
			int i;
			lock (waves)
			{
				for (i = 0; i < waves.Count; i++)
				{
					if (waves[i].freq == freq)
					{
						//SineWave sw = waves[i];
						//waves.RemoveAt(i);
						//return sw;
						return i;
					}
				}
			}
			return -1;
		}
		public void RemoveFreq(double freq)
		{
			int i;
			lock (waves)
			{
				for (i = 0; i < waves.Count; i++)
				{
					if (waves[i].freq == freq)
					{
						//SineWave sw = waves[i];
						waves.RemoveAt(i);
						//return sw;
					}
				}
			}
		}
		public System.Collections.Generic.List<SineWave> waves = new List<SineWave>();
		public override bool CanRead
		{
			get { return true; }
		}
		public override bool CanSeek
		{
			get { return false; }
		}
		public override bool CanWrite
		{
			get { return false; }
		}
		public override void Flush()
		{
			//throw new Exception("The method or operation is not implemented.");
		}
		public override long Length
		{
			get { throw new Exception("The method or operation is not implemented."); }
		}
		public override long Position
		{
			get
			{
				throw new Exception("The method or operation is not implemented.");
			}
			set
			{
				throw new Exception("The method or operation is not implemented.");
			}
		}
		public int sampleRate = 44100;
		public override unsafe int Read(byte[] buffer, int offset, int count)
		{
			//throw new Exception("The method or operation is not implemented.");
			lock (waves)
			{
				double pi = Math.PI * 2;
				int length = count / 2;
				fixed (byte* tmp = buffer)
				{
					short* data = (short*)(tmp + offset);
					int i;
					double pos;
					double freq;
					int ii;
					int w = this.waves.Count;
					for (i = 0; i < length; i++)
					{
						data[i] = 0;
					}
					for (ii = 0; ii < w; ii++)
					{
						SineWave sw = this.waves[ii];
						pos = sw.pos;
						freq = sw.freq;
						for (i = 0; i < length; i++)
						{
							data[i] += (short)(Math.Sin(pos) * short.MaxValue / 10);
							pos += (double)freq / sampleRate * pi;
							while (pos > pi)
								pos -= pi;
						}
						sw.pos = pos;
						this.waves[ii] = sw;
					}
				}
			}
			return count;
		}
		public override long Seek(long offset, System.IO.SeekOrigin origin)
		{
			throw new Exception("The method or operation is not implemented.");
		}
		public override void SetLength(long value)
		{
			throw new Exception("The method or operation is not implemented.");
		}
		public override void Write(byte[] buffer, int offset, int count)
		{
			throw new Exception("The method or operation is not implemented.");
		}
	}
}
