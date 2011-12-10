/*
 * tailcall.C
 * 
 * Copyright 2011  <xaxaxa@xaxaxa>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>

using namespace std;
double asdf();
void zxcv(int x)
{
	if(x==0)x++;
	asdf();
	//return 5;
}
double asdf()
{
	zxcv(3);
	return 6.0;
}
struct str
{
	//int i;
	double x;
};

int aaa=0;
void (*fp)(const str s);
void test123(int i, double x)
{
	if(i>5)
	{
		//str s;
		//cout << (unsigned long)&x << endl;
		str s{x};
		fp(s);
		return;
	}
	aaa++;
}
void fgh(int i, str& s)
{
	s.x+=0.1;
	fgh(i+1,s);
}
void asdfghjk(const str s)
{
	test123((int)s.x,s.x);
}






void func()
{
	func();
}

int main(int argc, char **argv)
{
	//func();
	fp=&asdfghjk;
	str s{7.0};
	fgh(8,s);
	//test123(7,8.0);
	//return (int)asdf();
	return 0;
}

