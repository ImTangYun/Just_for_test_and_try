/*
 *
 *by:yuntang
 *fielnae:response.h
 *
 */
#ifndef SRC_RESPONSE_H_
#define SRC_RESPONSE_H_
#include <string>
namespace http
{
using std::string;
class Response
{
public:
	Response(){}
	~Response(){}
	void DealWithRequest(char* data, int length, int fd);
private:
	string RequestFeile(char* data, int length);
};
}

#endif // SRC_RESPONSE_H_
