using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
namespace wave_lib
{
	#region native
	public enum WaveFormats
	{
		Pcm = 1,
		Float = 3
	}

	[StructLayout(LayoutKind.Sequential)]
	public class WaveFormat
	{
		public short wFormatTag;
		public short nChannels;
		public int nSamplesPerSec;
		public int nAvgBytesPerSec;
		public short nBlockAlign;
		public short wBitsPerSample;
		public short cbSize;

		public WaveFormat(int rate, int bits, int channels)
		{
			wFormatTag = (short)WaveFormats.Pcm;
			nChannels = (short)channels;
			nSamplesPerSec = rate;
			wBitsPerSample = (short)bits;
			cbSize = 0;

			nBlockAlign = (short)(channels * (bits / 8));
			nAvgBytesPerSec = nSamplesPerSec * nBlockAlign;
		}
	}

	public class WaveNative
	{
		// consts
		public const int MMSYSERR_NOERROR = 0; // no error

		public const int MM_WOM_OPEN = 0x3BB;
		public const int MM_WOM_CLOSE = 0x3BC;
		public const int MM_WOM_DONE = 0x3BD;

		public const int MM_WIM_OPEN = 0x3BE;
		public const int MM_WIM_CLOSE = 0x3BF;
		public const int MM_WIM_DATA = 0x3C0;

		public const int CALLBACK_FUNCTION = 0x00030000;    // dwCallback is a FARPROC 

		public const int TIME_MS = 0x0001;  // time in milliseconds 
		public const int TIME_SAMPLES = 0x0002;  // number of wave samples 
		public const int TIME_BYTES = 0x0004;  // current byte offset 

		// callbacks
		public delegate void WaveDelegate(IntPtr hdrvr, int uMsg, int dwUser, ref WaveHdr wavhdr, int dwParam2);

		// structs 

		[StructLayout(LayoutKind.Sequential)]
		public struct WaveHdr
		{
			public IntPtr lpData; // pointer to locked data buffer
			public int dwBufferLength; // length of data buffer
			public int dwBytesRecorded; // used for input only
			public int dwUser; // for client's use
			public int dwFlags; // assorted flags (see defines)
			public int dwLoops; // loop control counter
			public IntPtr lpNext; // PWaveHdr, reserved for driver
			public int reserved; // reserved for driver
		}

		private const string mmdll = "winmm.dll";

		// WaveOut calls
		[DllImport(mmdll)]
		public static extern int waveOutGetNumDevs();
		[DllImport(mmdll)]
		public static extern int waveOutPrepareHeader(IntPtr hWaveOut, ref WaveHdr lpWaveOutHdr, int uSize);
		[DllImport(mmdll)]
		public static extern int waveOutUnprepareHeader(IntPtr hWaveOut, ref WaveHdr lpWaveOutHdr, int uSize);
		[DllImport(mmdll)]
		public static extern int waveOutWrite(IntPtr hWaveOut, ref WaveHdr lpWaveOutHdr, int uSize);
		[DllImport(mmdll)]
		public static extern int waveOutOpen(out IntPtr hWaveOut, uint uDeviceID, WaveFormat lpFormat, WaveDelegate dwCallback, int dwInstance, int dwFlags);
		[DllImport(mmdll)]
		public static extern int waveOutReset(IntPtr hWaveOut);
		[DllImport(mmdll)]
		public static extern int waveOutClose(IntPtr hWaveOut);
		[DllImport(mmdll)]
		public static extern int waveOutPause(IntPtr hWaveOut);
		[DllImport(mmdll)]
		public static extern int waveOutRestart(IntPtr hWaveOut);
		[DllImport(mmdll)]
		public static extern int waveOutGetPosition(IntPtr hWaveOut, out int lpInfo, int uSize);
		[DllImport(mmdll)]
		public static extern int waveOutSetVolume(IntPtr hWaveOut, int dwVolume);
		[DllImport(mmdll)]
		public static extern int waveOutGetVolume(IntPtr hWaveOut, out int dwVolume);

		// WaveIn calls
		[DllImport(mmdll)]
		public static extern int waveInGetNumDevs();
		[DllImport(mmdll)]
		public static extern int waveInAddBuffer(IntPtr hwi, ref WaveHdr pwh, int cbwh);
		[DllImport(mmdll)]
		public static extern int waveInClose(IntPtr hwi);
		[DllImport(mmdll)]
		public static extern int waveInOpen(out IntPtr phwi, int uDeviceID, WaveFormat lpFormat, WaveDelegate dwCallback, int dwInstance, int dwFlags);
		[DllImport(mmdll)]
		public static extern int waveInPrepareHeader(IntPtr hWaveIn, ref WaveHdr lpWaveInHdr, int uSize);
		[DllImport(mmdll)]
		public static extern int waveInUnprepareHeader(IntPtr hWaveIn, ref WaveHdr lpWaveInHdr, int uSize);
		[DllImport(mmdll)]
		public static extern int waveInReset(IntPtr hwi);
		[DllImport(mmdll)]
		public static extern int waveInStart(IntPtr hwi);
		[DllImport(mmdll)]
		public static extern int waveInStop(IntPtr hwi);
	}

	#endregion
	public class WaveBuffer : IDisposable
	{
		public WaveBuffer prev, next;
		public byte[] data;
		public int size;
		public GCHandle data_gch;
		//public GCHandle this_gch;
		public WaveBuffer (int size, WaveBuffer prev)
		{
			this.header = new WaveNative.WaveHdr ();
			this.prev = prev;
			if (prev != null)
				prev.next = this;
			data = new byte[size];
			data_gch = GCHandle.Alloc (data, GCHandleType.Pinned);
			//this_gch = GCHandle.Alloc(this, GCHandleType.Pinned);
			this.size = size;
		}
		public WaveBuffer (byte[] data, int size)
		{
			this.data = data;
			this.size = size;
		}
		bool disposed = false;
		public bool hasData = false;
		public bool playing = false;
		private IntPtr hwaveout;
		public void Dispose()
		{
			if (disposed) return;
			WaveNative.waveOutUnprepareHeader(hwaveout, ref header, Marshal.SizeOf(header));
			data_gch.Free();
			//this_gch.Free();
			data = null;
			GC.SuppressFinalize(this);
			disposed = true;
		}
		public WaveNative.WaveHdr header;
		public int index;
		public void Play(IntPtr hwaveout)
		{
			this.playing = true;
			WaveNative.waveOutUnprepareHeader(this.hwaveout, ref header, Marshal.SizeOf(header));
			header.lpData = data_gch.AddrOfPinnedObject();
			header.dwBufferLength = size;
			header.dwBytesRecorded = 0;
			header.dwFlags = 0;
			header.dwLoops = 0;
			header.dwUser = index;
			WaveException.CheckError(WaveNative.waveOutPrepareHeader(hwaveout, ref header, Marshal.SizeOf(header)));
			WaveException.CheckError(WaveNative.waveOutWrite(hwaveout, ref header, Marshal.SizeOf(header)));
			this.hwaveout = hwaveout;
		}
		public void Record(IntPtr hwaveout)
		{
			this.playing = true;
			WaveNative.waveInUnprepareHeader(this.hwaveout, ref header, Marshal.SizeOf(header));
			header.lpData = data_gch.AddrOfPinnedObject();
			header.dwBufferLength = size;
			header.dwBytesRecorded = 0;
			header.dwFlags = 0;
			header.dwLoops = 0;
			header.dwUser = index;
			WaveException.CheckError(WaveNative.waveInPrepareHeader(hwaveout, ref header, Marshal.SizeOf(header)));
			WaveException.CheckError(WaveNative.waveInAddBuffer(hwaveout, ref header, Marshal.SizeOf(header)));
			this.hwaveout = hwaveout;
		}
		/*~WaveBuffer()
		{
			this.Dispose();
		}*/
		public void to_array_16_1(short[] a)
		{
			int l = a.Length < (data.Length / 2) ? a.Length : (data.Length / 2);
			unsafe
			{
				fixed (byte* b = data)
				{
					short* s = (short*)b;
					int i;
					for (i = 0; i < l; i++)
					{
						a[i] = s[i];
					}
				}
			}
		}
		public void to_array_16_2(short[] a)
		{
			int l = a.Length < (data.Length / 4) ? a.Length : (data.Length / 4);
			l *= 2;
			l -= 1;
			unsafe
			{
				fixed (byte* b = data)
				{
					short* s = (short*)b;
					int i;
					for (i = 0; i < l; i += 2)
					{
						a[i / 2] = (short)(s[i] / 2 + s[i + 1] / 2);
					}
				}
			}
		}
		public static void byte_to_short(byte[] _b, short[] _s, int length)
		{
			int b_l = Math.Min(_b.Length, length);
			if (b_l < 0) b_l = _b.Length;
			if (b_l == 0) return;
			int l = _s.Length < (b_l / 2) ? _s.Length : (b_l / 2);
			unsafe
			{
				fixed (byte* b = _b)
				{
					short* s = (short*)b;
					int i;
					for (i = 0; i < l; i++)
					{
						_s[i] = s[i];
					}
				}
			}
		}
		public static void byte_to_short2(byte[] _b, short[] _s, int length)
		{
			int b_l = Math.Min(_b.Length, length);
			if (b_l < 0) b_l = _b.Length;
			if (b_l == 0) return;
			int l = _s.Length < (b_l / 4) ? _s.Length : (b_l / 4);
			l *= 2;
			l -= 1;
			unsafe
			{
				fixed (byte* b = _b)
				{
					short* s = (short*)b;
					int i;
					for (i = 0; i < l; i += 2)
					{
						_s[i / 2] = (short)(s[i] / 2 + s[i + 1] / 2);
					}
				}
			}
		}
	}
	public class WaveBufferList : IDisposable
	{
		public WaveBuffer first;
		public WaveBuffer read;
		public WaveBuffer write;
		public WaveBuffer last;
		public WaveBuffer[] array;
		public System.Threading.EventWaitHandle wh_read;
		public System.Threading.EventWaitHandle wh_write;
		public bool disposed = false;
		public void Dispose()
		{
			if (disposed) return;
			WaveBuffer tmp = first;
			do
			{
				tmp = tmp.next;
				tmp.Dispose();
			}
			while (tmp != first);
			wh_read.Close();
			wh_write.Close();
			disposed = true;
			GC.SuppressFinalize(this);
		}
		~WaveBufferList()
		{
			this.Dispose();
		}
		public WaveBufferList(int length, int size)
		{
			int i;
			wh_read = new System.Threading.EventWaitHandle(false, System.Threading.EventResetMode.ManualReset);
			wh_write = new System.Threading.EventWaitHandle(true, System.Threading.EventResetMode.ManualReset);
			WaveBuffer prev = null;
			array = new WaveBuffer[length];
			for (i = 0; i < length; i++)
			{
				WaveBuffer tmp = new WaveBuffer(size, prev);
				prev = tmp;
				if (i == 0)
				{
					first = tmp;
					read = tmp;
					write = tmp;
				}
				if (i == length - 1)
				{
					last = tmp;
					last.next = first;
					first.prev = last;
				}
				tmp.index = i;
				array[i] = tmp;
			}
		}
		public WaveBuffer beginRead()
		{
			wh_read.WaitOne(System.Threading.Timeout.Infinite, true);
			return read;
		}
		public void endRead()
		{
			lock (this)
			{
				read.hasData = false;
				read = read.next;
				if (!read.hasData)
					wh_read.Reset();
				wh_write.Set();
			}
		}
		public void endReadAsync()
		{
			lock (this)
			{
				read = read.next;
				if (!read.hasData)
					wh_read.Reset();
			}
		}
		public void endRead2(WaveBuffer buf)
		{
			lock (this)
			{
				buf.hasData = false;
				buf.playing = false;
				wh_write.Set();
			}
		}
		public WaveBuffer beginWrite()
		{
			wh_write.WaitOne(System.Threading.Timeout.Infinite, true);
			return write;
		}
		public void endWrite()
		{
			lock (this)
			{
				write.hasData = true;
				write = write.next;
				if (write.hasData)
					wh_write.Reset();
				wh_read.Set();
			}
		}
		public void endWriteAsync()
		{
			lock (this)
			{
				//write.hasData = true;
				write = write.next;
				if (write.hasData)
					wh_write.Reset();
				//wh_read.Set();
			}
		}
		public void endWrite2(WaveBuffer buf)
		{
			lock (this)
			{
				buf.hasData = true;
				buf.playing = false;
				wh_read.Set();
			}
		}
		public void discard()
		{
			lock (this)
			{
				WaveBuffer tmp = write;
				do
				{
					if (tmp.prev.playing)
					{
						break;
					}
					tmp = tmp.prev;
					tmp.hasData = false;
				}
				while (tmp != read);
				write = tmp;
			}
		}
	}
	public class WaveException : Exception
	{
		public WaveException(int error_code)
			: base(error_code.ToString()) { }
		public WaveException(string msg)
			: base(msg) { }
		public static void CheckError(int error_code)
		{
			if (error_code != WaveNative.MMSYSERR_NOERROR)
				throw new WaveException(error_code);
		}
	}
	public delegate void PlayBufferDelegate(WaveBuffer data);
	public class WaveOut
	{
		public WaveOut(int buffers, int bufferSize)
		{
			wd = WaveCallback;
			format = new WaveFormat(44100, 16, 2);
			buf = new WaveBufferList(buffers, bufferSize);
		}
		public void ReallocBuffer(int buffers, int bufferSize)
		{
			if (buf.array.Length == buffers && buf.array[0].data.Length == bufferSize) return;
			lock (buf)
			{
				buf.Dispose();
				buf = new WaveBufferList(buffers, bufferSize);
			}
		}
		public WaveBufferList buf;
		private WaveNative.WaveDelegate wd;
		public WaveFormat format;
		public IntPtr handle;
		public bool isOpen = false;
		public bool isPlaying = false;
		public System.IO.Stream src;
		public void SetFormat(int sampleRate, int bits, int channels)
		{
			format = new WaveFormat(sampleRate, bits, channels);
		}
		private int buffersPlaying = 0;
		private System.Threading.EventWaitHandle ev1 = new System.Threading.AutoResetEvent(false);
		private System.Threading.EventWaitHandle ev2 = new System.Threading.AutoResetEvent(false);
		private System.Threading.EventWaitHandle ev3 = new System.Threading.AutoResetEvent(false);
		System.Threading.Thread th_play;
		System.Threading.Thread th_read;
		public void Open()
		{
			if (isOpen)
				throw new WaveException("already open");
			WaveException.CheckError(WaveNative.waveOutOpen(out handle, 0xFFFFFFFF, format, wd, 0, WaveNative.CALLBACK_FUNCTION));
			ev1.WaitOne(System.Threading.Timeout.Infinite, true);
			isOpen = true;
			System.Threading.Thread th = new System.Threading.Thread(PlayThread);
			th_play = th;
			th.Priority = System.Threading.ThreadPriority.Highest;
			th.Start();
			/*System.Threading.Thread th3 = new System.Threading.Thread(CallbackThread);
			th3.Priority = System.Threading.ThreadPriority.Lowest;
			th3.Start();*/
		}
		public void Close()
		{
			isOpen = false;
			//isPlaying = false;
			WaveNative.waveOutClose(handle);
			try { th_play.Abort(); }
			catch { }
			//ev1.WaitOne(System.Threading.Timeout.Infinite);
		}
		public void Play()
		{
			if (!isOpen)
				throw new WaveException("wave out device not open!");
			if (isPlaying)
			{
				WaveNative.waveOutRestart(handle);
				return;
			}
			if (src == null)
				throw new WaveException("the src field is null!");
			//start threads
			isPlaying = true;
			System.Threading.Thread th2 = new System.Threading.Thread(ReadThread);
			th_read = th2;
			th2.Priority = System.Threading.ThreadPriority.Highest;
			th2.Start();
		}
		public void Pause()
		{
			if (!isPlaying) return;
			WaveNative.waveOutPause(this.handle);
		}
		public void Resume()
		{
			if (!isPlaying) return;
			WaveNative.waveOutRestart(handle);
		}
		public void Stop()
		{
			isPlaying = false;
			this.buf.discard();
			WaveNative.waveOutReset(handle);
			try { th_read.Abort(); }
			catch { }
		}
		private void PlayThread()
		{
			while (isOpen)
			{
				WaveBuffer buf = this.buf.beginRead();
				if (BeforePlayBuffer != null) BeforePlayBuffer(buf);
				buffersPlaying++;
				buf.Play(this.handle);
				this.buf.endReadAsync();
				while (buffersPlaying > this.buf.array.Length - 1)
				{
					ev1.WaitOne(System.Threading.Timeout.Infinite, true);
				}
				//ev1.WaitOne(System.Threading.Timeout.Infinite);
			}
		}
		private void ReadThread()
		{
			while (isPlaying)
			{
				WaveBuffer b = buf.beginWrite();
				if (!isPlaying) return;
				b.size = this.src.Read(b.data, 0, b.data.Length);
				if (b.size <= 0)
				{
					isPlaying = false;
					return;
				}
				buf.endWrite();
			}
		}
		/*private void CallbackThread()
		{
			while (isOpen)
			{
				ev2.WaitOne(System.Threading.Timeout.Infinite, true);
				if (AfterPlayBuffer != null) AfterPlayBuffer(tmp_b);
				ev3.Set();
			}
		}*/
		//private WaveBuffer tmp_b;
		//the .net runtime will generate a method stub when the method is marshalled,so the method does not have to be in the global scope.
		public void WaveCallback(IntPtr hdrvr, int uMsg, int dwUser, ref WaveNative.WaveHdr wavhdr, int dwParam2)
		{
			if (uMsg != WaveNative.MM_WOM_DONE)
			{
				ev1.Set();
				return;
			}
			//tmp_b = buf.array[wavhdr.dwUser];
			buf.endRead2(buf.array[wavhdr.dwUser]);
			//ev2.Set();
			//ev3.WaitOne(System.Threading.Timeout.Infinite);
			if (AfterPlayBuffer != null) AfterPlayBuffer(buf.array[wavhdr.dwUser]);
			buffersPlaying--;
			ev1.Set();
			//buf.array[wavhdr.dwUser].hasData = false;
		}
		public event PlayBufferDelegate BeforePlayBuffer;
		public event PlayBufferDelegate AfterPlayBuffer;
	}
	public class WaveIn
	{
		public WaveIn(int buffers, int bufferSize)
		{
			wd = WaveCallback;
			format = new WaveFormat(44100, 16, 2);
			buf = new WaveBufferList(buffers, bufferSize);
		}
		public void ReallocBuffer(int buffers, int bufferSize)
		{
			if (buf.array.Length == buffers && buf.array[0].data.Length == bufferSize) return;
			lock (buf)
			{
				buf.Dispose();
				buf = new WaveBufferList(buffers, bufferSize);
			}
		}
		public WaveBufferList buf;
		private WaveNative.WaveDelegate wd;
		public WaveFormat format;
		public IntPtr handle;
		public bool isOpen = false;
		public bool isPlaying = false;
		public System.IO.Stream src;
		public void SetFormat(int sampleRate, int bits, int channels)
		{
			format = new WaveFormat(sampleRate, bits, channels);
		}
		private int buffersPlaying = 0;
		private System.Threading.EventWaitHandle ev1 = new System.Threading.AutoResetEvent(false);
		private System.Threading.EventWaitHandle ev2 = new System.Threading.AutoResetEvent(false);
		private System.Threading.EventWaitHandle ev3 = new System.Threading.AutoResetEvent(false);
		System.Threading.Thread th_play;
		System.Threading.Thread th_read;
		public void Open()
		{
			if (isOpen)
				throw new WaveException("already open");
			WaveException.CheckError(WaveNative.waveInOpen(out handle, -1, format, wd, 0, WaveNative.CALLBACK_FUNCTION));
			ev1.WaitOne(System.Threading.Timeout.Infinite, true);
			isOpen = true;
			System.Threading.Thread th = new System.Threading.Thread(WriteThread);
			th_play = th;
			th.Priority = System.Threading.ThreadPriority.Highest;
			th.Start();
			/*System.Threading.Thread th3 = new System.Threading.Thread(CallbackThread);
			th3.Priority = System.Threading.ThreadPriority.Lowest;
			th3.Start();*/
		}
		public void Close()
		{
			isOpen = false;
			//isPlaying = false;
			WaveNative.waveInClose(handle);
			try { th_play.Abort(); }
			catch { }
			//ev1.WaitOne(System.Threading.Timeout.Infinite);
		}
		public void Record()
		{
			if (!isOpen)
				throw new WaveException("wave out device not open!");
			if (isPlaying)
			{
				//WaveNative.waveInRestart(handle);
				return;
			}
			if (src == null)
				throw new WaveException("the src field is null!");
			//start threads
			isPlaying = true;
			System.Threading.Thread th2 = new System.Threading.Thread(PlayThread);
			th_read = th2;
			th2.Priority = System.Threading.ThreadPriority.Highest;
			th2.Start();
		}
		public void Pause()
		{
			if (!isPlaying) return;
			//WaveNative.waveOutPause(this.handle);
			wave_lib.WaveNative.waveInStop(this.handle);
		}
		public void Resume()
		{
			if (!isPlaying) return;
			//WaveNative.waveOutRestart(handle);
			wave_lib.WaveNative.waveInStart(this.handle);
		}
		public void Stop()
		{
			isPlaying = false;
			this.buf.discard();
			WaveNative.waveInReset(handle);
			try { th_read.Abort(); }
			catch { }
		}
		private void PlayThread()
		{
			wave_lib.WaveException.CheckError(wave_lib.WaveNative.waveInStart(this.handle));
			while (isPlaying)
			{
				WaveBuffer buf = this.buf.beginWrite();
				//if (BeforePlayBuffer != null) BeforePlayBuffer(buf);
				buffersPlaying++;
				buf.Record(this.handle);
				this.buf.endWriteAsync();
				while (buffersPlaying > this.buf.array.Length - 1)
				{
					ev1.WaitOne(System.Threading.Timeout.Infinite, true);
				}
				//ev1.WaitOne(System.Threading.Timeout.Infinite);
			}
			wave_lib.WaveException.CheckError(wave_lib.WaveNative.waveInStop(this.handle));
		}
		private void WriteThread()
		{
			while (isOpen)
			{
				WaveBuffer b = buf.beginRead();
				if (!isPlaying) return;
				this.src.Write(b.data, 0, b.data.Length);
				buf.endRead();
			}
		}
		/*private void CallbackThread()
		{
			while (isOpen)
			{
				ev2.WaitOne(System.Threading.Timeout.Infinite, true);
				if (AfterPlayBuffer != null) AfterPlayBuffer(tmp_b);
				ev3.Set();
			}
		}*/
		//private WaveBuffer tmp_b;
		//the .net runtime will generate a method stub when the method is marshalled,so the method does not have to be in the global scope.
		public void WaveCallback(IntPtr hdrvr, int uMsg, int dwUser, ref WaveNative.WaveHdr wavhdr, int dwParam2)
		{
			if (uMsg != WaveNative.MM_WIM_DATA)
			{
				ev1.Set();
				return;
			}
			//tmp_b = buf.array[wavhdr.dwUser];
			buf.endWrite2(buf.array[wavhdr.dwUser]);
			//ev2.Set();
			//ev3.WaitOne(System.Threading.Timeout.Infinite);
			if (AfterRecordBuffer != null) AfterRecordBuffer(buf.array[wavhdr.dwUser]);
			buffersPlaying--;
			ev1.Set();
			//buf.array[wavhdr.dwUser].hasData = false;
		}
		//public delegate void PlayBufferDelegate(WaveBuffer data);
		//public event PlayBufferDelegate BeforePlayBuffer;
		public event PlayBufferDelegate AfterRecordBuffer;
	}
	public class ReversedStream16 : System.IO.Stream
	{
		public System.IO.Stream s;
		public bool reversePosition = false;
		public bool reverseRead = true;
		public bool restorePosition = false;
		public override bool CanRead
		{
			get
			{
				return s.CanRead;
			}
		}

		public override bool CanSeek
		{
			get
			{
				return s.CanSeek;
			}
		}

		public override bool CanWrite
		{
			get
			{
				return s.CanWrite;
			}
		}

		public override void Flush()
		{
			s.Flush();
		}

		public override long Length
		{
			get
			{
				return s.Length;
			}
		}

		public override long Position
		{
			get
			{
				//throw new Exception("The method or operation is not implemented.");
				if (!CanRead) return 0;
				return reversePosition ? (s.Length - s.Position - 1) : (s.Position);
			}
			set
			{
				if (!CanRead) return;
				s.Position = reversePosition ? (s.Length - value - 1) : (value);
			}
		}

		public override int Read(byte[] buffer, int offset, int count)
		{
			long _pos = 0;
			if (CanSeek) _pos = s.Position;
			if (reverseRead)
			{
				long pos = s.Position - count;
				if (pos < 0)
				{
					count += (int)pos;
					if (count <= 0) return 0;
					pos = 0;
				}
				s.Position = pos;
				s.Read(buffer, 0, count);
				s.Position = pos;

				unsafe
				{
					fixed (byte* b = buffer)
					{
						short* _s = (short*)b;
						int i;
						int l = (int)(count / 2);
						int l2 = (int)(l / 2);
						for (i = 0; i < l2; i++)
						{
							short tmp = _s[i];
							_s[i] = _s[l - i - 1];
							_s[l - i - 1] = tmp;
						}
					}
				}
				if (restorePosition) s.Position = _pos;
				return count;
			}
			else
			{
				int i = s.Read(buffer, offset, count);
				if (restorePosition) s.Position = _pos;
				return i;
			}
		}

		public override long Seek(long offset, System.IO.SeekOrigin origin)
		{
			if (reversePosition)
			{
				switch (origin)
				{
					case (System.IO.SeekOrigin.Begin):
						offset = s.Length - offset - 1;
						break;
					case (System.IO.SeekOrigin.Current):
						offset += s.Position;
						offset = s.Length - offset - 1;
						offset -= s.Position;
						break;
					case (System.IO.SeekOrigin.End):
						offset += s.Length - 1;
						offset = s.Length - offset - 1;
						offset -= s.Length - 1;
						break;
				}
			}
			return s.Seek(offset, origin);
		}

		public override void SetLength(long value)
		{
			s.SetLength(value);
		}

		public override void Write(byte[] buffer, int offset, int count)
		{
			throw new Exception("The method or operation is not implemented.");
		}
	}
	public unsafe class FilterStream : System.IO.Stream
	{
		public System.IO.Stream s;
		public double[] a;
		byte[] tmp;
		public double[] tmp2;
		public int n = 0;
		public FilterStream()
		{
			a = new double[200];
			tmp2 = new double[200];
			int i;
			for (i = 0; i < a.Length; i++)
			{
				a[i] = 0.005;
			}
		}
		public override bool CanRead
		{
			get { return s.CanRead; }
		}

		public override bool CanSeek
		{
			get { return s.CanSeek; }
		}

		public override bool CanWrite
		{
			get { return s.CanWrite; }
		}

		public override void Flush()
		{
			s.Flush();
		}

		public override long Length
		{
			get { return s.Length; }
		}

		public override long Position
		{
			get
			{
				return s.Position;
			}
			set
			{
				s.Position = value;
			}
		}
		void filter(byte[] buffer, int start, int count)
		{
			if (tmp == null || tmp.Length < buffer.Length) tmp = new byte[buffer.Length - 1];
			int i;
			int ii;
			int l = a.Length;
			double _tmp;
			fixed (byte* b = buffer)
			{
				short* s = (short*)(b + start);
				fixed (byte* b2 = tmp)
				{
					short* s2 = (short*)b2;
					int _l = count / 2;
					for (i = start; i < _l; i++)
					{
						_tmp = 0;
						tmp2[n] = s[i];
						for (ii = 0; ii < l; ii++)
						{
							int tmp_i = n - ii;
							if (tmp_i < 0) tmp_i += l;
							_tmp += a[ii] * tmp2[tmp_i];
						}
						s2[i] = (short)_tmp;
						n++;
						if (n >= l) n = 0;
					}
					for (i = 0; i < _l; i++)
					{
						s[i] = s2[i];
					}
				}
			}

		}
		public override int Read(byte[] buffer, int offset, int count)
		{
			int i = s.Read(buffer, offset, count);
			filter(buffer, offset, i);
			return i;
		}

		public override long Seek(long offset, System.IO.SeekOrigin origin)
		{
			return s.Seek(offset, origin);
		}

		public override void SetLength(long value)
		{
			s.SetLength(value);
		}

		public override void Write(byte[] buffer, int offset, int count)
		{

		}
	}
	public unsafe class IIRFilterStream : System.IO.Stream
	{
		public System.IO.Stream s;
		public double[] a;
		public double[] b;
		float[] tmp;
		float[] tmp_2;
		float[] tmp_3;
		public double[] tmp2;
		public double[] tmp3;
		public int n = 0;
		IIRFilter[] filt;
		FilterTemp ftmp = new FilterTemp();
		double[] gain = new double[] { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,0 };
		public IIRFilterStream()
		{
			a = new double[3];
			b = new double[3];
			tmp2 = new double[3];
			tmp3 = new double[3];
			filt = new IIRFilter[11];
			/*a[0] = 4.2691493E-12;
			b[0] = 1;//59.77;
			a[1] = 2.9884047E-11;
			b[1] = -6.916783;
			a[2] = 8.965213E-11;
			b[2] = 20.514696;
			a[3] = 1.4942023E-10;
			b[3] = -33.820972;
			a[4] = 1.4942023E-10;
			b[4] = 33.472633;
			a[5] = 8.965213E-11;
			b[5] = -19.887283;
			a[6] = 2.9884047E-11;
			b[6] = 6.5677757;
			a[7] = 4.2691493E-12;
			b[7] = -0.9300651; 
			       int i;
			for (i = 0; i < tmp2.Length; i++)
			{
				tmp2[i] = 0;
			}
			for (i = 0; i < tmp3.Length; i++)
			{
				tmp3[i] = 0;
			}
			a[0] = 0.007794497   ;
			b[0] = 1.0;
			a[1] = 0.007794497;
			b[1] = -0.9844141;
			b[0] = 1;
			b[1] = -0.9858529556;
			a[0] = 0.0070735222153712487346848093532833;
			a[1] = 1;
			a[0] = 0.0059733191212301876787120761301242;
			a[1] = 0;
			a[2] = -1;
			b[0] = 1;
			b[1] = -1.9886260303;
			b[2] = 0.9886663993;
			
			a[0] = 0.005;
			a[1] = 0;
			b[0] = 1;
			b[1] = -0.995;

			a[0] = 0.003914777;
			a[1] = 0.003914777;
			b[0] = 1;
			b[1] = -0.9921767;

			a[0] = 3.7280596E-9;
			b[0] = 1.0;
			a[1] = 1.4912239E-8;
			b[1] = -3.9589534;
			a[2] = 2.2368358E-8;
			b[2] = 5.8777003;
			a[3] = 1.4912239E-8;
			b[3] = -3.8785305;
			a[4] = 3.7280596E-9;
			b[4] = 0.9597837;*/

			/*a[0] = 1.5221069E-5; b[0] = 1.0;
			a[1] = 0.0; b[1] = -3.9397557;
			a[2] = -3.0442137E-5; b[2] = 5.8718143;
			a[3] = 0.0; b[3] = -3.9228086;
			a[4] = 1.5221069E-5; b[4] = 0.9914155;*/


			/*a[0] = 3.0929523E-13; b[0] = 1.0;
			a[1] = 1.2371809E-12; b[1] = -3.9961007;
			a[2] = 1.8557714E-12; b[2] = 5.9883094;
			a[3] = 1.2371809E-12; b[3] = -3.9883168;
			a[4] = 3.0929523E-13; b[4] = 0.9961081;
			filt[0] = IIRFilter.create(a, b);

			a = new double[5];
			b = new double[5];
			a[0] = 3.619663E-5; b[0] = 1.0;
			a[1] = 0.0; b[1] = -3.9957547;
			a[2] = -7.239326E-5; b[2] = 5.9872866;
			a[3] = 0.0; b[3] = -3.987309;
			a[4] = 3.619663E-5; b[4] = 0.9957771;
			filt[1] = IIRFilter.create(a, b);

			a = new double[5];
			b = new double[5];
			a[0] = 2.2500626E-5; b[0] = 10.0;
			a[1] = 0.0; b[1] = -3.993669;
			a[2] = -4.5001252E-5; b[2] = 5.9811068;
			a[3] = 0.0; b[3] = -3.9812062;
			a[4] = 2.2500626E-5; b[4] = 0.99376833;
			filt[2] = IIRFilter.create(a, b);

			a = new double[5];
			b = new double[5];
			a[0] = 1.9699904E-5; b[0] = 1.0;
			a[1] = 0.0; b[1] = -3.9870906;
			a[2] = -3.939981E-5; b[2] = 5.961669;
			a[3] = 0.0; b[3] = -3.9620638;
			a[4] = 1.9699904E-5; b[4] = 0.9874856;
			filt[3] = IIRFilter.create(a, b);

			a = new double[5];
			b = new double[5];
			a[0] = 7.830125E-5; b[0] = 1.0;
			a[1] = 0.0; b[1] = -3.973555;
			a[2] = -1.566025E-4; b[2] = 5.922255;
			a[3] = 0.0; b[3] = -3.9238286;
			a[4] = 7.830125E-5; b[4] = 0.9751289;
			filt[4] = IIRFilter.create(a, b);

			a = new double[5];
			b = new double[5];
			a[0] = 3.0936152E-4; b[0] = 1.0;
			a[1] = 0.0; b[1] = -3.9446266;
			a[2] = -6.1872305E-4; b[2] = 5.840258;
			a[3] = 0.0; b[3] = -3.8464997;
			a[4] = 3.0936152E-4; b[4] = 0.95087427;
			filt[5] = IIRFilter.create(a, b);

			a = new double[5];
			b = new double[5];
			a[0] = 0.001207407; b[0] = 1.0;
			a[1] = 0.0; b[1] = -3.8795533;
			a[2] = -0.002414814; b[2] = 5.6643395;
			a[3] = 0.0; b[3] = -3.6888516;
			a[4] = 0.001207407; b[4] = 0.904163;
			filt[6] = IIRFilter.create(a, b);

			a = new double[5];
			b = new double[5];
			a[0] = 0.004604006; b[0] = 1.0;
			a[1] = 0.0; b[1] = -3.7221074;
			a[2] = -0.009208012; b[2] = 5.2706084;
			a[3] = 0.0; b[3] = -3.3645282;
			a[4] = 0.004604006; b[4] = 0.8175123;
			filt[7] = IIRFilter.create(a, b);

			a = new double[5];
			b = new double[5];
			a[0] = 0.05720029; b[0] = 1.0;
			a[1] = 0.0; b[1] = -2.2014115;
			a[2] = -0.11440058; b[2] = 2.466104;
			a[3] = 0.0; b[3] = -1.4459431;
			a[4] = 0.05720029; b[4] = 0.44768116;
			filt[8] = IIRFilter.create(a, b);

			a = new double[5];
			b = new double[5];
			a[0] = 0.20271581; b[0] = 1.0;
			a[1] = -0.81086326; b[1] = -1.074228;
			a[2] = 1.2162949; b[2] = 0.85296255;
			a[3] = -0.81086326; b[3] = -0.27399987;
			a[4] = 0.20271581; b[4] = 0.04226247;
			filt[9] = IIRFilter.create(a, b);*/


			a[0] = 1.2175793E-6; b[0] = 1.0;
			a[1] = 2.4351586E-6; b[1] = -1.9968768;
			a[2] = 1.2175793E-6; b[2] = 0.9968818;
			filt[0] = IIRFilter.create(a, b);

			a = new double[3];
			b = new double[3];
			a[0] = 0.0059913695; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.9977131;
			a[2] = -0.0059913695; b[2] = 0.997723;
			/*a[0] = 0.005543993; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.9985566;
			a[2] = -0.005543993; b[2] = 0.9985763;*/
			filt[1] = IIRFilter.create(a, b);

			a = new double[3];
			b = new double[3];
			a[0] = 0.005175295; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.9955531;
			a[2] = -0.005175295; b[2] = 0.99559295;
			/*a[0] = 0.0033451074; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.9977871;
			a[2] = -0.0033451074; b[2] = 0.99786514;*/
			filt[2] = IIRFilter.create(a, b);

			a = new double[3];
			b = new double[3];
			a[0] = 0.0044327457; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.9909768;
			a[2] = -0.0044327457; b[2] = 0.99113464;
			/*a[0] = 0.0068095205; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.992598;
			a[2] = -0.0068095205; b[2] = 0.99290144;*/
			filt[3] = IIRFilter.create(a, b);

			a = new double[3];
			b = new double[3];
			a[0] = 0.008826544; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.9817185;
			a[2] = -0.008826544; b[2] = 0.98234725;
			/*a[0] = 0.0075629745; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.9846437;
			a[2] = -0.0075629745; b[2] = 0.98585296;*/
			filt[4] = IIRFilter.create(a, b);

			a = new double[3];
			b = new double[3];
			a[0] = 0.017500006; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.9625078;
			a[2] = -0.017500006; b[2] = 0.9650006;
			/*a[0] = 0.015019886; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.9671015;
			a[2] = -0.015019886; b[2] = 0.97190326;*/
			filt[5] = IIRFilter.create(a, b);

			a = new double[3];
			b = new double[3];
			a[0] = 0.034408703; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.9213878;
			a[2] = -0.034408703; b[2] = 0.9311841;
			/*a[0] = 0.027746068; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.9256523;
			a[2] = -0.027746068; b[2] = 0.944574;*/
			filt[6] = IIRFilter.create(a, b);

			a = new double[3];
			b = new double[3];
			a[0] = 0.06660816; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.8289579;
			a[2] = -0.06660816; b[2] = 0.86678845;
			/*a[0] = 0.054042596; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.8187103;
			a[2] = -0.054042596; b[2] = 0.89205426;*/
			filt[7] = IIRFilter.create(a, b);

			a = new double[3];
			b = new double[3];
			a[0] = 0.12546748; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.6080322;
			a[2] = -0.12546748; b[2] = 0.74909604;
			/*a[0] = 0.10285119; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.5208068;
			a[2] = -0.10285119; b[2] = 0.79456395;*/
			filt[8] = IIRFilter.create(a, b);

			a = new double[3];
			b = new double[3];
			a[0] = 0.22655986; b[0] = 1.0;
			a[1] = 0.0; b[1] = -1.0579224;
			a[2] = -0.22655986; b[2] = 0.54688233;
			/*a[0] = 0.43160385; b[0] = 1.0;
			a[1] = -0.8632077; b[1] = -0.5087176;
			a[2] = 0.43160385; b[2] = 0.21769765;*/
			filt[9] = IIRFilter.create(a, b);

			a = new double[3];
			b = new double[3];
			a[0] = 0.43160385; b[0] = 1.0;
			a[1] = -0.8632077; b[1] = -0.5087176;
			a[2] = 0.43160385; b[2] = 0.21769765;
			filt[10] = IIRFilter.create(a, b);

		}
		public override bool CanRead
		{
			get { return s.CanRead; }
		}

		public override bool CanSeek
		{
			get { return s.CanSeek; }
		}

		public override bool CanWrite
		{
			get { return s.CanWrite; }
		}

		public override void Flush()
		{
			s.Flush();
		}

		public override long Length
		{
			get { return s.Length; }
		}

		public override long Position
		{
			get
			{
				return s.Position;
			}
			set
			{
				s.Position = value;
			}
		}
		void filter(byte[] buffer, int start, int count)
		{
			if (tmp == null || tmp.Length < count / 2) tmp = new float[count / 2];
			if (tmp_2 == null || tmp_2.Length < count / 2) tmp_2 = new float[count / 2];
			if (tmp_3 == null || tmp_3.Length < count / 2) tmp_3 = new float[count / 2];
			int i;
			int ii;
			//int l = a.Length;
			double _tmp;
			fixed (byte* _b = buffer)
			{
				short* s = (short*)(_b + start);
				int _l = count / 2;
				for (i = 0; i < _l; i++)
				{
					tmp[i] = (float)s[i];
				}
				for (i = 0; i < _l; i++)
				{
					tmp_2[i] = 0;
				}
				for (i = 0; i < filt.Length; i++)
				{
					/*for (ii = 0; ii < _l; ii++)
					{
						tmp_3[ii] = 0;
					}
					filt[i].filter(tmp, 0, tmp.Length, tmp_3, 0.1 *gain[i]);*/
					/*for (ii = 0; ii < 1; ii++)
					{
						filt[i].filter(tmp_3, 0, tmp_3.Length, tmp_3, 1);
					}*/
					filt[i].filter(tmp, 0, tmp.Length, tmp_2, 0.1 * gain[i]);
				}
				//filt[2].filter(tmp, 0, tmp.Length, tmp_2, 1);
				/*for (i = 0; i < _l; i++)
				{
					tmp_2[i] = tmp[i];
				}*/
				for (i = 0; i < _l; i++)
				{
					s[i] = (short)tmp_2[i];
				}
				/*fixed (byte* b2 = tmp)
				{
					short* s2 = (short*)b2;
					for (i = start; i < _l; i++)
					{
						_tmp = 0;
						tmp2[n] = s[i];
						for (ii = 0; ii < l; ii++)
						{
							int tmp_i = n - ii;
							if (tmp_i < 0) tmp_i += l;
							_tmp += a[ii] * tmp2[tmp_i];
						}
						for (ii = 1; ii < l; ii++)
						{
							int tmp_i = n - ii;
							if (tmp_i < 0) tmp_i += l;
							//if (double.IsNaN(tmp3[tmp_i]) || double.IsInfinity(tmp3[tmp_i])) tmp3[tmp_i] = 0;
							_tmp -= b[ii] * tmp3[tmp_i];
						}
						_tmp /= b[0];
						//if (double.IsNaN(_tmp) || double.IsInfinity(_tmp)) _tmp = 0;
						s2[i] = (short)(_tmp / 1);
						tmp3[n] = _tmp;
						n++;
						if (n >= l) n = 0;
					}
					for (i = 0; i < _l; i++)
					{
						s[i] = s2[i];
					}

				}*/
			}

		}
		public override int Read(byte[] buffer, int offset, int count)
		{
			int i = s.Read(buffer, offset, count);
			filter(buffer, offset, i);
			return i;
		}

		public override long Seek(long offset, System.IO.SeekOrigin origin)
		{
			return s.Seek(offset, origin);
		}

		public override void SetLength(long value)
		{
			s.SetLength(value);
		}

		public override void Write(byte[] buffer, int offset, int count)
		{

		}
	}
	public class FilterTemp
	{
		public float[] tmp;
	}
	public unsafe struct FIRFilter
	{
		public double[] a;
		public double[] tmp2;
		public int n;
		public static FIRFilter create(double[] a)
		{
			FIRFilter f;
			f.a = a;
			f.tmp2 = new double[a.Length];
			f.n = 0;
			return f;
		}
		public void filter(float[] buffer, int start, int count, float[] dst, double gain)
		{
			//if (tmp.tmp == null || tmp.tmp.Length < buffer.Length) tmp.tmp = new float[buffer.Length - 1];
			int i;
			int ii;
			int l = a.Length;
			double _tmp;
			fixed (float* b = buffer)
			{
				float* s = (float*)(b + start);
				fixed (float* s2 = dst)
				{
					int _l = count;
					for (i = start; i < _l; i++)
					{
						_tmp = 0;
						tmp2[n] = s[i];
						for (ii = 0; ii < l; ii++)
						{
							int tmp_i = n - ii;
							if (tmp_i < 0) tmp_i += l;
							_tmp += a[ii] * tmp2[tmp_i];
						}
						s2[i] += (float)(_tmp * gain);
						n++;
						if (n >= l) n = 0;
					}
				}
			}

		}
	}
	public unsafe struct IIRFilter
	{
		public double[] a;
		public double[] b;
		public double[] tmp2;
		public double[] tmp3;
		public int n;
		public static IIRFilter create(double[] a, double[] b)
		{
			IIRFilter f;
			if (a.Length != b.Length) throw new Exception("a.length!=b.length");
			f.a = a;
			f.b = b;
			f.tmp2 = new double[a.Length];
			f.tmp3 = new double[a.Length];
			f.n = 0;
			return f;
		}
		public void filter(float[] buffer, int start, int count, float[] dst, double gain)
		{
			//if (tmp.tmp == null || tmp.tmp.Length < buffer.Length) tmp.tmp = new float[buffer.Length - 1];
			int i;
			int ii;
			int l = a.Length;
			double _tmp;
			fixed (float* _b = buffer)
			{
				float* s = _b + start;
				fixed (float* s2 = dst)
				{
					int _l = count;
					for (i = start; i < _l; i++)
					{
						_tmp = 0;
						tmp2[n] = s[i];
						for (ii = 0; ii < l; ii++)
						{
							int tmp_i = n - ii;
							if (tmp_i < 0) tmp_i += l;
							_tmp += a[ii] * tmp2[tmp_i];
						}
						for (ii = 1; ii < l; ii++)
						{
							int tmp_i = n - ii;
							if (tmp_i < 0) tmp_i += l;
							//if (double.IsNaN(tmp3[tmp_i]) || double.IsInfinity(tmp3[tmp_i])) tmp3[tmp_i] = 0;
							_tmp -= b[ii] * tmp3[tmp_i];
						}
						_tmp /= b[0];
						//if (double.IsNaN(_tmp) || double.IsInfinity(_tmp)) _tmp = 0;
						s2[i] += (float)(_tmp * gain);
						tmp3[n] = _tmp;
						n++;
						if (n >= l) n = 0;
					}
				}
			}
		}
	}
}
