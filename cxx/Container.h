#ifndef CONTAINER_H
#define CONTAINER_H

#include <memory>
#include <map>
#include <mutex>

template <typename CONTAINER >
class Containable
{
public:
    typedef CONTAINER container_type;

    Containable(CONTAINER & container, const std::string name)
       : container_(container), name_(name)
    {}
    ~Containable()
    {}

    CONTAINER & container() {
        return container_;
    }
    const CONTAINER & container() const {
        return container_;
    }
    const std::string & name() {
        return name_;
    }
    const std::string & name() const {
        return name_;
    }

protected:
    // called by concrete class constructor
    virtual void addTo() {
        container_.add(name_, this);
    }
    // called by concrete class deconstructor
    virtual void removeFrom() {
        container_.remove(name_);
    }

private:
    std::string name_;
    CONTAINER & container_;
};

// Container: only contains a list of objects;
template <typename T >
class Container
{
    typedef T * ptr_type;

public:
    Container()
        : mutex_()
    {}
    ~Container()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        members_.clear();
    }

    template <typename IMPL>
    bool add(const std::string & name, IMPL * obj)
    {
        if (obj == nullptr)
            return false;

        std::lock_guard<std::mutex> lock(mutex_);

        return members_.insert( std::make_pair(name, static_cast<T *>(obj)) ).second;
    }

    void remove(const std::string & name)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        members_.erase(name);
    }

    T * get(const std::string & name)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        auto iter = members_.find(name);
        return (iter == members_.end()) ? nullptr : iter->second;
    }

private:
    std::mutex mutex_;
    std::map<std::string, ptr_type> members_;

private:
    Container(const Container & obj) = delete;
    Container(const Container && obj) = delete;
    Container& operator=(const Container & obj) = delete;
};

template <typename T >
class Context : public Container< T >,
         public Containable< Container<Context<T > > >
{

public:
    typedef  Context context_type;
    typedef  Container < context_type > container_type;
    typedef Containable< container_type > containable_type;

public:
    Context(container_type & container, const std::string name)
       : containable_type::Containable(container, name)
    {}
    ~Context()
    {}

private:
    Context(const Context & obj) = delete;
    Context(const Context && obj) = delete;
    Context & operator=(const Context & obj) = delete;
};

#endif
/*
#include <iostream>
using namespace std;

#include <Container.h>

class T1 : public Containable< Container<T1> >
{
public:
   typedef Container<T1> ContainerT1;
   typedef Containable< ContainerT1 > parent_type;

public:
   T1(ContainerT1 & c, const std::string & name )
      : parent_type::Containable(c,name), i_(0)
   {
       cout << "T1::T1" << endl;
       addTo();
   }
   T1(ContainerT1 & c, const std::string & name, int i )
      : parent_type::Containable(c,name), i_(i)
   {
       cout << "T1::T1(int i)" << endl;
       addTo();
   }
   virtual ~T1()
   {
       cout << "T1::~T1::i=" << i_ << endl;
       removeFrom();
   }
   virtual void print() {
       cout << "T1::print(): i=" << i_ << endl;
   }
protected:
   int i_;
};


void testContainer()
{
    cout << "*****************************" << endl;
    cout << " start container test"  << endl;
    T1::ContainerT1 c;

    T1 t1 (c, "t1");
    t1.print();

    T1 * t2 = c.get("t1");
    t2->print();

    c.remove("t1");
    cout << "\ntest get"  << endl;

    T1 * t3 = c.get("t1");
    if (t3) {
        cout << " found t1" << endl;
        t3->print();
    } else {
        cout << " not found t1" << endl;
    }

    cout << "finish container test"  << endl;
}

void testContext()
{
    cout << "*****************************" << endl;
    cout << "start context test"  << endl;
    Context<T1>::container_type cc;

    Context<T1> c(cc, "context");
    T1 t1 (c, "t1");
    t1.print();

    T1 * t2 = c.get("t1");
    t2->print();

    c.remove("t1");
    cout << "\ntest get"  << endl;

    T1 * t3 = c.get("t1");
    if (t3) {
        cout << " found t1" << endl;
        t3->print();
    } else {
        cout << " not found t1" << endl;
    }

    cout << "finish context test"  << endl;
}

int main()
{
    //
    testContainer();

    testContext();
}

*/
