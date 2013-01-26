#include <iostream>
#include <fcgiapp.h>


class ParserException: public std::exception
{
public:
	string message;
	int32_t number;
	ParserException();
	ParserException(int32_t number);
	ParserException(string message, int32_t number = 0);
	~ParserException() throw ();
	const char* what() const throw ();
};
ParserException::ParserException() :
		message(strerror(errno)), number(errno) {
}
ParserException::ParserException(int32_t number) :
		message(strerror(number)), number(number) {
}
ParserException::ParserException(string message, int32_t number) :
		message(message), number(number) {
}
ParserException::~ParserException() throw () {
}
const char* ParserException::what() const throw () {
	return message.c_str();
}


void doWriteCPPHead(string& out) {
	out+="int cppsp_main(FCGX_Stream *in, *out) {";
}
//translates a c++ web page into c++
void doParse(const string& in, string& out) {
	const char* s=in.data();
	const char* end=s+in.length();
	int out_initlen=out.length();
	while(true) {
		s=memmem(s,in.length(),"<%",2);
		if(s==NULL)break;
		if((++s)>=end)throw ParserException("reached EOF when looking past \"<%\"");
		switch(*s) {
			case '#':
			{
				out.resize(out_initlen);
				const char* s1=memmem(s,end-s,"%>",2);
				if(s1==NULL)
					throw ParserException("reached EOF when looking for matching \"%>\"");
				
				out.append(s,s1-s);
			}
		}
	}
}

int main(int argc, char** argv) {
	FCGX_Stream *in, *out, *err;
    FCGX_ParamArray envp;
    while (FCGX_Accept(&in, &out, &err, &envp) >= 0) {
		FCGX_FPrintF(out, "Content-type: text/plain\r\n\r\n");
		for(char** s=envp; *s!=NULL; s++) {
			FCGX_FPrintF(out, "%s\n", *s);
		}
	}
}
