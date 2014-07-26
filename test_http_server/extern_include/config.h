/*
 *
 *by:yuntang
 *filename:fonfig.h
 *
 */
#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_
#include <string>
#include <map>
using std::map;
using std::string;
namespace myspace
{
class Config
{
public:
	Config(const string &path, char mode);
	~Config(){fclose(fp_);}
	string GetString(const string &key);
	bool Exist(const string &key);
	int GetInt(const string &key);
	bool GetBool(const string &key);
private:
	char mode_;
	FILE* fp_;
	void Parse(const string &input, string &key, string &value);
	map<string, string> conf_;
};
}

#endif // CONFIG_CONFIG_H_
