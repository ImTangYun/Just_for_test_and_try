//
//
//
//
#ifndef TIME_UTILS
#define TIME_UTILS
#include <time.h>
class TimeUtils
{
    public:
        int get_microseconds_of_now();
        int get_miliseconds_of_now();
        int get_second_of_now();
};

class HumanTime
{
    public:
        HumanTime()
        {
            time_t now;
            time(&now);
            time_ = localtime(&now);
        }
        ~HumanTime()
        {
        }
        void FreshTime()
        {
            time_t now;
            time(&now);
            time_ = localtime(&now);
        }
        int year()
        {
            return time_->tm_year + 1900;
        }
        int month()
        {
            return time_->tm_mon + 1;
        }
        int day()
        {
            return time_->tm_mday;
        }
        int hour()
        {
            return time_->tm_hour;
        }
        int minutes()
        {
            return time_->tm_min;
        }
        int second()
        {
            return time_->tm_sec;
        }
    private:
        struct tm *time_;
};
#endif
