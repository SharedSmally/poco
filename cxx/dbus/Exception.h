/**
 * Wrapper class of DBusError
 * https://dbus.freedesktop.org/doc/api/html/dbus-errors_8h_source.html
 *
 */
#ifndef CPW_DBUS_EXCEPTION_H
#define CPW_DBUS_EXCEPTION_H

#ifndef DBUS_ERROR_H
#define DBUS_ERROR_H
#include <dbus-errors.h>
#endif

#include <Wrapper.h>

namespace cpw {
namespace dbus {

class Error : public  Wrapper<DBusError>
{
public:
    Error()
    {
        dbus_error_init (&obj_);
    }
    Error(const char *name,const char *message)
    {
        dbus_set_error_const (&obj_, name, message);
    }
    /*
    Error(Error && error)
    {
        dbus_move_error (&(error.error_), &error_);
    }*/
    ~Error()
    {
        dbus_error_free (&obj_);
    }

    const char * name() const {
        return obj_.name;
    }
    const char * message() const {
        return obj_.message;
    }
    bool hasName(const char * name) const {
        return dbus_error_has_name  (&obj_, name);
    }

    operator bool() const {
        return dbus_error_is_set(&obj_);
    }
};

class DBusException
{
    friend std::ostream & operator<<(std::ostream & out, const  DBusException & ex)
    {
        ex.print(out);
        return out;
    }

public:
    static void check(const Error & error)
    {
        if ( error )
        {
            throw DBusException(error);
        }
    }
    DBusException(const char * name, const char * message)
       : name_(name), message_(message)
    {
    }
    DBusException(const Error & error)
       : name_(error.name()), message_(error.message())
    {
    }
    const std::string name() const {
        return name_;
    }
    const std::string message() const {
        return message_;
    }
    void print(std::ostream & out) const
    {
        out << "name:" << name_ << "; message:" << message_;
    }

protected:
    std::string name_;
    std::string message_;
};

}
}

#endif
