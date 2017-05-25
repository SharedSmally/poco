#ifndef WRAPPER_H
#define WRAPPER_H

template <typename T >
class Wrapper
{
public:
    typedef T value_type;
    typedef T * ptr_type;
    typedef T & ref_type;

public:
    Wrapper()
    : obj_()
    {}
    template< class... Args >
    Wrapper( Args&&... args )
     : obj_(args...)
    {
    }
    virtual ~Wrapper()
    {}

    T & obj() { return obj_; }
    const T & obj() const { return obj_; }

    T * ptr() { return &obj_; }
    const T * ptr() const { return &obj_; }

    operator bool() const {
        return true;
     }
    bool operator ()() const {
        return true;
     }
private:
    T obj_;

private:
    Wrapper(const Wrapper & obj) = delete;
    Wrapper& operator=(const Wrapper & obj) = delete;
};

template <typename T >
class Wrapper<T *>
{
public:
    typedef T value_type;
    typedef T * ptr_type;
    typedef T & ref_type;

public:
    Wrapper()
    : ptr_(nullptr)
    {}
    Wrapper( T * ptr )
     : ptr_(ptr)
    {
    }
    virtual ~Wrapper()
    {}

    T & obj() { return *ptr_; }
    const T & obj() const { return *ptr_; }

    T * ptr() { return ptr_; }
    const T * ptr() const { return ptr_; }

    operator bool() const {
        return ptr_ != nullptr;
    }
    bool operator ()() const {
        return ptr_ != nullptr;
    }
private:
    T * ptr_;

private:
    Wrapper(const Wrapper & obj) = delete;
    Wrapper& operator=(const Wrapper & obj) = delete;
};

template <typename T >
class Wrapper < T & >
{
public:
    typedef T value_type;
    typedef T * ptr_type;
    typedef T & ref_type;

public:
    Wrapper( T & obj )
     : obj_(obj)
    {
    }
    virtual ~Wrapper()
    {}

    T & obj() { return obj_; }
    const T & obj() const { return obj_; }

    T * ptr() { return &obj_; }
    const T * ptr() const { return &obj_; }

    operator bool() const {
        return true;
     }
    bool operator ()() const {
        return true;
    }
private:
    T & obj_;

private:
    Wrapper(const Wrapper & obj) = delete;
    Wrapper& operator=(const Wrapper & obj) = delete;
};

#endif

#include <iostream>
#include <Wrapper.h>
using namespace std;

int main()
{
    int val=10;

    Wrapper<int> w1(20);
    cout << "v1: val=" << w1.obj() << "; ptr=" << w1.ptr() << ";bool=" << (bool)(w1) << endl;

    Wrapper<int * > w2(&val);
    cout << "v2: val=" ;
    if ( w2 ) {
        cout << w2.obj();
    } else {
        cout << "null";
    }
    cout << "; ptr=" << w2.ptr() << ";bool=" << (bool)(w2) << endl;

    Wrapper<int *> w3;
    cout << "v3: val=" ;
    if ( w3() ) {
        cout << w3.obj();
    } else {
        cout << "null";
    }
    cout << "; ptr=" << w3.ptr() << ";bool=" << (bool)(w3) << endl;

    Wrapper<int&> w4(val);
    cout << "v4: val=" << w4.obj() << "; ptr=" << w4.ptr() << ";bool=" << (bool)(w4) << endl;
};
