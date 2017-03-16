#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define CONSTANT_PROC 251150 //2 times more slow
//#define CONSTANT_PROC 125575

int lock2 = 0;
int lock3 = 0;
int lock4 = 0;

void do_work(unsigned int duration){
    int i = 0;

    for(i = 0;i < CONSTANT_PROC * duration;i++){
        asm volatile("nop");
    }
}

void func2(){
    while(lock2 == 0){}
    do_work(333);
    printf("t2\n");
    lock2 = 0;
    func2();

}

void func3(){
    while(lock3 == 0){}
    do_work(1000);
    printf("t3\n");
    lock3 = 0;
    func3();

}

void func4(){
    while(lock4 == 0){}
    do_work(2000);
    printf("t4\n");
    lock4 = 0;
    func4();
}

void unlock2(){
    lock2 = 1;
}

void unlock3(){
    lock3 = 1;
}

void unlock4(){
    lock4 = 1;
}

int main(){
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    pthread_t t2,t3,t4;

    //Timer for task 2
    struct sched_param parm;
    parm.sched_priority = 100;
    pthread_attr_setschedparam(&attr, &parm);
    pthread_setschedparam(t2,SCHED_OTHER,&parm);

    struct sigevent sig2;
    sig2.sigev_notify = SIGEV_THREAD;
    sig2.sigev_notify_function = unlock2;
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

    //Timer for task 3
    struct sched_param parm1;
    parm1.sched_priority = 150;
    pthread_attr_setschedparam(&attr, &parm1);
    pthread_setschedparam(t3,SCHED_OTHER,&parm1);

    struct sigevent sig3;
    sig3.sigev_notify = SIGEV_THREAD;
    sig3.sigev_notify_function = unlock3;
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

    //Timer for task 4
    struct sched_param parm2;
    parm2.sched_priority = 255;
    pthread_attr_setschedparam(&attr, &parm2);
    pthread_setschedparam(t4,SCHED_OTHER,&parm2);

    struct sigevent sig4;
    sig4.sigev_notify = SIGEV_THREAD;
    sig4.sigev_notify_function = unlock4;
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


    pthread_create(&t3,NULL, func3, NULL);
    pthread_create(&t4,NULL, func4, NULL);
    pthread_create(&t2,NULL, func2, NULL);

    //We test the program during a cycle
    while(1);
    //sleep(12);

    timer_delete(timerid2);
    timer_delete(timerid3);
    timer_delete(timerid4);

    return 0;
}