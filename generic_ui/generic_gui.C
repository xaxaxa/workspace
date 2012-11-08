/*
 * generic_gui.C
 *
 *  Created on: 2012-11-04
 *      Author: xaxaxa
 */

#include "generic_gui.H"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
namespace GenericUI
{
	struct xmlcb
	{
		function<int(int8_t*, int)> cb;
		xmlcb(const function<int(int8_t*, int)>& cb) :
				cb(cb) {

		}
	};
	int xmlcb1(void* ctx, char* buf, int len) {
		xmlcb* cb1 = (xmlcb*) ctx;
		return cb1->cb((int8_t*) buf, len);
	}
	int xmlcb2(void* ctx) {
		xmlcb* cb1 = (xmlcb*) ctx;
		delete cb1;
		return 0;
	}

	void GenericGUI::load(const function<int(int8_t*, int)>& cb) {
		LIBXML_TEST_VERSION
		xmlcb* cb1 = new xmlcb(cb);
		xmlDoc* doc = xmlReadIO(xmlcb1, xmlcb2, cb1, "stream://none", NULL, 0);
		parse(xmlDocGetRootElement(doc));
	}
	
	void GenericGUI::load(const char* file) {
		int fd = open(file, O_RDONLY);
		if (fd < 0) throw Exception(strerror(errno));
		load([fd](int8_t* buf, int len) ->int {return read(fd, buf, len);});
		close(fd);
	}

}
