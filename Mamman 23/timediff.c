#include "timediff.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int monthsLength[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
int main(int argc, char* argv[])
{
	int MAX_LINE = 100;
	char line[MAX_LINE];
	int values[12]; /* keep the values of the time */
	time tm1, tm2;

	FILE *file;
	if(argc != 2) {
		printf("Enter one file name\n");
		exit(1);
	}

	file = fopen(argv[1], "r");
	if(file == NULL) {
		printf("Error opening file\n");
		exit(1);
	}
	while(fgets(line, MAX_LINE, file) != NULL) {
		if (sscanf(line, "%d %d %d %d %d %d %d %d %d %d %d %d",
				   &values[0], &values[1], &values[2], &values[3], &values[4],
				   &values[5], &values[6], &values[7], &values[8], &values[9], &values[10], &values[11]) != 12) {
			printf("Invalid input\n");
			continue;
		}
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

		print_time(&tm1);
		print_time(&tm2);
		printf("The time difference in seconds is: %d\n", time_diff(&tm1, &tm2));
	}
	fclose(file);
	return 0;
}
int time_to_seconds(time* tm) {
	int i;
	int seconds = 0;
	seconds += tm->Year * 365 * 24 * 60 * 60;
	for (i = 0; i < tm->Month - 1; i++) {
		seconds += monthsLength[i] * 24 * 60 * 60;
	}
	seconds += (tm->Day - 1) * 24 * 60 * 60;
	seconds += tm->Hour * 60 * 60;
	seconds += tm->Minute * 60;
	seconds += tm->Second;
	return seconds;
}
int time_diff(time* tm1, time* tm2)
{
	return abs((time_to_seconds(tm1) - time_to_seconds(tm2)));
}

void print_time(time* tm)
{
	printf("%d/%02d/%02d %02d:%02d:%02d\n",
	   tm->Year, tm->Month, tm->Day,
	   tm->Hour, tm->Minute, tm->Second);
}

