#include <math.h>
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <signal.h>

static size_t counter_cycles = 0;

void shutdown()
{
  syslog(LOG_NOTICE, "Shutting down now (ran through %ld cycles)\n",
         counter_cycles);
  exit(0);
}

void lizcheneyd_sigint_handler()
{
  shutdown();
}

void lizcheneyd()
{
  signal(SIGINT, lizcheneyd_sigint_handler);

  for (;;) {
    sleep(20);

    for (double i = 0; i < 5000000; i++) {
      double n = sin(i) * 3;
    }

    counter_cycles++;
  }
}
