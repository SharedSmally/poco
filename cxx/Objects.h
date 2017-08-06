#ifndef OBJECTS_H
#define OBJECTS_H

#include <chrono>

#include <Object.h>
#include <MutexTraits.h>

#define PREDICT_IMPL

template <typename MUTEX>
class MObject : virtual public Object
{
public:
    typedef MUTEX mutex_type;
    typedef MutexTraits< MUTEX > traits_type;
    typedef typename traits_type::guard_lock_type guard_lock_type;
    typedef typename traits_type::scoped_lock_type scoped_lock_type;
    typedef typename traits_type::unique_lock_type unique_lock_type;
    typedef typename traits_type::shared_lock_type shared_lock_type;
    typedef typename traits_type::cv_type cv_type;

public:
    MObject() : mutex_() {}
    virtual ~MObject() {}

    mutex_type & mutex() {
        return mutex_;
    }
    mutex_type & mutex() const {
        return mutex_;
    }

protected:
    mutex_type mutex_;
};


typedef MObject<std::mutex> XObject;
typedef MObject<std::timed_mutex> XtObject;

typedef MObject<std::recursive_mutex> RObject;
typedef MObject<std::recursive_timed_mutex> RtObject;

#ifdef CXX_17
typedef MObject<std::shared_mutex> CObject;
#else
typedef MObject<std::shared_timed_mutex> CObject;
#endif
typedef MObject<std::shared_timed_mutex> CtObject;

/*
 * The lockable object simulate Java Object class:
 * notify; notifyAll; wait; wait(timeout)
 */
template < typename MUTEX >
class JObject : virtual public MObject < MUTEX >
{
public:
    typedef MUTEX mutex_type;
    typedef MObject < MUTEX > parent_type;
    typedef typename parent_type::guard_lock_type guard_lock_type;
    typedef typename parent_type::scoped_lock_type scoped_lock_type;
    typedef typename parent_type::unique_lock_type unique_lock_type;
    typedef typename parent_type::shared_lock_type shared_lock_type;
    typedef typename parent_type::cv_type cv_type;

    JObject() :
#ifdef PREDICT_IMPL
        done_(false),
#endif
        cv_()
    {  }
    virtual ~JObject()
    { }
    void notify()
    {
#ifdef PREDICT_IMPL
        unique_lock_type lock_(this->mutex());
        done_ = true;
#endif
        cv_.notify_one();
    }
    void notifyAll()
    {
#ifdef PREDICT_IMPL
        unique_lock_type lock_(this->mutex());
        done_ = true;
#endif
        cv_.notify_all();
    }
    void wait()
    {
        unique_lock_type lock_(this->mutex());

#ifdef PREDICT_IMPL
        cv_.wait(lock_, [&]{return done_;});
#else
        cv_.wait( lock_);
#endif
    }

    bool wait(uint32 secs)
    {
        unique_lock_type lock_(this->mutex());

#ifdef PREDICT_IMPL
        return cv_.wait_for( lock_,  std::chrono::seconds(secs), [&]{return done_;} ) ;
#else
        return cv_.wait_for( lock_,  std::chrono::seconds(secs) ) == std::cv_status::timeout;
#endif
    }
    bool waitMS(uint32 ms, uint32 secs = 0)
    {
        unique_lock_type lock_(this->mutex());

#ifdef PREDICT_IMPL
        return cv_.wait_for( lock_,  std::chrono::seconds(secs), [&]{return done_;} ) ;
#else
        return cv_.wait_for( lock_,  std::chrono::milliseconds (1000*secs+ms) )==std::cv_status::timeout;
#endif
    }
    bool waitUS(uint32 us, uint32 secs = 0)
    {
        uint64_t timeout = 1000000L*secs+us;
        unique_lock_type lock_(this->mutex());

#ifdef PREDICT_IMPL
        return cv_.wait_for( lock_, std::chrono::microseconds (timeout), [&]{return done_;} ) ;
#else
        return cv_.wait_for( lock_, std::chrono::microseconds (timeout) )==std::cv_status::timeout;
#endif
    }

    bool waitNS(uint32 ns, uint32 secs = 0)
    {
        uint64_t timeout = 1000000000L*secs + ns;
        unique_lock_type lock_(this->mutex());
#ifdef PREDICT_IMPL
        return cv_.wait_for( lock_, std::chrono::nanoseconds (timeout), [&]{return done_;} ) ;
#else
        return cv_.wait_for( lock_, std::chrono::nanoseconds(timeout) )==std::cv_status::timeout;
#endif
    }

protected:

#ifdef PREDICT_IMPL
    bool done_;
#endif

    cv_type cv_;
};

typedef JObject<std::mutex> XJObject;
typedef JObject<std::timed_mutex> XtJObject;

typedef JObject<std::recursive_mutex> RJObject;
typedef JObject<std::recursive_timed_mutex> RtJObject;

#ifdef CXX_17
typedef JObject<std::shared_mutex> CJObject;
#else
typedef JObject<std::shared_timed_mutex> CtJObject;
#endif
typedef JObject<std::shared_timed_mutex> CtJObject;

//////////////////////////////////
#ifndef GUARD_LOCK
#define GUARD_LOCK  guard_lock_type guard_(mutex());
#endif

#ifndef SCOPED_LOCK
#define SCOPED_LOCK  scoped_lock_type lock_(mutex());
#endif

#ifndef UNIQUE_LOCK
#define UNIQUE_LOCK  unique_lock_type lock_(mutex());
#endif

#ifndef SHARED_LOCK
#define SHARED_LOCK  shared_lock_type lock_(mutex());
#endif

#endif
