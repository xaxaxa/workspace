using System;
using System.Web;
using System.Web.UI;
using System.Data;
using MySql.Data.MySqlClient;

namespace dht_boot
{
	public partial class list : System.Web.UI.Page
	{
		//min and max is inclusive
		long getrand (System.Random r, long min, long max)
		{
			byte[] b = new byte[8];
			r.NextBytes (b);
			ulong tmp = System.BitConverter.ToUInt64 (b, 0);
			//Response.Output.WriteLine (tmp);
			//Response.Output.WriteLine (decimal.Round (((((decimal)tmp) * (decimal)(max - min) / (decimal)ulong.MaxValue))));
			return (long)decimal.Round (((((decimal)tmp) * (decimal)(max - min) / (decimal)ulong.MaxValue))) + min;
		}
		protected override void OnLoad (EventArgs e)
		{
			base.OnLoad (e);
			MySqlConnection con = new MySqlConnection ("Server=127.0.0.1;Database=dht;Uid=dht;Pwd=sgv54hnf3ha142s3dfg4ad5sh;");
			con.Open ();
			try {
				MySqlCommand cmd = new MySqlCommand ("SELECT MAX(id) AS max_id , MIN(id) AS min_id FROM nodes", con);
				IDataReader r;
				r = cmd.ExecuteReader (CommandBehavior.SingleRow);
				if (!r.Read ())
					return;
				long max = r.GetInt64 (0);
				long min = r.GetInt64 (1);
				r.Close ();
				cmd.CommandText = "SELECT id, endpoint FROM nodes WHERE id >= @id and lastupdate>(subtime(CURRENT_TIMESTAMP,'05:00:00')) order by id LIMIT 0,1";
				cmd.Parameters.Add ("@id", MySqlDbType.Int64);
				System.Random rnd = new System.Random ();
				System.Collections.Generic.SortedList<long,string > l = new System.Collections.Generic.SortedList<long, string> ();
				System.Collections.Generic.List<string > l2 = new System.Collections.Generic.List<string> ();
				int i;
				for (i=0; i<10; i++) {
					long id = getrand (rnd, min, max);
					//Response.Output.WriteLine (id);
					cmd.Parameters [0].Value = id;
					r = cmd.ExecuteReader (CommandBehavior.SingleRow);
					try {
						if (!r.Read ())
							continue;
						try {
							l.Add (r.GetInt64 (0), r.GetString (1));
							l2.Add (r.GetString (1));
						} catch {
						}
					} finally {
						r.Close ();
					}
				}
				//System.Collections.Generic.IList<string > tmp = l.Values;
				for (i=0; i<l2.Count; i++) {
					Response.Output.WriteLine (l2 [i]);
				}
			} finally {
				con.Close ();
			}
		}
	}
}

