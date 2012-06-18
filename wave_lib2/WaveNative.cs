//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  This material may not be duplicated in whole or in part, except for 
//  personal use, without the express written consent of the author. 
//
//  Email:  ianier@hotmail.com
//
//  Copyright (C) 1999-2003 Ianier Munoz. All Rights Reserved.
using System;
using System.Runtime.InteropServices;

namespace wave_lib2
{
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

		public WaveFormat (int rate, int bits, int channels)
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
		public delegate void WaveDelegate (IntPtr hdrvr,int uMsg,int dwUser,ref WaveHdr wavhdr,int dwParam2);

		// structs 

		[StructLayout(LayoutKind.Sequential)] public struct WaveHdr
		{
			public IntPtr lpData; // pointer to locked data buffer
			public int dwBufferLength; // length of data buffer
			public int dwBytesRecorded; // used for input only
			public IntPtr dwUser; // for client's use
			public int dwFlags; // assorted flags (see defines)
			public int dwLoops; // loop control counter
			public IntPtr lpNext; // PWaveHdr, reserved for driver
			public int reserved; // reserved for driver
		}

		private const string mmdll = "winmm.dll";

		// WaveOut calls
		[DllImport(mmdll)]
		public static extern int waveOutGetNumDevs ();

		[DllImport(mmdll)]
		public static extern int waveOutPrepareHeader (IntPtr hWaveOut, ref WaveHdr lpWaveOutHdr, int uSize);

		[DllImport(mmdll)]
		public static extern int waveOutUnprepareHeader (IntPtr hWaveOut, ref WaveHdr lpWaveOutHdr, int uSize);

		[DllImport(mmdll)]
		public static extern int waveOutWrite (IntPtr hWaveOut, ref WaveHdr lpWaveOutHdr, int uSize);

		[DllImport(mmdll)]
		public static extern int waveOutOpen (out IntPtr hWaveOut, int uDeviceID, WaveFormat lpFormat, WaveDelegate dwCallback, int dwInstance, int dwFlags);

		[DllImport(mmdll)]
		public static extern int waveOutReset (IntPtr hWaveOut);

		[DllImport(mmdll)]
		public static extern int waveOutClose (IntPtr hWaveOut);

		[DllImport(mmdll)]
		public static extern int waveOutPause (IntPtr hWaveOut);

		[DllImport(mmdll)]
		public static extern int waveOutRestart (IntPtr hWaveOut);

		[DllImport(mmdll)]
		public static extern int waveOutGetPosition (IntPtr hWaveOut, out int lpInfo, int uSize);

		[DllImport(mmdll)]
		public static extern int waveOutSetVolume (IntPtr hWaveOut, int dwVolume);

		[DllImport(mmdll)]
		public static extern int waveOutGetVolume (IntPtr hWaveOut, out int dwVolume);

		// WaveIn calls
		[DllImport(mmdll)]
		public static extern int waveInGetNumDevs ();

		[DllImport(mmdll)]
		public static extern int waveInAddBuffer (IntPtr hwi, ref WaveHdr pwh, int cbwh);

		[DllImport(mmdll)]
		public static extern int waveInClose (IntPtr hwi);

		[DllImport(mmdll)]
		public static extern int waveInOpen (out IntPtr phwi, int uDeviceID, WaveFormat lpFormat, WaveDelegate dwCallback, int dwInstance, int dwFlags);

		[DllImport(mmdll)]
		public static extern int waveInPrepareHeader (IntPtr hWaveIn, ref WaveHdr lpWaveInHdr, int uSize);

		[DllImport(mmdll)]
		public static extern int waveInUnprepareHeader (IntPtr hWaveIn, ref WaveHdr lpWaveInHdr, int uSize);

		[DllImport(mmdll)]
		public static extern int waveInReset (IntPtr hwi);

		[DllImport(mmdll)]
		public static extern int waveInStart (IntPtr hwi);

		[DllImport(mmdll)]
		public static extern int waveInStop (IntPtr hwi);
	}
}
namespace wave_lib3
{
	public class WaveNative
	{
		private const string alsa_lib = "libasound.so.2";
		public enum snd_pcm_stream_t
		{
			/** Playback stream */
			SND_PCM_STREAM_PLAYBACK = 0,
			/** Capture stream */
			SND_PCM_STREAM_CAPTURE,
			SND_PCM_STREAM_LAST = SND_PCM_STREAM_CAPTURE
		};

		public enum snd_pcm_access_t
		{
			/** mmap access with simple interleaved channels */
			SND_PCM_ACCESS_MMAP_INTERLEAVED = 0,
			/** mmap access with simple non interleaved channels */
			SND_PCM_ACCESS_MMAP_NONINTERLEAVED,
			/** mmap access with complex placement */
			SND_PCM_ACCESS_MMAP_COMPLEX,
			/** snd_pcm_readi/snd_pcm_writei access */
			SND_PCM_ACCESS_RW_INTERLEAVED,
			/** snd_pcm_readn/snd_pcm_writen access */
			SND_PCM_ACCESS_RW_NONINTERLEAVED,
			SND_PCM_ACCESS_LAST = SND_PCM_ACCESS_RW_NONINTERLEAVED
		};

		public enum snd_pcm_format_t
		{
			/** Unknown */
			SND_PCM_FORMAT_UNKNOWN = -1,
			/** Signed 8 bit */
			SND_PCM_FORMAT_S8 = 0,
			/** Unsigned 8 bit */
			SND_PCM_FORMAT_U8,
			/** Signed 16 bit Little Endian */
			SND_PCM_FORMAT_S16_LE,
			/** Signed 16 bit Big Endian */
			SND_PCM_FORMAT_S16_BE,
			/** Unsigned 16 bit Little Endian */
			SND_PCM_FORMAT_U16_LE,
			/** Unsigned 16 bit Big Endian */
			SND_PCM_FORMAT_U16_BE,
			/** Signed 24 bit Little Endian using low three bytes in 32-bit word */
			SND_PCM_FORMAT_S24_LE,
			/** Signed 24 bit Big Endian using low three bytes in 32-bit word */
			SND_PCM_FORMAT_S24_BE,
			/** Unsigned 24 bit Little Endian using low three bytes in 32-bit word */
			SND_PCM_FORMAT_U24_LE,
			/** Unsigned 24 bit Big Endian using low three bytes in 32-bit word */
			SND_PCM_FORMAT_U24_BE,
			/** Signed 32 bit Little Endian */
			SND_PCM_FORMAT_S32_LE,
			/** Signed 32 bit Big Endian */
			SND_PCM_FORMAT_S32_BE,
			/** Unsigned 32 bit Little Endian */
			SND_PCM_FORMAT_U32_LE,
			/** Unsigned 32 bit Big Endian */
			SND_PCM_FORMAT_U32_BE,
			/** Float 32 bit Little Endian, Range -1.0 to 1.0 */
			SND_PCM_FORMAT_FLOAT_LE,
			/** Float 32 bit Big Endian, Range -1.0 to 1.0 */
			SND_PCM_FORMAT_FLOAT_BE,
			/** Float 64 bit Little Endian, Range -1.0 to 1.0 */
			SND_PCM_FORMAT_FLOAT64_LE,
			/** Float 64 bit Big Endian, Range -1.0 to 1.0 */
			SND_PCM_FORMAT_FLOAT64_BE,
			/** IEC-958 Little Endian */
			SND_PCM_FORMAT_IEC958_SUBFRAME_LE,
			/** IEC-958 Big Endian */
			SND_PCM_FORMAT_IEC958_SUBFRAME_BE,
			/** Mu-Law */
			SND_PCM_FORMAT_MU_LAW,
			/** A-Law */
			SND_PCM_FORMAT_A_LAW,
			/** Ima-ADPCM */
			SND_PCM_FORMAT_IMA_ADPCM,
			/** MPEG */
			SND_PCM_FORMAT_MPEG,
			/** GSM */
			SND_PCM_FORMAT_GSM,
			/** Special */
			SND_PCM_FORMAT_SPECIAL = 31,
			/** Signed 24bit Little Endian in 3bytes format */
			SND_PCM_FORMAT_S24_3LE = 32,
			/** Signed 24bit Big Endian in 3bytes format */
			SND_PCM_FORMAT_S24_3BE,
			/** Unsigned 24bit Little Endian in 3bytes format */
			SND_PCM_FORMAT_U24_3LE,
			/** Unsigned 24bit Big Endian in 3bytes format */
			SND_PCM_FORMAT_U24_3BE,
			/** Signed 20bit Little Endian in 3bytes format */
			SND_PCM_FORMAT_S20_3LE,
			/** Signed 20bit Big Endian in 3bytes format */
			SND_PCM_FORMAT_S20_3BE,
			/** Unsigned 20bit Little Endian in 3bytes format */
			SND_PCM_FORMAT_U20_3LE,
			/** Unsigned 20bit Big Endian in 3bytes format */
			SND_PCM_FORMAT_U20_3BE,
			/** Signed 18bit Little Endian in 3bytes format */
			SND_PCM_FORMAT_S18_3LE,
			/** Signed 18bit Big Endian in 3bytes format */
			SND_PCM_FORMAT_S18_3BE,
			/** Unsigned 18bit Little Endian in 3bytes format */
			SND_PCM_FORMAT_U18_3LE,
			/** Unsigned 18bit Big Endian in 3bytes format */
			SND_PCM_FORMAT_U18_3BE,
			SND_PCM_FORMAT_LAST = SND_PCM_FORMAT_U18_3BE,

#if __BYTE_ORDER == __LITTLE_ENDIAN
			/** Signed 16 bit CPU endian */
			SND_PCM_FORMAT_S16 = SND_PCM_FORMAT_S16_LE,
			/** Unsigned 16 bit CPU endian */
			SND_PCM_FORMAT_U16 = SND_PCM_FORMAT_U16_LE,
			/** Signed 24 bit CPU endian */
			SND_PCM_FORMAT_S24 = SND_PCM_FORMAT_S24_LE,
			/** Unsigned 24 bit CPU endian */
			SND_PCM_FORMAT_U24 = SND_PCM_FORMAT_U24_LE,
			/** Signed 32 bit CPU endian */
			SND_PCM_FORMAT_S32 = SND_PCM_FORMAT_S32_LE,
			/** Unsigned 32 bit CPU endian */
			SND_PCM_FORMAT_U32 = SND_PCM_FORMAT_U32_LE,
			/** Float 32 bit CPU endian */
			SND_PCM_FORMAT_FLOAT = SND_PCM_FORMAT_FLOAT_LE,
			/** Float 64 bit CPU endian */
			SND_PCM_FORMAT_FLOAT64 = SND_PCM_FORMAT_FLOAT64_LE,
			/** IEC-958 CPU Endian */
			SND_PCM_FORMAT_IEC958_SUBFRAME = SND_PCM_FORMAT_IEC958_SUBFRAME_LE
#elif __BYTE_ORDER == __BIG_ENDIAN
        /** Signed 16 bit CPU endian */
        SND_PCM_FORMAT_S16 = SND_PCM_FORMAT_S16_BE,
        /** Unsigned 16 bit CPU endian */
        SND_PCM_FORMAT_U16 = SND_PCM_FORMAT_U16_BE,
        /** Signed 24 bit CPU endian */
        SND_PCM_FORMAT_S24 = SND_PCM_FORMAT_S24_BE,
        /** Unsigned 24 bit CPU endian */
        SND_PCM_FORMAT_U24 = SND_PCM_FORMAT_U24_BE,
        /** Signed 32 bit CPU endian */
        SND_PCM_FORMAT_S32 = SND_PCM_FORMAT_S32_BE,
        /** Unsigned 32 bit CPU endian */
        SND_PCM_FORMAT_U32 = SND_PCM_FORMAT_U32_BE,
        /** Float 32 bit CPU endian */
        SND_PCM_FORMAT_FLOAT = SND_PCM_FORMAT_FLOAT_BE,
        /** Float 64 bit CPU endian */
        SND_PCM_FORMAT_FLOAT64 = SND_PCM_FORMAT_FLOAT64_BE,
        /** IEC-958 CPU Endian */
        SND_PCM_FORMAT_IEC958_SUBFRAME = SND_PCM_FORMAT_IEC958_SUBFRAME_BE
#else
#error "Unknown endian"
#endif
		}

		public enum snd_pcm_type_t
		{
			/** Kernel level PCM */
			SND_PCM_TYPE_HW = 0,
			/** Hooked PCM */
			SND_PCM_TYPE_HOOKS,
			/** One or more linked PCM with exclusive access to selected
	            channels */
			SND_PCM_TYPE_MULTI,
			/** File writing plugin */
			SND_PCM_TYPE_FILE,
			/** Null endpoint PCM */
			SND_PCM_TYPE_NULL,
			/** Shared memory client PCM */
			SND_PCM_TYPE_SHM,
			/** INET client PCM (not yet implemented) */
			SND_PCM_TYPE_INET,
			/** Copying plugin */
			SND_PCM_TYPE_COPY,					
			/** Linear format conversion PCM */
			SND_PCM_TYPE_LINEAR,
			/** A-Law format conversion PCM */
			SND_PCM_TYPE_ALAW,
			/** Mu-Law format conversion PCM */
			SND_PCM_TYPE_MULAW,
			/** IMA-ADPCM format conversion PCM */
			SND_PCM_TYPE_ADPCM,
			/** Rate conversion PCM */
			SND_PCM_TYPE_RATE,
			/** Attenuated static route PCM */
			SND_PCM_TYPE_ROUTE,
			/** Format adjusted PCM */
			SND_PCM_TYPE_PLUG,
			/** Sharing PCM */
			SND_PCM_TYPE_SHARE,
			/** Meter plugin */
			SND_PCM_TYPE_METER,
			/** Mixing PCM */
			SND_PCM_TYPE_MIX,
			/** Attenuated dynamic route PCM (not yet implemented) */
			SND_PCM_TYPE_DROUTE,
			/** Loopback server plugin (not yet implemented) */
			SND_PCM_TYPE_LBSERVER,
			/** Linear Integer <-> Linear Float format conversion PCM */
			SND_PCM_TYPE_LINEAR_FLOAT,
			/** LADSPA integration plugin */
			SND_PCM_TYPE_LADSPA,
			/** Direct Mixing plugin */
			SND_PCM_TYPE_DMIX,
			/** Jack Audio Connection Kit plugin */
			SND_PCM_TYPE_JACK,
			/** Direct Snooping plugin */
			SND_PCM_TYPE_DSNOOP,
			/** Direct Sharing plugin */
			SND_PCM_TYPE_DSHARE,
			/** IEC958 subframe plugin */
			SND_PCM_TYPE_IEC958,
			/** Soft volume plugin */
			SND_PCM_TYPE_SOFTVOL,
			/** External I/O plugin */
			SND_PCM_TYPE_IOPLUG,
			/** External filter plugin */
			SND_PCM_TYPE_EXTPLUG,
			/** Mmap-emulation plugin */
			SND_PCM_TYPE_MMAP_EMUL,
			SND_PCM_TYPE_LAST = SND_PCM_TYPE_MMAP_EMUL
		};
		/** PCM state */
		public enum snd_pcm_state_t
		{
			/** Open */
			SND_PCM_STATE_OPEN = 0,
			/** Setup installed */ 
			SND_PCM_STATE_SETUP,
			/** Ready to start */
			SND_PCM_STATE_PREPARED,
			/** Running */
			SND_PCM_STATE_RUNNING,
			/** Stopped: underrun (playback) or overrun (capture) detected */
			SND_PCM_STATE_XRUN,
			/** Draining: running (playback) or stopped (capture) */
			SND_PCM_STATE_DRAINING,
			/** Paused */
			SND_PCM_STATE_PAUSED,
			/** Hardware is suspended */
			SND_PCM_STATE_SUSPENDED,
			/** Hardware is disconnected */
			SND_PCM_STATE_DISCONNECTED,
			SND_PCM_STATE_LAST = SND_PCM_STATE_DISCONNECTED
		};
		[DllImport(alsa_lib)]
		public static extern int snd_pcm_open (out IntPtr pcm, string name, snd_pcm_stream_t stream, Int32 mode=0);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_hw_params_malloc (out IntPtr param);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_hw_params_any (IntPtr pcm, IntPtr param);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_hw_params_set_access (IntPtr pcm, IntPtr param, snd_pcm_access_t _access);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_hw_params_set_format (IntPtr pcm, IntPtr param, snd_pcm_format_t val);

		[DllImport(alsa_lib)]
		public unsafe static extern int snd_pcm_hw_params_set_rate_near (IntPtr pcm, IntPtr param, ref uint val, int* dir);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_hw_params_set_channels (IntPtr pcm, IntPtr param, uint val);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_hw_params_set_buffer_size(IntPtr pcm, IntPtr param, ulong val);
		[DllImport(alsa_lib)]
		public static extern int snd_pcm_hw_params_set_periods(IntPtr pcm, IntPtr param, uint val, int dir);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_hw_params (IntPtr pcm, IntPtr param);

		[DllImport(alsa_lib)]
		public static extern void snd_pcm_hw_params_free (IntPtr param);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_prepare (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_reset (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_status (IntPtr pcm, IntPtr status);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_start (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_drop (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_drain (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_pause (IntPtr pcm, int enable);

		[DllImport(alsa_lib)]
		public static extern snd_pcm_state_t snd_pcm_state (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_hwsync (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_delay (IntPtr pcm, out long delayp);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_resume (IntPtr pcm);
		
		[DllImport(alsa_lib)]
		public static extern int snd_pcm_htimestamp (IntPtr pcm, out long avail, out TimeSpan tstamp);

		[DllImport(alsa_lib)]
		public static extern long snd_pcm_avail (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern long snd_pcm_avail_update (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_avail_delay (IntPtr pcm, out long availp, out long delayp);

		[DllImport(alsa_lib)]
		public static extern long snd_pcm_rewindable (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern long snd_pcm_rewind (IntPtr pcm, long frames);

		[DllImport(alsa_lib)]
		public static extern long snd_pcm_forwardable (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern long snd_pcm_forward (IntPtr pcm, long frames);

		[DllImport(alsa_lib)]
		public unsafe static extern long snd_pcm_writei (IntPtr pcm, void* buffer, ulong size);

		[DllImport(alsa_lib)]
		public unsafe static extern long snd_pcm_readi (IntPtr pcm, void *buffer, ulong size);

		[DllImport(alsa_lib)]
		public unsafe static extern long snd_pcm_writen (IntPtr pcm, void **bufs, ulong size);

		[DllImport(alsa_lib)]
		public unsafe static extern long snd_pcm_readn (IntPtr pcm, void **bufs, ulong size);

		[DllImport(alsa_lib)]
		public static extern int snd_pcm_wait (IntPtr pcm, int timeout);
		
		[DllImport(alsa_lib)]
		public static extern int snd_pcm_close (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern string snd_pcm_name (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern snd_pcm_type_t snd_pcm_type (IntPtr pcm);

		[DllImport(alsa_lib)]
		public static extern snd_pcm_stream_t snd_pcm_stream (IntPtr pcm);

	}
}
