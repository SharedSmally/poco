/**
 * Wrapper class of DBusError
 * https://dbus.freedesktop.org/doc/api/html/dbus-errors_8h_source.html
 *
 */
#ifndef CPW_DBUS_WRAPPER_H
#define CPW_DBUS_WRAPPER_H

namespace cpw {
namespace dbus {

template <typename T >
class Wrapper {

public:
    Wrapper()
    {}
    virtual ~Wrapper()
    {}

    /////////////////////////////
    T &get() {
        return obj_;
    }
    const T & get() const {
        return obj_;
    }
    T * ptr() {
        return &obj_;
    }
    const T * ptr() const {
        return &obj_;
    }

    ////////////////////////////
    T & operator*() {
        return obj_;
    }
    const T & operator*() const {
        return obj_;
    }
    T * operator->() {
        return &obj_;
    }
    const T * operator->() const {
        return &obj_;
    }
protected:
    T obj_;
};

template <typename T >
class Wrapper <T *> {

public:
    Wrapper()
       : ptr_(nullptr)
    {}
    virtual ~Wrapper()
    {}

    /////////////////////////////
    T &get() {
        return *ptr_;
    }
    const T & get() const {
        return *ptr_;
    }
    T * ptr() {
        return ptr_;
    }
    const T * ptr() const {
        return &ptr_;
    }
    /////////////////////////////
    T & operator*() {
        return *ptr_;
    }
    const T & operator*() const {
        return *ptr_;
    }
    operator bool() const {
        return ptr_ != nullptr;
    }
    T * operator->() {
        return ptr_;
    }
    const T * operator->() const {
        return ptr_;
    }

protected:
    T * ptr_;
};

}
}

#endif
