#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define CONSTANT_PROC 479950

int cpt_start = 0;
int cpt_end = 0;

void do_work(unsigned int duration){
    int i = 0;

	for(i = 0;i < CONSTANT_PROC * duration;i++){
		asm volatile("nop");
	}
}

void signal_handler()
{
    cpt_start++;
    if(cpt_start-cpt_end > 1) printf("Interruption predecente non traitee \n");
}

void work()
{
    while(cpt_end < cpt_start)
    {
        do_work(1000);
        printf("Signal recu!\n");
        cpt_end++;
    }
}

int main()
{
    printf("%d\n", getpid());
    int i = 0;
    signal(SIGUSR1, signal_handler);

	pause();

	while(1){
        work();
	}

	return 0;
}
