#include "lizcheneyd.h"
#include "shutdown.h"

#include <argparse.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

static int process_should_not_be_daemon = 0;

int main(int argc, char **argv)
{
  struct argparse ap;
  struct argparse_option options[] = {
    OPT_HELP(),
    OPT_BOOLEAN(0, "no-daemon", &process_should_not_be_daemon,
                "Do not daemonize"),
    OPT_INTEGER(0, "cycles", &cycles_before_shutdown,
                "Run for a limited number of cycles before shutdown"),
    OPT_END()
  };

  argparse_init(&ap, options, NULL, 0);

  argparse_describe(&ap, "Daemon that does nothing but pretends "
                         "to look for Liz Cheney photos.",
                         "Don't tell anyone!");
  
  argparse_parse(&ap, argc, (const char**)argv);


  if (!process_should_not_be_daemon) {
    if (daemon(0, 0) != 0) {
      printf("ERROR: Failed to daemonize.\n");
      return 1;
    }
  }

  srand(clock());

  lizcheneyd();
}
