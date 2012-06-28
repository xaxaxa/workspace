using System;
using System.Data;
using System.Data.SqlClient;
using Npgsql;
using System.Text;
using System.Runtime.Serialization;

namespace dbconvert
{
	[Serializable]
	public struct dbfield
	{
		public string name;
		public enum types
		{
			str=1,
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

	[Serializable]
	public struct dbtable
	{
		public string name;
		public dbfield[] fields;
		public int id;
	}

	[Serializable]
	public struct dbrow
	{
		public object[] fields;
		public int table_id;
	}

	public delegate void table_cb (dbtable t);

	public delegate void row_cb (dbrow t);

	public interface db: IDisposable
	{
		void connect ();

		void close ();

		void get_tables (table_cb cb);

		void get_rows (dbtable t, row_cb cb);

		void set_params (string name, string v);

		void create_table (dbtable t);
		
		void set_table (dbtable t);//adds a table but does not create it

		void insert_row (dbrow r);

		void reset_ids ();
		
		void commit ();
	};
	
	public class mssql_db: db
	{
		System.Collections.IDictionary param;
		System.Data.SqlClient.SqlConnection con;
		System.Data.SqlClient.SqlTransaction trans;
		public class table_entry
		{
			public dbtable t;
			public string insert_statement;

			public table_entry (dbtable t)
			{
				this.t = t;
			}
		}
		System.Collections.Generic.Dictionary<int,table_entry> table_mapping = new System.Collections.Generic.Dictionary<int, table_entry> ();

		public mssql_db ()
		{
			param = new System.Collections.Generic.Dictionary<string,string> ();
		}

		public void connect ()
		{
			if (con != null)
				return;
			con = new System.Data.SqlClient.SqlConnection ((string)param ["cs"]);
			try {
				con.Open ();
			} catch {
				con = null;
				throw;
			}
			trans = con.BeginTransaction (IsolationLevel.ReadUncommitted);
		}

		public void commit ()
		{
			trans.Commit ();
			trans = con.BeginTransaction (IsolationLevel.ReadUncommitted);
		}

		public void close ()
		{
			if (con != null)
				con.Close ();
			con = null;
			trans=null;
		}

		public void Dispose ()
		{
			close ();
		}

		public void get_tables (table_cb cb)
		{
			
		}

		public void get_rows (dbtable t, row_cb cb)
		{
			string cmd = "select * from " + t.name;
			System.Data.SqlClient.SqlCommand c = new System.Data.SqlClient.SqlCommand (cmd, con);
			c.Transaction=trans;
			System.Data.SqlClient.SqlDataReader r = c.ExecuteReader ();
			while (r.Read ()) {
				object[] o = new object[r.FieldCount];
				r.GetValues (o);
				dbrow row;
				row.fields = o;
				row.table_id=t.id;
				cb (row);
			}
			r.Close ();
		}

		public static string[] type_names = new string[]{null,"text","int2","int2","int4","int8","int2","int2","int4","int8","float4","float8","decimal","timestamp without time zone"};

		public void set_table (dbtable t)
		{
			table_mapping [t.id] = new table_entry (t);
		}

		public void create_table (dbtable t)
		{
			StringBuilder cmd = new StringBuilder ("create table " + t.name + "(");
			for (int i=0; i<t.fields.Length; i++) {
				if (i != 0)
					cmd.Append (',');
				cmd.Append (t.fields [i].name + " ");
				string type_name;
				if (t.fields [i].type == dbfield.types.str) {
					if (t.fields [i].len <= 0) {
						type_name = "ntext";
					} else {
						type_name = "nvarchar(" + t.fields [i].len.ToString () + ")";
					}
				} else
					type_name = type_names [(int)t.fields [i].type];
				cmd.Append (type_name);
			}
			System.Data.SqlClient.SqlCommand c = new System.Data.SqlClient.SqlCommand (cmd.ToString (), con);
			c.Transaction=trans;
			c.ExecuteNonQuery ();
			set_table (t);
		}

		public void insert_row (dbrow r)
		{
			table_entry t = table_mapping [r.table_id];
			if (t.insert_statement == null) {
				StringBuilder sb = new StringBuilder ("insert into " + t.t.name + "(");
				for (int i=0; i<t.t.fields.Length; i++) {
					if (i != 0)
						sb.Append (',');
					sb.Append (t.t.fields [i].name);
				}
				sb.Append (") values(");
				for (int i=0; i<t.t.fields.Length; i++) {
					if (i != 0)
						sb.Append (",@param" + i.ToString ());
					else
						sb.Append ("@param" + i.ToString ());
				}
				sb.Append (")");
				t.insert_statement = sb.ToString ();
			}
			System.Data.SqlClient.SqlCommand c = new System.Data.SqlClient.SqlCommand (t.insert_statement, con);
			for (int i=0; i<r.fields.Length; i++) {
				c.Parameters.Add (r.fields [i]);
			}
			c.Transaction=trans;
			c.ExecuteNonQuery ();
		}

		public void reset_ids ()
		{
			table_mapping.Clear ();
		}

		public void set_params (string name, string v)
		{
			param [name] = v;
		}
	}
	
	public class pgsql_db: db
	{
		System.Collections.IDictionary param;
		Npgsql.NpgsqlConnection con;
		Npgsql.NpgsqlTransaction trans;
		public class table_entry
		{
			public dbtable t;
			public string insert_statement;

			public table_entry (dbtable t)
			{
				this.t = t;
			}
		}
		System.Collections.Generic.Dictionary<int,table_entry> table_mapping = new System.Collections.Generic.Dictionary<int, table_entry> ();

		public pgsql_db ()
		{
			param = new System.Collections.Generic.Dictionary<string,string> ();
		}

		public void connect ()
		{
			if (con != null)
				return;
			con = new Npgsql.NpgsqlConnection ((string)param ["cs"]);
			trans = con.BeginTransaction (IsolationLevel.Chaos);
			try {
				con.Open ();
			} catch {
				con = null;
				throw;
			}
		}

		public void commit ()
		{
			trans.Commit ();
			trans = con.BeginTransaction (IsolationLevel.Chaos);
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

		public void get_rows (dbtable t, row_cb cb)
		{
			
		}

		public static string[] type_names = new string[]{null,"text","int2","int2","int4","int8","int2","int2","int4","int8","float4","float8","decimal","timestamp without time zone"};

		public void set_table (dbtable t)
		{
			table_mapping [t.id] = new table_entry (t);
		}

		public void create_table (dbtable t)
		{
			StringBuilder cmd = new StringBuilder ("create table " + t.name + "(");
			for (int i=0; i<t.fields.Length; i++) {
				if (i != 0)
					cmd.Append (',');
				cmd.Append (t.fields [i].name + " ");
				string type_name;
				if (t.fields [i].type == dbfield.types.str) {
					if (t.fields [i].len <= 0) {
						type_name = "text";
					} else {
						type_name = "varchar(" + t.fields [i].len.ToString () + ")";
					}
				} else
					type_name = type_names [(int)t.fields [i].type];
				cmd.Append (type_name);
			}
			Npgsql.NpgsqlCommand c = new Npgsql.NpgsqlCommand (cmd.ToString (), con);
			c.ExecuteNonQuery ();
			table_mapping [t.id] = new table_entry (t);
		}

		public void insert_row (dbrow r)
		{
			table_entry t = table_mapping [r.table_id];
			if (t.insert_statement == null) {
				StringBuilder sb = new StringBuilder ("insert into " + t.t.name + "(");
				for (int i=0; i<t.t.fields.Length; i++) {
					if (i != 0)
						sb.Append (',');
					sb.Append (t.t.fields [i].name);
				}
				sb.Append (") values(");
				for (int i=0; i<t.t.fields.Length; i++) {
					if (i != 0)
						sb.Append (",?");
					else
						sb.Append ('?');
				}
				sb.Append (")");
				t.insert_statement = sb.ToString ();
			}
			Npgsql.NpgsqlCommand c = new Npgsql.NpgsqlCommand (t.insert_statement, con);
			for (int i=0; i<r.fields.Length; i++) {
				c.Parameters.Add (r.fields [i]);
			}
			c.ExecuteNonQuery ();
		}

		public void reset_ids ()
		{
			table_mapping.Clear ();
		}

		public void set_params (string name, string v)
		{
			param [name] = v;
		}
	}
	/*public unsafe class format_helper
	{
		public byte[] buffer;
		//public byte* pbuffer;
		public int length;
		public void ensure_capacity(int l)
		{
			if(l<=buffer.Length)return;
			int tmpl=buffer.Length;
			while(tmpl<l)tmpl*=2;
			Array.Resize<byte>(ref buffer,tmpl);
		}
		public void flush_to(System.IO.Stream s)
		{
			s.Write (buffer,0,length);
			length=0;
		}
		public void write_int64(Int64 i)
		{
			ensure_capacity (length+8);
			fixed(byte* b=buffer)
			{
				Int64* tmp=(Int64*)(b+length);
				*tmp=i;
			}
			length+=8;
		}
		public void write_int32(Int32 i)
		{
			ensure_capacity (length+4);
			fixed(byte* b=buffer)
			{
				Int32* tmp=(Int32*)(b+length);
				*tmp=i;
			}
			length+=4;
		}
		public void write_int16(Int16 i)
		{
			ensure_capacity (length+2);
			fixed(byte* b=buffer)
			{
				Int16* tmp=(Int16*)(b+length);
				*tmp=i;
			}
			length+=2;
		}
		public void write_int8(sbyte i)
		{
			ensure_capacity (length+1);
			b[length]=(byte)i;
			length+=1;
		}
	}*/
	public class dbserializer
	{
		public System.IO.Stream s;
		public delegate void data_cb (object o);

		IFormatter formatter;
		[Serializable]
		public struct header
		{
			public int version;
		}

		public enum objtypes:byte
		{
			table=1,
			row
		}
		public dbserializer (System.IO.Stream s)
		{
			this.s = s;
			formatter = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter ();
		}
		//byte[] __tmp_int_buffer;
		public const int version = 1;
		public int cur_version = 1;

		public void write_header ()
		{
			header h;
			h.version = version;
			formatter.Serialize (s, h);
		}

		public void read (data_cb cb)
		{
			while (true) {
				object o = formatter.Deserialize (s);
				if (o == null)
					return;
				if (o is header) {
					cur_version = ((header)o).version;
				} else
					cb (o);
			}
		}
		/*public void write_header()
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
				*i = System.Net.IPAddress.HostToNetworkOrder (asdf.Length);
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
				Int16 i=(Int16)o;
				
				break;
			}
		}*/
		/*public void put_row(dbrow r)
		{
			s.WriteByte ((byte)objtypes.row);
			__writeint (r.table_id);
			__writeint (r.fields.Length);
		}*/
		public void put_row (dbrow r)
		{
			formatter.Serialize (s, r);
		}

		public void put_table (dbtable t)
		{
			formatter.Serialize (s, t);
		}
	}

	public class Main1
	{
		public static dbserializer s;
		public static void process_row(dbrow r)
		{
			s.put_row (r);
		}
		public static void Main (string[] args)
		{
			string[] tables = new string[]{"page_visit_log","buysell","business"};
			s = new dbserializer (new System.IO.FileStream ("tmp.out", System.IO.FileMode.Create));
			db db1 = new mssql_db ();
			db1.set_params ("cs", "data source=192.167.0.101;initial catalog=vanbusiness;persist security info=False;user id=user1;password=passw0rd!;packet size=32768");
			db1.connect ();
			s.write_header ();
			for (int i=0; i<tables.Length; i++) {
				dbtable t = new dbtable ();
				t.id = i;
				t.name = tables [i];
				s.put_table (t);
				db1.get_rows(t,process_row);
			}
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
