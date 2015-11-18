//
//
//
//
#include <stdio.h>
#include <unistd.h>
#include "time_utils.h"

int main()
{
    TimeUtils time_utils;
    printf("miliseconds:%d\n", time_utils.get_miliseconds_of_now());
    return 0;
}
