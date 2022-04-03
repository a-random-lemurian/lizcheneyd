#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#include "lizcheneyd.h"
#include "sleep.h"

static int sleep_mode = 0;

void msleep(int tms)
{
  struct timeval tv;
  tv.tv_sec = tms / 1000;
  tv.tv_usec = (tms % 1000) * 1000;
  select(0, NULL, NULL, NULL, &tv);
}

int get_sleep_mode()
{
  return sleep_mode;
}

int set_sleep_mode(int new_mode)
{
  sleep_mode = new_mode;
}

void wait_for_next_cycle()
{
  if (sleep_mode == SLEEP_SECOND) {
    sleep(get_length_of_cycle());
  }
  else if (sleep_mode == SLEEP_MILLISECOND) {
    /*
     * In this case, we just treat the
     * length of the cycle as
     * milliseconds, not seconds.
     */
    msleep(get_length_of_cycle());
  }
}
