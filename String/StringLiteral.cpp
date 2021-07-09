#include <iostream>
#include <algorithm>

/**
 * Literal class type that wraps a constant expression string.
 *
 * Uses implicit conversion to allow templates to *seemingly* accept constant strings.
 */
// template<size_t N>
// struct StringLiteral {
//     constexpr StringLiteral(const char (&str)[N]) {
//         std::copy_n(str, N, value);
//     }
//     
//     char value[N];
// };
// 
// template<StringLiteral lit>
// void Print() {
//     // The size of the string is available as a constant expression.
//     constexpr auto size = sizeof(lit.value);
// 
//     // and so is the string's content.
//     constexpr auto contents = lit.value;
// 
//     std::cout << "Size: " << size << ", Contents: " << contents << std::endl;
// }

template<typename CharT, std::size_t N>
struct basic_fixed_string
{
	constexpr basic_fixed_string(const CharT (&foo)[N+1]){ std::copy_n(foo, N+1, m_data); }
// 	auto operator<=>(const basic_fixed_string &, const basic_fixed_string &) = default;
	CharT m_data[N+1];
	
};
	
template<typename CharT, std::size_t N>
basic_fixed_string(const CharT (&str)[N])->basic_fixed_string<CharT, N-1>;

template<std::size_t N>
using fixed_string = basic_fixed_string<char, N>;


template<std::size_t N,fixed_string<N> Str>
struct B {};

using hello_B = B<5, "hello">;

template<basic_fixed_string Str>
struct A {};
int main()
{
	using hello_A = A<"hello">;
// 	auto l = StringLiteral("literal string");
//     Print<l>(); // Prints "Size: 15, Contents: literal string"
//     Print<"literal string">(); // Prints "Size: 15, Contents: literal string"
}
