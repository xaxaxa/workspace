using System;
using System.Web;
using System.Web.UI;
using MySql.Data.MySqlClient;
using System.Data;

namespace dht_boot
{
	public partial class _default : System.Web.UI.Page
	{
		protected override void OnLoad (EventArgs e)
		{
			base.OnLoad (e);
			MySqlCommand cmd;
			IDataReader r;
			MySqlConnection con = new MySqlConnection ("Server=127.0.0.1;Database=dht;Uid=dht;Pwd=sgv54hnf3ha142s3dfg4ad5sh;");
			con.Open ();
			try {
				string port = Request.QueryString ["port"];
				ushort tmp_i;
				if (port != null && port.Length > 0 && ushort.TryParse (port, out tmp_i)) {
					cmd = new MySqlCommand ("update nodes set lastupdate=CURRENT_TIMESTAMP where endpoint=@ep", con);
					cmd.Parameters.AddWithValue ("@ep", Request.UserHostAddress + " " + port);
					if (cmd.ExecuteNonQuery () <= 0) {
						cmd.CommandText = "insert into nodes(endpoint) values(@ep)";
						cmd.ExecuteNonQuery ();
					}
				}
				cmd = new MySqlCommand ("SELECT id, endpoint FROM nodes WHERE lastupdate>(subtime(CURRENT_TIMESTAMP,'12:00:00')) order by rand() LIMIT 20", con);
				r = cmd.ExecuteReader ();
				while (r.Read ())
					Response.Output.WriteLine (r.GetString (1));
				r.Close ();
				
			} finally {
				con.Close ();
			}
			
		}
	}
}

