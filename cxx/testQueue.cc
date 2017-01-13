#include <inttypes.h>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T >
class Queue
{
public:
	Queue(uint32_t maxSize = -1)
       : maxSize_(maxSize), running_(true)
	{ };
	virtual ~Queue() { };

	// enqueue immediately; return false if fails
	void enqueue(T & elem) {
		 std::unique_lock<std::mutex> lock(mutex_);
		 notFull_.wait(lock,
				 [&]{ return !running_ or container_.size() < maxSize_; });
		 container_.push_back(elem);
		 notEmpty_.notify_one();  //notify_all()
	}
	bool enqueue(T & elem, int timeoutMS) {
		 std::unique_lock<std::mutex> lock(mutex_);

		 if(!notFull_.wait_for(lock,std::chrono::milliseconds(timeoutMS),
				 [&] { return !running_ or container_.size() < maxSize_; })) {
			 return false;
		 }
		 container_.push_back(elem);
		 notEmpty_.notify_one();  //notify_all()
		 return true;
	}

	void dequeue(T & elem) {
		std::unique_lock<std::mutex> lock(mutex_);
		notEmpty_.wait(lock, [&]{ return !running_ or !container_.empty(); });
		elem = container_.front();
		container_.pop_front();
		notFull_.notify_one();  //notify_all()
	}
	bool dequeue(T & elem, int timeoutMS) {
		 std::unique_lock<std::mutex> lock(mutex_);
		 if(!notEmpty_.wait_for(lock,std::chrono::milliseconds(timeoutMS),
				 [&] { return !running_ or !container_.empty(); })) {
			 return false;
		 }
		 container_.push_back(elem);
		 notEmpty_.notify_one();  //notify_all()
		 return true;
	}

	void stop() {
		std::unique_lock<std::mutex> lock(mutex_);
		running_ = false;
		notFull_.notify_all();
		notEmpty_.notify_all();
	}

protected:
	bool running_;
	uint32_t maxSize_;
	std::deque<T> container_;

	std::mutex mutex_;
	std::condition_variable notFull_;
	std::condition_variable notEmpty_;
};


#include <cassert>
#include <iostream> 

int main() {
	Queue<int> q(2);
	std::thread producer([&]() {
    	for (int i = 0; i <= 100; ++i) {
        	std::this_thread::sleep_for(std::chrono::milliseconds(i % 10));
        	std::cout << " enqueue i= " << i << '\n';
        	q.enqueue(i);
    	}
	});

	std::thread consumer([&]() {
		for (int i = 0; i <= 100; ++i) {
			int item;
			q.dequeue(item);
			std::cout << " dequeue i= " << item << '\n';
			assert(item == i);

			/*
			if (q.dequeue(item, 5)) {
				++i;
				assert(item == i);
			}*/
		}
	});

	producer.join();
	consumer.join();
}
