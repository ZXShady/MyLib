#pragma once
#ifndef HAD_TYPEINFO_OPERATOR_EQUALS_HPP
#define HAD_TYPEINFO_OPERATOR_EQUALS_HPP
#include <had/version/namespace.hpp>
#include <had/utility/declval.hpp>
#include <had/type_traits/boolean_types.hpp>
HAD_NAMESPACE_BEGIN

template<typename L, typename R = L>
struct operator_equals {
private:
    // template parameters here to enable SFINAE
    template<typename T = L, typename U = R>
    static auto freefunc(T &&t, U &&u) -> decltype(operator==(t,u), void(), true_type{});
    static auto freefunc(...) -> false_type;
    template<typename T = L, typename U = R>
    static auto memfunc(T &&t, U &&u) -> decltype(t.operator==(u), void(), true_type{});
    static auto memfunc(...) -> false_type;
    template<typename T = L, typename U = R>
    static auto symbolfunc(T &&t, U &&u) -> decltype(t == u, void(), std::true_type{});
    static auto symbolfunc(...) -> false_type;
public:
    constexpr static bool is_free_function = 
        decltype(freefunc(HAD_NS declval<L>(), HAD_NS declval<R>()))::value;

    constexpr static bool is_member_function = 
        decltype(memfunc(HAD_NS declval<L>(), HAD_NS declval<R>()))::value;

    constexpr static bool is_symbol_function = 
        decltype(symbolfunc(HAD_NS declval<L>(), HAD_NS declval<R>()))::value;
    constexpr static bool is_overloaded = is_member_function || is_free_function;
};


HAD_NAMESPACE_END

#endif // !HAD_TYPEINFO_OPERATOR_EQUALS_HPP
