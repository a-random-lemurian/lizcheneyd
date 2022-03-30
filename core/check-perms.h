#ifndef CHECK_PERMS_H
#define CHECK_PERMS_H
#include <stddef.h>

/*
 * Create a temporary dummy file in the lizcheneyd directory
 * to check for access.
 */
int lizcheneyd_check_dir_access(const char* directory, size_t dir_siz);

/*
 * Check if lizcheneyd has root access using getuid().
 */
int lizcheneyd_root_access_check();

#endif /* CHECK_PERMS_H */
