http://en.cppreference.com/w/cpp

<memory>
shared_ptr<T>: ref counted smart pointer; multiple owners of the pointer; similar to poco shared_ptr
   the object pointed is not ref counter; shared_ptr<T> has the ref counter to record
   the number of reference. The deleter can be provided in the constructor

unique_ptr<T>: Stores a pointer to an owned object.one owner 
   The object is owned by no other unique_ptr. 
   The object is destroyed when the unique_ptr is destroyed.
   unique_ptr uniquely manages a resource. Each unique_ptr object stores a pointer to 
   the object that it owns or stores a null pointer. A resource can be owned by no more 
   than one unique_ptr object; when a unique_ptr object that owns a particular 
   resource is destroyed, the resource is freed. A unique_ptr object may be moved, 
   but not copied
      
weak_ptr<T>: not owner of the pointer, need transfer to shared_ptr<T> before using it.

lock:
mutex: Mutex class 
recursive_mutex: Recursive mutex class
timed_mutex: Timed mutex class
recursive_timed_mutex: Recursive timed mutex 
shared_timed_mutex: Read/Write locker (cxx14)

A recursive mutex is a lockable object, just like mutex, but allows the same thread 
to acquire multiple levels of ownership over the mutex object.


std::lock_guard<>: owns the lock from construction to destruction
std::unique_lock<>: may unlock/lock between its construction to destruction; 
   normally used with variable_condition
   if the lock is held when the object is destroyed, it is released in the destructor
   
sleep: 
   std::this_thread::sleep_for(std::chrono::seconds(1));

<future>
Providers
promise: A promise is an object that can store a value of type T to be retrieved by a 
         future object (possibly in another thread), offering a synchronization point.
   Producer: calls set_value to unblock consumer; pass future to consumer
   Consumer: calls get_future().get_value() to receive result(blocked if not set)(receive future)
   
packaged_task: 
	
template <class Ret, class... Args> class packaged_task<Ret(Args...)>;
future<Ret> packaged_task.get_future();

Futures
future: Future class
shared_future: Shared future class
Valid" futures are future objects associated to a shared state, and are 
constructed by calling one of the following functions:
  1. async
  2. promise::get_future
  3. packaged_task::get_future

A shared_future object behaves like a future object, except that it can be copied, and 
that more than one shared_future can share ownership over their end of a shared state.
shared_future objects can be implicitly converted from future objects, 
or explicitly obtained by calling future::share. 

The lifetime of the shared state lasts at least until the last object with which it is 
associated is destroyed. Retrieving the value from a shared_future (with member get) 
does not release its ownership over the shared state (unlike with futures). Therefore, 
if associated to shared_future objects, the shared state can survive the object from 
which it was obtained in the first place (if any).


Variadic template
C++11
Prior to C++11, templates (classes and functions) can only take a fixed number of 
arguments that have to be specified when a template is first declared. C++11 allows 
template definitions to take an arbitrary number of arguments of any type.

template<typename... Values> class tuple;

The number of arguments can be zero, so tuple<> some_instance_name; will work as well.

If one does not want to have a variadic template that takes 0 arguments, then this
definition will work as well:
   template<typename First, typename... Rest> class tuple;

Variadic templates may also apply to functions, thus not only providing a type-safe 
add-on to variadic functions (such as printf) - but also allowing a printf-like 
function to process non-trivial objects.
   template<typename... Params> 
   void printf(const std::string &str_format, Params... parameters);

If the function can simply pick one argument at a time - using a dumb expansion marker:

template<typename... Args> inline void pass(Args&&...) {}

which can be used as follows:

  template<typename... Args> inline void expand(Args&&... args) {
    pass( some_function(args)... );
  } 

  expand(42, "answer", true);
which will expand to something like:
  pass( some_function(arg1), some_function(arg2), some_function(arg3) etc... );

static const int size = sizeof...(Args);

1.recursive define function.
2.






 


