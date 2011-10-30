<?php ?>
<!DOCTYPE HTML>
<html>
<!-- manifest="cache.manifest"-->
<head>
    <title>Grooveshark for iPod</title>
    <meta name="apple-mobile-web-app-capable" content="yes" />
    <meta name="viewport" content="width=device-width; height=device-height; initial-scale=1.0; maximum-scale=1.0; user-scalable=0;" />
    <meta name="apple-mobile-web-app-status-bar-style" content="black" />
    <link rel="apple-touch-icon" href="favicon.png" />
    <link rel="apple-touch-startup-image" href="startup.png" />
	<script src="sorttable.js"></script>
	<script src="iscroll.js"></script>
	<script>
	var myscroll;
	function loaded() {
		myscroll = new iScroll('ccontainer');
		setTimeout(scrollTo, 0, 0, 1);
	}

	document.addEventListener('touchmove', function (e) { e.preventDefault(); }, false);
	document.addEventListener('touchstart', function (e) { window.scrollTo(0,1); }, false);
	document.addEventListener('DOMContentLoaded', loaded, false);
	window.onload=function(){setTimeout(scrollTo, 0, 0, 1);};
	</script>
    <style>
    body{padding:0px;margin:0px;overflow-x:hidden;overflow-y:hidden;}
    body,table,input,td
    {font-family: Helvetica;font-size: 14px;}
    body,table,td
    {cursor:default;}
    input
    {padding-left:0px;padding-right:0px;}
    .player_dock
    {
		width:100%;
    }
    .main_content
    {
	 border: none; margin: none; padding: none; width: 100%; 
    }
    #ccontainer
    {
		position:absolute;
		top:137px;
		bottom:-60px;
		left:0px;
		right:0px;
		overflow:auto;
	}
    .song_queue
    {
		/*height:58px;
		padding:2px;
		border-bottom:solid 1px #ccc;*/
		overflow-x:hidden;
		white-space: nowrap;
		margin-right:60px;
		padding-top:10px;
		padding-bottom:40px;
		/*overflow-y:hidden;*/
    }
    .song
    {
		border:solid 2px #ccc;
		border-radius:3px;
		display:inline-block;
		width:100px;
		height:50px;
		margin:2px;
		padding:2px;
		vertical-align: top;
		font-size:10px;
		line-height:10px;
		overflow:hidden;
		background:url(data:image/gif;base64,R0lGODlhBAA2ALMAAPz8/NLS0vX19dvb2%2BPj4%2Bzs7M7OzvDw8eDh4Ojo6N7e3ubm5%2Bbm5u/v79jY2NbW1iH5BAAAAAAALAAAAAAEADYAAARGEMhJq71UZCB0/2AodkdpnmWjNgXLFnAsF0lt18yiMzxD/MAgAUEsFhXIJHLAVDCfT4d0SnU8rlhsYMvter8GQyBMLpvPEQA7);
    }
    .songname2
    {
		font-size:13px;
		min-height: 16px; overflow: hidden;
    }
    .songartist2
    {
		font-size:12px;
		color:#777;
		min-height: 15px;line-height:11px;
    }
    .songalbum2
    {
		font-size:12px;
		color:#777;
		min-height: 15px;line-height:11px;
    }
    .songname,.songartist,.songalbum
    {
		min-height:10px;
    }
    .playing{border-color:#34c;}
    .menu
    {
		white-space: nowrap;
		cursor:default;
		margin-bottom:6px;
    }
    .menuitem
    {
		display:inline-block;
		padding:4px 6px;
		font-size:14px;
		/*font-weight:bold;*/
		color:#5555aa;
		border:solid 1px #fff;
		border-radius:5px;
    }
    .selected
    {
		background:#ddd;
		border:solid 1px #aaa;
    }
    .imgbutton
    {
		padding:6px;border:solid 2px #ccc;border-radius:2px;display:block;
		margin:2px;
		background-color:#fff;
    }
    .imgbutton:active
    {
		border-color:#34c;
    }
    .imgbutton2
    {
		padding:1px 16px;border:solid 2px #ccc;display:inline;
		margin-right:2px;
		margin-top:2px;
		background-color:#fff;
   }
    .imgbutton2:active
    {
		border-color:#34c;
    }
    .list_playbutton
    {
		background:url(data:image/gif;base64,R0lGODlhDAAMAIABAAAAAP///yH5BAEAAAEALAAAAAAMAAwAAAIXTICmi7na3IsSUjAvtpd1Hz1HKJJlpRQAOw%3D%3D);
		background-repeat:no-repeat;
		background-position:center;
		width:10px;
		padding-left:8px;
		padding-right:8px;
    }
    .list_content
    {
       width:60%;max-width:60%; overflow:hidden; display: inline-block;padding:3px 0px;
    } .list_content2{width:35%; max-width:35%;overflow:hidden; display: inline-block;padding:3px 0px;}
    .list_addbutton
    {
		width:6px;
		padding-left:8px;
		padding-right:8px;
		font-size:16px;
		font-weight:bold;
    }
    .alternate
    {
		background-color:#fff;
    }
tr{border-bottom:solid 1px #ccc;margin-bottom:4px;}
    .loading
    {
		width:32px;
		height:32px;
		background:url(data:image/gif;base64,R0lGODlhIAAgAMQYAFJvp3mOup6sy%2BDl7vHz%2BOXp8fT2%2BWV%2BsOjr8oiawae10OPn74mbwaKxzrrF2%2Bzv9ens8/L0%2BO/y99DX5sDJ3a%2B71e/y9vf5%2B////wAAAAAAAAAAAAAAAAAAAAAAAAAAACH/C05FVFNDQVBFMi4wAwEAAAAh%2BQQFCAAYACwAAAAAIAAgAAAFlCAmjmRpnmiqrmzrvnAsz6JBWLhFGKSd67yRL7cjXI5IAsmIPCpHzOatebSQLNSLdYSl4rJbUbcZxoyRX%2B8VvPaeq21yHP3WzuFccL28v2v7eWqBZIBibIN0h4aCi4SKZo97hZCMlI6Vk5KRm26ccohVmZ6JmKNVUUlLWU8iqE5DODs9N0RBNbSxtjS7vL2%2Bv8DBGCEAIfkEBQgAGAAsAAAFAAgAFgAABR%2BgQVikRYhXqo5Y61puLM90bd94ru88Dssm1UpUMhlCACH5BAkIABgALAAAAAAUACAAAAV0IHMAJHAwWKqu6VG98MHOGADDAM3ad5XrKt7tB6z1fCsDwcK0EAxC3IpwqVoJ0RcRY5lZssiisbfVgcu0s3g8XKvF72IcODcf0bN6%2Bu7mw/1ygHSCdmQrXSxfglRWVViCSk1OUIR7hn%2BXRS49MmIiJSYoYiEAIfkECQgAGAAsAAAAACAAIAAABcsgJo6kyBxAChxM6WJNEsxB0pBHpe/HWyaUoDBBAux2AB8pIBQGikddUiliNinPkTE6pVqbWdH22MUYCJa0hWD4OqFcEuFCrxPcwTBmjCRZXBZ4WHBkVFVXg1pRFWU%2Bgnp8UoYYj4R9hpWKcZiIkIuNL5lin5Oie6ScV56bXp2Wkqlgr4ylrpqFsW%2B3l62qs6AuppG0uXm/tb67sCJ/JYG2o6wYc3V0d9Cn0mdqa23Yw8AlwqhUQFdEysRUMTQ1NyM5UT2ThicqKy2GIQAh%2BQQJCAAYACwAAAAAIAAgAAAF5CAmjmRpjswBrMDBnGWTBHSQNORR7fwBkwmKcJggAXg8gEMhaAoUDlJgOAwYkTuAYsLtKqRUoXV0xAIE3a4AHB6LyshzmrseTdtXM3peF92pbhhwSXtpfRh/VXlxhWpsgIuEcxOHiWKRWY10j4pkWBVyfJyXnnqTlWEUgYOZp6OqmKCalK%2Brn6GGtbG4jnaptqaivniljK7DkMWSwn6/u7OoxG%2B30LrKrcyIzteyx83SgtTe2uCs3dmWsNxak1/IndNmS05PUe%2Bk8XE/I0FhRev7RMioYQPHCB1YfARcmIJFCwYhAAAh%2BQQJCAAYACwAAAAAIAAgAAAF1iAmjmRpnmiqYk0SvEHSrDSWUHie0I4i/AKFgxTI5QI0xWTJVBCNuABkMagOFhCSgMkUPKGBhWRMXmi5S%2B%2BoCB6QyYMzWi1iGwPutyQ%2B2s6/d3lvfCJ%2BXHQYdkeCcHKHgIt6e45dkFGMY4QYhpVrUBR4kpqcaZagmJN9aBOIipeilKWebbCqf7OBtYWrrZ%2BheqO8pr%2BDsazDqMG3db7Jxr20wM/IupvCuJHSto/YUWJ6ZtudzGBTVldZ4rLkd0mrTt2gPD5AQsM1KzdQO/gpLTAxZvQbGAIAIfkECQgAGAAsAAAAACAAIAAABc0gJo5kaZ5oqq5s676OIsyC4rypMu28wkKLgXCwgJAEPJ7ggSg4C4gHaSGpWhfH5E6AiHi/CNLAah1ktYLC91sQk6vmERKtXkfao/E7Lpon03Z3bntnf3VreCJ6ZHwYfkqHbIOMhZCBiRiLZZVbkV6YmnCcE4B2oG8SjY%2Bdl5ObclqknoJ5qKqxpYiuorB0rbWEvYa/irajuZLAlMKWprupx7OnwX24XXZhyq/VaExPUFIjVG9YzFs/QUNFxzgoOlo%2B7SYxNDU38vj5%2Bu0hACH5BAkIABgALAAAAAAgACAAAAXIICaOZGmeaKqubOu%2BcCy30DLcwwIZhOVbBAPpgSgYC4gHaSFpOheEi3RKICEi2CyCNHA6B5bp1EIqZLMFrrcJFkvJI/M5kh511203XCQ/10V3Xnliexh9aGp4YXplc3SJgouEjXN/GIFfkmOUfpCZbheFh1iWmGyab5yIdmsSg5txjqWtr6mxlZ6noKKyua6ooaqkvrXBt52sirvCj8mRy8ergLRRblUjV3Nbzl88P0BCI0RHSEojTGsLMyU1ODkQ6/Hy8/T19SEAIfkEBQgAGAAsAAAAACAAIAAABbAgJo5kaZ5oqq5s675wLM%2BiQVi4RRikneu80QNRKBYQD8JlySSQlMylc4SIWK8IS3RpIWm33VHhei18o2HRmZnGjMkR8/bSXnNJb7Ic7J2382V2dH18YnBxgnV%2BeId7aISPhnCObJCVknqJlneYgYsjmp1WlJxqnyKAo6GmhaiNqxiwqYinsbWzpIOgt6%2B1so1QUVMiwU0kVXAIPjk7PTfMQSJDRkcPNNfY2drb3N0kIQAh%2BQQFFAAYACwYAAYACAAUAAAFKKBBWKRFiFeqjqpKtukLyy3tWvBlx/jc179bbqcL8obG4pCQO41KpxAAOw%3D%3D);
    }
    input{border:solid 1px #999;border-radius:2px;}
    .sb
    {
		background:#e5e5e5;
		border:solid 2px #ccc;
		border-radius:2px;
		height:24px;width:0px;max-width:100%;
		background:url(data:image/gif;base64,R0lGODlhBAAYAMQAANbW1t3d3drZ2sbGxuHh4cDBwfHx8cPDw/T09Pf39%2B7u7snJyb%2B/vr29vfn6%2BtLT0////%2Bvr6%2BTk5M/Pz8zMzPv7/P3%2B/c/Qz%2Bjo6P79/vz7/Ofo6P39/efo58zLzOfn5yH5BAAAAAAALAAAAAAEABgAAAVBICRCnJVlWqU6rJO8CSIjRm0ouBLt0Yd1G4lQQigSAsiAYCkAOAGP6GNymUwoHgplwV0MvoOD%2BFAoFxhoRmPdCAEAOw%3D%3D);
    }
    h3
    {
		font-size:16px;
		font-weight:bold;
		margin:5px 0px 5px 0px;
    }
    .container
    {/*width:500px;*/}
    .label
    {border:none;}
    .button
    {padding:2px 6px;}
    </style>
    <script type="text/javascript">
    var queue=[];
    var nowplaying;
    var _md=false;
    var _dx;
    var _ds;
    var lastSaved;
    var dataChanged=true;
    /*
    class song
    {
		id; name; artist; album;
		//UI
		element;
    }
    */
    function createSongUI(song)
    {
		if(song.element)return;
		var div=document.createElement("div");
		div.setAttribute("class","song");
		
		var tmp;
		tmp=document.createElement("div");
		tmp.setAttribute("class","songname");
		//tmp.innerText=song.name;
		tmp.textContent=song.name;
		div.appendChild(tmp);
		
		tmp=document.createElement("div");
		tmp.setAttribute("class","songartist");
		//tmp.innerText=song.artist;
		tmp.textContent=song.artist;
		div.appendChild(tmp);
		
		tmp=document.createElement("div");
		tmp.setAttribute("class","songalbum");
		//tmp.innerText=song.album;
		tmp.textContent=song.album;
		div.appendChild(tmp);
		
		tmp=document.createElement("div");
		
		var tmp2;
		tmp2=document.createElement("img");
		tmp2.setAttribute("class","imgbutton2");
		tmp2.nodrag=true;
		tmp2.src="data:image/gif;base64,R0lGODlhDAAMAIAAAP///wAAACH5BAAAAAAALAAAAAAMAAwAAAIXBIKmi7na3IsS0jAvtpd1Hz1HKJJlpRQAOw%3D%3D";
		tmp2.song=song;
		tmp2.onclick=function()
		{
			play(this.song);
		};
		tmp.appendChild(tmp2);
		
		tmp2=document.createElement("img");
		tmp2.setAttribute("class","imgbutton2");
		tmp2.nodrag=true;
		tmp2.src="data:image/gif;base64,R0lGODlhDAAMAIAAAAAAAP///yH5BAAAAAAALAAAAAAMAAwAAAIYhINpe8qMXngyUpAszLM6%2Bl3dooEhyS0FADs%3D";
		tmp2.song=song;
		tmp2.onclick=function()
		{
			this.song.element.parentNode.removeChild(this.song.element);
			queue[this.song.i]=null;
			saveQueue();
			if(this.song==nowplaying)play(null);
		};
		tmp.appendChild(tmp2);
		
		div.appendChild(tmp);
		
		div.song=song;
		song.element=div;
    }
    function addToQueue(song)
    {
		var s=new Object();
		s.id=song.id;
		s.name=song.name;
		s.artist=song.artist;
		s.album=song.album;
		
		createSongUI(s);
		document.getElementById("song_queue").appendChild(s.element);
		s.i=queue.length;
		queue[queue.length]=s;
		saveQueue();
		calc_scroll();
		dataChanged=true;
		return s;
    }
    function saveQueue()
    {
		var temp_obj=[];
		//temp_obj.length=queue.length;
		var i;
		for(i=0;i<queue.length;i++)
		{
			try
			{
				var obj=queue[i];
				if(obj!=null)
				{
					var obj2=new Object();
					obj2.id=obj.id;
					obj2.name=obj.name;
					obj2.artist=obj.artist;
					obj2.album=obj.album;
					temp_obj[temp_obj.length]=obj2;
				}
			}catch(ex){}
		}
		localStorage.setItem("queue",JSON.stringify(temp_obj));
		saveQueueScroll();
    }
    function saveQueueScroll()
    {
		localStorage.setItem("queue_scroll_left",document.getElementById("song_queue").scrollLeft.toString());
		dataChanged=true;
    }
    function loadQueue()
    {
		var tmp=localStorage.getItem("queue");
		if(tmp!=null)
		{
			tmp=JSON.parse(tmp);
			queue=tmp;
			var i;
			for(i=0;i<tmp.length;i++)
			{
				tmp[i].i=i;
				createSongUI(tmp[i]);
				document.getElementById("song_queue").appendChild(tmp[i].element);
			}
		}
		tmp=localStorage.getItem("queue_scroll_left");
		if(tmp!=null)document.getElementById("song_queue").scrollLeft=parseInt(tmp);
		calc_scroll();
    }
    function play(song)
    {
		//if(song==nowplaying)return;
		try
		{
			if(nowplaying)
				nowplaying.element.setAttribute("class","song");
		}catch(ex){}
		nowplaying=song;
		if(nowplaying)
		{
			nowplaying.element.setAttribute("class","song playing");
			//nowplaying.element.scrollIntoView();
			document.getElementById("a1").pause();
			document.getElementById("a1").src="play.php?id="+song.id;
			document.getElementById("a1").load();
			document.getElementById("a1").play();
			scrollintoview(nowplaying.element);
			calc_scroll();
		} else {
			document.getElementById("a1").pause();
			document.getElementById("a1").src="";
			document.getElementById("a1").load();
		}
		recent_add(song);
    }
    function playPrev()
    {
		if(nowplaying)
		{
			var i;
			for(i=nowplaying.i-1;i>=0;i--)
				if(queue[i]!=null)
				{
					play(queue[i]);
					return;
				}
		}
		/*else if(queue.length>0)
		{
			var i;
			for(i=queue.length-1;i>=0;i--)
				if(queue[i]!=null)
				{
					play(queue[i]);
					return;
				}
		}*/
		//play(null);
    }
    function playNext()
    {
		if(nowplaying)
		{
			var i;
			for(i=nowplaying.i+1;i<queue.length;i++)
				if(queue[i]!=null)
				{
					play(queue[i]);
					return;
				}
		}
		/*else if(queue.length>0)
		{
			var i;
			for(i=0;i<queue.length;i++)
				if(queue[i]!=null)
				{
					play(queue[i]);
					return;
				}
		}*/
		//play(null);
    }
    function clearQueue()
    {
		var i;
			for(i=0;i<queue.length;i++)
				if(queue[i]!=null && queue[i].element!=null)
					queue[i].element.parentNode.removeChild(queue[i].element);
		queue.length=0;
		saveQueue();
		dataChanged=true;
		play(null);
		calc_scroll();
    }
    var selectedmenu;
    function selectmenu(m)
    {
		//if(m==selectedmenu)return;
		if(selectedmenu)
		{
			if(selectedmenu.getAttribute("myhref").charAt(0)=="#")
				document.getElementById(selectedmenu.getAttribute("myhref").substring(1)).style.display="none";
			else
				document.getElementById("main_content").style.display="none";
			selectedmenu.setAttribute("class","menuitem");
		}
		selectedmenu=m;
		if(selectedmenu)
		{
			if(selectedmenu.getAttribute("asdfghjkl")!=null)
				(new Function(selectedmenu.getAttribute("asdfghjkl")))();
			if(selectedmenu.getAttribute("myhref").charAt(0)=="#")
			{
				if(selectedmenu.getAttribute("mysrc")!=null && !document.getElementById(selectedmenu.getAttribute("myhref").substring(1)).loaded)
				{
					if(selectedmenu.getAttribute("mysrc").charAt(0)=="#")
						parseList(eval("("+selectedmenu.getAttribute("mysrc").substring(1)+")").list,document.getElementById(selectedmenu.getAttribute("myhref").substring(1)));
					else
					{
						document.getElementById(selectedmenu.getAttribute("myhref").substring(1)).loaded=true;
						loadlist(selectedmenu.getAttribute("mysrc"),document.getElementById(selectedmenu.getAttribute("myhref").substring(1)));
					}
				}
				document.getElementById(selectedmenu.getAttribute("myhref").substring(1)).style.display="";
			}
			else
			{
				document.getElementById("main_content").innerHTML="";
				document.getElementById("main_content").style.display="";
				loadlist(selectedmenu.getAttribute("myhref"));
			}
			selectedmenu.setAttribute("class","menuitem selected");
			localStorage.setItem("tab",selectedmenu.id);
		}
		setTimeout(function(){myscroll.refresh();}, 0);
    }
    function parseList(obj,e)
    {
		e.innerHTML="";
		var i;
		var div=e;
		var table=document.createElement("table");
		table.setAttribute("class","sortable");
		table.style.width="100%";
		table.style.maxWidth="100%";
		table.style.border="none";
		table.cellPadding="0";
		table.cellSpacing="0";
var thead=document.createElement("thead");
var thr;
thr=document.createElement("tr");
var th;
th=document.createElement("th");
th.textContent=" ";
thr.appendChild(th);
th=document.createElement("th");
th.textContent="Name";
th.setAttribute("class","list_content");
thr.appendChild(th);
th=document.createElement("th");
th.textContent="Artist";
th.setAttribute("class","list_content2");
thr.appendChild(th);
th=document.createElement("th");
th.textContent=" ";
thr.appendChild(th);
thead.appendChild(thr);
table.appendChild(thead);
		var tb=document.createElement("tbody");
		/*var tr1;
		tr1=document.createElement("tr");
		
		var td1;
		td1=document.createElement("td");
		td1.innerText=" ";
		tr1.appendChild(td1);
		
		td1=document.createElement("td");
		td1.innerText="name";
		tr1.appendChild(td1);
		
		td1=document.createElement("td");
		td1.innerText="artist";
		tr1.appendChild(td1);
		
		td1=document.createElement("td");
		td1.innerText="album";
		tr1.appendChild(td1);
		
		tb.appendChild(tr1);*/
		var b1=false;
		for(i=0;i<obj.length;i++)
		{
			var tr;
			tr=document.createElement("tr");
tr.setAttribute("class",(b1?"alternate":""));
			
			var td;
			td=document.createElement("td");
			td.setAttribute("class","list_playbutton");
			//td.innerText=" ";
			td.textContent=" ";
			td.song=obj[i];
			td.onclick=function()
			{
				play(addToQueue(this.song));
			};
			tr.appendChild(td);
			
			td=document.createElement("td");
			td.setAttribute("class","list_content");
			//td.innerText=obj[i].name;
			
			var tmp;
			tmp=document.createElement("div");
			tmp.setAttribute("class","songname2");
			//tmp.innerText=obj[i].name;
			tmp.textContent=obj[i].name+" ";
			td.appendChild(tmp);td.setAttribute("sorttable_customkey",tmp.textContent);
			var dl=document.createElement("a");
			dl.textContent="download";
			dl.href="play.php?download=1&id="+obj[i].id;dl.target="ifr1";
			td.appendChild(dl);
			tr.appendChild(td);
			
			td=document.createElement("td");
			td.setAttribute("class","list_content2");
			tmp=document.createElement("div");
			tmp.setAttribute("class","songartist2");
			tmp.textContent=obj[i].artist;
			td.appendChild(tmp);td.setAttribute("sorttable_customkey",tmp.textContent);
			
			tmp=document.createElement("div");
			tmp.setAttribute("class","songalbum2");
			tmp.textContent=obj[i].album;
			td.appendChild(tmp);
			
			tr.appendChild(td);
			
			td=document.createElement("td");
			td.setAttribute("class","list_addbutton");
			td.textContent="+";
			td.song=obj[i];
			td.c=0;
			td.onclick=function()
			{
				this.c++;
				this.textContent=this.c;
				addToQueue(this.song);
			};
			tr.appendChild(td);
			
			tb.appendChild(tr);
			b1=!b1;
		}
		table.appendChild(tb);
sorttable.makeSortable(table);
		div.appendChild(table);
    }
    function get_ajax()
    {
		if (window.XMLHttpRequest)
		{// code for IE7+, Firefox, Chrome, Opera, Safari
			return new XMLHttpRequest();
		}
		else
		{// code for IE6, IE5
			return new ActiveXObject("Microsoft.XMLHTTP");
		}
    }
    function loadlist(page,e)
    {
		if(e==null)e=document.getElementById("main_content");
		var xmlhttp=get_ajax();
		xmlhttp.tmp1=e;
		xmlhttp.onreadystatechange=function()
		{
			if (this.readyState==4)
			{
				//document.getElementById("main_content").innerHTML=xmlhttp.responseText;
				parseList(eval("("+this.responseText+")").list,this.tmp1);
				setTimeout(function(){myscroll.refresh();}, 0);
			}
		}
		xmlhttp.open("POST",page,true);
		xmlhttp.send();
    }
    function dosearch()
    {
		document.getElementById("searchresult").innerHTML="<div class=\"loading\"></div>";
		var xmlhttp=get_ajax();
		xmlhttp.onreadystatechange=function()
		{
			if (this.readyState==4)
			{
				//document.getElementById("main_content").innerHTML=xmlhttp.responseText;
				parseList(eval("("+this.responseText+")").list,document.getElementById("searchresult"));setTimeout(function(){myscroll.refresh();}, 0);
			}
		}
		xmlhttp.open("POST","list.php?type=search&q="+encodeURIComponent(document.getElementById("t_query").value),true);
		xmlhttp.send();
    }
    function scrollintoview(e)
    {
		var p=e.parentNode;
		var margin=4;
		if(p.scrollLeft>e.offsetLeft-margin)
		{
			p.scrollLeft=e.offsetLeft-margin;
		}
		else if(p.scrollLeft+p.offsetWidth-margin<e.offsetLeft+e.offsetWidth)
		{
			p.scrollLeft=e.offsetLeft+e.offsetWidth-p.offsetWidth+margin;
		}
    }
    function getrecent()
    {
		//return {list:[]};
		var list=[];
		var tmp=recent_init();
		var tmp1;
		//var tmp2;
		var i;
		for(i=tmp.end;i>=tmp.start;i--)
		{
			if((tmp1=localStorage.getItem("recent_"+i.toString()+"_details"))!=null)
			{
				//alert(tmp1);
				list[list.length]=JSON.parse(tmp1);
				if(list.length>=500)break;
			}
		}
		return {"list":list};
    }
    
    //var recentinit=false;
    function recent_init()
    {
		//if(recentinit)return;
		//recentinit=true;
		var retv=new Object();
		if(isNaN(retv.start=parseInt(localStorage.getItem("recent_start"))))
		{
			localStorage.setItem("recent_start","0");
			retv.start=0;
		}
		if(isNaN(retv.end=parseInt(localStorage.getItem("recent_end"))))
		{
			localStorage.setItem("recent_end","-1");
			retv.end=0;
		}
		return retv;
    }
    function recent_add(song)
    {
		if(song==null)return;
		var s=new Object();
		s.id=song.id;
		s.name=song.name;
		s.artist=song.artist;
		s.album=song.album;
		song=s;
		var tmp=recent_init();
		var i;
		var id=song.id;
		var is_first=true;
		for(i=tmp.start;i<=tmp.end;i++)
		{
			try
			{
				var temp1=parseInt(localStorage.getItem("recent_"+i.toString()+"_id"));
				if(temp1==id)
				{
					localStorage.removeItem("recent_"+i.toString()+"_id");
					localStorage.removeItem("recent_"+i.toString()+"_details");
					if(is_first)
					{
						do{tmp.start++;}
						while(localStorage.getItem("recent_"+tmp.start.toString()+"_id")==null && tmp.start<=tmp.end);
						localStorage.setItem("recent_start",tmp.start.toString());
					}
				}
				if(is_first && !isNaN(temp1))is_first=false;
			}
			catch(ex){alert(ex.message);}
		}
		tmp.end++;
		localStorage.setItem("recent_end",tmp.end.toString());
		localStorage.setItem("recent_"+tmp.end.toString()+"_id",song.id.toString());
		localStorage.setItem("recent_"+tmp.end.toString()+"_details",JSON.stringify(song));
		dataChanged=true;
    }
    function calc_scroll()
    {
		var sc=document.getElementById("scroll1");
		var e=document.getElementById("song_queue");
		var sb=document.getElementById("scrollbar1");
		sb.style.width=(e.clientWidth*sc.clientWidth/e.scrollWidth-4)+"px";
		sb.style.marginLeft=(e.scrollLeft/e.scrollWidth*sc.clientWidth)+"px";
    }
    function calc_scroll2()
    {
		var sc=document.getElementById("scroll1");
		var e=document.getElementById("song_queue");
		var sb=document.getElementById("scrollbar1");
		sb.style.marginLeft=(e.scrollLeft/e.scrollWidth*sc.clientWidth)+"px";
    }
    function _drag(e)
    {
		//alert(_ds+(e.clientX-_dx));
		document.getElementById("song_queue").scrollLeft=_ds+(_dx-e.targetTouches[0].pageX);
    }
    function scroll_left()
    {
		document.getElementById("song_queue").scrollLeft-=112;
		//document.getElementById("song_queue").scrollLeft=0;
    }
    function scroll_right()
    {
		document.getElementById("song_queue").scrollLeft+=112;
		//document.getElementById("song_queue").scrollLeft=document.getElementById("song_queue").scrollWidth-document.getElementById("song_queue").clientWidth;
    }
    function scroll_left2()
    {
		document.getElementById("song_queue").scrollLeft-=150;
    }
    function scroll_right2()
    {
		document.getElementById("song_queue").scrollLeft+=150;
    }
    function findPos(obj)
    {
		var curleft = curtop = 0;
		if (obj.offsetParent)
		{
			do
			{
				curleft += obj.offsetLeft;
				curtop += obj.offsetTop;
			} while (obj = obj.offsetParent);
			return [curleft,curtop];
		}else throw 1;
	}
	var sb_d=false;
	function sb_ts(element,e)
	{
		var sc=document.getElementById("scroll1");
		var sq=document.getElementById("song_queue");
		var pos=findPos(element);
		_sb_dx=e.targetTouches[0].pageX-pos[0];
		_sb_ds=sq.scrollLeft/sq.scrollWidth*sc.clientWidth;
		e.preventDefault();
	}
    function sb_md(element,e)
    {
		var pos=findPos(element);
		var x=e.clientX-pos[0];
		var sq=document.getElementById("song_queue");
		sq.scrollLeft=(x-(sq.clientWidth*document.getElementById("scroll1").clientWidth/sq.scrollWidth/2))*(sq.scrollWidth)/element.clientWidth;
    }
    function sb_mm(element,e)
    {
		//var pos=findPos(element);
		var x=e.targetTouches[0].clientX-element.offsetLeft+(_sb_ds-_sb_dx);
		var sq=document.getElementById("song_queue");
		sq.scrollLeft=x*(sq.scrollWidth)/element.clientWidth;
		e.preventDefault();
    }
    function getuserdata()
    {
		var queue=localStorage.getItem("queue");
		if(queue!=null)queue=JSON.parse(queue);
		var obj=
		{
			"queue":queue,
			"queue_scroll_left":localStorage.getItem("queue_scroll_left"),
			"recent":getrecent().list
		};
		return obj;
    }
    function setuserdata(obj,setRecent,setQueue)
    {
		if(setRecent===null)setRecent=true;
		if(setQueue===null)setQueue=true;
		if(setRecent && obj.recent!=null)
		{
			localStorage.setItem("recent_start",0);
			localStorage.setItem("recent_end",obj.recent.length-1);
			var i;
			var l=obj.recent.length;
			for(i=0;i<l;i++)
			{
				localStorage.setItem("recent_"+i+"_id",obj.recent[l-i-1].id);
				localStorage.setItem("recent_"+i+"_details",JSON.stringify(obj.recent[l-i-1]));
			}
		}
		if(setQueue)
		{
			if(obj.queue!=null)
			{
				clearQueue();
				queue=obj.queue;
				var i;
				for(i=0;i<queue.length;i++)
				{
					queue[i].i=i;
					createSongUI(queue[i]);
					document.getElementById("song_queue").appendChild(queue[i].element);
				}
				saveQueue();
			}
			if(obj.queue_scroll_left!=null)
			{
				document.getElementById("song_queue").scrollLeft=obj.queue_scroll_left;
				calc_scroll();
			}
		}
    }
    function savedata()
    {
		var xmlhttp=get_ajax();
		var tmp=localStorage.getItem("user_id");
		if(tmp==null || tmp==0)
		{
			xmlhttp.onreadystatechange=function()
			{
				if (this.readyState==4)
				{
					var obj=JSON.parse(this.responseText);
					localStorage.setItem("user_id",obj.id);
					localStorage.setItem("user_key",obj.key);
				}
			}
			xmlhttp.open("POST","savedata.php",true);
			xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
			xmlhttp.send("id=0&data="+encodeURIComponent(JSON.stringify(getuserdata())));
		}
		else
		{
			xmlhttp.open("POST","savedata.php",true);
			xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
			xmlhttp.send("id="+tmp+"&key="+encodeURIComponent(localStorage.getItem("user_key"))+"&data="+encodeURIComponent(JSON.stringify(getuserdata())));
			dataChanged=false;
			lastSaved=new Date();
		}
    }
    function importdata()
    {
		var xmlhttp=get_ajax();
		xmlhttp.onreadystatechange=function()
		{
			if (this.readyState==4)
			{
				var obj=JSON.parse(this.responseText);
				setuserdata(obj,document.getElementById("import_recent").checked,document.getElementById("import_queue").checked);
			}
		}
		xmlhttp.open("POST","getdata.php",true);
		xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
		xmlhttp.send("id="+document.getElementById("import_uid").value);
    }
    function _asdfghjkl()
    {
		document.getElementById("td_uid").value=localStorage.getItem("user_id");
		document.getElementById("td_key").value=localStorage.getItem("user_key");
		document.getElementById("s_lastsaved").textContent=lastSaved.toLocaleString();
    }
    function savedata2()
    {
		localStorage.setItem("user_id",parseInt(document.getElementById("td_uid").value));
		localStorage.setItem("user_key",document.getElementById("td_key").value);
		savedata();
		document.getElementById("s_lastsaved").textContent=lastSaved.toLocaleString();
    }
    </script>
</head>
<body onmouseup="_md=false;sb_d=false;" onmousemove="if(sb_d && event.button==0)sb_md(document.getElementById('scroll1'),event);" onresize="calc_scroll();">
<!--<div style="height:16px;width:100%;position:absolute;bottom:-60px;">aaaaa</div>-->
<div style="height:1px;width:100%;position:absolute;bottom:-200px;"></div>
<div class="container">
    <div class="player_dock">
		<audio id="a1" controls style="width:100%;height:32px;background:#000;display:block;"></audio>
		
		<div onresize="calc_scroll();" ontouchstart="if(!event.target.nodrag){_dx=event.targetTouches[0].pageX;_ds=this.scrollLeft;_md=true;}" ontouchmove="if(_md)_drag(event);" ontouchend="_md=false;saveQueueScroll();" ontouchcancel="_md=false;saveQueueScroll();" class="song_queue" id="song_queue" onscroll="calc_scroll2();"><div style="display:inline-block; height:62px;"></div></div>
		<!--onmousedown="if(!event.target.nodrag){_dx=event.clientX;_ds=document.getElementById('song_queue').scrollLeft;_md=true;}" onmousemove="if(_md && (event.button==0))_drag(event);"-->
		<!--<table style="position:absolute;top:107px;width:100%;padding:0px;margin:0px;" cellpadding="0" cellspacing="0">
		<tr>
		<td style="width:20px;" align="center">-->
		<img onclick="scroll_left();" ondblclick="scroll_left2();" style="position:absolute;top:107px;left:0px;" src="data:image/gif;base64,R0lGODlhDAAMAIABAAAAAP///yH5BAEAAAEALAAAAAAMAAwAAAIYjB%2BgiLqcHIDynWojbnjySoGhAxlSaT4FADs%3D" class="imgbutton" />
		<!--onmousedown="scroll_left();window.intv=setInterval(scroll_left,100);" onmouseup="clearInterval(window.intv);"-->
		<!--</td>
		<td>-->
		<div id="scroll1" onselectstart="return false;" onmousedown="sb_md(this,event);sb_d=true;" ontouchstart="sb_ts(this,event);" ontouchmove="sb_mm(this,event);" ontouchend="event.preventDefault();" ontouchcancel="event.preventDefault();" style="position:absolute;top:107px;left:34px;right:34px;height:30px;padding-top:2px;overflow:hidden;">
			<div id="scrollbar1" class="sb"></div>
		</div>
		<!--</td>
		<td style="width:20px;" align="center">-->
		<img onclick="scroll_right();" ondblclick="scroll_right2();" style="position:absolute;top:107px;right:0px;" src="data:image/gif;base64,R0lGODlhDAAMAIABAAAAAP///yH5BAEAAAEALAAAAAAMAAwAAAIXTICmi7na3IsSUjAvtpd1Hz1HKJJlpRQAOw%3D%3D" class="imgbutton" />
		<!--onmousedown="scroll_right();window.intv=setInterval(scroll_right,100);" onmouseup="clearInterval(window.intv);"-->
		<!--</td>
		</tr>
		</table>-->
		
		<div style="position:absolute;right:30px;top:42px;background:white;">
			<div style="width:30px;height:30px;"></div>
			<img class="imgbutton" onclick="playPrev();" src="data:image/gif;base64,R0lGODlhDAAMAIAAAAAAAP///yH5BAAAAAAALAAAAAAMAAwAAAIaTABmmNvHFHLRQfawlrreznVZAlrfGKJhUAAAOw%3D%3D" />
		</div>
		<div style="position:absolute;right:0px;top:42px;background:white;">
			<img class="imgbutton" onclick="clearQueue();" src="data:image/gif;base64,R0lGODlhDAAMAIAAAAAAAP///yH5BAAAAAAALAAAAAAMAAwAAAIYhINpe8qMXngyUpAszLM6%2Bl3dooEhyS0FADs%3D" />
			<img class="imgbutton" onclick="playNext();" src="data:image/gif;base64,R0lGODlhDAAMAIAAAAAAAP///yH5BAAAAAAALAAAAAAMAAwAAAIbDB5poIf5loOMOhavbnraznWVNGUXFY7muCQFADs%3D" />
		</div>
    </div>
    
	<div class="ccontainer" id="ccontainer">
		<div style="padding:10px 0px;">
			<div class="menu">
				<div class="menuitem" onclick="selectmenu(this);" id="menuitem1" myhref="#popular" mysrc="list.php?type=popular">Popular</div>
				<div class="menuitem" onclick="selectmenu(this);" id="menuitem2" myhref="#recent" mysrc="#getrecent()">Recent</div>
				<div class="menuitem" onclick="selectmenu(this);try{document.getElementById('t_query').focus();document.getElementById('t_query').select();}catch(ex){}" id="menuitem3" myhref="#search">Search</div>
				<div class="menuitem" onclick="selectmenu(this);" id="menuitem4" myhref="#importdata" asdfghjkl="_asdfghjkl();">Import data</div>
			</div>
			<div class="main_content" id="main_content" style="display:none;">
			</div>
			<div class="main_content" id="popular" style="display:none;">
				<div class="loading"></div>
			</div>
			<div class="main_content" id="recent" style="display:none;"></div>
			<div class="main_content" id="search" style="display:none;">
				<form onsubmit="try{dosearch();}catch(ex){}return false;">
					<table style="width:100%;">
						<tr>
							<td><input style="width:100%;" id="t_query" /></td>
							<td style="width:75px;" align="right"><input type="button" value="search" style="width:75px;height:20px;" onclick="dosearch();" /></td>
						</tr>
					</table>
				</form>
				<div id="searchresult"></div>
			</div>
			<div id="importdata" style="display:none;padding-left:10px;padding-bottom:10px;">
				<h3>User info</h3>
				<table style="width:100%">
					<tr>
						<td style="width:110px;">Your user id:</td>
						<td><input id="td_uid" style="width:100%" class="label" /></td>
					</tr>
					<tr>
						<td>Your user key:</td>
						<td><input id="td_key" style="width:100%" class="label" /></td>
					</tr>
					<tr>
						<td colspan="2" style="padding-top:10px;">
							<input type="button" class="button" value="Save data" onclick="savedata2();" />
						</td>
					</tr>
					<tr>
						<td colspan="2">Last saved at: <span id="s_lastsaved"></span></td>
					</tr>
				</table>
				<hr />
				<h3>Import data</h3>
				<table style="width:100%">
					<tr>
						<td style="width:60px;">User id:</td>
						<td><input style="width:100%" class="label" id="import_uid" /></td>
					</tr>
					<tr>
						<td colspan="2">
							<input type="checkbox" id="import_recent" checked="checked" /><label for="import_recent">Recent songs</label>
							<input type="checkbox" id="import_queue" checked="checked" /><label for="import_queue">Queue</label>
						</td>
					</tr>
					<tr>
						<td colspan="2" style="padding-top:10px;"><input type="button" class="button" value="Import" onclick="importdata();" /></td>
					</tr>
				</table>
			</div>
		</div>
	</div>
</div>

<script type="text/javascript">
var _retry=true;
function songFinished()
{
	//alert("aaaaa");
	_retry=true;
	playNext();
}
function songError()
{
	//alert("aaaaa");
	if(_retry)
	{
		_retry=false;
		play(nowplaying);
	}
	else
	{
		_retry=true;
		playNext();
	}
}
document.getElementById("a1").addEventListener("ended", songFinished, true);
document.getElementById("a1").addEventListener("error", songError, true);
/*addToQueue({"id":22990309,"name":"asdfaaaaaaaaaaaaaaaaaaaaaaaaaa","artist":"zxcv","album":"aaaaa"});
addToQueue({"id":25580291,"name":"ghdfh","artist":"dhdf","album":"sdgsdgsd"});
addToQueue({"id":23750337,"name":"ghdfgfhh","artist":"dhdhdgdf","album":"sdtryregsdgsd"});
addToQueue({"id":24416092,"name":"ghddgfhtr","artist":"sdfgdhdf","album":"sdgsdgfsdgsd"});
addToQueue({"id":25278141,"name":"ghddgfhtr","artist":"sdfgdhdf","album":"sdgsdgfsdgsd"});*/
loadQueue();
calc_scroll();
savedata();
setInterval(function(){if(dataChanged)savedata();},600000);
setTimeout(function(){
var tmp=localStorage.getItem("tab");
if(tmp!=null)
selectmenu(document.getElementById(tmp));
else
selectmenu(document.getElementById("menuitem1"));
},50);
</script>
</body>
</html>

</script>
<iframe name="ifr1" style="width:0px;height:0px;visibility:hidden"></iframe>
</body>
</html>
