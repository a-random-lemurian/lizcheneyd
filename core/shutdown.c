#include <syslog.h>
#include <stdlib.h>

#include "shutdown.h"
#include "lizcheneyd.h"

void lizcheneyd_should_shutdown(lizcheneyd_start_params* params)
{
  int cbs = params->cycles;
  int cycles_done = params->current_cycle;

  if (cbs != 0) {
    if (cycles_done > cbs) {
      lizcheneyd_shutdown_because_of(CYCLE_TARGET_REACHED, params);
    }
  }
}

void lizcheneyd_shutdown_because_of(int reason,
                                    lizcheneyd_start_params* params)
{
  char* reason_string = "None.";

  if (reason == CYCLE_TARGET_REACHED) {
    reason_string = "Target number of cycles reached.";
  }
  else if (reason == SIGINT_CAUGHT) {
    reason_string = "Caught SIGINT.";
  }

  size_t cycles = params->current_cycle;

  syslog(LOG_NOTICE,
         "Shutting down now (ran through %ld cycles). Reason: %s\n",
         cycles, reason_string);

  log_info("Shutting down now after %ld cycles. Reason: %s",
          cycles, reason_string);

  lizcheneyd_shutdown(0);
}

void lizcheneyd_shutdown(int exit_code)
{
  exit(exit_code);
}

