#ifndef STREAMABLE_H
#define STREAMABLE_H

#include <iostream>

class IStreamable
{
    friend std::istream & operator>>(std::istream & in, IStreamable & obj)
    {
        obj.input(in); return in;
    }

public:
    virtual ~IStreamable() {}
    void input( )  {  input(std::cin);   }
    virtual void input(std::istream & in ) = 0;
};

class OStreamable
{
    friend std::ostream & operator<<(std::ostream & out, const OStreamable & obj)
    {
        obj.output(out); return out;
    }

public:
    virtual ~OStreamable() {}
    void output( ) {  output(std::cout); }

    virtual void output(std::ostream & out ) const = 0;
};

class Streamable : virtual public IStreamable, virtual public OStreamable
{
public:
    virtual ~Streamable() {}
};

#endif
