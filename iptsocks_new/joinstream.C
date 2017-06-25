/*
 * joinstream.C
 *
 *  Created on: Mar 17, 2013
 *      Author: xaxaxa
 */
/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */
#include <cpoll/cpoll.H>
#include <cpoll/delegate.H>
using namespace CP;
static const int JoinStream_bufSize = 1024 * 32;
struct JoinStream
{
	uint8_t buf1to2[JoinStream_bufSize];
	uint8_t buf2to1[JoinStream_bufSize];
	RGC::Ref<Stream> s1;
	RGC::Ref<Stream> s2;
	Delegate<void(JoinStream&, uint8_t* data, int& len)> from1to2;
	Delegate<void(JoinStream&, uint8_t* data, int& len)> from2to1;
	bool* deletionFlag;
	char cbState; //1: 1to2; 2: 2to1
	bool stopCurrent;

	JoinStream() :
			deletionFlag(NULL), cbState(0) {
	}
	~JoinStream() {
		if (deletionFlag != NULL) *deletionFlag = true;
	}
	void stop() {
	}
	void closed(int i) {
		stop();
	}
	void start() {
		read1();
		read2();
	}
	void read1() {
		s1->read(buf1to2, JoinStream_bufSize, Callback(&JoinStream::read1cb, this));
	}
	void read2() {
		s2->read(buf2to1, JoinStream_bufSize, Callback(&JoinStream::read2cb, this));
	}
	void read1cb(int r) {
		stopCurrent = false;
		if (from1to2 != nullptr) {
			cbState = 1;
			bool d(false);
			deletionFlag = &d;
			from1to2(*this, buf1to2, r);
			if (d) return;
			deletionFlag = NULL;
			cbState = 0;
		}
		if (r <= 0 || stopCurrent) {
			return;
		}
		write1(r);
	}
	void read2cb(int r) {
		stopCurrent = false;
		if (from2to1 != nullptr) {
			cbState = 2;
			bool d(false);
			deletionFlag = &d;
			from2to1(*this, buf2to1, r);
			if (d) return;
			deletionFlag = NULL;
			cbState = 0;
		}
		if (r <= 0 || stopCurrent) {
			return;
		}
		write2(r);
	}
	void write1(int i) {
		s2->writeAll(buf1to2, i, Callback(&JoinStream::write1cb, this));
	}
	void write2(int i) {
		s1->writeAll(buf2to1, i, Callback(&JoinStream::write2cb, this));
	}
	void write1cb(int r) {
		if (r <= 0) {
			stop();
			return;
		}
		read1();
	}
	void write2cb(int r) {
		if (r <= 0) {
			stop();
			return;
		}
		read2();
	}
	void stop1to2() {
		if (cbState == 1) stopCurrent = true;
		else s1->cancelRead();
	}
	void stop2to1() {
		if (cbState == 2) stopCurrent = true;
		else s2->cancelRead();
	}
};

