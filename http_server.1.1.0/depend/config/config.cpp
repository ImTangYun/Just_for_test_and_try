/*
 *
 *by:yuntang
 *filename:fonfig.cpp
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include <iostream>

using namespace std;

namespace myspace
{
Config::Config(const string &path, char mode = '='): mode_(mode)
{
	FILE* fp = fopen(path.c_str(), "rb");
	if (fp == NULL) {
		printf("open %s failed!\n", path.c_str());
		exit(1);
	}
	char* line = NULL;
	size_t len;
	while (getline(&line, &len, fp) != -1) {
		printf("%s", line);
		string key;
		string value;
		Parse(line, key, value);
		conf_[key] = value;
	}
	for (auto iter = conf_.begin(); iter != conf_.end(); ++iter) {
		printf("key:%s, value:%s\n", iter->first.c_str(), iter->second.c_str());
	}
}
void Config::Parse(const string &input, string &key, string &value)
{
	uint32_t i = 0;
	while (i < input.length() && input[i] != mode_) {
		key += input[i];
		++i;
	}
	++i;
	for (; i < input.length() && input[i] != '\n'; ++i) {
		value += input[i];
	}
}
string Config::GetString(const string &key)
{
	string ret = "";
	map<string, string>::iterator iter = conf_.find(key);

	if (iter != conf_.end()) {
		ret = iter->second;
	}
	return ret;
}
bool Config::Exist(const string &key)
{
	if (key == "") {
		return false;
	} 
	printf("finding:%s\n", key.c_str());
	map<string, string>::iterator iter = conf_.find(key);
	return (iter != conf_.end());
}
int Config::GetInt(const string &key)
{
	string ret = "";
	map<string, string>::iterator iter = conf_.find(key);
	if (iter != conf_.end()) {
		ret = iter->second;
	}
	int int_num = atoi(ret.c_str());
	return int_num;
}
bool Config::GetBool(const string &key)
{
	string ret = "";
	map<string, string>::iterator iter = conf_.find(key);
	if (iter != conf_.end()) {
		ret = iter->second;
	}
	if ((ret == string("false")))
		return false;
	cout << ret << endl;
	return true;
}
}

