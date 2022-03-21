#include <math.h>
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <signal.h>

#include "shutdown.h"

void lizcheneyd_sigint_handler()
{
  shutdown_because_of(SIGINT_CAUGHT);
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

    should_shutdown();
  }
}
