//const struct sockaddr *addr
/*
struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
};
*/
class Address
{
public:
	Address();
	virtual ~Address();

	virtual struct sockaddr * address() = 0;
	virtual const struct sockaddr * address() const = 0;
	virtual socklen_t addrlen() const = 0;
	uint16_t domain() const
	{
		return address()->sa_family;
	}
	/*
	struct sockaddr & address() = 0;
	const struct sockaddr & address() const = 0;

	struct sockaddr * addressPtr()
	{
		return &(address());
	}
	const struct sockaddr * addressPtr() const
	{
		return &(address());
	}
	*/

};

//unix_socket = socket(AF_UNIX, type, 0);
/*
struct sockaddr_un {
    sa_family_t sun_family;               // AF_UNIX
    char        sun_path[108];            // pathname
};*/

class UnixAddress : public Address
{
public:
	UnixAddress(const std::string & pname)
	{
		memset(&addr_, '\0', sizeof(addr_));
		addr_.sun_family = AF_UNIX;
		pathname(pname);
	}
	UnixAddress(const char * pname, int len)
	{
		memset(&addr_, '\0', sizeof(addr_));
		addr_.sun_family = AF_UNIX;
		pathname(pname, len);
	}
	virtual ~UnixAddress()
	{}

	struct sockaddr * address()
	{
		return (struct sockaddr *)(&addr_);
	}
	const struct sockaddr * address() const
	{
		return (const struct sockaddr *)(&addr_);
	}
	socklen_t addrlen() const
	{
		return sizeof(addr_);
	}
	void pathname(const char *pname, size_t size )
	{
		size_t max =  sizeof(addr_.sun_path) - 1;
		if (size < max)
			max = size;
		memcpy( addr_.sun_path, pname, max);
		addr_.sun_path[max-1]='\0';
	}
	void pathname(const std::string & pname)
	{
		pathname(pname.c_str(), pname.size());
	}
	const char * pathname() const
	{
		return addr_.sun_path;
	}

private:
	struct sockaddr_un addr_;
};


/*
 *  struct sockaddr_in {
         sa_family_t    sin_family; // address family: AF_INET
         in_port_t      sin_port;   // port in network byte order
         struct in_addr sin_addr;  // internet address
     };
      struct in_addr { // Internet address.
          uint32_t  s_addr;     // address in network byte order
     };
 */
class Ip4Address : public Address
{
public:
	// host such as 10.0.0.1
	Ip4Address(const std::string & host, unsigned short port=0){
		memset(&addr_, '\0', sizeof(addr_));
		addr_.sin_family = AF_INET;
		addr_.sin_port = htons(port);
		inet_pton(AF_INET, host.c_str(), &addr_.sin_addr);
	}
	virtual ~Ip4Address()
	{}

	void port(uint16_t p)
	{
		addr_.sin_port = htons(p);
	}
	uint16_t port() const
	{
		return ntohs(addr_.sin_port);
	}

	// store this IP address in sa:
	//"192.0.2.33"
	void host(const std::string & h)
	{
		inet_pton(AF_INET, h.c_str(), &(addr_.sin_addr));
	}

	void host(uint32_t h)
	{
		addr_.sin_addr.s_addr = htonl(h);
	}
	uint32_t host() const
	{
		return ntohl(addr_.sin_addr.s_addr);
	}

	std::string hostString() const
	{
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(addr_.sin_addr), str, INET_ADDRSTRLEN);
		return std::string(str,INET_ADDRSTRLEN);
	}

	struct sockaddr * address()
	{
		return (struct sockaddr *)(&addr_);
	}
	const struct sockaddr * address() const
	{
		return (const struct sockaddr *)(&addr_);
	}
	socklen_t addrlen() const
	{
		return sizeof(addr_);
	}

private:
	struct sockaddr_in addr_;
};

/*
// IPv6 AF_INET6 sockets:
struct sockaddr_in6 {
    u_int16_t       sin6_family;   // address family, AF_INET6
    u_int16_t       sin6_port;     // port number, Network Byte Order
    u_int32_t       sin6_flowinfo; // IPv6 flow information
    struct in6_addr sin6_addr;     // IPv6 address
    u_int32_t       sin6_scope_id; // Scope ID
};
struct in6_addr {
    unsigned char   s6_addr[16];   // load with inet_pton()
};
 */
class Ip6Address : public Address
{
public:
	// host as "2001:db8:8714:3a90::12"
	Ip6Address(const std::string & host, unsigned short port = 0)
	{
		memset(&addr_, '\0', sizeof(addr_));
		addr_.sin6_family = AF_INET6;
		addr_.sin6_port = htons(port);
		inet_pton(AF_INET6, host.c_str(), &addr_.sin6_addr);
	}
	virtual ~Ip6Address();

	void port(uint16_t p)
	{
		addr_.sin6_port = htons(p);
	}
	uint16_t port() const
	{
		return ntohs(addr_.sin6_port);
	}

	//"2001:db8:8714:3a90::12"
	void host(const std::string & h)
	{
		inet_pton(AF_INET6, h.c_str(), &(addr_.sin6_addr));
	}

	std::string host() const
	{
		char str[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &(addr_.sin6_addr), str, INET6_ADDRSTRLEN);
		return std::string(str,INET6_ADDRSTRLEN);
	}


	struct sockaddr * address()
	{
		return (struct sockaddr *)(&addr_);
	}
	const struct sockaddr * address() const
	{
		return (const struct sockaddr *)(&addr_);
	}
	socklen_t addrlen() const
	{
		return sizeof(addr_);
	}

protected:
	struct sockaddr_in6 addr_;
};

/*
// General socket address holding structure, big enough to hold either
// struct sockaddr_in or struct sockaddr_in6 data:

struct sockaddr_storage {
    sa_family_t  ss_family;     // address family

    // all this is padding, implementation specific, ignore it:
    char      __ss_pad1[_SS_PAD1SIZE];
    int64_t   __ss_align;
    char      __ss_pad2[_SS_PAD2SIZE];
};
 */
class IpAddress : public Address
{
#define MAX_IP_ADDR_LEN =;
public:
	IpAddress(bool ip4, const std::string & h, uint16_t p = 0)
	{
		addr_.ss_family = ip4 ? AF_INET  : AF_INET6;
		host(h); port(p);
	}
	virtual ~IpAddress();

	struct sockaddr * address()
	{
		return (struct sockaddr *)(&addr_);
	}
	const struct sockaddr * address() const
	{
		return (const struct sockaddr *)(&addr_);
	}
	socklen_t addrlen() const
	{
		return sizeof(addr_);
	}

	void host(const std::string & h)
	{
	    switch(addr_.ss_family) {
	        case AF_INET:
	        	inet_pton(AF_INET, h.c_str(), &(((struct sockaddr_in *)(&addr_))->sin_addr));
	        	break;

	        case AF_INET6:
	        	inet_pton(AF_INET6, h.c_str(), &(((struct sockaddr_in6 *)(&addr_))->sin6_addr));
	        	break;
	    }
	}

	std::string host() const
	{
	    switch(addr_.ss_family) {
	        case AF_INET:
	            char s4[INET_ADDRSTRLEN];
	            inet_ntop(AF_INET, &(((struct sockaddr_in *)(&addr_))->sin_addr),s4, INET_ADDRSTRLEN );
	            return std::string(s4, INET_ADDRSTRLEN);

	        case AF_INET6:
	        	char s6 [INET6_ADDRSTRLEN];
	        	inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)(&addr_))->sin6_addr),s6, INET6_ADDRSTRLEN);
	            return std::string(s6, INET6_ADDRSTRLEN);

	        default:
	            return std::string();
	    }
	}
	void port(uint16_t p)
	{
		if (isIP4()) {
			((struct sockaddr_in *)(&addr_))->sin_port = htons(p);
		} else {
			((struct sockaddr_in6 *)(&addr_))->sin6_port = htons(p);
		}

	}
	uint16_t port() const
	{
			return isIP4() ? ntohs( ((const struct sockaddr_in *)(&addr_))->sin_port )
						:  ntohs( ((const struct sockaddr_in6 *)(&addr_))->sin6_port );
	}

	bool isIP4() const {
		return addr_.ss_family == AF_INET;
	}
	bool isIP6() const {
		return addr_.ss_family == AF_INET6;
	}

protected:
	struct sockaddr_storage addr_;
};
