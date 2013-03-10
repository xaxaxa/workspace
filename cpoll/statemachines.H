/*
 * statemachines.H
 *
 *  Created on: Feb 1, 2013
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
#ifndef STATEMACHINES_H_
#define STATEMACHINES_H_
#include <functional>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdexcept>
#include <delegate.H>

#ifndef likely
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#endif

using namespace std;
namespace CP
{
	//streamReader C-style API/ABI; this is to avoid breaking the ABI when the
	//internal state machine structure changes
	//usage example:
	//streamReader* sr=(streamReader*)malloc(streamReader_getSize()+4096);
	//streamReader_init(sr, 4096);
	// ...
	//streamReader_deinit(sr);
	struct streamReader;
	int streamReader_getSize();
	void streamReader_init(streamReader* sr, int capacity);
	void streamReader_deinit(streamReader* sr);
	tuple<uint8_t*, int> streamReader_beginPutData(streamReader* sr);
	void streamReader_endPutData(streamReader* sr, int len);
	void streamReader_readUntilString(streamReader* sr, string delim);
	void streamReader_readUntilChar(streamReader* sr, char delim);
	void streamReader_setCallback(streamReader* sr, const function<void(uint8_t*, int, bool)>& cb);
	tuple<uint8_t*, int> streamReader_getBufferData(streamReader* sr);
	void streamReader_reset(streamReader* sr);

	//another streamReader implementation that remembers all data that has ever been fed into it;
	//mainly for socketd
	struct persistentStreamReader
	{
		uint8_t* buffer;
		int capacity;
		int len;
		int pos;
		int searchPos;
		int state; //0: none; 1: readUntilString; 2: readUntilChar
		//function<void(uint8_t*, int)> output;
		Delegate<void(uint8_t*, int)> output;
		const char* delim1;
		int delim1_len;
		char delim2;
		inline void clearBuffer() {
			searchPos = pos = len;
			//delayProcessing = false;
		}
		void resize(int nc) {
			uint8_t* tmp = (uint8_t*) realloc(buffer, nc);
			if (tmp == NULL) throw runtime_error(strerror(errno));
			buffer = tmp;
		}
		void ensureCapacity(int c) {
			if (likely(c < capacity)) return;
			int nc = capacity;
			while (nc < c)
				nc *= 2;
			if (nc != capacity) resize(nc);
		}
		persistentStreamReader(int c = 4096) :
				capacity(c), len(0), pos(0), searchPos(0), state(0) {
			buffer = (uint8_t*) malloc(c);
			if (buffer == NULL) throw runtime_error(strerror(errno));
		}
		~persistentStreamReader() {
			free(buffer);
		}
		void process() {
			if (len <= searchPos) return;
			switch (state) {
				case 0:
					break;
				case 1:
				{
					uint8_t* tmp = (uint8_t*) memmem(buffer + searchPos, len - searchPos, delim1,
							delim1_len);
					if (tmp == NULL) {
						//overlap the search so that delimitors that are cut in half at
						//the end of the buffer can be caught
						searchPos = len - delim1_len;
					} else {
						int oldPos = pos;
						pos = searchPos = (tmp - buffer) + delim1_len;
						state = 0;
						output(buffer + oldPos, tmp - buffer - oldPos);
					}
					break;
				}
				case 2:
				{
					uint8_t* tmp = (uint8_t*) memchr(buffer + searchPos, delim2, len - searchPos);
					if (tmp == NULL) {
						searchPos = len;
					} else {
						int oldPos = pos;
						pos = searchPos = (tmp - buffer) + 1;
						state = 0;
						output(buffer + oldPos, tmp - buffer - oldPos);
					}
					break;
				}
			}
		}
		void readUntilString(const char* delim, int len) {
			state = 1;
			delim1 = delim;
			delim1_len = len;
			//printf("%i\n",delim.length());
			process();
		}
		void readUntilChar(char delim) {
			state = 2;
			delim2 = delim;
			process();
		}
		uint8_t* beginPutData(int len) {
			ensureCapacity(this->len + len);
			return buffer + this->len;
		}
		void endPutData(int len) {
			this->len += len;
			if (len > 0) process();
		}
		inline tuple<uint8_t*, int> getBufferData() {
			return make_tuple(buffer + pos, len - pos);
		}
		inline tuple<uint8_t*, int> getHistory(bool includeUnprocessed = true) {
			return make_tuple(buffer, includeUnprocessed ? len : pos);
		}
	};
}

#endif /* STATEMACHINES_H_ */
