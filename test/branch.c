#include <stdio.h>
#define CONDITION (i & 32) == 0
int main()
{
	int max=100000000;
	int sum=0;
	int i;
	for (i = 0; i < max; i++)
		if (CONDITION) sum++;
	printf("%i\n", sum);
	return sum;
}
