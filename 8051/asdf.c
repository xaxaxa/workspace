#include <8051.h>

unsigned int main(void)
{
	P0=0xff;
	P1=0xff;
	P2=0xff;
	P3=0xff;
	while(1)
	{
		P0=~P0;
		P1=~P1;
		P2=~P2;
		P3=~P3;
	}
}
