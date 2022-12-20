#ifndef LIZCHENEYD_H
#define LIZCHENEYD_H
#include "logging.h"
#include <stddef.h>

#ifdef __APPLE__
#warning lizcheneyd is not guaranteed to work on Mac!
#elif __WIN32__
#error lizcheneyd is not supported on Windows. \
       Remove this macro at your own risk.
#endif

#define LIZCHENEYD_LOG_FILE "/var/log/lizcheneyd.log"
#define LIZCHENEYD_PROBABILITY_NON_BIRTHDAY  4800
#define LIZCHENEYD_PROBABILITY_BIRTHDAY      500

typedef struct lizcheneyd_start_params {
  char *user_agent;
  char *log_file;
  int non_root_mode;

  // How many cycles to run before shutting down. 0 is infinite.
  size_t cycles;
  // Current cycle number.
  size_t current_cycle;

  // Time between cycles, in either seconds or milliseconds depending on sleep mode
  unsigned int cycle_time;

  // Sleep mode. 1 for milliseconds, 0 for seconds.
  int sleep_mode;

  // Likelihood of downloading an image when it is not Liz Cheney's birthday
  int probability_on_non_birthday;
  // Likelihood of downloading an image when it is Liz Cheney's birthday
  int probability_on_birthday;
} lizcheneyd_start_params;

/*
 * Initialize lizcheneyd's default parameters.
 */
void lizcheneyd_init_default_params(lizcheneyd_start_params *params);

/*
 * lizcheneyd's SIGINT handler.
 */
void lizcheneyd_sigint_handler();

/*
 * Decide whether to download images of Liz Cheney. When it is
 * Liz Cheney's birthday, this function will more often decide
 * to download images of Liz Cheney.
 */
void get_images_of_liz_cheney(lizcheneyd_start_params* params);

/*
 * Translate log messages sent out by lizcheneyd to it's own
 * special log into a suitable message for /var/log/syslog.
 */
void lizcheneyd_syslog_message(log_Event *le);

/*
 * Initialize lizcheneyd's logging.
 */
void lizcheneyd_init_logging(lizcheneyd_start_params* params);

/*
 * The main function of lizcheneyd. Contains an infinite loop which runs
 * lizcheneyd's regular operations.
 */
void lizcheneyd(lizcheneyd_start_params* params);

#endif /* LIZCHENEYD_H */
