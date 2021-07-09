#include<iostream>
#include<vector>

template<bool, typename T = void>
struct EnableIfT{};

template<typename T>
struct EnableIfT<true,T>{
	using Type = T;	
};

template<bool Cond, typename T = void>
using EnableIf = typename EnableIfT<Cond, T>::Type;

template<typename T>
EnableIf<true , T> FT(T t){std::cout<<"EnableIf<true , T>"<<std::endl; return t;};

// template<>
// EnableIf<false , int> FT<int>(int t){std::cout<<"EnableIf<false , T>"<<std::endl; return t;};


// this overload is always in the set of overloads
// ellipsis parameter has the lowest ranking for overload resolution
void test(...)
{
    std::cout << "Catch-all overload called\n";
}
 
// this overload is added to the set of overloads if
// C is a reference-to-class type and F is a pointer to member function of C
template <class C, class F>
auto test(C c, F f) -> decltype((void)(c.*f)(), void())
{
    std::cout << "Reference overload called\n";
}
 
// this overload is added to the set of overloads if
// C is a pointer-to-class type and F is a pointer to member function of C
template <class C, class F>
auto test(C c, F f) -> decltype((void)((c->*f)()), void())
{
    std::cout << "Pointer overload called\n";
}


//----------------------------------------------------------------------------
template<typename T, unsigned N>
void Len(T(&) [N]) { std::cout<<"std::size_t Len(T(&) [N]) "<<N<<std::endl; }

template<typename T>
void Len(T const& t) { std::cout<<"typename T::size_type Len(T const& t) "<<t.size()<<std::endl; }

void Len(...) { std::cout<<"typename T::size_type Len(...) "<<0<<std::endl;}

//-------------------------------------------------------------

template<typename T, unsigned N>
std::size_t Len2(T(&) [N]) { std::cout<<"std::size_t Len2(T(&) [N])"<<std::endl; return N;}

template<typename T>
typename T::size_type Len2(T const& t) { std::cout<<"typename T::size_type Len2(T const& t)"<<std::endl; return t.size();}

std::size_t  Len2(...) { std::cout<<"typename T::size_type Len2(...)"<<std::endl; return 0;}
 

struct X { void f() {} };

int main()
{
	int a[10];
	std::vector<int> v;
	int* p;
	std::allocator<int> all;
	
	std::cout<<"Call with a:\t";
	Len(a);
	std::cout<<"Call with a:\t";
	Len2(a);
	std::cout<<"Call with string:\t";
	Len("tmp");
	std::cout<<"Call with string:\t";
	Len2("tmp");
	std::cout<<"Call with vector:\t";
	Len(v);
	std::cout<<"Call with vector:\t";
	Len2(v);
	std::cout<<"Call with p";
	Len(p);
	std::cout<<"Call with p:\t";
	Len2(p);
// 	std::cout<<"Call with allocator";
// 	Len(all);
	
	std::cout<<"Call Len2";
	
// 	std::cout<<"Call with allocator:\t";
// 	Len2(all);
	
// 	X x;
// 	test( x, &X::f);
// 	test(&x, &X::f);
// 	test(42, 1337);
// 	
// 	std::string s = "Test";
// 	FT(s);
// 	
// 	int i = 9;
// 	FT(i);
	
	return 0;
}
