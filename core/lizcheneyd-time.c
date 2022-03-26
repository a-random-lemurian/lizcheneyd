#include <time.h>

#include "lizcheneyd-time.h"

int is_liz_cheney_birthday()
{
    time_t current_time = time(NULL);
    struct tm time_struct = *localtime(&current_time);

    if (time_struct.tm_mday == 28 && time_struct.tm_mon == 7) {
        return 1;
    }
    else {
        return 0;
    }
}
