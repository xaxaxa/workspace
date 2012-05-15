#include <8051.h>
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
//unsigned int t[]={0,903,852,759,716,638,568,506,478,426,379,0};
unsigned int t[]={0,956,852,759,716,638,568,506,478,426,379,0};
//unsigned int t[]={0,1912,1704,1517,1433,1276,1136,1012,956,852,759,0};
unsigned int time[]=	{500,500,500,500,500,500,500,500,500,500,500,500,500,500,
						500,500,500,500,500,500,500,500,500,500,500,500,500,500,500,500,500,500,500};
//unsigned char s[]={1,2,3,1,1,2,3,1,3,4,5,3,4,5,		5,6,5,4,3,1,5,6,5,4,3,1,1,5,1,1,5,1,0};
//unsigned char s[]={1,1,2,1,4,3,11,1,1,2,1,5,4,11,						1,1,8,6,4,3,9,11,7,7,6,4,5,4,11};
unsigned char s[]={1,1,2,1,4,3,0,1,1,2,1,5,4,0,1,1,8,6,4,3,2,0,7,7,6,4,5,4,0};
//unsigned char s[]={2,2,3,2,5,4,0,2,2,3,2,6,5,0,2,2,9,7,5,4,3,0,8,8,6,5,6,5,0};
//unsigned char s[]={1,2,3,4,5,6,7,8,9,10,0};
unsigned char t_h,t_l;
unsigned char display_seg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x7f};
//unsigned char seg_position[]={0x0e,0x0d,0x0b,0x07,0x00};
unsigned char seg_position[]={0x06,0x05,0x03,0x07,0x00};
int count=29;//33;
#define int_from_bit(i) (1<<i)
#define set_bit(x,i,v) x=(v?(x|int_from_bit(i)):x&~int_from_bit(i))
#define get_bit(x,i) ((x&int_from_bit(i))!=0)
//sbit speaker=P2^4;
void display(int i)
{
	//unsigned char tmp;
	
	/*P0=255;
	//tmp=((unsigned char)speaker>>4);
	P2 = ((unsigned char)speaker>>3)|seg_position[3];
	P0=display_seg[i/1000];
	delay(2);*/
	
	P0=255;
	P2 = seg_position[2];
	P0=display_seg[i%1000/100];
	delay(2);
	P0=255;
	P2 = seg_position[1];
	P0=display_seg[i%100/10];
	delay(2);
	P0=255;
	P2 = seg_position[0];
	P0=display_seg[i%10];
	delay(2);
	//P2=((unsigned char)speaker>>4);
	//P0=0;
}
int i;
int _c;
main()
{
	//char a;
	//int i;
	//int x=15;
	//int b=0;
	//char c=0;
	IE=0x8a;
	TMOD=0x11;
	//TCON=0x04;
	///TMOD=0x01;
	////EA=1;
	////ET0=1;
	
	//a=1;
	//P0=1;
	/*while(1)
	{
		//P0=P0 << 1;
		if(a >> 7)
		{
			a=1;
			goto aaaaa;
		}
		a=a<<1;
aaaaa:
		//a=(a==1)?0:1;
		//if(a==0)a=1;
		P0=~a;
		P1=~a;
		P2=~a;
		//P3=~a;
		delay(10);
	}*/
	//P0=0;
	//P1=0;
	//P2=0;
//aaa:
	//P0=~P0;
	/*while(1)
	{
		a=~a;
		P0=a;
		P1=a;
		P2=a;
		for(i=0;i<x;i++);
		if(b>1000)
		{
			b=0;
			if(c)x++; else x--;
			if(x<10)c=1;
			if(x>20)c=0;
		}
		b++;
	}*/
	//speaker=0;
	set_bit(P2,4,0);
	/*while(1)
	{
		speaker=~speaker;
		delay(1);
	}*/
	
	i=0;
	_c=0;
	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;
	TR1=1;
	while(1)
	{
		display(i);
		i++;
		set_bit(P2,4,!get_bit(P2,4));
	}
	/*while(1)
	{
		t_h=(65536-t[s[i]])/256;
		t_l=(65536-t[s[i]])%256;
		TH0=t_h;
		TL0=t_l;
		TR0=1;
		display(i);
		//delay(time[i]);
		i+=1;
		if(i>=count)i=0;
	}*/
	//goto aaa;
}
/*void a(void) interrupt 2
{
	int i;
	for(i=0;i<20;i++)
	{
		//P1=~P1;
		delay(20);
	}
}*/
void b(void) interrupt 1
{
	//speaker=~speaker;
	set_bit(P2,4,!get_bit(P2,4));
	TH0=t_h;
	TL0=t_l;
	//TR0=1;
	//TF0=0;
}
void c(void) interrupt 3
{
	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;
	_c+=50;
	//display(time[i]);
	if(_c<time[i])return;
	_c=0;
	if(s[i]==0)TR0=0;
	t_h=(65536-(t[s[i]]))/256;
	t_l=(65536-(t[s[i]]))%256;
	TH0=t_h;
	TL0=t_l;
	if(s[i]!=0)TR0=1;
	//display(i);
	//delay(time[i]);
	i+=1;
	if(i>=count)i=0;
	//TH1=(65536-time[i]*10)/256;
	//TL1=(65536-time[i]*10)%256;
}
