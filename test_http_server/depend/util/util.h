/*
 *file:util.h
 *by:yuntang
 *
 */
#ifndef UTIL_UTIL_H_
#define UTIL_UTIL_H_
#define MAX_SEND_BUFF 1024 * 1024
#include <string>
namespace myspace
{
using std::string;
class Util
{
public:
	Util();
	~Util();
	static bool HasFile(const string &path);
	static string FielType(const string &file_name);
	static string SendFileHead(const string &type);
	static bool SafeSendFile(char* buff, int length, int fd);
};
} // namespace myspace

#endif // UTIL_UTIL_H_
