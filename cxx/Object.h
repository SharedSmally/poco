#ifndef OBJECT_H
#define OBJECT_H

#include <functional>

#include <Types.h>
#include <Comparable.h>

class Object : virtual public Comparable
{
    friend std::ostream & operator<<(std::ostream & out, const Object & obj)
    {
        obj.print(out);
        return out;
    }

public:
    virtual ~Object()
    {}

    virtual size_t hashCode() const
    {
        return (size_t)(this);
    }
    virtual String toString() const
    {
        std::ostringstream oss;
        print(oss);
        return oss.str();
    }

    virtual CompareResult compare(const Comparable &obj) const
    {
        if (this==&obj)  return EQ;
        const Object * tmp = dynamic_cast<const Object *>(&obj);
        if (tmp==nullptr) return ::compare((size_t)(this), (size_t)(&obj));
        return _compare(*tmp);
    }

    virtual void print(std::ostream & out) const
    {
        out << hashCode();
    }

protected:
    virtual CompareResult _compare(const Object &obj) const
    {
        return ::compare(hashCode(), obj.hashCode());
    }
};

/*
template <>
struct std::hash<Object>
{
    size_t operator()(const Object & obj)
    {
        return obj.hashCode();
    }
};
*/

#endif
