
#include <mutex>
#include <memory>
#include <atomic>

#include <thread>
#include <iostream>

/*
 * Need to update the data frequently (data expired):
 * using mutex and atomic operations on double-checked
 * See https://www.justsoftwaresolutions.co.uk/threading/multithreading-in-c++0x-part-6-double-checked-locking.html
 */

#define SIMPLE_IMPL

template<typename T>
class ExpiredSingletonMgr {
    mutable std::mutex m;
    mutable std::shared_ptr<T> data;

public:
    //typedef typename std::shared_ptr<T>::deleter_type deleter_type;

    std::shared_ptr<T> get_data() {
#ifdef SIMPLE_IMPL
    	// Simple implementation:
    	std::lock_guard<std::mutex> lk(m);
        if(!data)  data.reset(new T);
        return data;
#else
        std::shared_ptr<T> result =
            //std::atomic_load_explicit(data.get(),std::memory_order_acquire);
        		std::atomic_load_explicit(&data,std::memory_order_seq_cst);

        if(!result) {
            std::lock_guard<std::mutex> lk(m);
            result=data;
            if(!result) {
                result.reset(new T);
                std::atomic_store_explicit(&data,result,std::memory_order_seq_cst);
                //std::atomic_store_explicit(data.get(),result,std::memory_order_release);
            }
        }
        return result;
#endif
    }

    void invalidate_cache() {
#ifdef SIMPLE_IMPL
        // Simple implementation
        std::lock_guard<std::mutex> lk(m);
        data.reset();
#else
    	std::lock_guard<std::mutex> lk(m);
        std::shared_ptr<T> dummy;
        std::atomic_store_explicit(&data,dummy,std::memory_order_seq_cst);
        //std::atomic_store_explicit(data.get(),dummy,std::memory_order_relaxed);
#endif
    }
};


class Singleton {
	friend class ExpiredSingletonMgr<Singleton>;
	friend class std::default_delete<Singleton>;

	Singleton() {
		std::cout << "Singleton constructor" << std::endl;
	}

public:
	~Singleton() {
		std::cout << "Singleton deconstructor" << std::endl;
	}

public:
	static Singleton & get() {
		static ExpiredSingletonMgr<Singleton> mgr_;
		return *(mgr_.get_data().get());
	}
	void print() {
		 std::cout<<"hello, singleton " << std::this_thread::get_id()  << std::endl;
	}
};

void print() {
    std::cout << "Singleton print() from thread  "
    		  << std::this_thread::get_id() <<  std::endl;
    Singleton::get().print() ;
}

int main() {
	std::thread t1(print);
	std::thread t2(print);
	t1.join(); t2.join();
}
