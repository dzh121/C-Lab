#include "complex.h"
#include <stdio.h>
#include <math.h>

void read_comp(complex *comp, double real, double img) {
    /* Assign real and imaginary parts to the complex number */
    comp->real = real;
    comp->imaginary = img;
}

void print_comp(complex *comp) {
    /* Print the real part */
    printf("%.2f", comp->real);

    /* Print the imaginary part with the correct sign */
    if (comp->imaginary >= 0)
        printf(" + (%.2f)i\n", comp->imaginary);
    else
        printf(" - (%.2f)i\n", -comp->imaginary);
}

void add_comp(complex *comp1, complex *comp2) {
    complex temp;

    /* Add real parts and imaginary parts separately */
    temp.real = comp1->real + comp2->real;
    temp.imaginary = comp1->imaginary + comp2->imaginary;

    /* Print the result */
    print_comp(&temp);
}

void sub_comp(complex *comp1, complex *comp2) {
    complex temp;

    /* Subtract real parts and imaginary parts separately */
    temp.real = comp1->real - comp2->real;
    temp.imaginary = comp1->imaginary - comp2->imaginary;

    /* Print the result */
    print_comp(&temp);
}

void mult_comp_real(complex *comp, double real) {
    complex temp;

    /* Multiply both real and imaginary parts by the num */
    temp.real = comp->real * real;
    temp.imaginary = comp->imaginary * real;

    /* Print the result */
    print_comp(&temp);
}

void mult_comp_img(complex *comp, double img) {
    complex temp;

    /* Calculate the product with imaginary num */
    temp.real = -(comp->imaginary * img);
    temp.imaginary = comp->real * img;

    /* Print the result */
    print_comp(&temp);
}

void mult_comp_comp(complex *comp1, complex *comp2) {
    complex temp;

    /* Multiply two complex numbers:
       (a + bi)(c + di) = (ac - bd) + (ad + bc)i */
    temp.real = comp1->real * comp2->real - comp1->imaginary * comp2->imaginary;
    temp.imaginary = comp1->real * comp2->imaginary + comp1->imaginary * comp2->real;

    /* Print the result */
    print_comp(&temp);
}

void abs_comp(complex *comp) {
    double temp;

    /* Calculate the abs using the formula: sqrt(real^2 + imaginary^2) */
    temp = pow(comp->real, 2) + pow(comp->imaginary, 2);
    temp = sqrt(temp);

    /* Print the abs */
    printf("%.2f\n", temp);
}
