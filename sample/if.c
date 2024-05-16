#include <stdio.h>
int test = 1;
int past = 2;
int main()
{
	int i, j;
	i = 123;
	j = 222;
	if (i != j)
	{
		printf("%d != %d\n", i, j);
	}
	else
	{
		printf("%d == %d\n", i, j);
	}
	i = 999;
	printf("%d\n", i);
}
