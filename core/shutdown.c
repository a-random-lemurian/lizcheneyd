#include "counters.h"
#include "shutdown.h"
#include <syslog.h>

void should_shutdown()
{
  if (cycles_before_shutdown != 0) {
    if (counter_cycles > cycles_before_shutdown) {
      shutdown_because_of(CYCLE_TARGET_REACHED);
    }
  }
}

void shutdown_because_of(int reason)
{
  char* reason_string = "None.";

  if (reason == CYCLE_TARGET_REACHED) {
    reason_string = "Target number of cycles reached.";
  }

  syslog(LOG_NOTICE,
         "Shutting down now (ran through %ld cycles). Reason: %s\n",
         counter_cycles, reason_string);

  shutdown(0);
}

void shutdown(int exit_code)
{
  exit(exit_code);
}

