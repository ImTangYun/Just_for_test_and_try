//
//
//
//
#include "log.h"

int main()
{
    Log log;
    log.WriteLog(DEBUG, __FILE__, __LINE__, __FUNCTION__, "%s", "hello world debug");
    log.WriteLog(INFO, __FILE__, __LINE__, __FUNCTION__, "%s", "hello world info");
    log.WriteLog(NOTICE, __FILE__, __LINE__, __FUNCTION__, "%s", "hello world notice");
    log.WriteLog(WARN, __FILE__, __LINE__, __FUNCTION__, "%s", "hello world warn");
    log.WriteLog(ERROR, __FILE__, __LINE__, __FUNCTION__, "%s", "hello world error");
    log.WriteLog(FATAL, __FILE__, __LINE__, __FUNCTION__, "%s", "hello world fatal");
    WLOG(DEBUG, "hello");
    FLOG(DEBUG, "hello");
    return 0;
}
