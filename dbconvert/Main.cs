using System;
using System.Data;
using System.Data.SqlClient;
using Npgsql;
using System.Text;

namespace dbconvert
{
	public struct dbfield
	{
		public string name;
		public enum types
		{
			str,
			int8,
			int16,
			int32,
			int64,
			uint8,
			uint16,
			uint32,
			uint64,
			float32,
			float64,
			float128,
			datetime
		};
		public types type;
		public int len;//only for varchar; <=0 means unlimited
	}

	public struct dbtable
	{
		public string name;
		public dbfield[] fields;
		public int id;
	}

	public struct dbrow
	{
		public object[] fields;
		public int table_id;
	}

	public delegate void table_cb (dbtable t);

	public delegate void row_cb (dbtable t);

	public interface db: IDisposable
	{
		void connect ();

		void close ();

		void get_tables (table_cb cb);

		void get_rows (row_cb cb);

		void set_params (System.Collections.IDictionary dic);
	};

	public class pgsql_db: db
	{
		System.Collections.IDictionary param;
		Npgsql.NpgsqlConnection con;

		public pgsql_db ()
		{
			param = new System.Collections.Generic.Dictionary<string,string> ();
		}

		public void connect ()
		{
			if (con != null)
				return;
			con = new Npgsql.NpgsqlConnection ((string)param ["cs"]);
			try {
				con.Open ();
			} catch {
				con = null;
				throw;
			}
		}

		public void close ()
		{
			if (con != null)
				con.Close ();
			con = null;
		}

		public void Dispose ()
		{
			close ();
		}

		public void get_tables (table_cb cb)
		{
			
		}

		public void get_rows (row_cb cb)
		{
			
		}

		public void set_params (System.Collections.IDictionary dic)
		{
			foreach (object o in dic) {
				param [o] = dic [o];
			}
		}
	}

	public class dbserializer
	{
		public System.IO.Stream s;
		public enum objtypes:byte
		{
			table=1,
			row
		}
		public dbserializer (System.IO.Stream s)
		{
			this.s = s;
		}
		byte[] __tmp_int_buffer;
		public const int version=1;
		public int cur_version=1;
		public void write_header()
		{
			__writeint(version);
		}
		public void read_header()
		{
			cur_version=__readint ();
		}
		unsafe void __writestring (string st)
		{
			byte[] asdf = System.Text.Encoding.UTF8.GetBytes (st);
			if (__tmp_int_buffer == null)
				__tmp_int_buffer = new byte[4];
			byte[] b = __tmp_int_buffer;
			fixed(byte* p=b) {
				int* i = (int*)p;
				*
				i = System.Net.IPAddress.HostToNetworkOrder (asdf.Length);
			}
			s.Write (b, 0, 4);
			s.Write (asdf, 0, asdf.Length);
		}
		unsafe string __readstring ()
		{
			if (__tmp_int_buffer == null)
				__tmp_int_buffer = new byte[4];
			byte[] b = __tmp_int_buffer;
			s.Read (b, 0, 4);
			int l;
			fixed(byte* p=b) {
				int* i = (int*)p;
				l = System.Net.IPAddress.NetworkToHostOrder (*i);
			}
			byte[] asdf = new byte[l];
			int o = 0;
			while (o<l) {
				int tmp = s.Read (asdf, o, l - o);
				if (tmp <= 0)
					throw new Exception ("eof encountered");
				o += tmp;
			}
			return System.Text.Encoding.UTF8.GetString (asdf);
		}

		unsafe void __writeint (int i)
		{
			if (__tmp_int_buffer == null)
				__tmp_int_buffer = new byte[4];
			fixed(byte* b=__tmp_int_buffer) {
				int* p = (int*)b;
				*
				p = System.Net.IPAddress.HostToNetworkOrder (i);
			}
			s.Write (__tmp_int_buffer, 0, 4);
		}

		int __readint ()
		{
			if (__tmp_int_buffer == null)
				__tmp_int_buffer = new byte[4];
			byte[] b = __tmp_int_buffer;
			int o = 0;
			while (o<b.Length) {
				int tmp = s.Read (b, o, b.Length - o);
				if (tmp <= 0)
					throw new Exception ("eof encountered");
				o += tmp;
			}
			return System.Net.IPAddress.NetworkToHostOrder (System.BitConverter.ToInt32 (b, 0));
		}

		public void put_table (dbtable t)
		{
			s.WriteByte ((byte)objtypes.table);
			__writeint (t.id);
			__writestring (t.name);
			__writeint (t.fields.Length);
			for (int i=0; i<t.fields.Length; i++) {
				__writestring (t.fields [i].name);
				__writeint (t.fields[i].len);
			}
		}
		public void write_object(dbfield.types t, object o)
		{
			switch(t)
			{
			case dbfield.types.int8:
				s.WriteByte ((byte)o);
				break;
			case dbfield.types.int16:
				break;
			}
		}
		public void put_row(dbrow r)
		{
			s.WriteByte ((byte)objtypes.row);
			__writeint (r.table_id);
			__writeint (r.fields.Length);
		}
	}

	public class MainClass
	{
		public static string targ_db = "Server=localhost;Database=fb_app;User Id=fb_app;Password=passw0rd!;CommandTimeout=120";
		public struct asdf
		{
			public string[] cols;
			public object[] data;
			//public string table;
			public string cmd;
		}
		public static int threads = 0;
		public static System.Collections.Generic.Queue<asdf> q = new System.Collections.Generic.Queue<asdf> ();
		public static object lck = new object ();

		public static void thr1 ()
		{
			int ii;
			NpgsqlTransaction trans = null;
			try {
				NpgsqlConnection conn2 = new NpgsqlConnection (targ_db);
				conn2.Open ();
				asdf a = new asdf ();
				while (true) {
					try {
						while (true) {
							lock (q) {
								if (q.Count > 0) {
									a = q.Dequeue ();
									threads++;
									if (trans == null)
										trans = conn2.BeginTransaction ();
									break;
								}
							}
							if (trans != null) {
								trans.Commit ();
								trans = null;
								lock (q) {
									threads--;
								}
							}
							System.Threading.Thread.Sleep (200);
						}
						try {
							NpgsqlCommand cmd2 = new NpgsqlCommand (a.cmd, conn2);
							for (ii=0; ii<a.data.Length; ii++) {
								object o = a.data [ii];
								if (typeof(bool).IsInstanceOfType (o)) {
									o = (((bool)o == true) ? 1 : 0);
								}
								cmd2.Parameters.Add ("col_" + ii.ToString (), o);
							}
							cmd2.ExecuteNonQuery ();
						} catch (Exception ex1) {
							Console.WriteLine ("exception: " + ex1.Message);
						}
					} finally {
						lock (q) {
							threads--;
						}
					}
				}
				conn2.Close ();
			} catch (Exception ex) {
				Console.WriteLine ("exception: " + ex.Message);
			}
		}

		public static void __main1 (string[] args)
		{
			SqlConnection conn = new SqlConnection ("data source=192.168.5.4;initial catalog=fb_app;persist security info=False;user id=user1;password=passw0rd!;packet size=32768");
			conn.Open ();
			
			NpgsqlConnection conn2 = new NpgsqlConnection (targ_db);
			conn2.Open ();
			
			
						
			string[] tables = new string[]{"bw3_backup","bw3_bookmark","bw3_favorites","bw3_likes","bw3_linkclick",
			"bw3_messages","bw3_posts","bw3_users"};
			//string[] tables = new string[]{"bw3_reports","bw3_statistics","bw3_replies"};
			bool delete = true;
			SqlCommand cmd = new SqlCommand ("", conn);
			
			SqlDataReader r;
			int i, ii;
			for (i=0; i<20; i++) {
				System.Threading.Thread th = new System.Threading.Thread (thr1);
				th.Start ();
			}
			for (i=0; i<tables.Length; i++) {
				Console.WriteLine ("tranferring table " + tables [i]);
				cmd.CommandText = "select * from " + tables [i];
				try {
					r = cmd.ExecuteReader ();
				} catch (Exception ex) {
					Console.WriteLine ("failed: " + ex.Message);	
					goto asdf;
				}
				//NpgsqlTransaction trans = conn2.BeginTransaction ();
				NpgsqlCommand cmd2 = new NpgsqlCommand ("", conn2);
				try {
					if (delete) {
						try {
							cmd2.CommandText = "delete from " + tables [i];
							cmd2.ExecuteNonQuery ();
						} catch (Exception ex1) {
							Console.WriteLine ("error deleting data: " + ex1.Message);
						}
					}
					string cmdt;
					string[] cols = new string[r.FieldCount];
					StringBuilder sb = new StringBuilder ("insert into ");
					sb.Append (tables [i]);
					sb.Append ("(");
					StringBuilder sb2 = new StringBuilder (" values(");
					for (ii=0; ii<r.FieldCount; ii++) {
						if (ii != 0) {
							sb.Append (",");
							sb2.Append (",");
						}
						cols [ii] = r.GetName (ii);
						sb.Append ("\"" + cols [ii] + "\"");
						sb2.Append ("@col_" + ii.ToString ());
						//cmd2.Parameters.AddWithValue ("@col_" + ii.ToString (), null);
						
					}
					sb.Append (")");
					sb2.Append (")");
					cmdt = sb.ToString () + sb2.ToString ();
					Console.WriteLine (cmdt);
					int rows = 0;
					while (r.Read ()) {
						asdf zxcv;
						zxcv.data = new object[r.FieldCount];
						zxcv.cols = cols;
						zxcv.cmd = cmdt;
						r.GetValues (zxcv.data);
						
						//cmd2.ExecuteNonQuery ();
						
						while (true) {
							lock (q) {
								if (q.Count < 50000)
									break;
							}
							System.Threading.Thread.Sleep (100);
						}
						q.Enqueue (zxcv);
						rows++;
					}
					Console.WriteLine (rows.ToString () + " rows transferred");
				} catch (Exception ex) {
					Console.WriteLine ("failed: " + ex.Message);	
					goto asdf;
				} finally {
					r.Close ();
				}
			asdf:
				;
				//conn2.EnlistTransaction (trans);
				//trans.Commit ();
				
			}
			Console.WriteLine ("waiting for all threads to exit...");
			while (threads>0)
				System.Threading.Thread.Sleep (300);
			//System.Diagnostics.Process.GetCurrentProcess ().Kill ();
		}
	}
}
