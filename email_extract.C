#include <iostream>
#include <cplib/cplib.hpp>
using namespace std;
using namespace xaxaxa;

class email_finder: public Object
{
	enum class states
	{
		reading=0,
		delim_found
	} state;
	void ProcessBuffer(const BufferRef& b)
	{
		
	}
};

int main(int ac, char** av)
{
	StandardStream ss;
	StreamReader sr(ss);
	
	
}

