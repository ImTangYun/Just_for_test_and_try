/*
 *by:yuntang
 *filename:conf.cpp
 *
 */
#include <stdio.h>
#include "http_conf.h"
namespace http
{
FTypeConfig::FTypeConfig(const string& path, char mode = '='):ftype_config_(path, mode)
{
}
FTypeConfig::~FTypeConfig()
{
}
string FTypeConfig::GetType(const string& file_type)
{
	if (ExistType(file_type)) {
		return ftype_config_.GetString(file_type);
	}
	return "application/x-gzip";
}
bool FTypeConfig::Init()
{
	return true;
}
bool FTypeConfig::ExistType(const string& file_type)
{
	return ftype_config_.Exist(file_type);
}
} // namespace http

