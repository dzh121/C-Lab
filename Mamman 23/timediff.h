#ifndef TIMEDIFF_H
#define TIMEDIFF_H

/* Max line length for line input */
#define MAX_LINE 200

/* Define the time structure */
typedef struct
{
    int Year;
    int Month;
    int Day;
    int Hour;
    int Minute;
    int Second;
} time;

/*
 * Prints a time in a readable format dd/mm/y hh/mm/ss.
 * For example, it may output 27/12/2024 11:30:09.
 * Parameters:
 * - tm: Pointer to the time to print.
 */
void print_time(time* tm);

/*
 * Calculates the difference in seconds between two times.
 * Parameters:
 * - tm1: Pointer to the first time.
 * - tm2: Pointer to the second time.
 * Returns:
 * - long: The difference in seconds between the two times.
 */
long time_diff(time* tm1, time* tm2);

/*
 * Converts a time to seconds.
 * Parameters:
 * - tm: Pointer to the time to convert.
 * Returns:
 * - long: The time converted to seconds.
 */
long time_to_seconds(time* tm);

#endif
