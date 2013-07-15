#include <cpoll/basictypes.H>
using namespace CP;
class C
{
public:
	int i;
	C() {
		
	}
	C(int i, int x):i(i){}
	C(const C& other):i(other.i) {
		printf("C::C(const C& other)\n");
	}
	C& operator=(const C& other) {
		i=other.i;
		printf("C::operator=()\n");
		return *this;
	}
};
int main() {
	Optional<C> tmp1;
	Optional<C> tmp2=Optional<C>::create(5,2);
	Optional<C> tmp3(tmp2);
	tmp1=tmp2;
	tmp2=undefined;
	if(tmp3) {
		printf("tmp3==true\n");
	}
	if((Optional<int>)undefined) {
		printf("undefined==true\n");
	}
	printf("%i %i\n",tmp1().i,tmp3().i);
}
