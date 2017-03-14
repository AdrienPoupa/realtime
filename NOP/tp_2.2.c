#define _POSIX_SOURCE

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void signal_handler(int sig)
{
    printf("child [%d] received signal %d\n", getpid(), sig);
}

int main(int argc, char *argv[])
{
    int n_task = 4;
    int i;

    pid_t pid;
    pid_t pid_array[n_task];

    for (i = 0; i < n_task; i++)
    {
        pid = fork();

        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }

        if (pid == 0) //child
        {
            printf("printed from child [%d]\n", getpid());
            signal(SIGUSR1, signal_handler);
            sleep(5);

            exit(0);
        }

        //parent
         pid_array[i] = pid;
    }

    //Code exécuté seulement par le parent
    sleep(1);

    for (i = 0; i < n_task; i++)
        kill(pid_array[i], SIGUSR1);

    for (i = 0; i < n_task; i++)
        wait(NULL);

    return (0);
}
