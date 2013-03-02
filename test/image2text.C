#include <Magick++.h>
#include <stdio.h>
#include <iostream>
//using namespace MagickLib;
using namespace Magick;
using namespace std;
int main(int argc, char** argv) {
	InitializeMagick(NULL);

	Image image;
	image.read(argv[1]);
	Pixels view(image);
	
	size_t columns = image.baseColumns(); size_t rows = image.baseRows(); 
	PixelPacket *pixels = view.get(0,0,columns,rows); 
	int max=0;
	for ( ssize_t row = 0; row < rows ; ++row ) {
		for ( ssize_t column = 0; column < columns ; ++column ) {
			PixelPacket& p=*(pixels++);
			int avg=((int)p.red+(int)p.green+(int)p.blue)/3;
			if(avg<10) cout << "#";
			else if(avg<30) cout << "@";
			else if(avg<50) cout << "%";
			else if(avg<80) cout << "&";
			else if(avg<120) cout << "$";
			else if(avg<170) cout << "/";
			else if(avg<220) cout << ".";
			else cout << " ";
			if(avg>max)max=avg;
		}
		cout << endl;
	}
	cerr << max << endl;
}
