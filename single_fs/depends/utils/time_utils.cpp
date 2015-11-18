//
//
//
//
#include <sys/time.h>
#include "time_utils.h"

int TimeUtils::get_microseconds_of_now()
{
    return 0;
}

int TimeUtils::get_miliseconds_of_now()
{
    return get_microseconds_of_now() / 1000;
}

int TimeUtils::get_second_of_now()
{
    return get_microseconds_of_now() / 1000000;
}
