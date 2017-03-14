#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CONSTANT_PROC 479950

void do_work(unsigned int duration){
    int i = 0;

	for(i = 0;i < CONSTANT_PROC * duration;i++){
		asm volatile("nop");
	}
}

int main()
{
    do_work(2000);
    return 0;
}
