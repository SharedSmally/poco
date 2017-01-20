#include <iostream>
#include <memory>
using namespace std;

class ICallable
{
public:
	virtual ~ICallable() {}
};
typedef std::shared_ptr<ICallable> CallablePtr;

class ICalling
{
public:
	virtual ~ICalling(){};
	virtual CallablePtr & ptr() = 0;

	template <typename T, typename...Args >
	void call(T & obj, Args...args);
};
typedef std::shared_ptr<ICalling> CallingPtr;

template <typename T, typename...Args >
class Calling : public ICalling
{
	friend class ICalling;
	typedef void (T::*fptr)(Args...args);
	class TCallabler : virtual public ICallable
	{
	public:
		TCallabler (fptr ptr) : ptr_(ptr){}
		virtual ~TCallabler() {}
		void call(T & obj, Args...args)
		{
			if (ptr_)
				(obj.*ptr_) (args...);
		}

	protected:
		fptr ptr_;
	};

public:
	Calling(fptr p)
	{
		ptr_ = std::static_pointer_cast< ICallable > (std::make_shared<TCallabler> (p) );
	}
	virtual ~Calling() {}

	CallablePtr & ptr() { return ptr_; };

protected:
	CallablePtr ptr_;
};

template <typename T, typename...Args >
inline void ICalling::call(T & obj, Args...args)
{
	typedef typename Calling<T, Args...>::TCallabler callable_type;

	std::shared_ptr<callable_type > impl =
		std::dynamic_pointer_cast<callable_type >(ptr());

	if (impl)
		impl->call(obj, args...);
}


class T1 {
public:
	void print()
	{
		cout << " Hello; T1::print()" << endl;
	}
	void print1(int i)
	{
		cout << " Hello; T1::print1(int): int=" << i << endl;
	}
};

void test1()
{
	T1 t1;

	Calling <T1> v1 (&T1::print);
	v1.call(t1);

	Calling <T1, int> v2 (&T1::print1);
	v2.call(t1, 10);
}
void test2()
{
	T1 t1;

	CallingPtr p1 = std::make_shared<Calling <T1> > (&T1::print);
	p1->call(t1);
	p1->call(t1, 20); // throw Exception

	CallingPtr p2 = std::make_shared<Calling <T1, int> > (&T1::print1);
	p2->call(t1, 10);
}

int main()
{
	test1();

	test2();

	return 0;
}
