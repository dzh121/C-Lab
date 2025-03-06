#include <stdio.h>
#include <stdlib.h>

#define COURSE_COST 200.0

#define ANSIC 1
#define ASSEMBLER 2

#define DATABASES 4
#define JAVA 8

#define ALGEBRAI 0x10
#define ALGEBRAII 0x20

double student_courses(unsigned courses)
{
	double cost=0;
	if(courses & (ALGEBRAI | ALGEBRAII) == 0)
	{
		printf( "NO ALGEBRA \n");
	}
	else
	{
		printf("ALGEBRA \n");
	}

	while(courses)
	{
		if (courses & 1) cost+=COURSE_COST;
		courses >>= 1;
	}
	return cost;
}


int main() {
	unsigned selected_courses = ANSIC | JAVA | ALGEBRAI | ALGEBRAII;

	double total_cost = student_courses(selected_courses);
	printf("Total cost: %.2f\n", total_cost);

	selected_courses = DATABASES | ASSEMBLER;

	total_cost = student_courses(selected_courses);
	printf("Total cost: %.2f\n", total_cost);

	selected_courses = 0;

	total_cost = student_courses(selected_courses);
	printf("Total cost: %.2f\n", total_cost);


	return 0;
}
