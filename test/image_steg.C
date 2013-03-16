#include <Magick++.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>


//using namespace MagickLib;
using namespace Magick;
using namespace std;
char getBit(char& cur_buf, int& cur_ind) {
	if(cur_ind>=sizeof(cur_buf)) {
		cur_buf=0;
		int i=read(0,&cur_buf,sizeof(cur_buf));
		if(i<=0)return -1;
	}
	char tmp = (cur_buf & (1 << cur_ind))>0?1:0;
	cur_ind++;
	return tmp;
}
int main(int argc, char** argv) {
	InitializeMagick(NULL);

	Image image;
	image.read(argv[1]);
	Pixels view(image);
	
	//int file=open(argv[2],O_RDONLY);
	char cur_buf;
	int cur_ind=sizeof(cur_buf);
	
	size_t columns = image.baseColumns(); size_t rows = image.baseRows(); 
	PixelPacket *pixels = view.get(0,0,columns,rows); 
	int max=0;
	for ( ssize_t row = 0; row < rows ; ++row ) {
		for ( ssize_t column = 0; column < columns ; ++column ) {
			PixelPacket& p=*(pixels++);
			char tmp;
			if((tmp=getBit(cur_buf,cur_ind))<0) goto sss;
			if(tmp) p.red &= ~(int)1;
			else p.red |= 1;
			
			if((tmp=getBit(cur_buf,cur_ind))<0) goto sss;
			if(tmp) p.green &= ~(int)1;
			else p.green |= 1;
			
			if((tmp=getBit(cur_buf,cur_ind))<0) goto sss;
			if(tmp) p.blue &= ~(int)1;
			else p.blue |= 1;
		}
	}
sss:
	image.write( "x.png" );
}
