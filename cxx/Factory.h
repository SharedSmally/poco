#ifndef FACTORY_H
#define FACTORY_H

#include <map>
#include <vector>
#include <mutex>
#include <string>
#include <memory>
#include <algorithm>
#include <iostream>
#include <iterator>

// super class for all interface classes
class Interfaceable
{
public:
    Interfaceable() { }
    virtual ~Interfaceable() {};
};
typedef std::shared_ptr<Interfaceable> InterfaceablePtr;

/////////////////////////////////////////////////////////////
class InterfaceFactory
{
public:
    InterfaceFactory() { }
    virtual ~InterfaceFactory() {};
    virtual InterfaceablePtr create() = 0;
/*
    template< class... Args >
    virtual ptr_type create( Args&&... args )
    {
        return nullptr;
    }
*/
};
typedef std::shared_ptr<InterfaceFactory> InterfaceFactoryPtr;

///////////////////////////////////////
//T must inherit from Interface
template <typename T, typename IMPL>
class InterfaceFactoryImpl : public  InterfaceFactory
{
public:
    typedef IMPL impl_type;
    typedef T interface_type;
    typedef std::shared_ptr<T> ptr_type;

public:
    InterfaceFactoryImpl()
    { }
    virtual ~InterfaceFactoryImpl()
    { }
    virtual InterfaceablePtr create()
    {
        return std::make_shared<IMPL>();
    }
    template< class... Args >
    ptr_type create( Args&&... args )
    {
        return std::make_shared<IMPL, Args&&...>(args...);
    }
};


/////////////////////////////////////////////////////////////
template<typename T>
class Factory
{
public:
    typedef T type;
    typedef T interface_type;
    typedef std::shared_ptr<T> ptr_type;
    typedef std::map<std::string, InterfaceFactoryPtr > container_type;

public:
    Factory()
    {}
    virtual ~Factory()
    {}

    template<typename IMPL>
    bool registerClass(const std::string & name)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        auto ret = impls_.insert ( std::make_pair(name, std::static_pointer_cast< InterfaceFactory >(
                std::make_shared< InterfaceFactoryImpl<T, IMPL> >())) );

        return ret.second; // false if already exists
    }
    void unregisterClass(const std::string & name)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        impls_.erase(name);
    }

    ptr_type create(const std::string & name)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        auto iter = impls_.find(name);
        if(iter == impls_.end())  //not find
            return ptr_type(nullptr);

        return std::dynamic_pointer_cast< T > (iter->second->create());
    }

    /*
    template< class... Args >
    ptr_type create(const std::string & name, Args&&... args )
    {
        std::lock_guard<std::mutex> guard(mutex_);
        auto iter = impls_.find(name);
        if(iter == impls_.end())  //not find
            return ptr_type(nullptr);

        return iter->second->create<Args&&...>(args...);
    }
    */

    template <typename CONTAINER >
    void getClassNames(CONTAINER & container)
    {
        std::transform( impls_.begin(), impls_.end(),
            std::back_inserter(container),
            [](const container_type::value_type &pair){return pair.first;});
    }


private:
    std::mutex mutex_;
    container_type impls_;

private:
    Factory(const Factory & obj) = delete;
    Factory(const Factory && obj) = delete;
    Factory& operator=(const Factory & obj) = delete;
};

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
    if ( !v.empty() ) {
        out << '[';
        std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
        out << "\b\b]";
    }
    return out;
}

#endif



//////////////////////////////
#include <Factory.h>
using namespace std;

class T0 : public Interfaceable
{
public:
    virtual ~T0() { cout << "~T0" << endl;}
    virtual void print() = 0;
    virtual void print(int n) = 0;
};
typedef std::shared_ptr<T0> T0Ptr;

class T1 : public T0 {
public:
    virtual ~T1() { cout << "~T1" << endl;}
    virtual void print() {
        cout << "T1::print()" << endl;;
    }
    virtual void print(int n) {
        cout << "T1::print(int n): n=" << n << endl;;
    }
};

class T2 : public T0 {
public:
    virtual ~T2() { cout << "~T2" << endl;}
    virtual void print() {
        cout << "T2::print()" << endl;;
    }
    virtual void print(int n) {
        cout << "T2::print(int n): n=" << n << endl;;
    }
};

class T3 : public T0 {
public:
    virtual ~T3() { cout << "~T3" << endl;}
    virtual void print() {
        cout << "T3::print()" << endl;;
    }
    virtual void print(int n) {
        cout << "T3::print(int n): n=" << n << endl;;
    }
};

int main() {
    cout << " testing test2" << endl;
    Factory<T0> f0;
    cout << "reg T1 as t1: ret=" << f0.registerClass<T1>("t1") << endl;
    cout << "reg T2 as t2: ret=" << f0.registerClass<T2>("t2") << endl;
    cout << "reg T3 as t1: ret=" << f0.registerClass<T1>("t1") << endl;

    T0Ptr ptr1 = f0.create("t1");
    if (ptr1)
    {
        ptr1->print();
        ptr1->print(10);
    }

    T0Ptr ptr2 = f0.create("t2");
    if (ptr2)
    {
        ptr2->print();
        ptr2->print(100);
    }

    T0Ptr ptr3 = f0.create("t3");
    if (ptr3)
    {
        ptr3->print();
        ptr3->print(100);
    }

    std::vector<std::string> names;
    f0.getClassNames(names);
    cout <<"class Names:"<< names << endl;

}
