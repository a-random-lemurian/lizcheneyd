#include "shutdown.h"
#include "lizcheneyd.h"
#include <syslog.h>
#include <stdlib.h>

void lizcheneyd_should_shutdown()
{
  if (get_cycles_before_shutdown() != 0) {
    if (get_cycles_done() > get_cycles_before_shutdown()) {
      lizcheneyd_shutdown_because_of(CYCLE_TARGET_REACHED);
    }
  }
}

void lizcheneyd_shutdown_because_of(int reason)
{
  char* reason_string = "None.";

  if (reason == CYCLE_TARGET_REACHED) {
    reason_string = "Target number of cycles reached.";
  }

  syslog(LOG_NOTICE,
         "Shutting down now (ran through %ld cycles). Reason: %s\n",
         get_cycles_done(), reason_string);

  lizcheneyd_shutdown(0);
}

void lizcheneyd_shutdown(int exit_code)
{
  exit(exit_code);
}

