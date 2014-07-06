#include <stdio.h>
#include <unistd.h>
#include "Work_flow.h"
#include "communicate.h"
#include "http_socket_handler.h"
#include "Cthread.h"
#include "http_conf.h"
using cthread::WorkFlow;
using http::HttpSocketHandler;
using myspace::Communicate;
using http::HttpConfig;
int main()
{
	HttpConfig http_config;
	HttpSocketHandler* listen_handler = new HttpSocketHandler(http_config.Port());
	Communicate* communicate = new Communicate(listen_handler);
	communicate->Start();
	WorkFlow work;
	work.Init(4);
	work.Start();
	sleep(1);
	sleep(100);
	work.Stop();
	sleep(5000);
	return 0;
}

