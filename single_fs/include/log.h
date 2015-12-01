//
//
//
//
#ifndef LOG
#define LOG

#include <stdarg.h>
#include <string>

// color of logs
#define NONE         "\033[m" 
#define RED          "\033[0;32;31m" 
#define LIGHT_RED    "\033[1;31m" 
#define GREEN        "\033[0;32;32m" 
#define LIGHT_GREEN  "\033[1;32m" 
#define BLUE         "\033[0;32;34m" 
#define LIGHT_BLUE   "\033[1;34m" 
#define DARY_GRAY    "\033[1;30m" 
#define CYAN         "\033[0;36m" 
#define LIGHT_CYAN   "\033[1;36m" 
#define PURPLE       "\033[0;35m" 
#define LIGHT_PURPLE "\033[1;35m" 
#define BROWN        "\033[0;33m" 
#define YELLOW       "\033[1;33m" 
#define LIGHT_GRAY   "\033[0;37m" 
#define WHITE        "\033[1;37m"

#define DEBUG   0
#define INFO    1
#define NOTICE  2
#define WARN    3
#define ERROR   4
#define FATAL   5

#define LOGGER Log::logger_
#define FIXED __FILE__, __LINE__, __FUNCTION__

#define WLOG(level, format, args...) LOGGER.WriteLog(level, FIXED, format, ##args)
#define FLOG(level, format, args...) LOGGER.WriteLog(level, FIXED, format, ##args)

using std::string;
class Log
{
    public:
        static Log logger_;
        Log()
        {
            fd_ = 2;
            level_ = DEBUG;
        }
        void WriteLog(int level, const char* file_name, int line,
                const char* function, const char* format, ...);
        void WriteLog(int level, const char* file_name, int line,
                const char* function, const char* format, va_list va);
        int WriteToFile(const char* data, int size);
        string LevelStr(int level);
        void set_level(int level)
        {
            level_ = level;
        }
    private:
        int level_;
        int fd_;
};

#endif
