#include "input.h"

void fillZero();

complex A, B, C, D, E, F;

int main(){
	char line[MAX_INPUT];
	fillZero();
	while (1)
	{
		read_line(line);
		process_line(line);
	}
}
void fillZero()
{
	read_comp(&A, 0, 0);
	read_comp(&B, 0, 0);
	read_comp(&C, 0, 0);
	read_comp(&D, 0, 0);
	read_comp(&E, 0, 0);
	read_comp(&F, 0, 0);
}