#include <stdlib.h>
#include <iostream>
using namespace std;

template <typename... Args >
class Functor {
public:
	typedef void (*ptr_type)(Args...);
	Functor() : ptr_(NULL) {
		cout << "Functor()" << endl;
	}
	Functor(ptr_type ptr) : ptr_(ptr) {
		cout << "Functor(ptr)" << endl;
	}
	Functor(const Functor & f) : ptr_(f.ptr_){
		cout << "Functor(Functor)" << endl;
	}
	~Functor(){
		cout << "~Functor()" << endl;
	}
	Functor & operator=(ptr_type ptr) {
		ptr_=ptr; return *this;
	}
	Functor & operator=(const Functor & f) {
		ptr_=f.ptr_; return *this;
	}
	operator bool() const {
		return ptr_!=NULL;
	}

	/*
    template <typename... Ts>
    void operator()(Ts&&...) const{}
    }; */

	void operator()(Args... args) {
		if (ptr_) ptr_(args...);
	}
private:
	ptr_type ptr_;
};

/*
template <typename ... Args >
Functor<Args ... > make_functor( typedef Functor<Args ... >::ptr_type ptr) {
	return Functor(ptr);
}
*/
void print0() {
	cout << "print0()" << endl;
}
void print1(int a) {
	cout << "print1(a):a=" << a << endl;
}
void print1_1(int a) {
	cout << "print1_1(a):a=" << a << endl;
}
void printRef1(int & a) {
	cout << "printRef1(a):a=" << a << endl;
	a *= 2;
}

void print2(int a, float b) {
	cout << "print2(a,b): a=" << a << "; b=" << b << endl;
}

int main() {
	int a=10; float b=100.0;

	{
		Functor<> f0(print0);f0();
		Functor<> f01; f01();
		f01=print0;f01();
	}
	cout << "end Functor<>\n"<<endl;

	{
		Functor<int> f1(print1); f1(a);
		f1=print1_1; f1(a);
	}
	cout << "end Functor1<int>\n"<<endl;

	{
		Functor<int&> f1(printRef1); f1(a);
		cout << "a=" << a << endl;
	}
	cout << "end FunctorRef<int>\n"<<endl;

	Functor<int, float> f2(print2);
	f2(a, b);
}
