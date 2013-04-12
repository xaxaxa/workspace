	function qs(name)
	{
	  name = name.replace(/[\[]/, "\\\[").replace(/[\]]/, "\\\]");
	  var regexS = "[\\?&]" + name + "=([^&#]*)";
	  var regex = new RegExp(regexS);
	  var results = regex.exec(window.location.search);
	  if(results == null)
		return null;
	  else
		return decodeURIComponent(results[1].replace(/\+/g, " "));
	}
	function getLargestPicture(obj)
	{
		if("images" in obj) {
			var img=null;
			for(var i=0;i<obj.images.length;i++)
				if(img==null || 
					(obj.images[i].width*obj.images[i].height) >
					(obj.images[img].width*obj.images[img].height))
					img=i;
			if(img!=null && obj.images[img].source!=null)
				return ["images."+img.toString(),obj.images[img].source];
		}
		if("source" in obj) {
			return ["source",obj.source];
		}
		return null;
	}
	function resolutionDiff(w1,h1,w2,h2) {
		return Math.abs(Math.log(w1/w2))+Math.abs(Math.log(h1/h2));
	}
	function getPicture(obj,w,h)
	{
		if("images" in obj) {
			var img=null;
			var imgDiff=0;
			for(var i=0;i<obj.images.length;i++) {
				var tmp=obj.images[i];
				var d=resolutionDiff(tmp.width,tmp.height,w,h);
				if(img==null || d < imgDiff) {
					img=i;
					imgDiff=d;
				}
			}
			if(img!=null && obj.images[img].source!=null)
				return ["images."+img.toString(),obj.images[img].source];
		}
		if("source" in obj) {
			return ["source",obj.source];
		}
		return null;
	}
	String.prototype.trim = function() {
		return this.replace(/^\s+|\s+$/g,"");
	}
	String.prototype.ltrim = function() {
		return this.replace(/^\s+/,"");
	}
	String.prototype.rtrim = function() {
		return this.replace(/\s+$/,"");
	}
	function esc_str(s)
	{
		return s.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/\r\n/g, "<br />").replace(/\n\r/g, "<br />").replace(/\r/g, "<br />").replace(/\n/g, "<br />");
	}
