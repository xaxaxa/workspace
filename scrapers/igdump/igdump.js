var Client = require('instagram-private-api').V1;
var fs = require('fs');
var http = require('https');
var sleep = require('sleep');

if(process.argv.length<6) {
	console.log("usage: igdump.js cookiesdir username password user_to_dump");
	process.exit(1)
}
var cookiesdir = process.argv[2];
var username = process.argv[3];
var password = process.argv[4];
var dumpuser = process.argv[5];
var exitOnExists = true;

var device = new Client.Device(username);
var storage = new Client.CookieFileStorage(cookiesdir + '/session.json');
// And go for login 
Client.Session.create(device, storage, username, password)
    .then(login_done);

var allPosts = {};

function login_done(session) {
	Client.Account.searchForUser(session, dumpuser).then(function(account) {
		console.log(account.id);
		startDump(session, account.id);
	});
}
function startDump(session, userid) {
	var feed = new Client.Feed.UserMedia(session, userid);
	function dumpNext() {
		feed.get().then(function(results) {
			if(results.length<=0) {
				process.exit(0);
				return;
			}
			var stop=false;
			for(var i=0;i<results.length;i++) {
				removeSession(results[i]);
				if(allPosts[results[i]._params.code])
					stop = true;
				allPosts[results[i]._params.code] = true;
			}
			fs.appendFileSync('metadata.json', JSON.stringify(results, null, 4));
			processResults(results, stop?(function(){}):dumpNext);
		});
	}
	dumpNext();
}
function processResults(results, cb) {
	var i=0;
	var err1=null;
	function processNext() {
		if(i>=results.length) {
			cb(err1);
			return;
		}
		console.log("processing result "+i.toString());
		processResult(results[i], function(err) {
			if(err1==null) err1=err;
			if(err != null) {
				fs.appendFileSync('errors.txt', results[i]._params.code);
			}
			i++;
			processNext();
		});
	}
	processNext();
}
function processResult(result, cb) {
	var media = result._params.carouselMedia;
	
	// if carouselMedia is empty, this is a single image/video post
	if(media == null || media.length==0) {
		function downloadCb1(err) {
			if(err!=null) console.log(err);
			cb(err);
		}
		var fn=result._params.code;
		if(result._params.videos != null) {
			var v=pickVideo(result._params.videos);
			download(fn + ".mp4", v.url, downloadCb1);
		} else {
			var img=pickImage(result._params.images);
			download(fn + ".jpg", img.url, downloadCb1);
		}
		return;
	}
	
	// download all images/videos in carouselMedia
	var i=0;
	var err1=null;
	function processNext() {
		if(i>=media.length) {
			cb(err1);
			return;
		}
		
		function downloadCb(err) {
			if(err!=null) {
				console.log(err);
				hasError = true;
			}
			if(err1 == null) err1 = err;
			i++;
			processNext();
		}
		
		var fn=result._params.code + "." + i.toString();
		var m=media[i];
		if(m._params.videos != null) {
			var v=pickVideo(m._params.videos);
			download(fn + ".mp4", v.url, downloadCb);
		} else {
			var img=pickImage(m._params.images);
			download(fn + ".jpg", img.url, downloadCb);
		}
	}
	processNext();
}

//cb: void(err)
function download(fn, url, cb) {
	if(fs.existsSync(fn)) {
		console.log("exists: "+fn);
		if(exitOnExists) process.exit(0);
		setTimeout(cb, 200);
		return;
	}
	console.log(url);
	setTimeout(function() {
		var file = fs.createWriteStream(fn);
		var request = http.get(url, function(response) {
			response.pipe(file);
			file.on('finish', function() {
				cb(null);
			});
		}).on('error', function(err) { // Handle errors
			file.close(cb);
			fs.unlink(dest); // Delete the file async. (But we don't check the result)
			if (cb) cb(err);
		});
	}, 300 + Math.random()*800);
}

function pickImage(images) {
	var maxSize=0;
	var v=null;
	for(var i=0;i<images.length;i++) {
		var s=images[i].width*images[i].height;
		if(s>maxSize || v==null) {
			maxSize = s;
			v = images[i];
		}
	}
	return v;
}
function pickVideo(videos) {
	var maxSize=0;
	var maxType=0;
	var v=null;
	for(var i=0;i<videos.length;i++) {
		var s=videos[i].width*videos[i].height;
		if(s>maxSize || v==null || (s==maxSize && videos[i].type > maxType)) {
			maxSize = s;
			maxType = videos[i].type;
			v = videos[i];
		}
	}
	return v;
}

function removeSession(obj) {
	if(!(obj instanceof Object)) return;
	if(obj instanceof Array) {
		for(var i=0;i<obj.length;i++)
			removeSession(obj[i]);
		return;
	}
	for(var prop in obj) {
		if(prop === '_session' || prop === 'session' || prop === 'Session') delete obj[prop];
		else removeSession(obj[prop]);
	}
}
