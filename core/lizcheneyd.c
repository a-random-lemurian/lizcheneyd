#include <math.h>
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "imgdown.h"
#include "shutdown.h"
#include "logging.h"
#include "lizcheneyd-time.h"
#include "lizcheneyd.h"
#include "check-perms.h"
#include "sleep.h"


// TODO: Fix segfault on catching sigint
static lizcheneyd_start_params* params_at_sigint;

void lizcheneyd_sigint_handler()
{
  lizcheneyd_shutdown_because_of(SIGINT_CAUGHT, params_at_sigint);
}

static struct liz_cheney_image liz_cheney_images[] = {
  {"https://upload.wikimedia.org/wikipedia/commons/9/9a/"
   "Liz_Cheney_official_116th_Congress_portrait.jpg",
   "d6f8cfe9f487b0055e3074ea4ba9d8ad1a63767b4e339804687b8a2aabdda9b4"},
  {"https://upload.wikimedia.org/wikipedia/commons/8/88/"
   "Liz_Cheney_15800286.jpg",
   "d7afaef37dc6992655a5e1eee186b4b46972f236ebd8dc94d0e774682e64bd79"}
};

void get_images_of_liz_cheney(lizcheneyd_start_params* params)
{
  int n;

  for (int i = 0; i < 400; i++) {
    n = rand();
  }

  int liz_cheney_probability = params->probability_on_non_birthday;
  if (is_liz_cheney_birthday()) {
    /* Download more images of Liz Cheney
     * if the daemon detects that it is
     * her birthday. Or less, depending on
     * the value set by the user.
     */
    liz_cheney_probability = params->probability_on_birthday;
  }

  size_t max_number =
      params->probability_on_birthday + params->probability_on_non_birthday;

  if (rand() % max_number > liz_cheney_probability) {
    log_info("Cycle %ld - Getting image of Liz Cheney.", params->current_cycle);
    
    size_t i = rand() % (sizeof(liz_cheney_images)/
                         sizeof(struct liz_cheney_image));
    
    log_trace("Downloading image from %s",liz_cheney_images[i].url);


    lizcheneyd_get_image_of(
        &liz_cheney_images[i]);
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

void lizcheneyd_init_logging(lizcheneyd_start_params* params)
{
  FILE* fp = fopen(params->log_file, "a");

  if (fp != NULL) {
    log_add_fp(fp, LCD_LOG_INFO);
    log_info("Successfully opened file for logging.");
  }
  else if (fp == NULL) {
    log_error("Unable to open %s!", params->log_file);
  }

  log_add_callback(lizcheneyd_syslog_message, NULL, LCD_LOG_INFO);
}

void lizcheneyd_init_default_params(lizcheneyd_start_params* params)
{
  params->log_file = LIZCHENEYD_LOG_FILE;
  params->cycles = 0;          /* Infinite cycles. */
  params->cycle_time = 20;     /* 20 seconds. */
  params->sleep_mode = SLEEP_SECOND;
  params->probability_on_birthday = LIZCHENEYD_PROBABILITY_BIRTHDAY;
  params->probability_on_non_birthday = LIZCHENEYD_PROBABILITY_NON_BIRTHDAY;
}

void lizcheneyd(lizcheneyd_start_params* params)
{

  lizcheneyd_root_access_check();

  syslog(LOG_NOTICE, "Started lizcheneyd.");

  lizcheneyd_init_logging(params);

  signal(SIGINT, lizcheneyd_sigint_handler);
  log_trace("Registered SIGINT handler.");
  log_trace("Registered all signal handlers.");

  for (;;) {
    wait_for_next_cycle(params);

    params->current_cycle++;

    syslog(LOG_DEBUG, "Completed cycle %ld.", params->current_cycle);

    get_images_of_liz_cheney(params);
    lizcheneyd_should_shutdown(params);

    log_trace("Completed cycle %d.", params->current_cycle);

    params_at_sigint = params;
  }
}
