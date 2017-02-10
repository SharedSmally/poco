#ifndef FDOBJECT_H
#define FDOBJECT_H

#include <unistd.h>
#include <fcntl.h>    /* Obtain O_* constant definitions */
#include <inttypes.h>

class FDObject
{
public:
	virtual ~FDObject()
	{
		if (fd_ != -1)
			close(fd_);
	};
	int fd()
	{
		return fd_;
	}
protected:
	FDObject(int fd)
	: fd_(fd)
	{
		//if (fd_ < 0) throw IOException();
	}
protected:
	int fd_;
};

#include <sys/inotify.h>
/*
int inotify_init(void);
int inotify_init1(int flags);
int inotify_add_watch(int fd, const char *pathname, uint32_t mask);
int inotify_rm_watch(int fd, int wd);
*/
class NotifyFD : public FDObject
{
public:
	NotifyFD(bool nonblock=true, bool exec=false)
       : FDObject(inotify_init1(getFlags(nonblock, exec)))
	{ }
	virtual ~NotifyFD() {}

protected:
	static int getFlags(bool nonblock, bool exec)
	{
		int flags = nonblock ? IN_NONBLOCK : 0;
		if (exec)
			flags |=  IN_CLOEXEC;
		return flags;
	}
};

class FD2Object
{
public:
	virtual ~FD2Object() {};
	virtual int readFD() = 0;
	virtual int writeFD() = 0;
};

/*
int pipe(int pipefd[2]);
int pipe2(int pipefd[2], int flags);
flags:  O_CLOEXEC;   O_DIRECT;  O_NONBLOCK
*/
class PipeFD : public FD2Object
{
public:
	PipeFD(bool nonblock=true, bool exec=false)
	{
		int flags = nonblock ? O_NONBLOCK : 0;
		if (exec)
			flags |=  O_CLOEXEC;
		int ret = pipe2(pfd, flags);
	}
	virtual ~PipeFD()
	{
		close(pfd[0]);
		close(pfd[1]);
	}
	virtual int readFD() //read pfd[0]
	{
		return pfd[0];
	}
	virtual int writeFD() //write pfd[1]
	{
		return pfd[1];
	}

protected:
	int pfd[2];
};

#include <sys/eventfd.h>
//int eventfd(unsigned int initval, int flags);
// read/write uint64_t
class EventFD : public FD2Object, public FDObject
{
public:
	EventFD(bool nonblock=true, bool exec=false,
			bool semaphore=false, int init=0)
       : FDObject( eventfd(init, getFlags(nonblock, exec, semaphore)))
	{
	}
	virtual ~EventFD()
	{
	}
	virtual int readFD()
	{
		return fd();
	}
	virtual int writeFD()
	{
		return fd();
	}

protected:
	static int getFlags(bool nonblock, bool exec, bool semaphore)
	{
		int flags = nonblock ? EFD_NONBLOCK : 0;
		if (exec)  flags |=  EFD_CLOEXEC;
		if (semaphore)  flags |=  EFD_SEMAPHORE;
		return flags;
	}
};

#endif
