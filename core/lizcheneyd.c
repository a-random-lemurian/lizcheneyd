#include <math.h>
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "imgdown.h"
#include "shutdown.h"


static size_t counter_cycles = 0;
static int cycles_before_shutdown = 0;

int get_cycles_before_shutdown()
{
  return cycles_before_shutdown;
}

size_t get_cycles_done()
{
  return counter_cycles;
}

void lizcheneyd_sigint_handler()
{
  shutdown_because_of(SIGINT_CAUGHT);
}

void get_images_of_liz_cheney()
{
  int n;

  for (int i = 0; i < 400; i++) {
    n = rand();
  }

  if (rand() % 5000 > 4800) {
    get_image("Liz Cheney");
  }
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

    syslog(LOG_DEBUG, "Completed cycle %ld.", counter_cycles);

    get_images_of_liz_cheney();
    should_shutdown();
  }
}
