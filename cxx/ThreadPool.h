
class Runnable {
public:
	virtual ~Runnable() {}
	virtual void run() = 0;
};

class ThreadPool
{
public:
	ThreadPool()
	{ };
	virtual ~ThreadPool()
	{ };
	void start(int size) {
		for (int i=0; i<size; i++)
			threads_.push_back(std::thread(&ThreadPool::run, std::ref(*this)));
	}
	void wait() {
		 for(auto it=threads_.begin(); it!=threads_.end();++it) {
			it->join();
		}
	}
protected:
	virtual void run() = 0;

protected:
	std::vector<std::thread> threads_;
};

class ThreadPooler : public ThreadPool
{
public:
	ThreadPooler(Runnable & runner)
		: runner_(runner)
	{ };
	virtual ~ThreadPooler()
	{ };

protected:
	virtual void run() {
		runner_.run();
	}

protected:
	Runnable & runner_;
};

inline void sleep(int sec) {
	std::this_thread::sleep_for(std::chrono::seconds(sec));
}
