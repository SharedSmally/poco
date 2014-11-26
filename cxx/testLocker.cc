#include <thread>
#include <mutex>
#include <chrono>

#include <iostream>
#include <functional>

//std::lock_guard<>: own the mutex
//simplify the locking and unlocking of a mutex and provide exception safety
//is very strict on lock ownership — it owns the lock from construction to destruction

//std::unique_lock<>: more flexible than lock_guard<>
//exception safety: if the lock is held when the object is destroyed, it is released in the destructor
//1. can transfer ownership of the lock between instances
//std::unique_lock<std::mutex> acquire_lock() {
//    static std::mutex m;
//    return std::unique_lock<std::mutex>(m);
//}
//2.std::unique_lock<> does not have to own the lock on the mutex it is associated with.
//  commonly used with std::condition_variable.

/*
void update_strings()
{
    std::unique_lock<std::mutex> lk(m);
    if(strings_to_process.empty())
    {
        lk.unlock();
        std::vector<std::string> local_strings=load_strings();
        lk.lock();
        strings_to_process.insert(strings_to_process.end(),
                                  local_strings.begin(),local_strings.end());
    }
}
*/

class MyObject {
	std::mutex m;
	unsigned counter;

public:
	MyObject() : counter(0){};
	void increment(int cnt) {
		for (int i=0; i<cnt;i++) {
			std::lock_guard<std::mutex> lk(m);
			 ++counter;
		}
	}

	void decrement(int cnt) {
		for (int i=0; i<cnt;i++) {
			std::lock_guard<std::mutex> lk(m);
			--counter;
		}
	}

	unsigned query() {
		std::lock_guard<std::mutex> lk(m);
		return counter;
	}

	/*
void append_with_lock_guard(std::string const& extra) {
    std::lock_guard<std::mutex> lk(m);
    s+=extra;
}

void append_with_manual_lock(std::string const& extra) {
    m.lock();
    try {
        s+=extra;
        m.unlock();
    } catch(...)  {
        m.unlock();
        throw;
    }
}
	 */
};

int main () {
	MyObject obj;
	int cnt = 100000;

	std::thread t1(&MyObject::increment, &obj, cnt);
	std::thread t2(&MyObject::decrement, &obj, cnt);
	for (int i=0; i< 10; i++) {
		std::cout << "counter=" << obj.query() << std::endl;
		std::this_thread::sleep_for (std::chrono::seconds(1));
	}

	t1.join(); t2.join();
	std::cout << "final result=" << obj.query() << std::endl;
}
