#include <iostream>
//#include <cplib/cplib.hpp>
using namespace std;
//using namespace xaxaxa;

class x
{
public:
	int* refc;
	virtual void Destructor()=0;
	virtual void CopyTo(x& other) const=0;
	x()
	{
		refc=new int;
		*refc=1;
	}
	void destruct()
	{
		if(refc==NULL)return;
		(*refc)--;
		if(*refc<=0)
		{
			Destructor();
			delete refc;
		}
		refc=NULL;
	}
	x(const x& other)
	{
		refc=other.refc;
		(*refc)++;
		other.CopyTo(*this);
	}
	x& operator=(const x& other)
	{
		destruct();
		refc=other.refc;
		(*refc)++;
		other.CopyTo(*this);
		return *this;
	}
	virtual ~x()
	{
		destruct();
	}
};












class a:public x
{
public:
	int* i;
	a()
	{
		i=new int;
		cout << "constructor" << endl;
	}
	virtual void Destructor()
	{
		cout << "destructor" << endl;
		delete i;
	}
	virtual void CopyTo(x& other) const
	{
		((a&)other).i=i;
	}
	
	virtual ~a()
	{
		destruct();
	}
};

int main(int c, char** v)
{
	a a1;
	*a1.i=12;
	
	a a2;
	a2=a1;
	*a2.i=5;
	
	cout << "*a1.i="<<*a1.i << endl;
	cout << "*a2.i="<<*a2.i << endl;
	cout << "*a2.refcount="<<*a2.refc << endl;
	
	a a3(a2);
	cout << "*a2.refcount="<<*a2.refc << endl;
}
