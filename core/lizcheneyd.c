#include <math.h>
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "imgdown.h"
#include "shutdown.h"
#include "logging.h"

static size_t counter_cycles = 0;
static int cycles_before_shutdown = 0;

int get_cycles_before_shutdown()
{
  return cycles_before_shutdown;
}

size_t get_cycles_done()
{
  return counter_cycles;
}

void set_cycles_before_shutdown(int new)
{
  cycles_before_shutdown = new;
}

void lizcheneyd_sigint_handler()
{
  lizcheneyd_shutdown_because_of(SIGINT_CAUGHT);
}

void get_images_of_liz_cheney()
{
  int n;

  for (int i = 0; i < 400; i++) {
    n = rand();
  }

  if (rand() % 5000 > 4800) {
    log_info("Cycle %ld - Getting image of Liz Cheney.", counter_cycles);
    lizcheneyd_get_image_of("Liz Cheney");
  }
}

void lizcheneyd_syslog_message(log_Event *le)
{
  openlog("lizcheneyd", LOG_PID, LOG_DAEMON);

  int pri = LOG_INFO;

  if (le->level == LCD_LOG_ERROR) {
    pri = LOG_ERR;
  }
  else if (le->level == LCD_LOG_FATAL) {
    pri = LOG_CRIT;
  }
  else if (le->level == LCD_LOG_DEBUG || LCD_LOG_TRACE) {
    pri = LOG_DEBUG;
  }
  else if (le->level == LCD_LOG_WARN) {
    pri = LOG_WARNING;
  }

  syslog(pri, le->udata, le->fmt);

  closelog();
}

static const char* lizcheneyd_log_file = "/var/log/lizcheneyd.log";

void lizcheneyd_init_logging()
{
  FILE* fp = fopen(lizcheneyd_log_file, "a");

  if (fp != NULL) {
    log_add_fp(fp, LCD_LOG_INFO);
    log_info("Successfully opened file for logging.");
  }
  else if (fp == NULL) {
    log_error("Unable to open %s!", lizcheneyd_log_file);
  }

  log_add_callback(lizcheneyd_syslog_message, NULL, LCD_LOG_INFO);
}

void lizcheneyd()
{
  syslog(LOG_NOTICE, "Started lizcheneyd.");

  lizcheneyd_init_logging();

  signal(SIGINT, lizcheneyd_sigint_handler);
  log_trace("Registered SIGINT handler.");
  log_trace("Registered all signal handlers.");

  for (;;) {
    sleep(20);

    counter_cycles++;

    syslog(LOG_DEBUG, "Completed cycle %ld.", counter_cycles);

    get_images_of_liz_cheney();
    lizcheneyd_should_shutdown();

    log_trace("Completed cycle %d.", counter_cycles);
  }
}
