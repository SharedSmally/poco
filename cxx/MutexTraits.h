#ifndef MUTEXTRAITS_H
#define MUTEXTRAITS_H

#include <mutex>
#include <shared_mutex>
#include <condition_variable>

// mutex type:
//mutex           recursive_mutex             shared_mutex (c++17)
//timed_mutex     recursive_timed_mutex       shared_timed_mutex (c++14)
//
//lock_guard    scoped_lock (for multiple mutex)
//unique_lock   shared_lock

// conditional_variable types:
//  condition_variable ;
//  condition_variable_any ;

template <typename MUTEX >
struct MutexTraits
{
    typedef MUTEX mutex_type;
    typedef std::lock_guard<MUTEX >   guard_lock_type;

#ifdef CXX_17
    typedef std::scoped_lock <MUTEX > scoped_lock_type;  //C++17
#else
    typedef std::lock_guard <MUTEX > scoped_lock_type;
#endif

    typedef std::unique_lock<MUTEX >  unique_lock_type;
    typedef std::shared_lock<MUTEX >  shared_lock_type;
    typedef std::condition_variable_any  cv_type;

};

template < >
struct MutexTraits < std::mutex >
{
    typedef std::mutex mutex_type;
    typedef std::lock_guard<std::mutex >   guard_lock_type;

#ifdef CXX_17
    typedef std::scoped_lock <std::mutex > scoped_lock_type;
#else
    typedef std::lock_guard<std::mutex >   scoped_lock_type;
#endif

    typedef std::unique_lock<std::mutex >  unique_lock_type;
    typedef std::unique_lock<std::mutex >  shared_lock_type;
    typedef std::condition_variable   cv_type;
};

#endif
