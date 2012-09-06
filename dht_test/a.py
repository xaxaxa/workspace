#! /usr/bin/env python
#
# This library is free software, distributed under the terms of
# the GNU Lesser General Public License Version 3, or any later version.
# See the COPYING file included in this archive
#


import pygtk, urllib2, os
pygtk.require('2.0')
import sys, gtk, gobject, cairo
import math

from twisted.internet import gtk2reactor
#import entangled.kademlia.protocol
gtk2reactor.install()
import twisted.internet.reactor
import entangled.dtuple

import entangled.kademlia.contact
import entangled.kademlia.msgtypes

import hashlib

class EntangledViewer(gtk.DrawingArea):
	def __init__(self, node, *args, **kwargs):
		gtk.DrawingArea.__init__(self, *args, **kwargs)
		self.node = node
		self.timeoutID = gobject.timeout_add(5000, self.timeout)
		self.comms = {}
		self.incomingComms = {}
		# poison the node with our GUI hooks
		self.node._protocol.__gui = self
		self.node._protocol.__realSendRPC = self.node._protocol.sendRPC
		self.node._protocol.sendRPC = self.__guiSendRPC
	
		self.node._protocol.__realDatagramReceived = self.node._protocol.datagramReceived
		self.node._protocol.datagramReceived = self.__guiDatagramReceived
		self.msgCounter = 0
		self.printMsgCount = False
		
	def __guiSendRPC(self, contact, method, args, rawResponse=False):
		#print 'sending'
		self.drawComms(contact.id, method)
		self.msgCounter += 1
		return self.node._protocol.__realSendRPC(contact, method, args, rawResponse)
	
	def __guiDatagramReceived(self, datagram, address):
		msgPrimitive = self.node._protocol._encoder.decode(datagram)
		message = self.node._protocol._translator.fromPrimitive(msgPrimitive)
		if isinstance(message, entangled.kademlia.msgtypes.ErrorMessage):
			msg = 'error'
		elif isinstance(message, entangled.kademlia.msgtypes.ResponseMessage):
			msg = 'response'
		else:
			msg = message.request
		self.drawIncomingComms(message.nodeID, msg)
		return self.node._protocol.__realDatagramReceived(datagram, address)
	
	# Draw in response to an expose-event
	__gsignals__ = { "expose-event": "override" }
	
	# Handle the expose-event by drawing
	def do_expose_event(self, event):
		# Create the cairo context
		cr = self.window.cairo_create()
		# Restrict Cairo to the exposed area; avoid extra work
		cr.rectangle(event.area.x, event.area.y,
				event.area.width, event.area.height)
		cr.clip()

		self.draw(cr, *self.window.get_size())
	
	def draw(self, cr, width, height):
		# draw a rectangle for the background			
		cr.set_source_rgb(1.0, 1.0, 1.0)
		cr.rectangle(0, 0, width, height)
		cr.fill()

		# a circle for the local node
		cr.set_source_rgb(1.0, 0.0, 0.0)
		radius = min(width/5, height/5)
		
		w = width/2
		h = height/2
		s = radius / 2.0 - 20
		radial = cairo.RadialGradient(w/2, h/2, s, w+w/2, h+h/2, s)
		radial.add_color_stop_rgb(0,  0.6, 0, 0.2)
		radial.add_color_stop_rgb(1,  0.1, 0.2, 0.9)
		
		cr.arc(w, h, s, 0, 2 * math.pi)
		cr.set_source(radial)
		cr.fill()
		
		if len(self.comms):
			cr.set_line_width(5)
			cr.set_source_rgba(0, 0.7, 0.8, 0.5)
		else:
			cr.set_source_rgba(0.0, 0.0, 0.4, 0.7)
		cr.arc(w, h, s+1, 0, 2 * math.pi)
		
		cr.stroke()
		cr.set_line_width(2)
		
		blips = []
		kbucket = {}
		for i in range(len(self.node._routingTable._buckets)):
			for contact in self.node._routingTable._buckets[i]._contacts:	
				blips.append(contact)
				kbucket[contact.id] = i
		# ...and now circles for all the other nodes
		if len(blips) == 0:
			spacing = 180
		else:
			spacing = 360/(len(blips))
		degrees = 0
		radius = min(width/6, height/6) / 3 - 20
		if radius < 5:
			radius = 5
		r = width/2.5
		for blip in blips:
			x = r * math.cos(degrees * math.pi/180)
			y = r * math.sin(degrees * math.pi/180)	

			w = width/2 + x
			h = height/2 + y
			if w < 0:
				w = radius
			elif w > width:
				w = width-radius
			if h < 0:
				h = radius
			elif h > height:
				h = height - radius
				

			radial = cairo.RadialGradient(w-w/2, h-h/2, 5, w+w/2, h+h/2, 10)
			radial.add_color_stop_rgb(0,  0.4, 1, 0)
			radial.add_color_stop_rgb(1,  1, 0, 0)
			cr.arc(w, h, radius, 0, 2 * math.pi)
			cr.set_source(radial)
			cr.fill()
			
			cr.set_source_rgb(0.2,0.2,0.2)
			cr.set_font_size(12.0)
			cr.move_to(w+radius+5, h-10)
			cr.set_font_size(12.0)
			cr.show_text(blip.address)
			cr.move_to(w+radius+5, h+5)
			cr.show_text(str(blip.port))
			cr.set_source_rgb(1,1,1)
			
			cr.set_font_size(8.0)
			cr.set_source_rgb(0.4,0.4,0.4)
			cr.move_to(w+radius+5, h+17)
			cr.show_text('k-bucket: %d' % kbucket[blip.id])
			cr.set_font_size(14.0)
			cr.stroke()
			
			if blip.id in self.incomingComms:
				cr.set_source_rgba(0.8, 0.0, 0.0, 0.6) 
				cr.move_to(width/2, height/2)
				cr.line_to(w, h)
				cr.stroke()
				
				cr.move_to(width/2+x/3, height/2+y/3)
				cr.show_text(self.incomingComms[blip.id])
				cr.stroke()
				cr.set_line_width(5)
			
			else:
				cr.set_source_rgba(0.4, 0.0, 0.0, 0.7)
				
			cr.arc(w, h, radius+1, 0, 2 * math.pi)
			cr.stroke()
			 
			if blip.id in self.comms:
				cr.set_line_width(5)
				cr.set_source_rgba(0.0, 0.7, 0.8, 0.4)
				cr.move_to(width/2, height/2)
				cr.line_to(w, h)
				cr.stroke()
				
				cr.set_source_rgba(0.0, 0.3, 0.8, 0.7)
				cr.move_to(width/2+x/1.2, height/2+y/1.2)
				cr.show_text(self.comms[blip.id])
				cr.stroke()
			cr.set_line_width(2)
			degrees += spacing
		
		cr.set_line_width(5)
		cr.set_source_rgba(0.6, 0.6, 0.6, 0.4)
		i = 0
		for lostComm in self.comms:
			if lostComm not in blips:
				cr.move_to(width/2, height/2)
				cr.line_to(100*i, 0)
				cr.stroke()
			i += 1
		
		if self.printMsgCount == True:
			cr.set_source_rgb(0.2,0.2,0.2)
			cr.set_font_size(12.0)
			cr.move_to(20, 20)
			cr.show_text('Messages sent: %d' % self.msgCounter)
			cr.stroke()
		

	def timeout(self):
		""" Timeout handler to update the GUI """
		#print 'timeout'
		self.window.invalidate_rect(self.allocation, False)
		return True
	
	def drawMsgCounter(self):
		self.printMsgCount = True
		gobject.timeout_add(3000, self.removeMsgCount)
		
	def drawComms(self, contactID, msg):
		if contactID not in self.comms:
			self.comms[contactID] = msg
			gobject.timeout_add(750, self.removeComm, contactID)
			self.window.invalidate_rect(self.allocation, False)
	
	def drawIncomingComms(self, contactID, msg):
		if contactID not in self.incomingComms:
			self.incomingComms[contactID] = msg
			gobject.timeout_add(750, self.removeIncomingComm, contactID)
			self.window.invalidate_rect(self.allocation, False)
	
	def removeIncomingComm(self, contactID):
		try:
			del self.incomingComms[contactID]
		finally:
			self.window.invalidate_rect(self.allocation, False)
			return False
	
	def removeComm(self, contactID):
		try:
			del self.comms[contactID]
		finally:
			self.window.invalidate_rect(self.allocation, False)
			return False
		
	def removeMsgCount(self):
		if self.printMsgCount == True:
			self.printMsgCount = False
			self.msgCounter = 0
			self.window.invalidate_rect(self.allocation, False)
		return False
		
	

class EntangledViewerWindow(gtk.Window):
	def __init__(self, node):
		gtk.Window.__init__(self)
		
		self.node = node
		self.connect("delete-event", gtk.main_quit)
		
		# Layout the window
		vbox = gtk.VBox(spacing=3)
		self.add(vbox)
		vbox.show()
	
		# Add the view screen
		self.viewer = EntangledViewer(node)
		self.viewer.show()
		vbox.pack_start(self.viewer)
	
		# Add the controls
		notebook = gtk.Notebook()
		notebook.set_tab_pos(pos=gtk.POS_TOP)
		notebook.show()
		vbox.pack_start(notebook,expand=False, fill=False)
		
		
		#notebook.append_page(frame, gtk.Label('Low-level DHT controls'))
		lowlevelVbox = gtk.VBox(spacing=3)
		lowlevelVbox.show()
 
		#frame.add(kademliaTabVbox)
		notebook.append_page(lowlevelVbox, gtk.Label('Low-level DHT controls'))

		frame = gtk.Frame()
		frame.set_label('Basic Kademlia')
		frame.show()
		lowlevelVbox.pack_start(frame)

		kademliaTabVbox = gtk.VBox(spacing=3)
		kademliaTabVbox.show()
		frame.add(kademliaTabVbox)

		# Store
		hbox = gtk.HBox(False, 8)
		hbox.show()
		label = gtk.Label("Key:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		entryKey = gtk.Entry()
		hbox.pack_start(entryKey, expand=True, fill=True)
		entryKey.show()
		label = gtk.Label("Value:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		entryValue = gtk.Entry()
		hbox.pack_start(entryValue, expand=True, fill=True)
		entryValue.show()
		button = gtk.Button('Store')
		hbox.pack_start(button, expand=False, fill=False)
		button.connect("clicked", self.storeValue, entryKey.get_text, entryValue.get_text)
		button.show()
		kademliaTabVbox.pack_start(hbox, expand=False, fill=False)
		
		# Find value
		hbox = gtk.HBox(False, 8)
		hbox.show()
		label = gtk.Label("Key:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		entryKey = gtk.Entry()
		hbox.pack_start(entryKey, expand=True, fill=True)
		entryKey.show()
		label = gtk.Label("Value:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		labelValue = gtk.Label('---unknown---')
		hbox.pack_start(labelValue, expand=True, fill=True)
		labelValue.show()
		button = gtk.Button('Retrieve')
		hbox.pack_start(button, expand=False, fill=False)
		button.connect("clicked", self.getValue, entryKey, labelValue.set_text)
		button.show()
		kademliaTabVbox.pack_start(hbox, expand=False, fill=False)
	
		################# "Entangled DHT"-specific stuff ####################
		#frame = gtk.Frame()
		#frame.set_label('Store keyword-searchable data in the DHT')
		#frame.show()
		#notebook.append_page(frame, gtk.Label('Entangled Extensions'))
		highlevelVbox = gtk.VBox(spacing=3)
		highlevelVbox.show()
		#frame.add(entangledTabVbox)
		notebook.append_page(highlevelVbox, gtk.Label('High-level DHT controls'))
		
		# Publish (with indexing)
		hbox = gtk.HBox(False, 8)
		hbox.show()
		label = gtk.Label("Full Name:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		entryName = gtk.Entry()
		hbox.pack_start(entryName, expand=True, fill=True)
		entryName.show()
		label = gtk.Label("Value:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		entryValue2 = gtk.Entry()
		hbox.pack_start(entryValue2, expand=True, fill=True)
		entryValue2.show()
		button = gtk.Button('Publish')
		hbox.pack_start(button, expand=False, fill=False)
		button.connect("clicked", self.publishData, entryName.get_text, entryValue2.get_text)
		button.show()
		highlevelVbox.pack_start(hbox, expand=False, fill=False)
	
		# Search for keyword
		hbox = gtk.HBox(False, 8)
		hbox.show()
		label = gtk.Label("Keyword Search:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		entryKeyword = gtk.Entry()
		hbox.pack_start(entryKeyword, expand=True, fill=True)
		entryKeyword.show()
		label = gtk.Label("Hits:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		labelValue2 = gtk.Label('---unknown---')
		labelValue2.set_selectable(True)
		hbox.pack_start(labelValue2, expand=True, fill=True)
		labelValue2.show()
		button = gtk.Button('Search')
		hbox.pack_start(button, expand=False, fill=False)
		button.connect("clicked", self.searchForKeywords, entryKeyword, labelValue2.set_text)
		button.show()
		highlevelVbox.pack_start(hbox, expand=False, fill=False)

		# Remove data
		hbox = gtk.HBox(False, 8)
		hbox.show()
		label = gtk.Label("Full Name:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		entryName = gtk.Entry()
		hbox.pack_start(entryName, expand=True, fill=True)
		entryName.show()
		button = gtk.Button('Remove')
		hbox.pack_start(button, expand=False, fill=False)
		button.connect("clicked", self.removeData, entryName.get_text)
		button.show()
		highlevelVbox.pack_start(hbox, expand=False, fill=False)


		frame = gtk.Frame()
		frame.set_label('Entangled Extensions')
		frame.show()
		lowlevelVbox.pack_start(frame)

		entangledTabVbox = gtk.VBox(spacing=3)
		entangledTabVbox.show()
		frame.add(entangledTabVbox)
		
		# Delete (direct Kademlia extension - shown on first page in GUI, along with Kademlia RPCs)
		hbox = gtk.HBox(False, 8)
		hbox.show()
		label = gtk.Label("Key:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		entryKey = gtk.Entry()
		hbox.pack_start(entryKey, expand=True, fill=True)
		entryKey.show()
		button = gtk.Button('Delete')
		hbox.pack_start(button, expand=False, fill=False)
		button.connect("clicked", self.deleteValue, entryKey.get_text)
		button.show()
		entangledTabVbox.pack_start(hbox, expand=False, fill=False)
		
		################# "Entangled Tuple Space"-specific stuff ####################
		#frame = gtk.Frame()
		#frame.set_label('Store keyword-searchable data in the DHT')
		#frame.show()
		#notebook.append_page(frame, gtk.Label('Entangled Extensions'))
		tupleSpaceVbox = gtk.VBox(spacing=3)
		tupleSpaceVbox.show()
		#frame.add(entangledTabVbox)
		notebook.append_page(tupleSpaceVbox, gtk.Label('Entangled Tuple Space'))
		
		# Put
		hbox = gtk.HBox(False, 8)
		hbox.show()
		label = gtk.Label("Tuple:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		entryTuplePut = gtk.Entry()
		hbox.pack_start(entryTuplePut, expand=True, fill=True)
		entryTuplePut.show()
		button = gtk.Button('Put')
		hbox.pack_start(button, expand=False, fill=False)
		button.connect("clicked", self.putTuple, entryTuplePut.get_text)
		button.show()
		tupleSpaceVbox.pack_start(hbox, expand=False, fill=False)
	
		# Get
		hbox = gtk.HBox(False, 8)
		hbox.show()
		label = gtk.Label("Template:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		entryTemplate = gtk.Entry()
		hbox.pack_start(entryTemplate, expand=True, fill=True)
		entryTemplate.show()
		label = gtk.Label("Consumed tuple:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		labelValueTuple = gtk.Label('---unknown---')
		labelValueTuple.set_selectable(True)
		hbox.pack_start(labelValueTuple, expand=True, fill=True)
		labelValueTuple.show()
		button = gtk.Button('Get')
		hbox.pack_start(button, expand=False, fill=False)
		button.connect("clicked", self.getTuple, entryTemplate, labelValueTuple.set_text, True)
		button.show()
		button = gtk.Button('Get (non-blocking)')
		hbox.pack_start(button, expand=False, fill=False)
		button.connect("clicked", self.getTuple, entryTemplate, labelValueTuple.set_text, False)
		button.show()
		tupleSpaceVbox.pack_start(hbox, expand=False, fill=False)
		
		# Read
		hbox = gtk.HBox(False, 8)
		hbox.show()
		label = gtk.Label("Template:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		entryTemplate2 = gtk.Entry()
		hbox.pack_start(entryTemplate2, expand=True, fill=True)
		entryTemplate2.show()
		label = gtk.Label("Read tuple:")
		hbox.pack_start(label, False, False, 0)
		label.show()
		labelValueTuple2 = gtk.Label('---unknown---')
		labelValueTuple2.set_selectable(True)
		hbox.pack_start(labelValueTuple2, expand=True, fill=True)
		labelValueTuple2.show()
		button = gtk.Button('Read')
		hbox.pack_start(button, expand=False, fill=False)
		button.connect("clicked", self.readTuple, entryTemplate2, labelValueTuple2.set_text, True)
		button.show()
		button = gtk.Button('Read (non-blocking)')
		hbox.pack_start(button, expand=False, fill=False)
		button.connect("clicked", self.readTuple, entryTemplate2, labelValueTuple2.set_text, False)
		button.show()
		tupleSpaceVbox.pack_start(hbox, expand=False, fill=False)
		
		
		

	def publishData(self, sender, nameFunc, valueFunc):
		name = nameFunc()
		value = valueFunc()
		self.viewer.msgCounter = 0
		self.viewer.printMsgCount = False
		def completed(result):
			self.viewer.printMsgCount = True
		df = self.node.publishData(name, value)
		df.addCallback(completed)
		
	def storeValue(self, sender, keyFunc, valueFunc):
		key = keyFunc()
		
		h = hashlib.sha1()
		h.update(key)
		hKey = h.digest()
		
		value = valueFunc()
		
		self.viewer.msgCounter = 0
		self.viewer.printMsgCount = False
		
		def completed(result):
			self.viewer.printMsgCount = True
		df = self.node.iterativeStore(hKey, value)
		df.addCallback(completed)

	def getValue(self, sender, entryKey, showFunc):
		sender.set_sensitive(False)
		key = entryKey.get_text()
		entryKey.set_sensitive(False)
		h = hashlib.sha1()
		h.update(key)
		hKey = h.digest()
		self.viewer.msgCounter = 0
		self.viewer.printMsgCount = False
		def showValue(result):
			sender.set_sensitive(True)
			entryKey.set_sensitive(True)
			if type(result) == dict:
				value = result[hKey]
				if type(value) != str:
					value = '%s: %s' % (type(value), str(value))
			else:
				value = '---not found---'
			showFunc(value)
			self.viewer.printMsgCount = True
		def error(failure):
			print 'GUI: an error occurred:', failure.getErrorMessage()
			sender.set_sensitive(True)
			entryKey.set_sensitive(True)
		
		df = self.node.iterativeFindValue(hKey)
		df.addCallback(showValue)
		df.addErrback(error)
	
	def deleteValue(self, sender, keyFunc):
		key = keyFunc()
		
		h = hashlib.sha1()
		h.update(key)
		hKey = h.digest()
		self.viewer.msgCounter = 0
		self.viewer.printMsgCount = False
		def completed(result):
			self.viewer.printMsgCount = True
		df = self.node.iterativeDelete(hKey)
		df.addCallback(completed)
		
	def searchForKeywords(self, sender, entryKeyword, showFunc):
		sender.set_sensitive(False)
		keyword = entryKeyword.get_text()
		entryKeyword.set_sensitive(False)
		self.viewer.msgCounter = 0
		self.viewer.printMsgCount = False
		def showValue(result):
			sender.set_sensitive(True)
			entryKeyword.set_sensitive(True)
			sourceListString = ''
			for name in result:
				sourceListString += '%s\n' % name
			result = sourceListString[:-1]
			showFunc(result)
			self.viewer.printMsgCount = True
		def error(failure):
			print 'GUI: an error occurred:', failure.getErrorMessage()
			sender.set_sensitive(True)
			entryKeyword.set_sensitive(True)
		
		df = self.node.searchForKeywords(keyword)
		df.addCallback(showValue)
		df.addErrback(error)
		
	def removeData(self, sender, nameFunc):
		name = nameFunc()
		self.viewer.msgCounter = 0
		self.viewer.printMsgCount = False
		def completed(result):
			self.viewer.printMsgCount = True
		df = self.node.removeData(name)
		df.addCallback(completed)

	###### Tuple Space Controls ######
	def putTuple(self, sender, tupleFunc):
		dTuple = self._tupleFromStr(tupleFunc())
		if dTuple == None:
			return
		self.viewer.msgCounter = 0
		self.viewer.printMsgCount = False
		def completed(result):
			self.viewer.printMsgCount = True
		df = self.node.put(dTuple)
		df.addCallback(completed)
		
	def _tupleFromStr(self, text):
		tp = None
		try:
			exec 'tp = %s' % text
			if type(tp) != tuple:
				raise Exception
		except Exception:
			dialog = gtk.MessageDialog(self, gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT,
									   gtk.MESSAGE_ERROR, gtk.BUTTONS_OK,
										"Please enter a valid Python tuple,\ne.g. (1, 'abc', 3.14)")
			dialog.set_title('Error')
			dialog.run()
			dialog.destroy()
		finally:
			return tp
		
	def getTuple(self, sender, entryTemplate, showFunc, blocking):
		template = self._tupleFromStr(entryTemplate.get_text())
		if template == None:
			return
		sender.set_sensitive(False)
		entryTemplate.set_sensitive(False)
		self.viewer.msgCounter = 0
		self.viewer.printMsgCount = False
		def showValue(result):
			sender.set_sensitive(True)
			entryTemplate.set_sensitive(True)
			if result == None:
				result = '---no matching tuple found---'
			else:
				result = str(result)
			showFunc(result)
			self.viewer.printMsgCount = True
		def error(failure):
			print 'GUI: an error occurred:', failure.getErrorMessage()
			sender.set_sensitive(True)
			entryTemplate.set_sensitive(True)
		
		if blocking == True:
			df = self.node.get(template)
		else:
			df = self.node.getIfExists(template)
		df.addCallback(showValue)
		df.addErrback(error)
		
	def readTuple(self, sender, entryTemplate, showFunc, blocking):
		template = self._tupleFromStr(entryTemplate.get_text())
		if template == None:
			return
		sender.set_sensitive(False)
		entryTemplate.set_sensitive(False)
		self.viewer.msgCounter = 0
		self.viewer.printMsgCount = False
		def showValue(result):
			sender.set_sensitive(True)
			entryTemplate.set_sensitive(True)
			if result == None:
				result = '---no matching tuple found---'
			else:
				result = str(result)
			showFunc(result)
			self.viewer.printMsgCount = True
		def error(failure):
			print 'GUI: an error occurred:', failure.getErrorMessage()
			sender.set_sensitive(True)
			entryTemplate.set_sensitive(True)
		
		if blocking == True:
			df = self.node.read(template)
		else:
			df = self.node.readIfExists(template)
		df.addCallback(showValue)
		df.addErrback(error)


if __name__ == '__main__':
	'''if len(sys.argv) < 2:
		print 'Usage:\n%s UDP_PORT  [KNOWN_NODE_IP  KNOWN_NODE_PORT]' % sys.argv[0]
		print 'or:\n%s UDP_PORT  [FILE_WITH_KNOWN_NODES]' % sys.argv[0]
		print '\nIf a file is specified, it should containg one IP address and UDP port\nper line, seperated by a space.'
		sys.exit(1)
	try:
		int(sys.argv[1])
	except ValueError:
		print '\nUDP_PORT must be an integer value.\n'
		print 'Usage:\n%s UDP_PORT  [KNOWN_NODE_IP  KNOWN_NODE_PORT]' % sys.argv[0]
		print 'or:\n%s UDP_PORT  [FILE_WITH_KNOWN_NODES]' % sys.argv[0]
		print '\nIf a file is specified, it should contain one IP address and UDP port\nper line, seperated by a space.'
		sys.exit(1)
	
	if len(sys.argv) == 4:
		knownNodes = [(sys.argv[2], int(sys.argv[3]))]
	elif len(sys.argv) == 3:
		knownNodes = []
		f = open(sys.argv[2], 'r')
		lines = f.readlines()
		f.close()
		for line in lines:
			ipAddress, udpPort = line.split()
			knownNodes.append((ipAddress, int(udpPort)))
	else:
		knownNodes = None'''
	port=4069;
	boot_url="http://xa.us.to/dht_boot?port="+str(port); # bootstrap server
	os.system("upnpc -r "+str(port)+" udp >/dev/null 2>&1 &");
	knownNodes=[];
	req=urllib2.urlopen(boot_url);
	for l in req:
		try:
			a=l.strip().split(" ");
			knownNodes.append((a[0], int(a[1])));
		except: pass
	
	node = entangled.dtuple.DistributedTupleSpacePeer( port )
	
	window = EntangledViewerWindow(node)

	window.set_default_size(640, 640)
	window.set_title('Entangled Viewer - DHT on port %s' % port)

	window.present()
	
	node.joinNetwork(knownNodes)
	twisted.internet.reactor.run()



