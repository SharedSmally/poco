#ifndef SELECTOR_H
#define SELECTOR_H

#include <sys/select.h>

#include <Thread.h>
#include <IOEvents.h>

class Selector : public Thread
{
public:
	Selector() : running_(true)
	{
		start();
	}
	virtual ~Selector() {}


protected:
	virtual void run();

protected:
	virtual bool handleEvents(int fd, int events) = 0;

	virtual int initReadfds(fd_set & readfds)
	{
		return -1;
	}
	virtual int initWritefds(fd_set & writefds)
	{
		return -1;
	}
	virtual int initExceptfds(fd_set & exceptfds)
	{
		return -1;
	}

protected:
	bool running_;
};

void Selector::run()
{
	fd_set readfds, writefds, exceptfds;
	int fd1, fd2, fd3, fd, fdmax;
	int ret, events;
	bool flags;

	while (running_)
	{
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);

		fd1 = initReadfds(readfds);
		fd2 = initReadfds(writefds);
		fd3 = initReadfds(exceptfds);

		fdmax = fd1 > fd2 ? fd1 : fd2;

		if ( fd3 > fdmax)
			fdmax = fd3;
		++fdmax;

		ret = select(fdmax,  fd1 > 0 ? &readfds : NULL, fd2 > 0 ? &writefds : NULL,
				fd3 > 0 ? &exceptfds : NULL, NULL);

		if (ret <= 0)
			continue;

		flags = true;
		for(fd = 0; fd < fdmax; ++fd ) {
			events = 0;
			if (fd1 > 0 && FD_ISSET(fd, &readfds))
				IOEvents::setReadable(events);

			if (fd2 > 0 && FD_ISSET(fd, &writefds))
				IOEvents::setWriteable(events);

			if (fd3 > 0 && FD_ISSET(fd, &exceptfds))
				IOEvents::setException(events);

			if (events)
				flags &= handleEvents(fd, events);
		}
		if (!flags)
			break;
	}
}

#endif
/*
int select(int nfds, fd_set *readfds, fd_set *writefds,
              fd_set *exceptfds, struct timeval *timeout);

void FD_CLR(int fd, fd_set *set);
int  FD_ISSET(int fd, fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_ZERO(fd_set *set);

int pselect(int nfds, fd_set *readfds, fd_set *writefds,
       fd_set *exceptfds, const struct timespec *timeout,
                   const sigset_t *sigmask);
*/
