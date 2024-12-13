#include "input.h"

/* Fills each complex number with zeros */
void fillZero();

/* Complex variables */
complex A, B, C, D, E, F;

int main(){
	/* Define the input line */
	char line[MAX_INPUT];
	/* Fill all complex numbers with zeros */
	fillZero();
	/* Read and process commands */
	while (1)
	{
		read_line(line);
		process_line(line);
	}
}
void fillZero()
{
	/* Fill each complex number with zeros */
	read_comp(&A, 0, 0);
	read_comp(&B, 0, 0);
	read_comp(&C, 0, 0);
	read_comp(&D, 0, 0);
	read_comp(&E, 0, 0);
	read_comp(&F, 0, 0);
}