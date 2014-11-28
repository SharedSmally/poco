#include <stdlib.h>
#include <iostream>
using namespace std;

//shared_type: copy; reference; shared_pointer[point_with/without_refCounter]
template <typename OBJ, typename... Args >
class ObjFunctor {
public:
	typedef OBJ obj_type;
	typedef void (OBJ::*ptr_type)(Args...);
	//typedef R (OBJ::*ptr_type)(Args...);
	ObjFunctor(OBJ obj) : obj_(obj), ptr_(NULL) {
		cout << "ObjFunctor()" << endl;
	}
	ObjFunctor(OBJ obj, ptr_type ptr)
	    : obj_(obj), ptr_(ptr) {
		cout << "ObjFunctor(ptr)" << endl;
	}
	ObjFunctor(const ObjFunctor & f)
	    : obj_(f.obj_),ptr_(f.ptr_) {
		cout << "ObjFunctor(ObjFunctor)" << endl;
	}
	~ObjFunctor(){
		cout << "~ObjFunctor()" << endl;
	}
	ObjFunctor & operator=(const OBJ & obj) {
		obj_=obj; return *this;
	}
	ObjFunctor & operator=(ptr_type ptr) {
		ptr_=ptr; return *this;
	}
	ObjFunctor & operator=(const ObjFunctor & f) {
		obj_=f.obj_; ptr_=f.ptr_; return *this;
	}
	operator bool() const {
		return ptr_!=NULL;
	}

	/*
    template <typename... Ts>
    void operator()(Ts&&...) const{}
    }; */

	void operator()(Args... args) {
		//return (objptr ->* (funcptr))( /* args go here */ );
		if (ptr_) (obj_.*ptr_)(args...);
	}
private:
	OBJ obj_;
	ptr_type ptr_;
};

/*
template <typename ... Args >
ObjFunctor<Args ... > make_ObjFunctor( typedef ObjFunctor<Args ... >::ptr_type ptr) {
	return ObjFunctor(ptr);
}
*/
class MyObj {
public:
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
};

int main() {
	int a=10; float b=100.0;
	MyObj obj;

	{
		ObjFunctor<MyObj> f0(obj, &MyObj::print0);f0();
		ObjFunctor<MyObj> f01(obj); f01();
		f01=&MyObj::print0;f01();
	}
	cout << "end ObjFunctor<>\n"<<endl;

	{
		ObjFunctor<MyObj, int> f1(obj,&MyObj::print1); f1(a);
		f1=&MyObj::print1_1; f1(a);
	}
	cout << "end ObjFunctor1<int>\n"<<endl;

	{
		ObjFunctor<MyObj,int&> f1(obj,&MyObj::printRef1); f1(a);
		cout << "a=" << a << endl;
	}
	cout << "end ObjFunctorRef<int>\n"<<endl;

	ObjFunctor<MyObj, int, float> f2(obj,&MyObj::print2);
	f2(a, b);
}
