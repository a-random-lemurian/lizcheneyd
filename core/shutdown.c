#include "shutdown.h"
#include "lizcheneyd.h"
#include <syslog.h>
#include <stdlib.h>

void should_shutdown()
{
  if (get_cycles_before_shutdown() != 0) {
    if (get_cycles_done() > get_cycles_before_shutdown()) {
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
         get_cycles_done(), reason_string);

  shutdown(0);
}

void shutdown(int exit_code)
{
  exit(exit_code);
}

