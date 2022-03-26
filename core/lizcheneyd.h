#ifndef LIZCHENEYD_H
#define LIZCHENEYD_H
#include "logging.h"
#include <stddef.h>

/*
 * Return the number of cycles that lizcheneyd will run for, when
 * in limited cycle mode. When the function returns 0, it means
 * that there is no limit on the number of cycles that lizcheneyd
 * will run for.  
 */
int get_cycles_before_shutdown();

/*
 * Return the number of cycles that have passed since lizcheneyd
 * was started.
 */
size_t get_cycles();

/*
 * Set cycles before shutdown, if lizcheneyd will only run for
 * a limited number of cycles.
 */
void set_cycles_before_shutdown(int new_value);

/*
 * lizcheneyd's SIGINT handler.
 */
void lizcheneyd_sigint_handler();

/*
 * Decide whether to download images of Liz Cheney. When it is
 * Liz Cheney's birthday, this function will more often decide
 * to download images of Liz Cheney.
 */
void get_images_of_liz_cheney();

/*
 * Translate log messages sent out by lizcheneyd to it's own
 * special log into a suitable message for /var/log/syslog.
 */
void lizcheneyd_syslog_message(log_Event *le);

/*
 * Initialize lizcheneyd's logging.
 */
void lizcheneyd_init_logging();

/*
 * The main function of lizcheneyd. Contains an infinite loop which runs
 * lizcheneyd's regular operations.
 */
void lizcheneyd();

#endif /* LIZCHENEYD_H */
