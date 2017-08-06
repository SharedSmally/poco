#ifndef STREAMABLE_H
#define STREAMABLE_H

#include <iostream>

class InStreamable
{
    friend std::istream & operator>>(std::istream & in, InStreamable & obj)
    {
        obj.input(in); return in;
    }

public:
    virtual ~InStreamable() {}
    void input( )  {  input(std::cin);   }
    virtual void input(std::istream & in ) = 0;
};

class OutStreamable
{
    friend std::ostream & operator<<(std::ostream & out, const OutStreamable & obj)
    {
        obj.output(out); return out;
    }

public:
    virtual ~OutStreamable() {}
    void output( ) {  output(std::cout); }

    virtual void output(std::ostream & out ) const = 0;
};

class Streamable : virtual public InStreamable, virtual public OutStreamable
{
public:
    virtual ~Streamable() {}
};

#endif
