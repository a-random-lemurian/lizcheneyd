#ifndef LIZCHENEYD_H
#define LIZCHENEYD_H
#include <stddef.h>

int get_cycles_before_shutdown();
size_t get_cycles_done();
void set_cycles_before_shutdown(int new_value);
void lizcheneyd_sigint_handler();
void get_images_of_liz_cheney();
void lizcheneyd_syslog_message(log_Event *le);
void lizcheneyd_init_logging();
void lizcheneyd();

#endif /* LIZCHENEYD_H */
