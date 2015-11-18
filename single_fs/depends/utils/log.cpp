//
//
//
//
#include <stdio.h>
#include <string.h>
#include <string>
#include "time_utils.h"
#include "log.h"

Log Log::logger_;
void Log::WriteLog(int level, const char* file_name, int line,
        const char* function, const char* format, ...)
{
    va_list va;
    va_start(va, format);
    WriteLog(level, file_name, line, function, format, va);
    va_end(va);
}

void Log::WriteLog(int level, const char* file_name, int line,
        const char* function, const char* format, va_list va)
{
    if (level < level_) return;
    char buffer[4096];

    HumanTime human_time;
    int len = snprintf(buffer, sizeof(buffer), "[%s][%4d-%02d-%02d, %02d:%02d:%02d]",
            LevelStr(level).c_str(), human_time.year(), human_time.month(),
            human_time.day(), human_time.hour(), human_time.minutes(), human_time.second());
    len += snprintf(buffer + len, sizeof(buffer) - len, "[%s:%d:%s]: ", file_name,
            line, function);
    len +=vsnprintf(buffer + len, sizeof(buffer) - len, format, va);
    if (len < sizeof(buffer)) {
        buffer[len] = '\n';
        len++;
    } else {
        buffer[len  - 1] = '\n';
    }
    WriteToFile(buffer, len);
}

int Log::WriteToFile(const char* data, int size)
{
    if (size <= 0) return size;
    if (fd_ < 0) fd_ = 2;
    int ret = write(fd_, data, size);
    return ret;
}

string Log::LevelStr(int level)
{
    switch (level) {
        case 0:
            return GREEN"DEBUG "NONE;
            break;
        case 1:
            return LIGHT_GREEN"INFO  "NONE;
            break;
        case 2:
            return YELLOW"NOTICE"NONE;
            break;
        case 3:
            return BLUE"WARN  "NONE;
            break;
        case 4:
            return RED"ERROR "NONE;
            break;
        case 5:
            return PURPLE"FATAL "NONE;
            break;
        default:
            break;
            return "";
    }
}
