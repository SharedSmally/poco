
#include <mutex>
#include <memory>

#include <thread>
#include <iostream>

/*
 * Singleton: expensive to create a new object.
 */
template <typename T>
class SingletonMgr {
    mutable std::once_flag flag;
    mutable std::unique_ptr<T> data;

    void do_init() const {
        data.reset(new T);
    }

public:
    /*
     * Concurrent calls to get_data() are now safe:
     * If the data has been initialized they can just proceed concurrently.
     * If not, then all threads calling concurrently except one will wait
     * until the remaining thread has completed the initialization
     */
    T & get_data() const  {
        std::call_once(flag, &SingletonMgr::do_init, this);
        return *data;
    }
};

class Singleton {
	friend class SingletonMgr<Singleton>;
	friend class std::default_delete<Singleton>;

	Singleton() {
		std::cout << "Singleton constructor" << std::endl;
	}
	~Singleton() {
		std::cout << "Singleton deconstructor" << std::endl;
	}

public:
	static Singleton & get() {
		static SingletonMgr<Singleton> mgr_;
		return mgr_.get_data();
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
