#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

int do_work(int duration)
{
    time_t  t0, t1;
    clock_t c0, c1;

    t0 = time(NULL);
    c0 = clock();

	printf("############################################\n");
    printf("wait (ms):               %d\n", (int) duration);
    printf("begin (wall):            %ld\n", (long) t0);
    printf("begin (CPU):             %d\n", (int) c0);
    printf("####\n");

    #ifdef WIN32
        Sleep(milliseconds);
    #elif _POSIX_C_SOURCE >= 199309L
        struct timespec ts;
        ts.tv_sec = duration / 1000;
        ts.tv_nsec = (duration % 1000) * 1000000;
        nanosleep(&ts, NULL);
    #else
        usleep(milliseconds * 1000);
    #endif

    t1 = time(NULL);
    c1 = clock();

    printf("end (wall):              %ld\n", (long) t1);
    printf("end (CPU);               %d\n", (int) c1);
    printf("elapsed time:            %f\n", (long) (t1 - t0) + (float) (c1 - c0)/CLOCKS_PER_SEC);
}

int main()
{
    do_work(1000);
    do_work(2000);
    do_work(3000);
    do_work(4000);
    do_work(5000);
    do_work(6000);
    do_work(7000);
    return 0;
}
