#include <stdio.h>

#define MAX_LENGTH 20
int main() {
	int x,y, r;
	printf("Enter two numbers:\n");
	// scanf("%d%d", &x, &y);
	while((r = scanf("%d", &x)) != EOF && r != 0)
	{
		printf("First Number is:\n");
		printf("%d (Base 10)\n", x);
	}
	printf("%d", r);
	printf("---------------------------------------------------\n");
	printf("First Number is:\n");
	printf("%d (Base 10)\n", x);
	printf("Second Number is:\n");
	printf("%d (Base 10)\n", y);
	return 0;
}
