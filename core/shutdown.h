#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#define CYCLE_TARGET_REACHED 1
#define SIGINT_CAUGHT 2

#include "lizcheneyd.h"

void lizcheneyd_should_shutdown(lizcheneyd_start_params *params);
void lizcheneyd_shutdown_because_of(int reason,
                                    lizcheneyd_start_params *params);
void lizcheneyd_shutdown(int exit_code);

#endif /* SHUTDOWN_H */
