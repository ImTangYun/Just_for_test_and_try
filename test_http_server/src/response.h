/*
 *
 *by:yuntang
 *fielnae:response.h
 *
 */
#ifndef SRC_RESPONSE_H_
#define SRC_RESPONSE_H_
#include <string>
#include "http_conf.h"
namespace http
{
using std::string;
class Response
{
public:
	Response():ftype_(string("../conf/ftype.conf"), '='){}
	~Response(){}
	void DealWithRequest(char* data, int length, int fd);
private:
	string RequestFeile(char* data, int length);
	void SendHead(const string &file_name);
	FTypeConfig ftype_;
};
}

#endif // SRC_RESPONSE_H_
