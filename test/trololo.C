//#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wand/MagickWand.h>
//using namespace std;


#define ThrowWandException(wand) \
{ \
  char \
    *description; \
 \
  ExceptionType \
    severity; \
 \
  description=MagickGetException(wand,&severity); \
  (void) fprintf(stderr,"%s %s %lu %s\n",GetMagickModule(),description); \
  description=(char *) MagickRelinquishMemory(description); \
  exit(-1); \
}

int main(int argc, char** argv)
{
	if(argc<2)
	{
		printf("usage: %s imagefile", argv[0]);
		return 1;
	}
	MagickWand* w;
	MagickWandGenesis();
	w=NewMagickWand();
	if(MagickReadImage(w,argv[1])==MagickFalse)
		ThrowWandException(w);
	PixelIterator* it;
	PixelWand** pixels;
	register unsigned int x; unsigned int y;
	unsigned long width;
	MagickPixelPacket pixel;
	for (y=0; y < MagickGetImageHeight(w); y++)
	{
		pixels=PixelGetNextIteratorRow(it,&width);
		for (x=0; x < width; x++)
		{
			//get the lsb of the pixel here
			
			//PixelGetMagickColor(pixels[x],&pixel);
			//printf("%i",pixel.blue);
			
		}
	}
}
