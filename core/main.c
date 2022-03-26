#include <argparse.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "lizcheneyd.h"
#include "shutdown.h"
#include "imgdown.h"
#include "logging.h"

static int dont_fork = 0;
static int cycles_before_shutdown = 0;
static char* user_agent = NULL;

int main(int argc, char **argv)
{
  struct argparse ap;
  struct argparse_option options[] = {
    OPT_HELP(),

    /* When lizcheneyd is started as a systemd service, without
     * the --no-return option, systemd reports that the daemon fails
     * to start. */
    OPT_BOOLEAN(0, "no-return", &dont_fork,
                "Do not return until daemon has finished running."),
    OPT_BOOLEAN(0, "no-daemon", NULL, "Ignored (retained for backwards compatibility)"),
    OPT_INTEGER(0, "cycles", &cycles_before_shutdown,
                "Run for a limited number of cycles before shutdown"),
    OPT_STRING(0, "uagent", &user_agent,
               "Set a user agent"),
    OPT_END()
  };

  argparse_init(&ap, options, NULL, 0);

  argparse_describe(&ap, "Daemon that regularly downloads pictures of "
                         "Liz Cheney from the internet.",
                         "Don't tell anyone!\n"

                         "The --no-return option should be used when\n"
                         "starting lizcheneyd under an init daemon\n"
                         "such as systemd.");

  argparse_parse(&ap, argc, (const char**)argv);

  set_cycles_before_shutdown(cycles_before_shutdown);

  if (user_agent != NULL) {
    lizcheneyd_set_uagent(user_agent);
  }


  if (!dont_fork) {
    log_info("Making daemon system call.");
    if (daemon(0, 0) != 0) {
      printf("ERROR: Failed to daemonize.\n");
      return 1;
    }
  }
  else {
    log_info("--no-return flag set, not forking.");
  }

  srand(clock());

  lizcheneyd();
}
