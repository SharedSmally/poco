#ifndef FSMonitor_H
#define FSMonitor_H

#include <map>
#include <string>
#include <FDObject.h>

/*
int inotify_init(void);
int inotify_init1(int flags);
int inotify_add_watch(int fd, const char *pathname, uint32_t mask);
int inotify_rm_watch(int fd, int wd);
*/
class FSMonitor
{
public:
	FSMonitor(bool nonblock=true, bool exec=false)
       : fd_(nonblock, exec)
	{
		fileMasks_ =  IN_OPEN | IN_CLOSE;
		dirMasks_ =  IN_OPEN | IN_CLOSE;
	}
	~FSMonitor()
	{
	}
	int fd()
	{
		return fd_.fd();
	}
	void fileMasks(uint32_t masks)
	{
		fileMasks_ = masks;
	}
	void dirMasks(uint32_t masks)
	{
		fileMasks_ = masks;
	}
	void addWatch(const std::string & path)
	{
		int wd = inotify_add_watch(fd(), path.c_str(), dirMasks_);
		watchs_.insert(std::make_pair(path, wd));
	}
	void removeWatch(const std::string & path)
	{
		auto iter = watchs_.find(path);
		if (iter != watchs_.end())
			inotify_rm_watch(fd(), iter->second);
	}

	virtual bool onEvent(uint32_t events);

protected:
	NotifyFD fd_;
	uint32_t fileMasks_;
	uint32_t dirMasks_;
	std::map<std::string, int> watchs_;
};

#include <unistd.h>
#include <iostream>

bool MSMonitor::onEvent(uint32_t events)
{
    ssize_t len;
    char *ptr;
    const struct inotify_event *event;
    char buf[4096] __attribute__ ((aligned(__alignof__(struct inotify_event))));

     /* Loop while events can be read from inotify file descriptor. */
    while (true) {
          len = read(fd(), buf, sizeof buf);
          if (len == -1 && errno != EAGAIN)
              return false;

          if (len <=0 )
              return true;

          /* Loop over all events in the buffer */
          for (ptr = buf; ptr < buf + len;  ptr += sizeof(struct inotify_event) + event->len)
          {
               event = (const struct inotify_event *) ptr;
               if (event->mask & IN_OPEN)
                     std::cout <<  "IN_OPEN: ";
               if (event->mask & IN_CLOSE_NOWRITE)
                     std::cout << "IN_CLOSE_NOWRITE: ";
               if (event->mask & IN_CLOSE_WRITE)
                     std::cout << "IN_CLOSE_WRITE: ";
               //auto iter = wds_.find(event->wd);
               //if (iter != wds.end())
               //	     std::cout << " for file/dir=" << iter->second;
               if (event->len)
                      std::cout << " name=" << event->name;
               if (event->mask & IN_ISDIR)
                      std::cout << " [directory]\n";
               else
                     std::cout << " [file]\n";
        }
    }
}

#endif
