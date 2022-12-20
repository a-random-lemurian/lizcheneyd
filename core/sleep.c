#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#include "lizcheneyd.h"
#include "sleep.h"

void msleep(int tms)
{
  struct timeval tv;
  tv.tv_sec = tms / 1000;
  tv.tv_usec = (tms % 1000) * 1000;
  select(0, NULL, NULL, NULL, &tv);
}

void wait_for_next_cycle(lizcheneyd_start_params* params)
{
  if (params->sleep_mode == SLEEP_SECOND) {
    sleep(params->cycle_time);
  }
  else if (params->sleep_mode == SLEEP_MILLISECOND) {
    /*
     * In this case, we just treat the
     * length of the cycle as
     * milliseconds, not seconds.
     */
    msleep(params->cycle_time);
  }
}
