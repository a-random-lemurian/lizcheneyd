#include "lizcheneyd.h"
#include "shutdown.h"
#include "imgdown.h"

#include <argparse.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

static int process_should_not_be_daemon = 0;
static char* user_agent = NULL;

int main(int argc, char **argv)
{
  struct argparse ap;
  struct argparse_option options[] = {
    OPT_HELP(),
    OPT_BOOLEAN(0, "no-daemon", &process_should_not_be_daemon,
                "Do not daemonize"),
    OPT_INTEGER(0, "cycles", &cycles_before_shutdown,
                "Run for a limited number of cycles before shutdown"),
    OPT_STRING(0, "uagent", &user_agent,
               "Set a user agent"),
    OPT_END()
  };

  argparse_init(&ap, options, NULL, 0);

  argparse_describe(&ap, "Daemon that regularly downloads pictures of "
                         "Liz Cheney from the internet.",
                         "Don't tell anyone!");

  argparse_parse(&ap, argc, (const char**)argv);

  if (user_agent != NULL) {
    set_lizcheneyd_user_agent(user_agent);
  }

  if (!process_should_not_be_daemon) {
    if (daemon(0, 0) != 0) {
      printf("ERROR: Failed to daemonize.\n");
      return 1;
    }
  }

  srand(clock());

  lizcheneyd();
}
