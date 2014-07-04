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
	char* p = buff;
	int offset = 0;
	if (length < MAX_SEND_BUFF) {
		printf("i am coming here\n");
		return send(fd, buff, length, 0) > 0 ;
	} else {
		while (1) {
			int s_length = ((length - offset - 1) >= MAX_SEND_BUFF)?MAX_SEND_BUFF:(length - offset);
			p = buff;
			int ret = send(fd, p + offset, s_length, 0);
			if (ret < 0)
				return false;
			else if (ret < MAX_SEND_BUFF) {
				return true;
			} else {
				offset += MAX_SEND_BUFF;
			}
		}
	}

	return true;
}
} // namespace myspace

