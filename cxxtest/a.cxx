/*
 * a.cxx
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

class fuck
{
public:
	int v1;
	int v2;
	int asdf(int x, int y)
	{
		return x+y+v1+v2;
	}
};
class dummy{};


int (*fptr)(void* th, int x, int y);
int (fuck::*fptr2)(int x, int y);
int (dummy::*fptr3)(int x, int y);
int main(int argc, char **argv)
{
	fuck you;
	you.v1=1;
	you.v2=2;
	//cout << you.asdf(3,4) << endl;
	
	fptr=(int(*)(void*,int,int))&fuck::asdf;
	fptr2=&fuck::asdf;
	fptr3=(int (dummy::*)(int,int))fptr2;
	
	dummy* x=(dummy*)&you;
	
	cout << (you.*fptr2)(3,4) << endl;
	cout << fptr(&you,3,4) << endl;
	cout << (x->*fptr3)(3,4) << endl;
	return 0;
}

