#ifndef TYPES_H
#define TYPES_H

#include <inttypes.h>

#include <map>
#include <list>
#include <deque>
#include <vector>
#include <mutex>
#include <memory>
#include <iostream>
#include <exception>

#include <typeinfo>
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>

#include <chrono>

#define CXX_14
//#define CXX_17

typedef uint32_t uint32;
typedef uint64_t uint64;

typedef std::string String;
typedef std::list<std::string> LStrings;
typedef std::deque<std::string> QStrings;
typedef std::vector<std::string> Strings;

typedef std::chrono::system_clock Clock;
typedef std::chrono::steady_clock SdClock;
typedef std::chrono::high_resolution_clock HrClock;

typedef Clock::time_point Time;
typedef Clock::duration Duration;

class NotFoundException : public std::exception
{
public:
    NotFoundException(const std::string & msg) : msg_(msg){}
    virtual ~NotFoundException() {}
    virtual const char* what() const throw()
    {
        return msg_.c_str();
    }

protected:
    std::string msg_;
};

#endif
