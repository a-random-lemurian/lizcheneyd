#ifndef SHUTDOWN_CONDITIONS_H
#define SHUTDOWN_CONDITIONS_H

#define CYCLE_TARGET_REACHED 1
#define SIGINT_CAUGHT 2

void lizcheneyd_should_shutdown();
void lizcheneyd_shutdown_because_of(int reason);
void lizcheneyd_shutdown(int exit_code);

#endif /* SHUTDOWN_CONDITIONS_H */
