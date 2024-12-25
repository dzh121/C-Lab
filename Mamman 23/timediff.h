#ifndef TIMEDIFF_H
#define TIMEDIFF_H

typedef struct
{
    int Year;
    int Month;
    int Day;
    int Hour;
    int Minute;
    int Second;
} time;

void print_time(time*);

int time_diff(time*, time*);

int time_to_seconds(time*);

#endif
