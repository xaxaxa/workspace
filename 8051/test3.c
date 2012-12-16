#include <8051.h>
//sbit output=P2^4;
unsigned char code display_seg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x7f};
//unsigned char seg_position[]={0x0e,0x0d,0x0b,0x07,0x00};
unsigned char code seg_position[]={0xFE,0xFD,0xFB,0xF7,0xF0};
void delay(int i)
{
	int a,b;
	for(a=0;a<i;a++)
	{
		for(b=0;b<120;b++);
		//TF1=0;
		/*TH1=(65536-1000)/256;
		TL1=(65536-1000)%256;
		TR1=1;
		while(TF1==0);
		TF1=0;
		TR1=0;*/
	}
}
void display(unsigned long int i)
{
	//unsigned char tmp;
	unsigned char d0=0xFF;
	unsigned char d1=0xFF;
	unsigned char d2=0x7F;
	unsigned char d3=0xFF;
	//if(i>=(60*60*100))
	if(i/600>=60*100)
	{
		i=i/60/60/(unsigned long int)100*(unsigned long int)1000000+(i%((unsigned long int)60*(unsigned long int)60*(unsigned long int)100))/(unsigned long int)6000*(unsigned long int)10000;
		i/=(unsigned long int)10000;
		//d2=0xFF;
		//d1=0x7F;
		//d3=0x7F;
	}
	else if(i/60>=60*100)
	{
		i=i/60/60/(unsigned long int)100*(unsigned long int)1000000+(i%((unsigned long int)60*(unsigned long int)60*(unsigned long int)100))/(unsigned long int)6000*(unsigned long int)10000 + i%(unsigned long int)6000;
		i/=1000;
		d2=0xFF;
		d1=0x7F;
		d3=0x7F;
	}
	else if(i>=60000)
	{
		i=i/6000*10000+i%6000;
		i/=100;
		//d2=0xFF;
		//d1=0x7F;
		//d3=0x7F;
	}
	else if(i>=6000)
	{
		//i/=100;
		i=i/6000*10000+i%6000;
		i/=10;
		d2=0xFF;
		d1=0x7F;
		d3=0x7F;
	}
	P0=255;
	//tmp=((unsigned char)speaker>>4);
	P2 = seg_position[3];
	P0=display_seg[i/1000] & d3;
	delay(3);
	
	P0=255;
	P2 = seg_position[2];
	P0=display_seg[i%1000/100] & d2;
	delay(3);
	P0=255;
	P2 = seg_position[1];
	P0=display_seg[i%100/10] & d1;
	delay(3);
	P0=255;
	P2 = seg_position[0];
	P0=display_seg[i%10] & d0;
	//delay(3);
	//P2=((unsigned char)speaker>>4);
	//P0=0;
}

unsigned long int i=0;
void main()
{
	//output=1;
	IE=0x8a;
	TMOD=0x11;
	TH0=(65536-10000)/256;
	TL0=(65536-10000)%256;
	TR0=1;
	while(1)
	{
		//output=~output;
		//for(i=0;i<2;i++);
		display(i);
		//delay(100);
		//i++;
	}
}
void timer() interrupt 1
{
	TH0=(65536-10000)/256;
	TL0=(65536-10000)%256;
	i++;
	//while(1);
}




