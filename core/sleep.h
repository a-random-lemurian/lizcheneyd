#ifndef LIZCHENEYD_SLEEP_H
#define LIZCHENEYD_SLEEP_H

int get_sleep_mode();
int set_sleep_mode(int new_mode);
void wait_for_next_cycle();

#define SLEEP_SECOND 0
#define SLEEP_MILLISECOND 1

#endif /* LIZCHENEYD_SLEEP_H */
