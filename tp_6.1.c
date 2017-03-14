#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define CONSTANT_PROC 479950

void do_work(unsigned int duration){
    int i = 0;

	for(i = 0;i < CONSTANT_PROC * duration;i++){
		asm volatile("nop");
	}
}

void signalHandler(){
}

void main (void)
{
    signal(SIGALRM, signalHandler);

    while(1){
        alarm(1);
        do_work(500);
        pause();
    }
}

/* Ce code n'est pas correct car il est bloquant */
