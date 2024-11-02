#include <stdio.h>
#include <ctype.h>

unsigned long turnOnBit(unsigned long x, int bit)
{
	/* Move 1 to the left by bit and OR it with x */
	return x | (1 << bit);
}

unsigned long checkIfBitIsOn(unsigned long x, int bit)
{
	/* Shift number to right and AND to check if it is on*/
	return (x >> bit) & 1;
}

void printBinary(unsigned long x)
{
	/* Loop through the bits of x (unsigned  long has 32 bits) */
	int i;
	for (i = 31; i >= 0; i--)
	{
		/* Print the bit at position i */
		printf("%lu", (x >> i) & 1);
	}
	printf(" (Base 2)\n");
}

int main()
{
	/* Declare two unsigned long variables */
	unsigned long x, y;
	/* Get two numbers from the user */
	printf("Enter first number:\n");
	scanf("%lu", &x);
	printf("Enter second number:\n");
	scanf("%lu", &y);
	printf("---------------------------------------------------\n");

	/* Print the numbers in binary and decimal */
	printf("First Number is:\n");
	printf("%lu (Base 10)\n", x);
	printBinary(x);
	printf("Second Number is:\n");
	printf("%lu (Base 10)\n", y);
	printBinary(y);
	printf("---------------------------------------------------\n");

	/* Turn on bit number 12 in x */
	x = turnOnBit(x, 12);
	printf("First number after turning on bit number 12 is:\n");
	/* Print the number in binary and decimal */
	printf("%lu (Base 10)\n", x);
	printBinary(x);
	printf("---------------------------------------------------\n");

	/* Check if bit number 6 is on in y and print accordingly */
	printf("Bit number 6 for y is on? ");
	if (checkIfBitIsOn(y, 6))
		printf("YES\n");
	else
		printf("NO\n");

	return 0;
}
