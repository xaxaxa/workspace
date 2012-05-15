#include <8051.h>
#include <math.h>
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
unsigned long int pow(unsigned long int a, unsigned long int b)
{
	unsigned long int tmp=1;
	while(b>0)
	{
		tmp=tmp*a;
		b--;
	}
	return tmp;
}
unsigned char get_digit(unsigned long int i, unsigned char d)
{
	//return i%1000/100;
	return i%(pow(10,d)*10)/pow(10,d);
}
unsigned char get_digits(unsigned long int i)
{
	unsigned char tmp=0;
	while(i>0)
	{
		tmp++;
		i=i/10;
	}
	return tmp;
}
void display(unsigned long int i)
{
	//unsigned char tmp;
	unsigned char d0=0xFF;
	unsigned char d1=0xFF;
	unsigned char d2=0xFF;
	unsigned char d3=0xFF;
	//if(i>=(60*60*100))
	/*if(i/600>=60*100)
	{
		i=i/60/60/100*1000000+(i%((unsigned long int)60*(unsigned long int)60*(unsigned long int)100))/6000*10000;
		i/=10000;
		//d2=0xFF;
		//d1=0x7F;
		//d3=0x7F;
	}
	else if(i/60>=60*100)
	{
		i=i/60/60/100*1000000+(i%((unsigned long int)60*(unsigned long int)60*(unsigned long int)100))/6000*10000 + i%6000;
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
	}*/
	P0=255;
	P2 = seg_position[3];
	if(i>=10000)
	{
		d3=0x7F;
	}
	P0=display_seg[get_digit(i,3)] & d3;
	delay(2);
	
	P0=255;
	P2 = seg_position[2];
	P0=display_seg[get_digit(i,2)] & d2;
	delay(2);
	P0=255;
	P2 = seg_position[1];
	P0=display_seg[get_digit(i,1)] & d1;
	delay(2);
	P0=255;
	P2 = seg_position[0];
	P0=display_seg[get_digit(i,0)] & d0;
	delay(2);
	//P2=((unsigned char)speaker>>4);
	//P0=0;
}
void func()
{
	P2=~P2;
}

void main()
{
	void (*f)();
	f=&func;
	while(1)
	{
		display(get_digits(1234567));
	}
}

