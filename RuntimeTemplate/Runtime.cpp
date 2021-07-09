#include <tuple>
#include <memory>
// test
#include <iostream>

#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <boost/mpl/apply.hpp>
// test
#include <boost/range/algorithm/for_each.hpp>

/*! \internal
 */
namespace detail
{
/*! \internal
 */
namespace runtime_template
{


template <
    typename Template
    , typename Types
    , typename Map  // top level map iterator
    , typename LastMap  // top level map iterator
    , int Index
    , bool Done = std::is_same<Map, LastMap>::value
>
struct apply_recursive_t;

template <
    typename Template
    , typename Types
    , typename Map  // top level map iterator
    , typename LastMap  // top level map iterator
    , typename First
    , typename Last
    , int Index
    , bool Enable = !std::is_same<First, Last>::value
>
struct apply_mapping_recursive_t;

template <
    typename Template
    , typename Types
    , typename Map  // top level map iterator
    , typename LastMap  // top level map iterator
    , typename First
    , typename Last
    , int Index // current argument
    , bool Enable /* = !std::is_same<First, Last>::value */
>
struct apply_mapping_recursive_t
{
    typedef void result_type;
    template <typename TypeIds, typename T>
    inline static void apply(const TypeIds& typeIds, T&& t)
    {   namespace mpl = boost::mpl;
        typedef typename mpl::deref<First>::type key_value_pair;
        typedef typename mpl::first<key_value_pair>::type typeId;   // mpl::int
        if (typeId::value == std::get<Index>(typeIds))
        {
            apply_recursive_t<
                Template
                , typename mpl::push_back<
                    Types
                    , typename mpl::second<key_value_pair>::type
                >::type
                , typename mpl::next<Map>::type
                , LastMap
                , Index + 1
            >::apply(typeIds, std::forward<T>(t));
        }
        else
        {
            apply_mapping_recursive_t<
                Template
                , Types
                , Map
                , LastMap
                , typename mpl::next<First>::type
                , Last
                , Index
            >::apply(typeIds, std::forward<T>(t));
        }
    }
};


template <
    typename Template
    , typename Types
    , typename Map  // top level map iterator
    , typename LastMap  // top level map iterator
    , typename First
    , typename Last
    , int Index
>
struct apply_mapping_recursive_t<
    Template
    , Types
    , Map
    , LastMap
    , First
    , Last
    , Index
    , false
>
{
    typedef void result_type;
    template <typename TypeIds, typename T>
    inline static void apply(const TypeIds& /* typeIds */, T&& /* t */)
    {
        BOOST_ASSERT(false);
    }
};


template <
    typename Template
    , typename Types
    , typename Map  // top level map iterator
    , typename LastMap  // top level map iterator
    , int Index
    , bool Done /* = std::is_same<Map, LastMap>::value */
>
struct apply_recursive_t
{
    typedef void result_type;
    template <typename TypeIds, typename T>
    inline static void apply(const TypeIds& typeIds, T&& t)
    {   namespace mpl = boost::mpl;
        typedef typename mpl::deref<Map>::type Mapping; // [key;type] pair vector
        apply_mapping_recursive_t<
            Template
            , Types
            , Map
            , LastMap
            , typename mpl::begin<Mapping>::type
            , typename mpl::end<Mapping>::type
            , Index
        >::apply(typeIds, std::forward<T>(t));
    }
};


template <
    typename Template
    , typename Types
    , typename Map
    , typename LastMap
    , int Index
>
struct apply_recursive_t<
    Template
    , Types
    , Map
    , LastMap
    , Index
    , true
>
{
    typedef void result_type;
    template <typename TypeIds, typename T>
    inline static void apply(const TypeIds& /* typeIds */, T&& t)
    {   namespace mpl = boost::mpl;
        typename mpl::apply<
            mpl::unpack_args<Template>
            , Types
        >::type()(std::forward<T>(t));
    }
};


template <typename Template>
struct make_runtime_template_t
{
    typedef void result_type;
    template <typename Base>
    inline void operator()(std::unique_ptr<Base>* base) const
    {
        base->reset(new Template());
    }
};

}   // namespace runtime_template
}   // namespace detail


template <
    typename Template
    , typename Map
    , typename Types
    , typename T
>
inline void invoke_runtime_template(const Types& types, T&& t)
{   namespace mpl = boost::mpl;
    BOOST_STATIC_ASSERT(mpl::size<Map>::value == std::tuple_size<Types>::value);
    detail::runtime_template::apply_recursive_t<
        Template
        , mpl::vector<>
        , typename mpl::begin<Map>::type
        , typename mpl::end<Map>::type
        , 0
    >::apply(types, std::forward<T>(t));
}

template <
    typename Template
    , typename Map
    , typename Base
    , typename Types
>
inline void make_runtime_template(const Types& types, std::unique_ptr<Base>* base)
{
    invoke_runtime_template<
        detail::runtime_template::make_runtime_template_t<Template>
        , Map
    >(types, base);
}

/*! \overload
 */
template <
    typename Base
    , typename Template
    , typename Map
    , typename Types
>
inline std::unique_ptr<Base> make_runtime_template(const Types& types)
{
    std::unique_ptr<Base> result;

    make_runtime_template<Template, Map>(types, &result);
    return result;
}

////////////////////////////////////////////////////////////////////////////////

namespace mpl = boost::mpl;
using mpl::_;

class MyClassInterface {
public:
    virtual ~MyClassInterface() {}
    virtual double foo(double) = 0;
};

template <int P1, int P2, int P3>
class MyClass
: public MyClassInterface {
public:
    double foo(double /*a*/) {
        // complex computation dependent on P1, P2, P3
        std::wcout << typeid(MyClass<P1, P2, P3>).name() << std::endl;
        return 42.0;
    }
    // more methods and fields (dependent on P1, P2, P3)
};

// wrapper for transforming types (mpl::int) to values
template <typename P1, typename P2, typename P3>
struct MyFactory
{
    inline void operator()(std::unique_ptr<MyClassInterface>* result) const
    {
        result->reset(new MyClass<P1::value, P2::value, P3::value>());
    }
};

template <int I>
struct MyConstant
    : boost::mpl::pair<
        boost::mpl::int_<I>
        , boost::mpl::int_<I>
    > {};

std::unique_ptr<MyClassInterface> Factor(const std::tuple<int, int, int>& constants) {
    typedef mpl::vector<
        MyConstant<0>
        , MyConstant<1>
        , MyConstant<2>
        , MyConstant<3>
        // ...
    > MyRange;
    std::unique_ptr<MyClassInterface> result;
    invoke_runtime_template<
        MyFactory<_, _, _>
        , mpl::vector<MyRange, MyRange, MyRange>
    >(constants, &result);
    return result;
}


///////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>

#include <boost/variant.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/mpl.hpp> //important: allows compatibility fusion-mpl
#include <boost/fusion/include/joint_view.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/set.hpp>
#include <boost/fusion/include/fold.hpp>
#include <boost/fusion/include/for_each.hpp>


#include <boost/mpl/push_back.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/insert.hpp>
#include "boost/mpl/transform.hpp"
#include <functional>

#include "../Wrapper/Wrapper.h"

namespace fusion=boost::fusion;
namespace mpl=boost::mpl;

struct A
{
    int a1;
    double a2;
    float a3;
};

BOOST_FUSION_ADAPT_STRUCT(
    A,
    (int,a1)
    (double,a2)
    (float,a3)
)

struct printer
{
    template <typename T>
    void operator()(T) const
    {
        std::cout << typeid(T).name() << ";";
    }
};

template<char c>
T::char_<c> compose(char t){ return T::char_<t>();}

int main(int /*argc*/, char* /*argv*/[])
{
	std::vector<char> cs = {'a', 'b', 'c'};
	T::char_<'a'>::Type t;
	
    boost::for_each(cs, [](const char& c) { std::cout<<"STart"<<std::endl; });
	
	typedef transform< cs,[](char c){ return compose(c); } >::type result;
// 	typedef transform< cs,T::char_<std::placeholders::_1> >::type result;
	
    std::cout << "TEST"<<std::endl;
	typedef fusion::result_of::as_vector<fusion::joint_view<mpl::vector<bool,std::string>,A> >::type  types1;
//     typedef boost::make_variant_over<types1>::type variant1;
// 
//     variant1 a = 3.5f;
//     variant1 b = true;
// 
//     std::cout << a << " " << b << ". " << fusion::result_of::size<variant1::types>::type::value << " elements in the variant" << std::endl;
//     fusion::for_each(variant1::types(),printer());
//     std::cout << std::endl;
// 
//     typedef mpl::fold<A,mpl::set<std::string,int>,mpl::insert<mpl::_1,mpl::_2>>::type types2;
//     typedef boost::make_variant_over<fusion::result_of::as_vector<types2>::type>::type variant2;
// 
//     variant2 a2 = 4;
//     variant2 b2 = "bla"; 
// 
//     std::cout << a2 << " " << b2 << ". " << fusion::result_of::size<variant2::types>::type::value << " elements in the variant" << std::endl;
//     fusion::for_each(variant2::types(),printer());
//     std::cout << std::endl;
	
    return 0;
}
