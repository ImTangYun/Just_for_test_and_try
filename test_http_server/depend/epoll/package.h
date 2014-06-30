/*
 *by:yuntang
 *filename:package.h
 *
 */
#ifndef PACKAGE_H_
#define PACKAGE_H_
namespace myspace
{
class Package
{
public:
	Package(void*ptr, int length, int fd);
	~Package();
	const void* data();
	void SetData(void*ptr);
	int length();
	void SetTime(int time);
	int Time();
	int GetFd();
private:
	void* data_;
	int length_;
	int fd_;
	int time_;
};
} // namespace myspace

#endif // PACKAGE_H_
