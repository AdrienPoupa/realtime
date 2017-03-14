#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void signal_handler()
{
    printf("Fin du traitement du signal\n");
}

int main()
{
    signal(SIGUSR1, signal_handler);

	pause();
	return 0;
}
