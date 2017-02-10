
class Runnable {
public:
	virtual ~Runnable() {}
	virtual void run() = 0;
};

class Thread
{
public:
	Thread()
	{ };
	virtual ~Thread()
	{ };
	void join()
	{
		thread_.join();
	}
protected:
	virtual void run() = 0;

protected:
	void start()
	{
		thread_=std::move( std::thread( [this] { run(); } ) );
	}

protected:
	std::thread thread_;
};

class Threader : public Thread
{
public:
	Threader(Runnable & runner)
		: runner_(runner)
	{
		start();
	};
	virtual ~Threader()
	{ };

protected:
	virtual void run()
	{
		runner_.run();
	}

protected:
	Runnable & runner_;
};

class ThreadPool
{
public:
	ThreadPool()
	{ };
	virtual ~ThreadPool()
	{ };
	
	void join() {
		 for(auto it=threads_.begin(); it!=threads_.end();++it) {
			it->join();
		}
	}
protected:
	virtual void run() = 0;

	void start(int size) {
		for (int i=0; i<size; i++)
			threads_.push_back(std::thread( [this] { run(); } ) );
	}
	
protected:
	std::vector<std::thread> threads_;
};

class ThreadPooler : public ThreadPool
{
public:
	ThreadPooler(Runnable & runner, int size = std::thread::hardware_concurrency() )
		: runner_(runner)
	{
	     start(size);
	};
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
