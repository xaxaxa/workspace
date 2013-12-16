#include <stdio.h>
#include "htmlparser.H"
#include <string>
#include <unistd.h>

using namespace std;
int main() {
	//const char* s="<html><a href='sss' />x<b>zxc</b><![CDATA[<a<<>>>><<>]]></html>";
	HTMLParser p;
	p.reset();
	p.buffer=(char*)malloc(8192);
	p.bufferLength=0;
	int bufferSize=8192;
	
	while(true) {
		//make sure there is at least 4KB of leftover space in the buffer
		if(bufferSize<p.bufferLength+4096) {
			bufferSize*=2;
			p.buffer=(char*)realloc(p.buffer,bufferSize);
		}
		int br=read(0,p.buffer+p.bufferLength,bufferSize-p.bufferLength);
		if(!(p.eof=(br<=0))) p.bufferLength+=br;
		while(p.read()) {
			string tmp(p.tokenInfo.data,p.tokenInfo.dataLen);
			printf("token \"%s\": %s\n",tokenTypes[p.currentToken],tmp.c_str());
			switch(p.currentToken) {
				case HTMLParser::T_beginTag:
				case HTMLParser::T_endTag: {
					tmp=string(p.tokenInfo.beginTag.tagName,p.tokenInfo.beginTag.tagNameLen);
					printf("\ttagName: %s\n",tmp.c_str());
				} break;
				case HTMLParser::T_attribute: {
					string tmp1(p.tokenInfo.attribute.name,p.tokenInfo.attribute.nameLen);
					string tmp2(p.tokenInfo.attribute.value,p.tokenInfo.attribute.valueLen);
					printf("\tname: %s\n\tvalue: %s\n",tmp1.c_str(),tmp2.c_str());
				} break;
				case HTMLParser::T_text: {
					tmp=string(p.tokenInfo.text.text,p.tokenInfo.text.textLen);
					printf("\ttext: %s\n",tmp.c_str());
				} break;
			}
		}
		if(br<=0) break;
		p.clean();
	}
	return 0;
}
