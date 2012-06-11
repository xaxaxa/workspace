using System;
using Gtk;
using wave_lib3;

class sine_gen
{
	public double position, freq;	//freq is in periods per sample
	public sine_gen (double freq)
	{
		position = 0;
		this.freq = freq;
	}

	public void generate (short[] buf, int len, double offset=0, double amplitude=1)
	{
		//memset(buf,len*sizeof(*buf),0);
		//return;
		double position_inc = freq * 2 * Math.PI;
		for (int i=0; i<len; i++) {
			buf [i] = (short)(offset + Math.Sin (position) * amplitude);
			position += position_inc;
		}
		while (position>=2*Math.PI)
			position -= 2 * Math.PI;
	}
};

public partial class MainWindow: Gtk.Window
{
	public MainWindow (): base (Gtk.WindowType.Toplevel)
	{
		Build ();
	}
	
	protected void OnDeleteEvent (object sender, DeleteEventArgs a)
	{
		Application.Quit ();
		a.RetVal = true;
	}

	protected unsafe void OnButton1Clicked (object sender, System.EventArgs e)
	{
		IntPtr ptr;
		WaveNative.snd_pcm_open (out ptr, "default", WaveNative.snd_pcm_stream_t.SND_PCM_STREAM_PLAYBACK, 0);
		Console.WriteLine (ptr.ToString ());
		IntPtr param;
		WaveNative.snd_pcm_hw_params_malloc (out param);
		WaveNative.snd_pcm_hw_params_any (ptr, param);
		WaveNative.snd_pcm_hw_params_set_access (ptr, param, WaveNative.snd_pcm_access_t.SND_PCM_ACCESS_RW_INTERLEAVED);
		WaveNative.snd_pcm_hw_params_set_format (ptr, param, WaveNative.snd_pcm_format_t.SND_PCM_FORMAT_S16_LE);
		uint srate = 44100;
		WaveNative.snd_pcm_hw_params_set_rate_near (ptr, param, ref srate, null);
		WaveNative.snd_pcm_hw_params_set_channels (ptr, param, 1);
		WaveNative.snd_pcm_hw_params (ptr, param);
		WaveNative.snd_pcm_hw_params_free (param);
		WaveNative.snd_pcm_prepare (ptr);
		sine_gen sg = new sine_gen (100.0 / 44100);
		short[] buf = new short[4096];
		fixed(short* bufp=buf) {
			while (true) {
				sg.generate (buf, buf.Length, 0, short.MaxValue);
				WaveNative.snd_pcm_writei (ptr, bufp, (ulong)buf.Length);
			}
		}
	}
}
