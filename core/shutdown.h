#ifndef SHUTDOWN_CONDITIONS_H
#define SHUTDOWN_CONDITIONS_H

#define CYCLE_TARGET_REACHED 1
#define SIGINT_CAUGHT 2

void should_shutdown();
void shutdown_because_of(int reason);
void shutdown(int exit_code);

#endif /* SHUTDOWN_CONDITIONS_H */
