#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

#define CONSTANT_PROC 345000000

void do_work(unsigned int duration);

int main()
{
    do_work(5000);
    return 0;
}

void do_work(unsigned int duration){
    time_t  t0, t1;
    clock_t c0, c1;

    t0 = time(NULL);
    c0 = clock();
	
	printf("wait (ms):               %d\n", (int) duration);
    printf("begin (wall):            %ld\n", (long) t0);
    printf("begin (CPU):             %d\n", (int) c0);
	printf("############################################\n");

    unsigned int i = CONSTANT_PROC * duration;

	while(i>0){
		asm volatile("nop");
		i--;
	}

    t1 = time(NULL);
    c1 = clock();

    printf("end (wall):              %ld\n", (long) t1);
    printf("end (CPU);               %d\n", (int) c1);
    printf("elapsed time:            %f\n", (long) (t1 - t0) + (float) (c1 - c0)/CLOCKS_PER_SEC);
}
