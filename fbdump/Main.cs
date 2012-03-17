using System;
using Gtk;

namespace fbdump
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			GLib.Thread.Init ();
			Gdk.Threads.Init ();
			Application.Init ();
			MainWindow win = new MainWindow ();
			win.Show ();
			Application.Run ();
		}
	}
}
