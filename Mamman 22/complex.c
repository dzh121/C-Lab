#include "complex.h"
#include <stdio.h>
#include <math.h>

void read_comp(complex *a, double real, double imaginary) {
	a->real = real;
	a->imaginary = imaginary;
}

void print_comp(complex *a) {
	printf("%.2f", a->real);
	if (a->imaginary >= 0)
		printf(" + (%.2f)i\n", a->imaginary);
	else
		printf(" - (%.2f)i\n", -a->imaginary);
}

void add_comp(complex *a, complex *b) {
	complex temp;

	temp.real = a->real + b->real;
	temp.imaginary = a->imaginary + b->imaginary;

	print_comp(&temp);
}

void sub_comp(complex *a, complex *b) {
	complex temp;

	temp.real = a->real - b->real;
	temp.imaginary = a->imaginary - b->imaginary;

	print_comp(&temp);
}

void mult_comp_real(complex *a, double real) {
	complex temp;

	temp.real = a->real * real;
	temp.imaginary = a->imaginary *	real;

	print_comp(&temp);
}

void mult_comp_img(complex *a, double img) {
	complex temp;

	temp.real = -(a->imaginary * img);
	temp.imaginary = a->real *	img;

	print_comp(&temp);
}

void mult_comp_comp(complex *a, complex *b) {
	complex temp;

	temp.real = a->real*b->real - a->imaginary*b->imaginary;
	temp.imaginary = a->real*b->imaginary + a->imaginary*b->real;

	print_comp(&temp);
}

void abs_comp(complex *a) {
	double temp;

	temp = pow(a->real,2) + pow(a->imaginary,2);
	temp = sqrt(temp);

	printf("%.2f\n", temp);
}
