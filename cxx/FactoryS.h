#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <map>
#include <memory>

template <typename T>
class Factory
{
public:
    typedef std::shared_ptr<T> ptr_type;

public:
    Factory()    {}
    ~Factory()    {}

    ptr_type create(const std::string & name);
    
    template <typename IMPL>
    bool registerFactory(const std::string & name);

private:
    class FactoryImpl
    {
    public:
        virtual ~FactoryImpl(){};
        virtual ptr_type create() = 0;
    };
    typedef std::shared_ptr<FactoryImpl> factory_ptr_type;
    typedef std::map<std::string, factory_ptr_type> container_type;

    template <typename IMPL>
    class TFactoryImpl : public FactoryImpl
    {
    public:
        virtual ~TFactoryImpl(){};
        virtual ptr_type create()
        {
            return std::static_pointer_cast<T> (std::make_shared<IMPL>());
        }
    };

private:
    container_type container_;
    Factory(const Factory & obj) = delete;
    Factory& operator=(const Factory & obj) = delete;
};


template <typename T>
typename Factory<T>::ptr_type Factory<T>::create(const std::string & name)
{
    auto iter = container_.find(name);
    return iter==container_.end() ? ptr_type(nullptr) : iter->second->create();
}

template <typename T>
template <typename IMPL>
bool Factory<T>::registerFactory(const std::string & name)
{
    return container_.insert(std::make_pair(name, std::make_shared< TFactoryImpl<IMPL > >() )).second;
}

#endif

##############################################################
#include <iostream>
using namespace std;

#include <Factory.h>

class C1
{
public:
    virtual ~C1() { cout << "C1::~C1"<<endl;};
    virtual void print() = 0;
};

class C2 : public C1
{
public:
    virtual ~C2() { cout << "C2::~C2"<<endl;}
    virtual void print() {
        cout << "C2::print"<<endl;
    }
};

class C3 : public C1
{
public:
    virtual ~C3() { cout << "C3::~C3"<<endl;}
    virtual void print() {
        cout << "C3::print"<<endl;
    }
};

typedef Factory<C1> FactoryC1;

int main()
{
    FactoryC1 f1;
    f1.registerFactory<C2>("c2");
    f1.registerFactory<C3>("c3");

    FactoryC1::ptr_type c2 = f1.create("c2");
    c2->print();

    FactoryC1::ptr_type c3 = f1.create("c3");
    c3->print();

}
