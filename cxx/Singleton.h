#ifndef SINGLETON_H
#define SINGLETON_H

#include <map>
#include <mutex>
#include <memory>
#include <string>
#include <typeinfo>
#include <system_error>

template <typename To, typename From, typename Deleter>
std::unique_ptr<To, Deleter> dynamic_unique_cast(std::unique_ptr<From, Deleter>&& p) {
     if (To* cast = dynamic_cast<To*>(p.get()))
     {
           std::unique_ptr<To, Deleter> result(cast, std::move(p.get_deleter()));
            p.release();
            return result;
     }
     return std::unique_ptr<To, Deleter>(nullptr); // or throw std::bad_cast()
}

class ISingleton {
public:
    virtual ~ISingleton()
    {}
};

class SingletonMgr {
    typedef std::unique_ptr<ISingleton> ptr_type;
public:
    static SingletonMgr & getInstance()
    {
        static SingletonMgr mgr_;
        return mgr_;
    }

    virtual ~SingletonMgr()
    {}

    template <typename T >
    T & get() {
        std::string clsname = typeid(T).name();

        auto iter = container_.find(clsname);
        if ( iter != container_.end() )
            return *( dynamic_cast<T*>(iter->second.get()));

        std::unique_lock<std::mutex > lock_(mutex_);

        iter = container_.find(clsname);
        if ( iter != container_.end() )
            return *( dynamic_cast<T*>(iter->second.get()));

        T * ptr = new T();
        if (container_.insert(std::make_pair(clsname, ptr_type(ptr))).second) {
            return *ptr;
        } else {
            delete ptr;
            throw std::system_error(EINVAL, std::generic_category());
        }
    }

protected:
    std::mutex mutex_;
    std::map<std::string, ptr_type> container_;
};

inline SingletonMgr & singletonMgr() {
    return  SingletonMgr::getInstance();
}

template <typename T >
class Singleton {
    friend class SingletonMgr;

public:
    virtual ~Singleton()
    {}

    static T & getInstance()
    {
        return singletonMgr().get<T>();
    }

protected:
    Singleton()
    : value_()
    {}

    T value_;
};

#endif
