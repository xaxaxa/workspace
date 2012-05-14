#include <8051.h>
unsigned char code a[]={0x9,0x8,0xC,0x4,0x6,0x2,0x3,0x1};
unsigned char i=0;

#define int_from_bit(i) ((unsigned char)(1<<i))
#define set_bit(x,i,v) x=(v?(x|int_from_bit(i)):x&~int_from_bit(i))
#define get_bit(x,i) (x&int_from_bit(i))

main()
{
	int x;
	int b=0;
	IE=0x8a;
	TMOD=0x11;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	TR0=1;
	while(1)
	{
		//P2=~P2;
		b=b?0:1;
		//P2=(((P2&8)==0)?P2|8:P2&247);
		/*if(b)
			P2=P2|16;
		else
			P2=P2&~((unsigned char)16);*/
		set_bit(P2,4,b);
		for(x=0;x<100;x++);
	}
}
void i1(void) interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	P1=a[i];
	//set_bit(P1,7,i%2);
	//P1=P2;
	i++;
	if(i>7)i=0;
	//set_bit(P2,4,!get_bit(P2,4));
	//P2=(((P2&16)!=0)?P2|16:P2&~16);
}
