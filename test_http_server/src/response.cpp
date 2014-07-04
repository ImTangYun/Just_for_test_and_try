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
// #include "conf.h"
namespace http
{
using myspace::Util;
void Response::DealWithRequest(char* data, int length, int fd)
{
	char buff[100];
	snprintf(buff, sizeof(buff), "no file");
	string file_name = RequestFeile(data, length);
	if (!Util::HasFile(file_name)) {
		Util::SafeSendFile(buff, 7, fd);
	} else {
		snprintf(buff, sizeof(buff), "hello world\n");
		char buff[10000];
		int f = open(file_name.c_str(), O_RDONLY);
		int len = read(f, buff, sizeof(buff));
		Util::SafeSendFile(buff, len, fd);
		close(f);
	}
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
}

