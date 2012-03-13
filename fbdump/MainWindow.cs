using System;
using Gtk;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Net;
using System.IO;
using System.Collections.Generic;

public struct fb_response
{
	public bool is_array;
	public JToken data;
	public string prev_url;
	public string next_url;
}

public partial class MainWindow: Gtk.Window
{
	public const string graph_base = "https://graph.facebook.com/";
	public delegate void object_callback (JToken obj);

	public MainWindow (): base (Gtk.WindowType.Toplevel)
	{
		Build ();
		this.t_saveto.Text = System.IO.Directory.GetCurrentDirectory ();
		r_follownext.Activate ();
		Gdk.Geometry g;
		g.MinHeight=this.HeightRequest;
		g.MaxHeight=this.HeightRequest;
		g.MinWidth=0;
		g.MaxWidth=int.MaxValue;
		this.SetGeometryHints (this,g,Gdk.WindowHints.MinSize|Gdk.WindowHints.MaxSize);
	}
	
	protected void OnDeleteEvent (object sender, DeleteEventArgs a)
	{
		Application.Quit ();
		a.RetVal = true;
	}

	public JToken readjson (Stream s)
	{
		JsonSerializer js = new JsonSerializer ();
		return js.Deserialize<JToken> (new JsonTextReader (new StreamReader (s)));
	}

	public fb_response get_object (string url)
	{
		WebRequest req = WebRequest.Create (url);
		Stream s = req.GetResponse ().GetResponseStream ();
		JToken obj = readjson (s);
		s.Close ();
		fb_response resp = new fb_response ();
		switch (obj.Type) {
		case JTokenType.Array:
			resp.is_array = true;
			resp.data = obj;
			break;
		case JTokenType.Object:
			JObject obj1 = (JObject)obj;
			JToken tmp;
			if (obj1.TryGetValue ("data", out tmp)) {
				resp.is_array = true;
				resp.data = tmp;
			} else {
				resp.is_array = false;
				resp.data = obj;
			}
			if (obj1.TryGetValue ("paging", out tmp)) {
				JObject x = (JObject)tmp;
				JToken tmp1;
				if (x.TryGetValue ("previous", out tmp1))
					resp.prev_url = tmp1.Value<string> ();
				if (x.TryGetValue ("next", out tmp1))
					resp.next_url = tmp1.Value<string> ();
			}
			break;
		default:
			resp.is_array = false;
			resp.data = obj;
			break;
		}
		return resp;
	}
	public enum followmode
	{
		none=0,
		prev=1,
		next=2
	}

	public class fbdump_params
	{
		public string atoken;//, param;
		public SortedSet<string> expand;
		public followmode f;
		public object_callback cb;
		public bool stop_on_existing;
	}
	public void process_fb_obj (JToken obj, fbdump_params p)
	{
		p.cb (obj);
		if (obj.Type == JTokenType.Object) {
			JObject obj1 = (JObject)obj;
			foreach (JProperty pr in obj1.Properties()) {
				if (p.expand.Contains (pr.Name)) {
					dump_fb_object (graph_base + pr.Value.Value<string> (), p);
				}
			}
		}
	}

	public void dump_fb_object (string url, fbdump_params p, bool raw_url=false)
	{
		Gdk.Threads.Enter ();
		try {
			/*Gtk.Label l = new Gtk.Label ("dumping "+url);
			vbox1.Add (l);
			l.Show ();*/
			Console.WriteLine ("dumping "+url);
		} finally {
			Gdk.Threads.Leave ();
		}
		if (!raw_url) {
			string tmp = "access_token=" + p.atoken;
			if (url.IndexOf ('?') < 0)
				url += "?" + tmp;
			else
				url += "&" + tmp;
		}
		fb_response r = get_object (url);
		if (r.is_array) {
			JArray a = (JArray)r.data;
			int i;
			for (i=0; i<a.Count; i++) {
				process_fb_obj (a [i], p);
			}
		} else
			process_fb_obj (r.data, p);
		
		if (p.f == followmode.prev && r.prev_url != null && r.prev_url.Length > 0)
			dump_fb_object (r.prev_url, p, true);
		else if (p.f == followmode.next && r.next_url != null && r.next_url.Length > 0)
			dump_fb_object (r.next_url, p, true);
	}

	string saveto;

	public void cb1 (JToken obj)
	{
		if (obj.Type == JTokenType.Object) {
			JObject obj1 = (JObject)obj;
			System.IO.FileStream fs = new System.IO.FileStream (saveto + "/" + obj1 ["id"].Value<string> (), FileMode.Create);
			JsonWriter w = new JsonTextWriter (new StreamWriter (fs));
			obj.WriteTo (w);
			w.CloseOutput = true;
			w.Close ();
		}
	}
	void enable_start()
	{
		button1.Sensitive = true;
		button2.Sensitive = false;
	}
	void disable_start()
	{
		button1.Sensitive = false;
		button2.Sensitive = true;
	}
	bool running()
	{
		return button2.Sensitive;
	}
	void thr1 ()
	{
		string url=graph_base + t_path.Text.TrimStart ('/');
		Gdk.Threads.Enter ();
		fbdump_params p = new fbdump_params ();
		try {
			p.atoken = t_atoken.Text;
			saveto = t_saveto.Text.TrimEnd ('/', '\\');
			p.stop_on_existing=c_stop.Active;
			if (r_dontfollow.Active)
				p.f = followmode.none;
			else if (r_followprev.Active)
				p.f = followmode.prev;
			else if (r_follownext.Active)
				p.f = followmode.next;
			if (c_expand.Active)
				p.expand = new SortedSet<string> (t_expand.Text.Split (' '));
			else
				p.expand = new SortedSet<string> ();
			if (c_param.Active && t_param.Text.Length > 0)
			{
				string tmp = "&" + t_param.Text;
				if (url.IndexOf ('?') < 0)
					url += "?" + tmp;
				else
					url += "&" + tmp;
			}
			disable_start ();
		} finally {
			Gdk.Threads.Leave ();
		}
		p.cb = cb1;
		dump_fb_object (url, p);
		Gdk.Threads.Enter ();
		try {
			enable_start ();
		} finally {
			Gdk.Threads.Leave ();
		}
	}
	System.Threading.Thread th;
	protected void OnButton1Clicked (object sender, System.EventArgs e)
	{
		th = new System.Threading.Thread (thr1);
		th.Start ();
	}

	protected void OnButton2Clicked (object sender, System.EventArgs e)
	{
		try
		{
			if(th!=null && running ())
			{
				th.Abort ();
			}
		}
		catch{}
		enable_start ();
	}
}
