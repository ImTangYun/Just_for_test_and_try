//
//
//
//
#ifndef TIME_UTILS
#define TIME_UTILS
#include <time.h>
#include <sys/time.h>
#include <vector>
using std::vector;

class TimeUtils
{
    public:
        int get_microseconds_of_now();
        int get_miliseconds_of_now();
        int get_second_of_now();
};

class TimeCounter
{
    public:
        TimeCounter():capacity_(10)
        {
            gettimeofday(&starttime_,0);
        }
        TimeCounter(int capacity):capacity_(capacity)
        {
            gettimeofday(&starttime_,0);
        }
        double GetTimeCosts(int num)
        {
            if (num >= times_.size() && times_.size() > 0) {
                num = times_.size();
            } else {
                return 0;
            }
            return times_[num - 1];
        }
        void AddNow()
        {
            struct timeval endtime;
            gettimeofday(&endtime,0);
            double timeuse = 1000000*(endtime.tv_sec - starttime_.tv_sec)
                + endtime.tv_usec - starttime_.tv_usec;
            timeuse /=1000;
            times_.push_back(timeuse);
        }
    private:
        vector<double> times_;
        int capacity_;
        struct timeval starttime_;
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
