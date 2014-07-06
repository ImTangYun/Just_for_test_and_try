/*
 *
 *by:yuntang
 *fielnae:response.h
 *
 */
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "response.h"
#include "util.h"
namespace http
{
using myspace::Util;
void Response::DealWithRequest(char* data, int length, int fd)
{
	char buff[100];
	snprintf(buff, sizeof(buff), "no file");
	string file_name = http_config_.CGI();
	file_name += RequestFeile(data, length);
	printf("request file: %s\n", file_name.c_str());
	if (!Util::HasFile(file_name)) {
		SendHead(http_config_.CGI() + "./default.html", fd);
		SendFileSize(http_config_.CGI() + "./default.html", fd);
		SendFile(http_config_.CGI() + "./default.html", fd);
	} else {
		SendHead(file_name, fd);
		printf("%s\n", file_name.c_str());
		SendFileSize(file_name, fd);
		SendFile(file_name, fd);
	}
}
void Response::SendFile(const string &file_name, int fd)
{
	char buff[10000];
	int f = open(file_name.c_str(), O_RDONLY);
	int len;
	while (1) {
		len = read(f, buff, sizeof(buff));
		if (len <= 0) break; 
		Util::SafeSendFile(buff, len, fd);
	}
	close(f);
}
string Response::FileEnd(const string &file_name)
{
	if (file_name == "") return ""; 
	uint32_t i = file_name.length() - 1;
	for (; i >= 0 && file_name[i] != '.'; --i);
	++i;
	string ret;
	for (; i < file_name.length(); ++i) {
		ret += file_name[i];
	}
	printf("filename:%sfiletype:%s\n", file_name.c_str(), ret.c_str());
	return ret;
}
string Response::RequestFeile(char* data, int length)
{
	char buf[1000];
	char request[10];
	buf[0] = '.';
	buf[1] = '/';
	if (sscanf(data, "%s /%s", request, buf + 2) != 2) {
		printf("error in response.cpp-> Response->RequestFeile()\n");
		exit(1);
	}
	if (string(request) != string("GET")) {
		printf("error in response.cpp-> Response->RequestFeile()\n");
		exit(1);
	}

	return buf;
}
void Response::SendHead(const string &file_name, int fd)
{
	char buff[1000];
	int len = 0;
	char* p = buff;
	len += snprintf(buff, sizeof(buff), "HTTP/1.0 200 OK\r\n");
	len += snprintf(p + len, sizeof(buff) - len, "Content-type: %s\r\n",
			ftype_.GetType(FileEnd(file_name)).c_str());
	Util::SafeSendFile(buff, len, fd);
	printf("test:\n%s\n", buff);
}
string Response::GetType(const string &type)
{
	return ftype_.GetType(type);
}

void Response::SendFileSize(const string &file_name, int fd)
{
    unsigned long filesize = -1;
    struct stat statbuff;
    if(stat(file_name.c_str(), &statbuff) < 0){
        return;
    }else{
        filesize = statbuff.st_size;
    }
	char buff[100];
    int len = snprintf(buff, sizeof(buff), "Content-Length: %lu\r\n\n", filesize);
	Util::SafeSendFile(buff, len, fd);
	printf("%s\n", buff);
} 
} // namespace http

