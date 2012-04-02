using System;
using System.Web;
using System.Web.UI;
using System.Data;
using MySql.Data.MySqlClient;

namespace dht_boot
{
	public partial class update : System.Web.UI.Page
	{
		protected override void OnLoad (EventArgs e)
		{
			base.OnLoad (e);
			string port = Request.QueryString ["port"];
			if (port == null || port.Length <= 0)
				return;
			MySqlConnection con = new MySqlConnection ("Server=127.0.0.1;Database=dht;Uid=dht;Pwd=sgv54hnf3ha142s3dfg4ad5sh;");
			con.Open ();
			try {
				MySqlCommand cmd = new MySqlCommand ("update nodes set lastupdate=CURRENT_TIMESTAMP where endpoint=@ep", con);
				cmd.Parameters.AddWithValue ("@ep", Request.UserHostAddress + " " + port);
				if (cmd.ExecuteNonQuery () > 0)
					return;
				cmd.CommandText = "insert into nodes(endpoint) values(@ep)";
				cmd.ExecuteNonQuery ();
			} finally {
				con.Close ();
			}
		}
	}
}

