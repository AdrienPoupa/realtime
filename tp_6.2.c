#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#define CONSTANT_PROC 479950

void do_work(unsigned int duration){
    int i = 0;

	for(i = 0;i < CONSTANT_PROC * duration;i++){
		asm volatile("nop");
	}
}

void signalHandler (Signal)
{
    //printf ("Sleep Handler\n");
    do_work(500);
}


void main (void)
{
    struct timespec tim;
    tim.tv_sec  = 0;
    tim.tv_nsec = 500000000L;

    signalHandler();
    nanosleep(&tim, NULL);
}
