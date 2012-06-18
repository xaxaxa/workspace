using System;
using System.Collections.Generic;
using System.Text;

//windows
namespace wave_lib2
{
	public unsafe class WaveOut
	{
		public uint _o;
		public delegate uint wave_callback (byte* buf,uint length);

		private wave_callback callback;
		private wave_callback callback2;

		public WaveOut ()
		{
			_o = create_output2 ();
		}

		public bool _open = false;

		public void Open ()
		{
			open_output2 (_o);
			_open = true;
		}

		public void Close ()
		{
			close_output2 (_o);
			_open = false;
		}

		public void Start ()
		{
			/*if (open)
			{
				start_output2(_o);
			}
			else
			{
				open_output2(_o);
				open = true;
			}*/
			start_output2 (_o);
		}

		public void Stop ()
		{
			if (_open)
				stop_output2 (_o);
		}

		public ushort ch = 2;

		public void InitFormat (ushort channels, uint sampleRate, ushort bitsPerSample)
		{
			ch = channels;
			init_format2 (_o, channels, sampleRate, bitsPerSample);
		}

		public wave_callback Callback {
			get {
				return callback;
			}
			set {
				callback = value;
				set_callback2 (_o, value);
			}
		}

		public wave_callback event_onPlayBuffer {
			get {
				return callback2;
			}
			set {
				callback2 = value;
				set_event_onPlayBuffer (_o, value);
			}
		}

		private short[] tmp_b;

		private unsafe uint _onPlayBuffer (byte* _buf, uint length)
		{
			short* buf = (short*)_buf;
			length = length >> 1;
			if (tmp_b == null || tmp_b.Length != (int)length)
				tmp_b = new short[length];
			fixed (short* tmp2 = tmp_b) {
				short* __src = (short*)buf;
				short* __dst = (short*)tmp2;
				int i;
				uint l = length;
				for (i = 0; i < l; i++) {
					*
					(__dst + i) = *(__src + i);
				}

			}
			OnPlayBuffer (tmp_b);
			return 0;
		}
		public delegate void OnPlayBuffer_d (short[] data);

		public event OnPlayBuffer_d OnPlayBuffer;

		public void init_onPlayBuffer ()
		{
			event_onPlayBuffer = _onPlayBuffer;
		}

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?test1@@YAXXZ")]
		public static extern void __test1 ();

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?create_output@@YAIXZ")]
		public static extern uint create_output ();

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?open_output@@YAXII@Z")]
		public static extern void open_output (uint _o, wave_callback callback);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?stop_output@@YAXI@Z")]
		public static extern void stop_output (uint _o);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?start_output@@YAXI@Z")]
		public static extern void start_output (uint _o);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?set_callback@@YAXII@Z")]
		public static extern void set_callback (uint _o, wave_callback callback);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?create_output2@@YAIXZ")]
		public static extern uint create_output2 ();

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?open_output2@@YAXI@Z")]
		public static extern void open_output2 (uint _o);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?close_output2@@YAXI@Z")]
		public static extern void close_output2 (uint _o);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?stop_output2@@YAXI@Z")]
		public static extern void stop_output2 (uint _o);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?start_output2@@YAXI@Z")]
		public static extern void start_output2 (uint _o);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?set_callback2@@YAXII@Z")]
		public static extern void set_callback2 (uint _o, wave_callback callback);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?set_event_onPlayBuffer@@YAXII@Z")]
		public static extern void set_event_onPlayBuffer (uint _o, wave_callback callback);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?init_format2@@YAXIGKG@Z")]
		public static extern void init_format2 (uint _o, ushort channels, uint sampleRate, ushort bitsPerSample);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?create_equalizer@@YAIGKGH@Z")]
		public static extern uint create_equalizer (ushort channels, uint sampleRate, ushort bitsPerSample, int length);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?equalizer_process@@YAXIPAD0@Z")]
		public static extern void equalizer_process (uint equalizer, byte* _in, byte* _out);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?equalizer_get_freq@@YAMIH@Z")]
		public static extern float equalizer_get_freq (uint equalizer, int index);

		[System.Runtime.InteropServices.DllImport("wave_lib.dll", CallingConvention = System.Runtime.InteropServices.CallingConvention.Winapi, EntryPoint = "?equalizer_set_freq@@YAXIHM@Z")]
		public static extern void equalizer_set_freq (uint equalizer, int index, float value);

		public static wave_callback get_stream_callback (System.IO.Stream s)
		{
			stream_cb cb1 = new stream_cb (4096);
			cb1.s = s;
			return cb1.d;
		}
		public class stream_cb
		{
			public System.IO.Stream s;
			byte[] tmp;
			//Exocortex.DSP.ComplexF[] _c;
			public ushort channels = 2;
			public uint sampleRate = 44100;
			public ushort bitsPerSample = 16;
			public IEqualizer _eq;
			double[] _d;

			public static void to_double (double[] dst, short* src, uint l)
			{
				uint i;
				for (i = 0; i < l; i++) {
					dst [i] = (double)(src [i]) / short.MaxValue;
				}
			}

			public static void to_short (short* dst, double[] src, uint l)
			{
				uint i;
				for (i = 0; i < l; i++) {
					dst [i] = (short)(src [i] * 1.0);
				}
			}

			public uint cb (byte* buf, uint length)
			{
				if (s != null) {
					lock (this.d) {
						if (tmp == null || tmp.Length != (length * 2))
							tmp = new byte[length * 2];
						uint __tmp = (uint)s.Read (tmp, 0, (int)length);
						fixed (byte* tmp2 = tmp) {
							//WaveOut.equalizer_process(eq, tmp2, buf);
							short* tmp3 = (short*)(tmp2);
							//if (_c == null || _c.Length != (int)(__tmp / 2)) _c = new Exocortex.DSP.ComplexF[(int)(__tmp / 2)];
							if (_d == null || _d.Length != (int)(__tmp / 2))
								_d = new double[(int)(__tmp / 2)];
							//wave_lib2.VBFFT.to_complex(_c, (short*)(tmp2), (uint)(__tmp / 2));
							/*//wave_lib2.VBFFT.forward(_c);*/
							//wave_lib2.VBFFT.inverse(_c);
							to_double (_d, tmp3, (uint)_d.Length);
							/*wave_lib2.FFT2 f = new FFT2(_d.Length / 2);
							f.transform(_d, false);
							int j;
							double __tmpd = 1.0;
							int __tmpl=_d.Length;
							for (j = 0; j < __tmpl; j++)
							{
								if (j > (__tmpl / ((double)(120.0 / 44100.0) * (double)sampleRate)))
								{
									__tmpd = 0.05;
								}
								else
								{
									__tmpd = 1.0;
								}
								_d[j] *= __tmpd;
								//__tmpd /= 1.01;
							}
							double mult = 1;
							// multiply transforms to get convolution
							for (j = 0; j != _d.Length; j += 2)
							{
								double a = _d[j] - _d[j + 1];
								double b = _d[j] + _d[j + 1];
								_d[j] = a * mult;
								_d[j + 1] = b * mult;
								//mult += 0.001;
							}
							f.transform(_d, true);*/
							FFTFilter.Process (_d, this._eq, this.sampleRate);
							to_short ((short*)buf, _d, (uint)_d.Length);
							/*Exocortex.DSP.Fourier.FFT(_c, Exocortex.DSP.FourierDirection.Forward);
							Exocortex.DSP.Fourier.FFT(_c, Exocortex.DSP.FourierDirection.Forward);
							Exocortex.DSP.Fourier.FFT(_c, Exocortex.DSP.FourierDirection.Forward);*/
							//Exocortex.DSP.Fourier.FFT(_c, Exocortex.DSP.FourierDirection.Backward);
							//wave_lib2.VBFFT.to_short(tmp3, _c, (uint)_c.Length);
							/*Exocortex.DSP.Fourier.FFT(_c, Exocortex.DSP.FourierDirection.Forward);
							//__tmp /= 2;
							//System.Array.Resize<Exocortex.DSP.ComplexF>(ref _c, _c.Length / 2);
							Exocortex.DSP.ComplexF mult = new Exocortex.DSP.ComplexF(1, 0);
							int j;
							// multiply transforms to get convolution
							for (j = 0; j != _c.Length; j += 2)
							{
								Exocortex.DSP.ComplexF a = _c[j] - _c[j + 1];
								Exocortex.DSP.ComplexF b = _c[j] + _c[j + 1];
								_c[j] = a * mult;
								_c[j + 1] = b * mult;
								//mult += 0.001;
							}
							Exocortex.DSP.Fourier.FFT(_c, Exocortex.DSP.FourierDirection.Backward);
							wave_lib2.VBFFT.to_short(tmp3, _c, (uint)_c.Length);*/
							/*byte* __src = (byte*)tmp2;
							byte* __dst = (byte*)buf;
							int i;
							uint l = __tmp;
							for (i = 0; i < l; i++)
							{
								*(__dst + i) = *(__src + i);
							}*/

						}
						//if (__tmp == 0) throw new Exception();
						return __tmp;
					}
				}
				return 0;
			}
			//public delegate uint cb_d(byte* buf, uint length);
			public wave_callback d;
			public uint eq;

			public stream_cb (int buf_size)
			{
				_eq = new lowpass_eq ();
				d = this.cb;
				/*eq = WaveOut.create_equalizer(channels, sampleRate, bitsPerSample, buf_size);
				WaveOut.equalizer_set_freq(eq, 0, 0.5F);
				WaveOut.equalizer_set_freq(eq, 1, 0.5F);
				WaveOut.equalizer_set_freq(eq, 2, 0.5F);
				WaveOut.equalizer_set_freq(eq, 3, 0.5F);
				WaveOut.equalizer_set_freq(eq, 4, 0.5F);
				WaveOut.equalizer_set_freq(eq, 5, 0.2F);
				WaveOut.equalizer_set_freq(eq, 6, 0F);
				WaveOut.equalizer_set_freq(eq, 7, -0.2F);
				WaveOut.equalizer_set_freq(eq, 8, -0.2F);
				WaveOut.equalizer_set_freq(eq, 9, -0.5F);
				WaveOut.equalizer_set_freq(eq, 10, -0.5F);
				WaveOut.equalizer_set_freq(eq, 11, -0.5F);*/
			}
		}

		public class lowpass_eq : IEqualizer
		{
			double IEqualizer.GetPoint (uint freq)
			{
				//return (freq > 400 && freq < 800) ? 0.0 : 1.0;
				if (freq < 400) {
					return 2.0;
				} else if (freq < 800) {
					return 0.0;
				} else if (freq < 10000) {
					return 0.3;
				} else if (freq > 18000 && freq < 20000) {
					return 0.0;
				} else {
					return 0.5;
				}
			}
		}
	}
}

//linux
namespace wave_lib3
{
	//compatibility layer
	public class format_wrapper
	{
		public uint channels = 2;
		public uint srate = 44100;
		public ushort bits = 16;

		public int nAvgBytesPerSec {
			get {
				return (int)(srate * bits / 8 * channels);
			}
		}
	}

	public unsafe class WaveOut
	{
		//public delegate uint wave_callback (byte* buf,uint length);
		public wave_lib2.WaveOut.wave_callback Callback, event_onPlayBuffer;

		public event wave_lib2.WaveOut.OnPlayBuffer_d OnPlayBuffer;
		public event wave_lib.PlayBufferDelegate BeforePlayBuffer;
		public event wave_lib.PlayBufferDelegate AfterPlayBuffer;

		public System.IO.Stream src = null;
		private short[] tmp_b;
		public format_wrapper format = new format_wrapper ();

		private unsafe uint _onPlayBuffer (byte* _buf, uint length)
		{
			short* buf = (short*)_buf;
			length = length * 2;
			if (tmp_b == null || tmp_b.Length != (int)length)
				tmp_b = new short[length];
			fixed (short* tmp2 = tmp_b) {
				short* __src = (short*)buf;
				short* __dst = (short*)tmp2;
				int i;
				uint l = length;
				for (i = 0; i < l; i++) {
					*
					(__dst + i) = *(__src + i);
				}

			}
			if (OnPlayBuffer != null)
				OnPlayBuffer (tmp_b);
			return 0;
		}

		public void init_onPlayBuffer ()
		{
			event_onPlayBuffer = _onPlayBuffer;
		}
		
		public IntPtr pcm;
		public bool _open = false;
		public int bs, buffers;
		System.Threading.Thread th = null;

		public WaveOut (int buffers=0, int buffersize=4096)
		{
			bs = buffersize;
			this.buffers = buffers;
		}

		public void ReallocBuffer (int buffers=0, int buffersize=4096)
		{
			bs = buffersize;
			this.buffers = buffers;
		}

		public void Open ()
		{
			
		}

		public void Close ()
		{
			
		}

		public void Pause ()
		{
			WaveNative.snd_pcm_pause (pcm, 1);
		}

		public void Resume ()
		{
			WaveNative.snd_pcm_pause (pcm, 0);
		}

		public void Play ()
		{
			Start ();
		}

		public void Start ()
		{
			if (_open)
				Stop ();
			_open = true;
			WaveNative.snd_pcm_open (out pcm, "default", WaveNative.snd_pcm_stream_t.SND_PCM_STREAM_PLAYBACK, 0);
			IntPtr param;
			WaveNative.snd_pcm_hw_params_malloc (out param);
			WaveNative.snd_pcm_hw_params_any (pcm, param);
			WaveNative.snd_pcm_hw_params_set_buffer_size (pcm, param, (ulong)bs);
			WaveNative.snd_pcm_hw_params_set_periods (pcm, param, (uint)buffers, 0);
			WaveNative.snd_pcm_hw_params_set_access (pcm, param, WaveNative.snd_pcm_access_t.SND_PCM_ACCESS_RW_INTERLEAVED);
			WaveNative.snd_pcm_format_t fmt;
			switch (format.bits) {
			case 8:
				fmt = WaveNative.snd_pcm_format_t.SND_PCM_FORMAT_S8;
				break;
			case 16:
				fmt = WaveNative.snd_pcm_format_t.SND_PCM_FORMAT_S16_LE;
				break;
			case 32:
				fmt = WaveNative.snd_pcm_format_t.SND_PCM_FORMAT_FLOAT;
				break;
			default:
				fmt = WaveNative.snd_pcm_format_t.SND_PCM_FORMAT_S16_LE;
				break;
			}
			WaveNative.snd_pcm_hw_params_set_format (pcm, param, fmt);
			WaveNative.snd_pcm_hw_params_set_rate_near (pcm, param, ref format.srate, null);
			WaveNative.snd_pcm_hw_params_set_channels (pcm, param, format.channels);
			WaveNative.snd_pcm_hw_params (pcm, param);
			WaveNative.snd_pcm_hw_params_free (param);
			WaveNative.snd_pcm_prepare (pcm);
			th = new System.Threading.Thread (thr1);
			th.Priority = System.Threading.ThreadPriority.Highest;
			th.Start ();
		}

		public void Stop ()
		{
			if (!_open)
				return;
			//WaveNative.snd_pcm_close (pcm);
			_open = false;
			if (th != null) {
				try {
					th.Join (2000);
				} catch {
					
				}
				th = null;
			}
		}

		public void thr1 ()
		{
			byte[] buf = new byte[bs * (format.bits / 8) / format.channels];//new byte[(int)(srate * 0.1 * channels * bits / 8)];//100ms buffer
			wave_lib.WaveBuffer wb = new wave_lib.WaveBuffer (buf, buf.Length);
			fixed(byte* bufp=buf)
			{
				while (_open) {
					if (src == null)
						Callback (bufp, (uint)(buf.Length / (format.bits / 8) / format.channels));
					else {
						int off = 0;
						while (off<buf.Length) {
							int br = src.Read (buf, off, buf.Length - off);
							if (br <= 0) {
								Stop ();
								return;
							}
							off += br;
						}
					}
					if (BeforePlayBuffer != null)
						BeforePlayBuffer (wb);
					WaveNative.snd_pcm_writei (pcm, bufp, (ulong)(buf.Length / (format.bits / 8) / format.channels));
					if (event_onPlayBuffer != null)
						event_onPlayBuffer (bufp, (uint)buf.Length);
					if (AfterPlayBuffer != null)
						AfterPlayBuffer (wb);
				
				}
				WaveNative.snd_pcm_drop (pcm);
			}
		}

		public void SetFormat (int sampleRate, int bits, int channels)
		{
			InitFormat ((uint)channels, (uint)sampleRate, (ushort)bits);
		}

		public void InitFormat (uint channels, uint sampleRate, ushort bitsPerSample)
		{
			format.channels = channels;
			format.srate = sampleRate;
			format.bits = bitsPerSample;
		}
	}
}