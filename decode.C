#include <iostream>
#include <cplib/cplib.hpp>
using namespace xaxaxa;
#define GET_SHIFT_COUNT(i) ( ((i)/8)*8+(7-((i)%8)) )
int main()
{
	StandardStream ss;
	Buffer b(4096);
	Int br;
	static const Int width=8;
	static const Int bufSize=512;
	Long buf[bufSize];
	Long* cur_data=buf;
	UInt cur_index=0;
	while((br=ss.Read(b))>0) {
		for(Int i=0;i<br;i++) {
			if(b.Data[i]=='0') {
				Long s=((Long)1)<<GET_SHIFT_COUNT(cur_index);
				*cur_data=*cur_data&~s;
				cur_index++;
			} else if(b.Data[i]=='1') {
				Long s=((Long)1)<<GET_SHIFT_COUNT(cur_index);
				*cur_data=*cur_data|s;
				cur_index++;
			}
			if(cur_index>=width*8) {
				//ss.Write(BufferRef(cur_data,width));
				cur_data++;
				if((cur_data-buf)>=bufSize) {
					ss.Write(BufferRef((Byte*)buf,bufSize*width));
					cur_data=buf;
				}
				cur_index=0;
				*cur_data=0;
			}
		}
	}
	Int tmp=(cur_data-buf)*width+(cur_index/width);
	//cout << tmp << endl;
	ss.Write(BufferRef((Byte*)buf,tmp));
	//will not write out incomplete bytes
}
