#ifndef COMPLEX_H
#define COMPLEX_H

/*
 * Struct to represent a complex number.
 * Fields:
 * - real: The real part of the complex number (double).
 * - imaginary: The imaginary part of the complex number (double).
 */
typedef struct
{
	double real;       /* Real part of the complex number */
	double imaginary;  /* Imaginary part of the complex number */
} complex;

/*
 * Initializes a complex number with the given real and imaginary parts.
 * Parameters:
 * - comp: Pointer to the complex number to init.
 * - real: The real part of the complex number.
 * - img: The imaginary part of the complex number.
 */
void read_comp(complex* comp, double real, double img);

/*
 * Prints a complex number in a readable format.
 * For example, it may output "a + (b)i" or "a - (b)i".
 * Parameters:
 * - comp: Pointer to the complex number to print.
 */
void print_comp(complex* comp);

/*
 * Adds two complex numbers.
 * The result is printed to screen.
 * Parameters:
 * - comp1: Pointer to the first complex number.
 * - comp2: Pointer to the second complex number to add.
 */
void add_comp(complex* comp1, complex* comp2);

/*
 * Subtracts the second complex number from the first complex number.
 * The result is printed to screen.
 * Parameters:
 * - comp1: Pointer to the first complex number.
 * - comp2: Pointer to the second complex number to subtract.
 */
void sub_comp(complex* comp1, complex* comp2);

/*
 * Multiplies a complex number by a real number.
 * The result is printed to screen.
 * Parameters:
 * - comp: Pointer to the complex number to multiply.
 * - real: The real number to multiply by.
 */
void mult_comp_real(complex* comp, double real);

/*
 * Multiplies a complex number by an imaginary number.
 * The result is printed to screen.
 * Parameters:
 * - comp: Pointer to the complex number to multiply.
 * - imag: The imaginary scalar to multiply by.
 */
void mult_comp_img(complex* comp, double imag);

/*
 * Multiplies two complex numbers.
 * The result is printed to screen.
 * Parameters:
 * - comp1: Pointer to the first complex number.
 * - comp2: Pointer to the second complex number to multiply.
 */
void mult_comp_comp(complex* comp1, complex* comp2);

/*
 * Calculates and prints the absolute value of a complex number.
 * Parameters:
 * - comp: Pointer to the complex number to calculate the absolute value for.
 */
void abs_comp(complex* comp);


#endif
