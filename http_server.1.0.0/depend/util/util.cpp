/*
 *file:util.cpp
 *by:yuntang
 *
 */
#include <unistd.h>
#include <socket.h>
#include "util.h"
namespace myspace
{
bool Util::HasFile(const string &path)
{
	return access(path.c_str(), R_OK) == 0;
}
string Util::FielType(const string &file_name)
{
	if (file_name.size() == 0) return "";
	string end;
	uint32_t i;
	for (i = file_name.size() - 1; i >= 0 && file_name[i] != '.'; --i);
	if (i == 0) return "";
	for (uint32_t j = i + 1; j < file_name.size(); ++j) {
		end += file_name[j];
	}
	return end;
}
string Util::SendFileHead(const string &type)
{
	if (type == "jpg") return "jpg";
	return "";
}
bool Util::SafeSendFile(char* buff, int length, int fd)
{
	int left = length;
	int total = 0;
	while (total < length) {
		int n = send(fd, buff + total, left, 0);
		if (n < 0)
			return false;
		left -= n;
		total += n;
	}
	return true;
}
} // namespace myspace

