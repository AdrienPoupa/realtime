#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define CONSTANT_PROC 479950

static int i = 0;

void do_work(unsigned int duration){
    int i = 0;

	for(i = 0;i < CONSTANT_PROC * duration;i++){
		asm volatile("nop");
	}
}

void func1(){
    do_work(333);
    printf("333\n");
}

void func2(){
    do_work(1000);
    printf("1000\n");
}

void func3(){
    do_work(2000);
    printf("2000\n");
}

//Function handler called every 1 second
void sighler (union sigval val){
    func1();
    func2();
    func3();
    func1();
    func2();
    func1();
    func3();
    func1();
    func2();
    func1();
    func3();
    func1();
    func2();
}

int main(){
    int Ret;

    pthread_attr_t attr;
    pthread_attr_init( &attr );

    struct sched_param parm;
    parm.sched_priority = 255;
    pthread_attr_setschedparam(&attr, &parm);

    struct sigevent sig;
    sig.sigev_notify = SIGEV_THREAD;
    sig.sigev_notify_function = sighler;
    sig.sigev_value.sival_int = 42;
    sig.sigev_notify_attributes = &attr;

    //create a new timer.
    timer_t timerid;
    Ret = timer_create(CLOCK_REALTIME, &sig, &timerid);

    if (Ret == 0){
        struct itimerspec in, out;
        in.it_value.tv_sec = 0;
        in.it_value.tv_nsec = 10;
        in.it_interval.tv_sec = 13;
        in.it_interval.tv_nsec = 0;

        //issue the periodic timer request here.
        Ret = timer_settime(timerid, 0, &in, &out);

        if(Ret == 0){
            sleep(13);
        }
        else{
            printf("timer_settime() failed with %d\n", errno);
        }

        //delete the timer.
        timer_delete(timerid);
    }
    else{
        printf("timer_create() failed with %d\n", errno);
    }

    return Ret;
}
