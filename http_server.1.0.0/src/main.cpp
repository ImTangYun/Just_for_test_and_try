#include <stdio.h>
#include <unistd.h>
#include "communicate.h"
#include "http_socket_handler.h"
#include "http_conf.h"
using http::HttpSocketHandler;
using myspace::Communicate;
using http::HttpConfig;
int main()
{
	HttpConfig http_config;
	HttpSocketHandler* listen_handler = new HttpSocketHandler(http_config.Port());
	Communicate* communicate = new Communicate(listen_handler);
	communicate->Start();
	while (true) sleep(10);
	return 0;
}

