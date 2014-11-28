#include <iostream>
using namespace std;

/*
template<typename... Arguments>
void tfunc(const Arguments&... args)
{
    const unsigned numargs = sizeof...(Arguments);

    X xobj[numargs]; // array of some previously defined type X

    helper_func(xobj, args...);
}
*/

template <typename T1, typename ... Args>
void print_1(T1 & arg, Args&& ...args) {
	cout << " arg1: " << arg << endl;
}
template <typename T1, typename T2, typename ... Args>
void print_2(T1 & arg1, T2 &arg2, Args&& ...args) {
	cout << " arg1: " << arg1 << endl;
	cout << " arg2: " << arg2 << endl;
}


template <typename ... Args>
void print_me1(Args&&...args) {
	cout << "print_me" << endl;
	print_1(args...);
}


template <typename ... Args>
void print_me2(Args&&...args) {
	cout << "print_me" << endl;
	print_2(args...);
}

/*
template <typename ... Args>
void print_me(Args&&...args) {
	const unsigned numargs = sizeof...(Args);
	cout << "print_me" << endl;
	if (numargs==1) {
		X xobj[0]
		print_1(xobj, args...);
	} else if (numargs==2) {
		print_2(args...);
	}
}
*/
int main() {
	int a=10, b=100;
	print_me1(a);
	print_me2(a,b);
/*
	cout << "print_me ......" << endl;
	print_me(a);
	print_me(a,b);
	*/
}
