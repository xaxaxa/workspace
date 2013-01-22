<?php
include "functions.php";
?>
{"list":
[
<?php
	switch (Request.QueryString["type"])
	{
		case "popular":
			{	SciLorsGroovesharkAPI.Groove.Functions.PopularSongs.popularGetSongsResultsResponse r = functions.getPopular();
				int i;
				bool b = false;
				for (i = 0; i < r.result.Songs.Count; i++)
				{
					if (b)
					{?>,<?php }
					b = true;
					?>
{"id":<?php echo r.result.Songs[i].SongID ?>,"name":"<?php echo functions.esc_str3(r.result.Songs[i].Name) ?>","artist":"<?php echo functions.esc_str3(r.result.Songs[i].ArtistName) ?>","album":"<?php echo functions.esc_str3(r.result.Songs[i].AlbumName) ?>"}<?php
				}
			}
			break;
		case "search":
			{
				SciLorsGroovesharkAPI.Groove.Functions.SearchArtist.getSearchArtistResultsResponse r = functions.search(Request.QueryString["q"]);
				int i;
				bool b = false;
				for (i = 0; i < r.result.result.Count; i++)
				{
					if (b)
					{?>,<? php }
					b = true;
					?>
{"id":<?php echo r.result.result[i].SongID ?>,"name":"<?php echo functions.esc_str3(r.result.result[i].Name) ?>","artist":"<?php echo functions.esc_str3(r.result.result[i].ArtistName) ?>","album":"<?php echo functions.esc_str3(r.result.result[i].AlbumName) ?>"}<%
				}
			}
			break;
	}
?>
]
}
