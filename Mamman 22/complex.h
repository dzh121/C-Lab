#ifndef COMPLEX_H
#define COMPLEX_H

typedef  struct
{
	double real;
	double imaginary;
} complex;

void read_comp(complex *a, double real, double imaginary);
void print_comp(complex *a);
void add_comp(complex *a, complex *b);
void sub_comp(complex *a, complex *b);
void mult_comp_real(complex *a, double real);
void mult_comp_img(complex *a, double img);
void mult_comp_comp(complex *a, complex *b);
void abs_comp(complex *a);

#endif
