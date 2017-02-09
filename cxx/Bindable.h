#ifndef BINDABLE_H
#define BINDABLE_H

#include <deque>
#include <memory>
#include <typeinfo>
#include <inttypes.h>

//////////////////////////////////////
// The interface that can be binded
//////////////////////////////////////
class Bindable
{
public:
    Bindable(int id) : ctxid_(id) { }
    virtual ~Bindable() {};
    uint32_t contextID() const
    {
    return ctxid_;
    };
    void contextID(uint32_t id)
    {
    ctxid_ = id;
    }

protected:
    Bindable() : ctxid_(0) {};
    uint32_t ctxid_;
};
typedef std::shared_ptr<Bindable> BindablePtr;
///////////////////

class IBinding
{
public:
    IBinding(){}
    IBinding(BindablePtr bindable)
        : bindable_(bindable)
    {}
    virtual ~IBinding() {}

    BindablePtr & getBindable() {
        return bindable_;
    }

    virtual bool isSameType(const std::type_info &)
    {
        return false;
    }

protected:
    BindablePtr bindable_;
};
typedef std::shared_ptr<IBinding> IBindingPtr;

template <typename INTERFACE, typename IMPL>
class Binder : public IBinding
{
public:
    template <typename ...Args>
    Binder(Args&&...args)
    {
         bindable_ = std::static_pointer_cast<Bindable>(
            std::static_pointer_cast<INTERFACE>(std::make_shared<IMPL>(args...)) );
    }
    virtual ~Binder() {}

    virtual bool isSameType(const std::type_info & type)
    {
        return type == typeid(INTERFACE);
    }
};


// provide/export Bindables
class Provider
{
public:
    Provider(uint32_t ctxid)
       : ctxid_(ctxid)
    {};
    virtual ~Provider() {};
    uint32_t contextID()
    {
    return ctxid_;
    };

    template <typename INTERFACE>
    bool bind( std::shared_ptr<INTERFACE> & ptr)
    {
    for (auto iter = bindables_.begin(); iter!=bindables_.end(); ++iter)
    {
        if ((*iter)->isSameType(typeid(INTERFACE)))
        {
         ptr = std::dynamic_pointer_cast< INTERFACE >((*iter)->getBindable());
         if (ptr)
         {
             ptr->contextID(contextID());
             return true;
         }
        }
    }
    return false;
    }

    template <typename INTERFACE, typename IMPL, typename ...Args>
    void subscribe(Args&&...args)
    {
    bindables_.push_back( std::static_pointer_cast<IBinding>(
        std::make_shared< Binder<INTERFACE, IMPL> >(args...)) );
    }

    std::deque< IBindingPtr > & bindables()
    {
       return bindables_;
    }

protected:
    uint32_t ctxid_;
    std::deque< IBindingPtr > bindables_;
};


/*
// consume/import Bindables:
class Consumer
{
public:
    void bind(Provider & provider)
    {
    BIND(run); BIND(print);
    }
    void unbind(Provider & provider)
    {
    UNBIND(run); UNBIND(print);
    }
    ..................
protected:
    IRunnablePtr run;
    IPrintablePtr print;
};

class Component: virtual public Provider, Consumer
{

}
*/
#ifndef BIND
#define BIND(obj)  if (!obj)  provider.bind(obj);
#endif

#ifndef UNBIND
#define UNBIND(obj)  if (obj && obj->contextID()==provider.contextID()) obj.reset();
#endif

#endif
