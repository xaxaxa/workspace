#include <string.h>

static const char* tokenTypes[]={"none","beginTag","attribute","afterBegin","text","endTag","fastEnd"};
//low level html parser
struct HTMLParser
{
	enum {
		S_normal=0,	//<html>  |	<a>sssss
					//^	      |    ^
		S_attribute	//<a s="1">   | <b>
					//  ^         |   ^
	} state;
	enum {
		T_none=0,		//beginning of document
		T_beginTag, 	//<a aaaaa="a">
						//^^
		T_attribute,	//<a aaaaa="a">
						//   ^^^^^^^^^
		T_afterBegin,	//<a aaaaa="a">
						//            ^
		T_text,			//<a aaaaa="a">fgdssjghj<b />
						//             ^^^^^^^^^
		T_endTag,		//<a></a>
						//   ^^^^
		T_fastEnd,		//<a />
						//   ^^
	} currentToken;
	struct TokenInfo {
		char* data;
		int dataLen;
		union {
			struct {
				char* tagName;
				int tagNameLen;
			} beginTag,endTag;
			struct {
				char* name;
				char* value;
				int nameLen,valueLen;
			} attribute;
			struct {} afterBegin;
			struct {
				char* text;
				int textLen;
			} text;
			struct {} fastEnd;
		};
	} tokenInfo;
	char* buffer;		//set by user; user may increase the size of the buffer
						//at any time other than concurrently with a function call;
						//buffer may also be relocated (e.g. realloc)
	int bufferLength;
	bool eof;			//whether or not to return potentially incomplete data:
						//<a>sssss
						//   ^
						//if eof=true, T_text "sssss" will be returned;
						//if eof=false, read() will return false
	int pos,rpos;		//rpos>=pos at all times
						//rpos is only used when searching for a character
	void reset() {
		pos=rpos=0;
		state=S_normal;
		currentToken=T_none;
		eof=false;
	}
	int dataLeft() {
		return bufferLength-pos;
	}
	int __max(int a, int b) { return a<b?b:a; }
	inline bool isWhiteSpace(char c) {
		return c==' ' || c=='\t' || c=='\n' || c=='\r';
	}
	int strFind(int start, int len, char c) {
		char* tmp=(char*)memchr(buffer+start,(int)c,len);
		if(tmp==NULL) return -1;
		return tmp-buffer;
	}
	int _doSearch(int startPos, char c) {
		startPos=__max(startPos,rpos);
		char* tmp=(char*)memchr(buffer+startPos,(int)c,bufferLength-startPos);
		if(tmp==NULL) {
			if(eof) return bufferLength;
			rpos=bufferLength;
			return -1;
		}
		return tmp-buffer;
	}
	int _doSearch(int startPos, const char* s) {
		int sL=strlen(s);
		startPos=__max(startPos,rpos);
		char* tmp=(char*)memmem(buffer+startPos,bufferLength-startPos,s,sL);
		if(tmp==NULL) {
			if(eof) return bufferLength;
			rpos=bufferLength-(sL-1);
			return -1;
		}
		return tmp-buffer;
	}
	int _doSearch_beginTag(int startPos) {
		startPos=__max(startPos,rpos);
		//printf("_doSearch_beginTag: startPos=%i bufferLength=%i\n",startPos,bufferLength);
		//' ', '>', '\t', '\n', '\r'
		for(int i=startPos;i<bufferLength;i++) {
			char c=buffer[i];
			if(c==' ' || c=='>' || c=='\t' || c=='\n' || c=='\r' || c=='/')
				return i;
		}
		if(eof) return bufferLength;
		rpos=bufferLength;
		return -1;
	}
	int _doSearch_attribute(int startPos) {
		startPos=__max(startPos,rpos);
		char q=0;
		for(int i=startPos;i<bufferLength;i++) {
			char c=buffer[i];
			if(q!=0) {
				if(c==q) q=0;
				continue;
			}
			if(c==' ' || c=='>' || c=='\t' || c=='\n' || c=='\r' || c=='/')
				return i;
			if(c=='"' || c=='\'') q=c;
		}
		if(eof) return bufferLength;
		rpos=bufferLength;
		return -1;
	}
	//1: matched; 0: did not match; -1: need more data
	int _match(const char* s, int len) {
		int dL=dataLeft();
		for(int i=0;i<len;i++) {
			if(dL<i+1) return -1;
			if(buffer[pos+i]!=s[i]) return 0;
		}
		return 1;
	}
	bool read() {
		if(pos>=bufferLength) return false;
		switch(state) {
			case S_normal: {
				tokenInfo.data=buffer+pos;
				if(buffer[pos]=='<') {
					if(dataLeft()<2) return false;
					switch(_match("<![CDATA[",9)) {
						case -1: return false;
						case 1: {
							int i=_doSearch(pos+9,"]]>");
							if(i==-1) return false;
							currentToken=T_text;
							tokenInfo.dataLen=i-pos+3;
							tokenInfo.text={buffer+pos+9,i-(pos+9)};
							rpos=pos=i+3;
							return true;
						} break;
						default: break;
					}
					if(buffer[pos+1]=='/') {
						int i=_doSearch(pos+2,'>');
						if(i==-1) return false;
						currentToken=T_endTag;
						tokenInfo.dataLen=i-pos+1;
						tokenInfo.endTag={buffer+pos+2,i-(pos+2)};
						rpos=pos=i+1;
					} else {
						int i=_doSearch_beginTag(pos+1);
						if(i==-1) return false;
						currentToken=T_beginTag;
						tokenInfo.dataLen=i-pos;
						tokenInfo.beginTag={buffer+pos+1,i-(pos+1)};
						rpos=pos=i;
						state=S_attribute;
					}
				} else {
					int i=_doSearch(pos,'<');
					if(i==-1) return false;
					currentToken=T_text;
					tokenInfo.dataLen=i-pos;
					tokenInfo.text={buffer+pos,i-pos};
					rpos=pos=i;
				}
				return true;
			} break;
			case S_attribute: {
				while(pos<bufferLength && isWhiteSpace(buffer[pos])) pos++;
				tokenInfo.data=buffer+pos;
				rpos=pos;
				if(pos>=bufferLength) return false;
				if(buffer[pos]=='/') {
					if(dataLeft()<2) return false;
					if(buffer[pos+1]=='>') {
						//found: />
						currentToken=T_fastEnd;
						tokenInfo.dataLen=2;
						rpos=pos=pos+2;
						state=S_normal;
						return true;
					}
				}
				if(buffer[pos]=='>') {
					currentToken=T_afterBegin;
					tokenInfo.dataLen=1;
					rpos=pos=pos+1;
					state=S_normal;
					return true;
				}
				int i=_doSearch_attribute(pos+1);
				if(i==-1) return false;
				currentToken=T_attribute;
				tokenInfo.dataLen=i-pos;
				int ii=strFind(pos,i-pos,'=');
				if(ii==-1) {
					tokenInfo.attribute={buffer+pos,NULL,i-pos,0};
				} else {
					tokenInfo.attribute={buffer+pos,buffer+ii+1,ii-pos,i-ii-1};
					if(tokenInfo.attribute.valueLen>=2) {
						if((buffer[ii+1]=='"' && buffer[i-1]=='"')
							|| (buffer[ii+1]=='\'' && buffer[i-1]=='\'')) {
							tokenInfo.attribute.value++;
							tokenInfo.attribute.valueLen-=2;
						}
					}
				}
				rpos=pos=i;
				return true;
			} break;
		}
	}
	//remove unneeded data from buffer
	void clean() {
		int shift=pos;
		if(shift<=0) return;
		memmove(buffer,buffer+shift,bufferLength-shift);
		pos-=shift;
		rpos-=shift;
		bufferLength-=shift;
	}
};
