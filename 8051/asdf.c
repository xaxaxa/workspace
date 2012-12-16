#include <8051.h>
unsigned char code display_seg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x7f};
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
	
	/*P0=255;
	//tmp=((unsigned char)speaker>>4);
	P2 = ((unsigned char)speaker>>3)|seg_position[3];
	P0=display_seg[i/1000];
	delay(2);*/
	
	P0=255;
	P2 = seg_position[3];
	P0=display_seg[(i%(unsigned long)10000)/(unsigned long)1000];
	delay(2);
	P0=255;
	P2 = seg_position[2];
	P0=display_seg[(i%(unsigned long)1000)/(unsigned long)100];
	delay(2);
	P0=255;
	P2 = seg_position[1];
	P0=display_seg[(i%(unsigned long)100)/10];
	delay(2);
	P0=255;
	P2 = seg_position[0];
	P0=display_seg[i%10];
	delay(2);
	//P2=((unsigned char)speaker>>4);
	//P0=0;
}

void asd() {
	int i;
	while(1) {
		P0=~P0;
		P1=~P1;
		P2=~P2;
		P3=~P3;
		for(i=0;i<10;i++) {}
	}
}
unsigned int main(void)
{
	unsigned int i;
	void __data (*a)();
	volatile int* b;
	P0=0xff;
	P1=0xff;
	P2=0xff;
	P3=0xff;
	a=&asd;
	b=(int*)a;
	*b=5432;
	for(i=0;i<100;i++)
		display(*b);
	*b=1234;
	for(i=0;i<100;i++)
		display(*b);
	a();
	while(1)
	{
		/*P0=~P0;
		P1=~P1;
		P2=~P2;
		P3=~P3;
		for(i=0;i<3;i++) {}*/
	}
}


