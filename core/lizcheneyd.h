#ifndef LIZCHENEYD_H
#define LIZCHENEYD_H
#include <stddef.h>

void lizcheneyd();
int get_cycles_before_shutdown();
size_t get_cycles_done();
void set_cycles_before_shutdown(int new);

#endif /* LIZCHENEYD_H */
