//
// by:yuntang
// filename: cgi.h
//
#ifndef SRC_CGI_H_
#define SRC_CGI_H_
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "util.h"
namespace http
{
using std::string;
using myspace::Util;
class CGI
{
public:
    CGI();
    ~CGI();
    static void GenerateSendFile(const string &src, const string &dis);
    static void SafeRemoveFile(const string &dis);
};
}

#endif // SRC_CGI_H_
