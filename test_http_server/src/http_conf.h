/*
 *by:yuntang
 *filename:conf.h
 *
 */
#ifndef SRC_CONF_H_
#define SRC_CONF_H_
#include <string>
#include "config.h"

namespace http
{
using std::string;
using myspace::Config;
class FTypeConfig
{
public:
	FTypeConfig(const string &path, char mode);
	~FTypeConfig();
	bool ExistType(const string& file_type);
	string GetType(const string& file_type);
	bool Init();
private:
	Config ftype_config_;
};
class HttpConfig
{
public:
	HttpConfig();
	~HttpConfig();
	string CGI()
	{
		return http_config_.GetString("cgi");
	}
	int Thread_num()
	{
		return http_config_.GetInt("thread_num");
	}
	string Tmp()
	{
		return http_config_.GetString("tmp");
	}
	int Port()
	{
		return http_config_.GetInt("PORT");
	}
private:
	Config http_config_;
};
}
#endif // SRC_CONF_H_

