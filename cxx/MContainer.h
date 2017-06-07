#ifndef MCONTAINER_H
#define MCONTAINER_H

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
private:
    std::string name_;
    CONTAINER & container_;
};

// Manageable Container:
// manage a list of objects by create();
template <typename T >
class MContainer
{
    typedef std::unique_ptr<T> ptr_type;

public:
    MContainer()
        : mutex_()
    {}
    ~MContainer()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        members_.clear();
    }

    template< class... Args >
    T * create(const std::string & name, Args&&... args )
    {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = members_.find(name);
        if (it != members_.end())
            return it->second.get();

        T * ptr = new T(*this, name, args...);
        if (ptr == nullptr)
            return nullptr;

        auto iter = members_.insert (std::make_pair(name, std::unique_ptr<T>(ptr)));
        if (iter.second) {
            return iter.first->second.get();
        } else {
            delete ptr;
            return nullptr;
        }
    }

    template< class IMPL, class... Args >
    T * createImpl(const std::string & name, Args&&... args )
    {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = members_.find(name);
        if (it != members_.end())
            return it->second.get();

        T * ptr = new IMPL(*this, name, args...);
        if (ptr == nullptr)
            return nullptr;

        auto iter = members_.insert (std::make_pair(name, std::unique_ptr<T>(ptr)));
        if (iter.second) {
            return iter.first->second.get();
        } else {
            delete ptr;
            return nullptr;
        }
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
        return (iter == members_.end()) ? nullptr : iter->second.get();
    }

private:
    std::mutex mutex_;
    std::map<std::string, ptr_type> members_;

private:
    MContainer(const MContainer & obj) = delete;
    MContainer(const MContainer && obj) = delete;
    MContainer& operator=(const MContainer & obj) = delete;
};

template <typename T >
class MContext : public MContainer< T >,
         public Containable< MContainer<MContext<T > > >
{

public:
    typedef  MContext context_type;
    typedef  MContainer < context_type > container_type;
    typedef Containable< container_type > containable_type;

public:
    MContext(container_type & container, const std::string name)
       : containable_type::Containable(container, name)
    {}
    ~MContext()
    {}
};

#endif

/*
#include <iostream>
using namespace std;

#include <MContainer.h>

class T1 : public Containable< MContainer<T1> >
{
public:
   typedef MContainer<T1> MContainerT1;
   typedef Containable< MContainerT1 > parent_type;

public:
   T1(MContainerT1 & c, const std::string & name )
      : parent_type::Containable(c,name), i_(0){
       cout << "T1::T1" << endl;
   }
   T1(MContainerT1 & c, const std::string & name, int i )
      : parent_type::Containable(c,name), i_(i)
   {
       cout << "T1::T1(int i)" << endl;
   }
   virtual ~T1() {
       cout << "T1::~T1::i=" << i_ << endl;
   }
   virtual void print() {
       cout << "T1::print(): i=" << i_ << endl;
   }
protected:
   int i_;
};

class T2 : public T1
{
public:
   T2(MContainerT1 & c, const std::string & name, int i, double j )
      : T1::T1(c,name,i), j_(j)
   {
       cout << "T2::T2(int i, int j)" << endl;
   }
   virtual ~T2() {
       cout << "T2::~T2::j=" << j_ << endl;
   }
   virtual void print() {
       cout << "T2::print(): j=" << j_ << endl;
   }
protected:
   double j_;
};

class T3
{
public:
    T3(T1::MContainerT1 & c, const std::string & name, int i)
    { }
};


void testContainer()
{
    cout << "*****************************" << endl;
    cout << " start container test"  << endl;
    T1::MContainerT1 c;
    T1 * t1 = c.create("t1");
    t1->print();

    T1 * t2 = c.create("t2", 10);
    t2->print();

    c.remove("t1");
    cout << "\ntest get"  << endl;

    T1 * t3 = c.get("t2");
    if (t3) {
        cout << " found t2" << endl;
        t3->print();
    } else {
        cout << " not found t2" << endl;
    }

    T1 * t4 = c.get("t1");
    if (t4) {
        cout << "found t1" << endl;
        t4->print();
    } else {
        cout << " not found t1" << endl;
    }
    cout << "\ntest impl"  << endl;

    T1 * t5 = c.createImpl<T2>("t5", 100, 12.34);
    t5->print();
    T1 * t6 = c.get("t5");
    if (t6) {
        cout << "found t5" << endl;
        t6->print();
    } else {
        cout << " not found t6" << endl;
    }

    //T1 * t7 = c.createImpl<T3>("t5", 100);  //compile error

    cout << "finish container test"  << endl;
}

void testContext()
{
    cout << "*****************************" << endl;
    cout << "start context test"  << endl;
    MContext<T1>::container_type cc;

    MContext<T1> * c = cc.create("context");

    T1 * t1 = c->create("t1");
    t1->print();

    T1 * t2 = c->create("t2", 10);
    t2->print();

    c->remove("t1");
    cout << "\ntest get"  << endl;

    T1 * t3 = c->get("t2");
    if (t3) {
        cout << " found t2" << endl;
        t3->print();
    } else {
        cout << " not found t2" << endl;
    }

    T1 * t4 = c->get("t1");
    if (t4) {
        cout << "found t1" << endl;
        t4->print();
    } else {
        cout << " not found t1" << endl;
    }
    cout << "\ntest impl"  << endl;

    T1 * t5 = c->createImpl<T2>("t5", 100, 12.34);
    t5->print();
    T1 * t6 = c->get("t5");
    if (t6) {
        cout << "found t5" << endl;
        t6->print();
    } else {
        cout << " not found t6" << endl;
    }

    //T1 * t7 = c->createImpl<T3>("t5", 100);  //compile error

    cout << "finish context test"  << endl;
}

int main()
{
    //
    testContainer();

    testContext();
}
*/
