#ifndef CONTEXT_H
#define CONTEXT_H

#include <Types.h>
#include <sstream>

//Query objects based on class name (interface, )
const static char SEPARATOR = '/';
template <typename T >
class Context : virtual public T
{
protected:
    typedef std::unique_ptr<Context> ptr_type;
    typedef std::map<String, ptr_type> contexts_type;

public:
    Context(const std::string & name)
      : ctx_(nullptr), name_(name)
    {}
    virtual ~Context()
    {}

    Context & createContext(const std::string & name)
    {
        if (name.empty())
            throw NotFoundException(name);

        Strings tmp;
        split(tmp, name, SEPARATOR);
        Context * ctx = name[0]==SEPARATOR ? &(getRoot()) : this;

        for (size_t ind=0; ind<tmp.size(); ++ind ) {
            auto iter = ctx->subctxs_.find(tmp[ind]);
            if ( iter == ctx->subctxs_.end() ) {
                auto myiter = ctx->subctxs_.insert(std::make_pair(tmp[ind], ptr_type( new Context(tmp[ind], ctx)))) ;
                if (!myiter.second) {
                    throw NotFoundException(name);
                } else {
                    ctx = myiter.first->second.get();
                }
            } else {
                ctx = iter->second.get();
            }
        }
        return *ctx;
    }


    bool isRoot() const
    {
        return ctx_ == nullptr;
    }
    Context & getRoot()
    {
        return parent() == nullptr ? *this : parent()->getRoot();
    }
    const Context & getRoot() const
    {
        return parent() == nullptr ? *this : parent()->getRoot();
    }
    Context * parent()
    {
        return ctx_;
    }
    const Context * parent() const
    {
        return ctx_;
    }
    const std::string & name()
    {
        return name_;
    }
    const std::string & name() const
    {
        return name_;
    }
    std::string  fullname() const
    {
        QStrings names;
        contextNames(names);
        return join(names, SEPARATOR);
    }
    void contextNames(Strings & names) const
    {
        QStrings  qnames;
        contextNames(qnames);
        for (auto it = qnames.begin(); it != qnames.end(); ++it )
            names.push_back(*it);

    }
    Strings contextNames() const
    {
        Strings  names;
        contextNames(names);
        return names;
    }
    bool containsContext(const String & name)
    {
        if (name.empty())
            return false;

        Strings tmp; int index=0;
        split(tmp, name, SEPARATOR);
        return name[0]==SEPARATOR ? getRoot().containsContext(tmp, index) : containsContext(tmp, index);
    }
    bool containsContext(const Strings & names)
    {
        int index=0;
        return containsContext(names,index);
    }

    Context & getContext(const String & name)
    {
        if (name.empty())
            throw NotFoundException(name);

        Strings tmp; int index=0;
        split(tmp, name, SEPARATOR);
        return name[0]==SEPARATOR ? getRoot().getContext(tmp,index) : getContext(tmp, index);
    }
    Context & getContext(const Strings & names)
    {
        int index=0;
        return getContext(names,index);
    }

private:
    Context(const String & name, Context * ctx)
    :  ctx_(ctx), name_(name)
    {};

protected:
    void contextNames(QStrings & names) const
    {
        names.push_front(name_);
        if (parent() != nullptr)
            parent()->contextNames(names);
    }

    bool containsContext(const Strings & names, int index)
    {
        return subctxs_.find(names[index]) != subctxs_.end();
    }

    Context & getContext(const Strings & names, int index)
    {
        auto iter = subctxs_.find(names[index]);

        if ( iter == subctxs_.end() )
            throw NotFoundException(name);

        return iter->second->getContext(names, ++index);
    }

private:
    Context * ctx_;
    std::string name_;
    contexts_type subctxs_;
};

#endif
