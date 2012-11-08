#include <iostream>
#include <functional>
using namespace std;

namespace AsyncHelpers
{
	struct AsyncReader
	{
		const char* delim;
		int delimLen;
		function<void()> callback;
		void init(const char* delim, int len) {
			this->delim=delim;
			this->delimLen=len;
		}
		AsyncReader(const char* delim, int len) {
			Init(delim, len);
		}
		AsyncReader(const char* delim) {
			Init(delim, strlen(delim));
		}
		void putData(const char* data, int len) {
			
		}
	};
}
