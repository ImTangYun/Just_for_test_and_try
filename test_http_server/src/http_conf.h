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
class HttpConfig;
}
#endif // SRC_CONF_H_

