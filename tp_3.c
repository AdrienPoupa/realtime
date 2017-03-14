#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define CONSTANT_PROC 479950

void do_work(unsigned int duration){
    int i = 0;

	for(i = 0;i < CONSTANT_PROC * duration;i++){
		asm volatile("nop");
	}
}

void signal_handler()
{
    printf("Debut traitement signal\n");
	//do_work(5000);
    printf("####Fin du traitement du signal\n");
}

int main()
{
    printf("%d\n", getpid());
    int i = 0;
    signal(SIGUSR1, signal_handler);

	pause();

	while(1){}

	return 0;
}

/*

On intègre la fonction do_work (cf. q1) dans notre fonction signal handler de manière à
rendre son travail plus conséquent et alors nous essayons d’envoyer une rafale de signal et à partir
de ce moment nous observons que nous ne récupérons pas tous les signaux émis. Ils ne sont pas tous
captés. Le souci est lié au fait que lorsque que le programme est entrain de traiter un signal il ne peut
capter un autre signal.

La raison pour laquelle tous les signaux ne sont pas traités c’est que lors d’un traitement les
multiples interruptions reçus ne sont pas comptabilisées. On n’en compte qu’une seule en réalité. De
ce fait toutes les interruptions arrivant durant un traitement ne seront comptabilisées comme une
seule et unique, d’où les pertes de signaux

*/