/*
 *by:yuntang
 *filename:package.cpp
 *
 */
#include <time.h>
#include "package.h"
namespace myspace
{
Package::Package(void*ptr, int length, int fd)
{
	data_ = ptr;
	length_ = length;
	fd_ = fd;
	time_ = time(0);
}
Package::~Package()
{
}
int Package::GetFd()
{
	return fd_;
}
const void* Package::data()
{
	return data_;
}
int Package::length()
{
	return length_;
}
int Package::Time()
{
	return time_;
}
void Package::SetTime(int time)
{
	time_ = time;
}
} // namespace myspace

