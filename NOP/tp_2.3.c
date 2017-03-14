#include <unistd.h>
#include <stdio.h>
#include <signal.h>

/**
 * The type sig_atomic_t is used in C99 to guarantee
 * that a variable can be accessed/modified in an atomic way
 * in the case an interruption (reception of a signal for example) happens.
 */
static volatile sig_atomic_t done_waiting = 0;

static void     handler()
{
  printf("Signal caught\n");
  done_waiting = 1;
}

void    my_pause()
{
  /**
   *  In ISO C, the signal system call is used
   *  to call a specific handler when a specified
   *  signal is received by the current process.
   *  In POSIX.1, it is encouraged to use the sigaction APIs.
   **/
  signal(SIGINT, handler);
  done_waiting = 0;
  while ( !done_waiting )
    ;
}

int     main()
{
  my_pause();
  printf("Hey ! The first call to my_pause returned !\n");
  my_pause();
  printf("The second call to my_pause returned !\n");
  return (0);
}
