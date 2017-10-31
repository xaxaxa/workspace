import time,os,sys,random,urlparse,urllib2,traceback
from urllib import quote
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.firefox.firefox_profile import FirefoxProfile
import argparse


parser = argparse.ArgumentParser()
parser.add_argument("-s", "--starturl", help="open browser with this initial url")
parser.add_argument("-p", "--profile", help="use this browser profile directory")
parser.add_argument("-b", "--browser", help="use browser (chrome|firefox)")
args = parser.parse_args()


# configuration


START_URL = args.starturl

firstId = None
lastId = None
done = False

class FBUIException(Exception):
    pass


def findVisibleElementBySelector(css_selector):
	arr = driver.find_elements_by_css_selector(css_selector)
	for e in arr:
		if e.is_displayed():
			return e
	return None

def clickButtonBySelector(css_selector):
	btn = findVisibleElementBySelector(css_selector)
	if btn:
		btn.click()
		return
	raise FBUIException("no visible buttons found for selector")


def clickFbNext():
	"""act = ActionChains(driver)
	
	for i in xrange(10):
		try:
			act.move_to_element(driver.find_element_by_css_selector("img.spotlight")).perform()
			break
		except:
			if i==9: raise
			time.sleep(0.5)
			pass
	"""
	for i in xrange(10):
		try:
			# btn = driver.find_element_by_css_selector(".snowliftPager.next.hilightPager")
			# clickButtonBySelector("img.spotlight")
			clickButtonBySelector(".snowliftPager.next")
			#driver.find_element_by_css_selector("#fbPhotoSnowliftCaption").send_keys(Keys.RIGHT)
			# driver.find_element_by_css_selector("img.spotlight").click()
			break
		except:
			if i==9: raise
			time.sleep(0.5)
			pass

def getFbDownloadLink():
	for i in xrange(10):
		try:
			driver.find_element_by_css_selector("a.fbPhotoSnowliftDropdownButton[data-action-type=\"open_options_flyout\"]").click()
			break
		except:
			if i==9: raise
			time.sleep(0.2)
			pass
	nTries = 5
	for i in xrange(nTries):
		try:
			btns = driver.find_elements_by_css_selector("a._54nc[data-action-type=\"download_photo\"]")
			foundButton = False
			for btn in btns:
				if btn.is_displayed():
					return btn.get_attribute("href")

			if not foundButton:
				raise FBUIException("no visible download buttons found")
			break
		except:
			if i==nTries-1: raise
			time.sleep(0.5)

# returns tuple (url, filename_extension); filename_extension is without the dot
def getFbImageSrc():
	for i in xrange(10):
		try:
			img = findVisibleElementBySelector("div.stage img.spotlight")
			if img: return (img.get_attribute("src"), "jpg")

			vid = findVisibleElementBySelector(".videoStageContainer video")
			if vid: return (vid.get_attribute("src"), "mp4")

			raise FBUIException("no image or video found")
		except:
			if i==9: raise
			time.sleep(0.5)

def serializeCookies(cookieList):
	tmp = ''.join(["%s=%s; " % (str(cook["name"]), str(cook["value"]))
					for cook in cookieList])
	if len(tmp) >= 2:
		tmp = tmp[:-2]
	return tmp

def getFbId(url):
	urlComp = urlparse.urlparse(url)
	q = urlparse.parse_qs(urlComp.query)
	try:
		return q["fbid"][0]
	except:
		pathComp = urlComp.path.split("/")
		for i in xrange(len(pathComp)):
			if pathComp[i] == "photos" or pathComp[i] == "videos":
				if (i + 2) < len(pathComp):
					return pathComp[i + 2]
		raise FBUIException("don't understand the format of the url " + url)

existsAction = None
def doDownload(url, fbid, ext):
	global firstId,lastId,existsAction,done
	if fbid == firstId:
		print "repeat detected (id is equal to the id of the first photo); exiting"
		done = True
		return

	if firstId == None:
		firstId = fbid

	filename = fbid + "." + ext
	
	if os.path.isfile(filename):
		print "%s exists" % filename
		
		cmd = existsAction
		if existsAction == None:
			print "type: s=skip, sa=skip always, x=exit, r=redownload, ra=redownload always"
			cmd = raw_input().strip()

		if cmd == "s":
			return
		if cmd == "sa":
			existsAction = "s"
			return
		if cmd == "x":
			done = True
			return
		if cmd == "r":
			pass
		if cmd == "ra":
			existsAction = "r"

	lastId = fbid

	opener = urllib2.build_opener()
	opener.addheaders = []
	opener.addheaders.append(("Cookie", serializeCookies(driver.get_cookies())))
	opener.addheaders.append(("User-Agent", userAgent))
	f = opener.open(url.replace("https","http"))
	data = f.read()
	
	with open(filename + ".tmp", 'w+') as outputFile:
		outputFile.write(data)
	
	os.rename(filename + ".tmp", filename)


def getChromeDriver():
	options = webdriver.ChromeOptions()
	#options.add_argument("user-data-dir=/home/" + os.environ["USER"] + "/.config/chromium/")
	#options.add_argument("user-data-dir=/home/" + os.environ["USER"] + "/chrome1/")
	options.add_argument("user-data-dir="+args.profile)
	return webdriver.Chrome(chrome_options=options)


def getFirefoxDriver():
	profile = FirefoxProfile(args.profile)
	return webdriver.Firefox(firefox_profile=profile)

if args.browser=='chrome':
	driver = getChromeDriver()
else:
	driver = getFirefoxDriver()

# when finding an element using find_elements_by_*, if not found, wait
# up to seconds until element exists
driver.implicitly_wait(0) # seconds
driver.get(START_URL)
userAgent = driver.execute_script("return navigator.userAgent")

failfile = open("failed.txt", "ab")

pagesSinceRefresh = 0
while True:
	url = None
	ext = "jpg"
	fbid = None
	pagesSinceRefresh += 1
	if pagesSinceRefresh > 50:
		driver.refresh()
		pagesSinceRefresh = 0
	while fbid == None:
		try:
			fbid = getFbId(driver.current_url)
		except Exception, ex:
			traceback.print_exc()
			print "could not get fbid from current url; please manually resolve and hit enter"
			print "OR type the fbid and hit enter"
			fbid1 = raw_input().strip()
			if len(fbid1) > 0:
				fbid = fbid1
	try:
		url = getFbDownloadLink()
	except Exception, ex:
		print "error getting download link: ", ex
		print "using <img> src instead"
		try:
			url, ext = getFbImageSrc()
		except Exception, ex2:
			traceback.print_exc()

	if url != None:
		print url
		try:
			doDownload(url, fbid, ext)
		except Exception, ex:
			traceback.print_exc()
			print "download failed: " + url
			failfile.write(fbid + '\n')
			failfile.flush()
	else:
		failfile.write(fbid + "\n")
		failfile.flush()

	if done:
		break

	try:
		for x in xrange(2):
			clickFbNext()
			if fbid != getFbId(driver.current_url): break
			driver.refresh();

		if fbid==getFbId(driver.current_url):
			raise FBUIException("fbid from the url is still the same after clicking next")
	except Exception, ex:
		traceback.print_exc()
		print "clicking next failed; please manually click next and hit enter"
		raw_input()
		print "continuing"
	time.sleep(0.1 + abs(random.gauss(0,0.5)))

print "done"

driver.close()

