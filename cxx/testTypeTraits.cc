#include <iostream>
using namespace std;

template <typename T >
struct TypeTraits {
	typedef T type;

	typedef T value_type;
	typedef const T cvalue_type;

	typedef T & ref_type;
	typedef const T & cref_type;

	typedef T * ptr_type;
	typedef const T * cptr_type; //value pointed to is const
	typedef T * const ptrc_type; //pointer is const
	typedef const T * const cptrc_type;
};

template <typename T >
struct TypeTraits<T &> {
	typedef T type;

	typedef T value_type;
	typedef const T cvalue_type;

	typedef T & ref_type;
	typedef const T & cref_type;

	typedef T * ptr_type;
	typedef const T * cptr_type; //value pointed to is const
	typedef T * const ptrc_type; //pointer is const
	typedef const T * const cptrc_type;
};

template <typename T >
struct TypeTraits<T *> {
	typedef T & type;

	typedef T value_type;
	typedef const T cvalue_type;

	typedef T & ref_type;
	typedef const T & cref_type;

	typedef T * ptr_type;
	typedef const T * cptr_type;
	typedef T * const ptrc_type;
	typedef const T * const cptrc_type;
};

template <>
struct TypeTraits<void> {
	typedef void type;

	typedef void value_type;
	typedef void cvalue_type;

	typedef void ref_type;
	typedef void cref_type;

	typedef void * ptr_type;
	typedef const void * cptr_type;
	typedef void * const ptrc_type;
	typedef const void * const cptrc_type;
};

int val = 10;
const int VAL=100;

template <typename T >
void testInt () {
	//typename is neede  because ‘TypeTraits<T>’ is a dependent scope
	typename TypeTraits<T>::value_type a1(val);
	typename TypeTraits<T>::cvalue_type ca(VAL);

	typename TypeTraits<T>::ref_type r1(val);
	typename TypeTraits<T>::cref_type cr(VAL);

	typename TypeTraits<T>::ptr_type p1=&val;
	typename TypeTraits<T>::cptr_type cp=&VAL;
	typename TypeTraits<T>::ptrc_type pc=&val;
	typename TypeTraits<T>::cptrc_type cpc=&VAL;

	cout << "type: a1=" << a1 << "; ac=" << ca << endl;
	cout << "ref:  r1=" << r1 << "; rc=" << cr << endl;

	cout << "ptr:  p1=" << p1 << "; cp=" << cp << endl;
	cout << "ptrc: pc=" << pc << "; cpp=" << cpc << endl;
	cout << endl;
}

int main() {
	testInt<int>();
	testInt<int&>();
	testInt<int*>();
}
