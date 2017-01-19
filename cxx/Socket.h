#ifndef CHECK_UNIXFN
#define CHECK_UNIXFN(ret)  ;
#endif

template <typename T >
size_t size(const T & value)
{
	return sizeof(T);
}
template < >
size_t size(const  std::string & value)
{
	return value.size();
}

class Socket : public FDObject
{
public:
	Socket(int domain, int type, int protocol=0);
	Socket(const Address & addr, int type, int protocol=0);
	virtual ~Socket();
	void address(Address & addr)
	{
		socklen_t len = addr.addrlen();
		int ret = ::getsocketname(fd_, addr_.address(), &len);
		CHECK_UNIXFN(ret);
		//int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	}
	void peerAddress(Address & addr)
	{
		socklen_t len = addr.addrlen();
		int ret = ::getpeername(fd_, addr_.address(), &len);
		CHECK_UNIXFN(ret);
		//int getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	}

protected:
	// int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
	// int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	template <typename T >
	void getOpt(int level, int optname, T & value)
	{
		socklen_t len = size(value);
		int ret =  getsockopt(fd_, level, optname, &value, &len);
		CHECK_UNIXFN(ret);
	}
	template <typename T >
	void setOpt(int level, int optname, const T & value)
	{
		int ret =  setsockopt(fd_, level, optname, &value, size(value));
		CHECK_UNIXFN(ret);
	}

	void getOpt(int level, int optname, bool & value)
	{
		int intVal;  socklen_t len = size(intval);
		int ret =  getsockopt(fd_, level, optname, &intval, &len);
		CHECK_UNIXFN(ret);
		value = (intval != 0);
	}
	void setOpt(int level, int optname, bool value)
	{
		int intVal = value ? 1 : 0;
		int ret =  setsockopt(fd_, level, optname, &intVal, size(intVal));
		CHECK_UNIXFN(ret);
	}
};

class IPSocket : public Socket
{
public:
	IPSocket(int domain, int type, int protocol=0);
	virtual ~IPSocket(){};
};

class TcpSocket : public Socket
{
public:
	TcpSocket(int domain, int type, int protocol=0);
	virtual ~TcpSocket(){};
};

class UdpSocket : public Socket
{
public:
	UdpSocket(){};
	virtual ~UdpSocket(){};
};

class SctpSocket : public Socket
{
public:
	SctpSocket(){};
	virtual ~SctpSocket(){};
};
