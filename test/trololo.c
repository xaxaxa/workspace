//#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wand/MagickWand.h>
#include <unistd.h>
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
//	PixelGetMagickColor();
	if(argc<2)
	{
		printf("usage: %s imagefile\n", argv[0]);
		return 1;
	}
	MagickWand* w;
	MagickWandGenesis();
	w=NewMagickWand();
	//PixelGetMagickColor();
	if(MagickReadImage(w,argv[1])==MagickFalse)
		ThrowWandException(w);
	PixelIterator* it;
	PixelWand** pixels;
	register unsigned int x; unsigned int y;
	unsigned long width;
	MagickPixelPacket pixel;
	char tmp=0;
	int tmp_index=0;
	for (y=0; y < MagickGetImageHeight(w); y++)
	{
		pixels=PixelGetNextIteratorRow(it,&width);
		for (x=0; x < width; x++)
		{
			//get the lsb of the pixel here
			PixelGetMagickColor(pixels[x],&pixel);
			char bit=((int)pixel.red)&1;
			tmp=tmp|(bit << tmp_index);
			tmp_index++;
			if(tmp_index>=8)
			{
				write(1,&tmp,1);
				tmp=0;tmp_index=0;
			}
			bit=((int)pixel.green)&1;
                        tmp=tmp|(bit << tmp_index);
                        tmp_index++;
                        if(tmp_index>=8)
                        {
                                write(1,&tmp,1);
                                tmp=0;tmp_index=0;
                        }
			bit=((int)pixel.blue)&1;
                        tmp=tmp|(bit << tmp_index);
                        tmp_index++;
                        if(tmp_index>=8)
                        {
                                write(1,&tmp,1);
                                tmp=0;tmp_index=0;
                        }


			//move the next line before 'char bit'
			//PixelGetMagickColor(pixels[x],&pixel);
			//printf("%i",pixel.blue);
			
		}
	}
}
