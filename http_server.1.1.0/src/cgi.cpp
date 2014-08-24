//
// by:yuntang
// filename: cgi.cpp
//
#include "cgi.h"

namespace http
{
CGI::CGI(){}

CGI::~CGI(){}

void CGI::GenerateSendFile(const string &src, const string &dis)
{
    char buff[256];
    snprintf(buff, sizeof(buff), "php %s > %s",
            src.c_str(), dis.c_str());
    system(buff);
}

void CGI::SafeRemoveFile(const string &dis)
{
    if (Util::HasFile(dis)) {
        char buff[256];
        snprintf(buff, sizeof(buff), "rm %s", dis.c_str());
        system(buff);
    }
}
} // namespace http
