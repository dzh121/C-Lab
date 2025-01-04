#include "timediff.h"
#include <stdio.h>
#include <stdlib.h>

/* The length of the months in days */
int monthsLength[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

/* The main function
 * Reads a file with time values and prints the time difference in seconds
 * Parameters:
 * - argc: The number of arguments.
 * - argv: The arguments array.
 */
int main(int argc, char* argv[])
{
	/* Define the line */
	char line[MAX_LINE];
	/* Define line count */
	int line_count = 0;
	/* Define the values to keep time */
	int values[12];
	/* Define the time structures */
	time tm1, tm2;
	/* Define the file */
	FILE *file;

	/* Check if the user entered a file name */
	if(argc == 1) {
		/* If the user did not enter a file name, print an error message and exit */
		fprintf(stderr, "Enter a file name as an argument\n");
		exit(1);
	}
	if(argc > 2) {
		/* If the user entered too many arguments, print an error message and exit */
		fprintf(stderr,"Too many arguments\n");
		exit(1);
	}

	/* Open the file */
	file = fopen(argv[1], "r");
	if(file == NULL) {
		/* If the file could not be opened, print an error message and exit */
		fprintf(stderr,"Error opening file\n");
		exit(2);
	}

	/* Read the file line by line */
	while(fgets(line, MAX_LINE, file) != NULL) {
		line_count++;
		/* Check if the line is valid and read the 12 values (Year, month, day, hour, minute, second)*2 */
		if (sscanf(line, "%d %d %d %d %d %d %d %d %d %d %d %d",
				   &values[0], &values[1], &values[2], &values[3],
				   &values[4],&values[5], &values[6], &values[7],
				   &values[8], &values[9], &values[10], &values[11]) != 12) {
			/* If the line is invalid(one is not an int or not enough values) -> print an error message and continue to the next line */
			fprintf(stderr, "Invalid input in line %d\n", line_count);
			continue;
		}
		/* Set the values to the time structures */
		tm1.Year = values[0];
		tm1.Month = values[1];
		tm1.Day = values[2];
		tm1.Hour = values[3];
		tm1.Minute = values[4];
		tm1.Second = values[5];

		tm2.Year = values[6];
		tm2.Month = values[7];
		tm2.Day = values[8];
		tm2.Hour = values[9];
		tm2.Minute = values[10];
		tm2.Second = values[11];

		/* Print the time structures */
		printf("Time 1: ");
		print_time(&tm1);
		printf("Time 2: ");
		print_time(&tm2);
		/* Print the time difference in seconds */
		printf("The time difference is: %ld seconds\n\n", time_diff(&tm1, &tm2));
	}
	/* Close the file after reading all the lines*/
	fclose(file);
	return 0;
}
long time_to_seconds(time* tm) {
	/* Define the total seconds */
	long seconds = 0;
	/* Define the loop var */
	int i;

	/* Add the seconds of the years */
	seconds += tm->Year * 365 * 24 * 60 * 60;
	/* Add the seconds of the months */
	for (i = 0; i < tm->Month - 1; i++) {
		/* Add the seconds of the month by each month days*/
		seconds += monthsLength[i] * 24 * 60 * 60;
	}
	/* Add the seconds of the days, hours, minute, seconds */
	seconds += (tm->Day - 1) * 24 * 60 * 60;
	seconds += tm->Hour * 60 * 60;
	seconds += tm->Minute * 60;
	seconds += tm->Second;

	/* Return the total seconds */
	return seconds;
}
long time_diff(time* tm1, time* tm2)
{
	/* Define the total seconds of the time structures */
	long total_seconds1 = time_to_seconds(tm1);
	long total_seconds2 = time_to_seconds(tm2);
	/* Check if the first time is bigger than the second */
	if (total_seconds1 > total_seconds2)
	{
		/* Return the difference */
		return total_seconds1 - total_seconds2;
	}
	/* Return the difference */
	return total_seconds2 - total_seconds1;
}

void print_time(time* tm)
{
	/* Print the time structure */
	printf("%02d/%02d/%d %02d:%02d:%02d\n",
	   tm->Day, tm->Month, tm->Year,
	   tm->Hour, tm->Minute, tm->Second);
}

