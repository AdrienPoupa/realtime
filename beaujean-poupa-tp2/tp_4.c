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

/*

Pour réduire la perte il nous faut chercher à rendre le plus disponible le handler pour l’écoute
d’entrée de signaux tout en autorisant les traitements. En séparant le traitement de l’écoute cela
nous permet de rendre bien plus disponible notre processus pour l’entrée d’un signal.

Ce découpage semble alors parfait et nous permettre d’intercepter toutes les interruptions.
Mais ce n’est pourtant toujours pas le cas car le gestionnaire d’interruption à toujours un temps
d’exécution qui est non nulle et supérieur à zéro. De ce fait si l’on observe deux interruptions sur un
temps ou le gestionnaire d’interruption a déjà intercepté un signal alors il ne pourra pas le traiter. 

*/
