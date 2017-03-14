#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define CONSTANT_PROC 251150 //2 times more fast
//#define CONSTANT_PROC 125575

void do_work(unsigned int duration){
    int i = 0;

    for(i = 0;i < CONSTANT_PROC * duration;i++){
        asm volatile("nop");
    }
}

void func2(){
    do_work(333);
    printf("t2\n");
}

void func3(){
    do_work(1000);
    printf("t3\n");
}

void func4(){
    do_work(2000);
    printf("t4\n");
}

int main(){
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    pid_t child1, child2, child3;

    if ((child1 = fork()) == 0)
    {
        struct sigevent sig2;
        sig2.sigev_notify = SIGEV_THREAD;
        sig2.sigev_notify_function = func2;
        sig2.sigev_value.sival_int = 42;
        sig2.sigev_notify_attributes = &attr;

        timer_t timerid2;
        timer_create(CLOCK_REALTIME, &sig2, &timerid2);

        struct itimerspec in2, out2;
        in2.it_value.tv_sec = 0;
        in2.it_value.tv_nsec = 2;
        in2.it_interval.tv_sec = 2;
        in2.it_interval.tv_nsec = 0;

        timer_settime(timerid2, 0, &in2, &out2);

        //We test the program during a cycle
        sleep(12);

        timer_delete(timerid2);

        _exit(0);
    }
    if ((child2 = fork()) == 0)
    {
        struct sigevent sig3;
        sig3.sigev_notify = SIGEV_THREAD;
        sig3.sigev_notify_function = func3;
        sig3.sigev_value.sival_int = 42;
        sig3.sigev_notify_attributes = &attr;

        timer_t timerid3;
        timer_create(CLOCK_REALTIME, &sig3, &timerid3);

        struct itimerspec in3, out3;
        in3.it_value.tv_sec = 0;
        in3.it_value.tv_nsec = 3;
        in3.it_interval.tv_sec = 3;
        in3.it_interval.tv_nsec = 0;

        timer_settime(timerid3, 0, &in3, &out3);

        //We test the program during a cycle
        sleep(12);

        timer_delete(timerid3);

        _exit(0);
    }
    if ((child3 = fork()) == 0)
    {
        struct sigevent sig4;
        sig4.sigev_notify = SIGEV_THREAD;
        sig4.sigev_notify_function = func4;
        sig4.sigev_value.sival_int = 42;
        sig4.sigev_notify_attributes = &attr;

        timer_t timerid4;
        timer_create(CLOCK_REALTIME, &sig4, &timerid4);

        struct itimerspec in4, out4;
        in4.it_value.tv_sec = 0;
        in4.it_value.tv_nsec = 4;
        in4.it_interval.tv_sec = 4;
        in4.it_interval.tv_nsec = 0;

        timer_settime(timerid4, 0, &in4, &out4);

        //We test the program during a cycle
        sleep(12);

        timer_delete(timerid4);

        _exit(0);
    }

    sleep(12);

    return 0;
}
