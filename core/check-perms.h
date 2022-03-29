#ifndef CHECK_PERMS_H
#define CHECK_PERMS_H
#include <stddef.h>

int lizcheneyd_check_dir_access(const char* directory, size_t dir_siz);
int lizcheneyd_root_access_check();

#endif /* CHECK_PERMS_H */
