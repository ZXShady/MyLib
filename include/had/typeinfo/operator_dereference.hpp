#pragma once
#ifndef HAD_TYPEINFO_OPERATOR_DEREFERENCE_HPP
#define HAD_TYPEINFO_OPERATOR_DEREFERENCE_HPP
#include <had/version/namespace.hpp>
#include <had/utility/declval.hpp>
#include <had/type_traits/boolean_types.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
struct operator_dereference {
private:
    // template parameters here to enable SFINAE
    template<typename T2 = T>
    static auto freefunc(T2 &&t) -> decltype(operator*(t), void(), true_type{});
    static auto freefunc(...) -> false_type;
    template<typename T2 = T>
    static auto memfunc(T2 &&t) -> decltype(t.operator*(), void(), true_type{});
    static auto memfunc(...) -> false_type;
    template<typename T2 = T>
    static auto symbolfunc(T2 &&t) -> decltype(*t, void(), true_type{});
    static auto symbolfunc(...) -> false_type;
public:
    constexpr static bool is_free_function = 
        decltype(freefunc(HAD_NS declval<T>()))::value;

    constexpr static bool is_member_function = 
        decltype(memfunc(HAD_NS declval<T>()))::value;

    constexpr static bool is_symbol_function = 
        decltype(symbolfunc(HAD_NS declval<T>()))::value;

    constexpr static bool is_overloaded = is_member_function || is_free_function;
};


HAD_NAMESPACE_END

#endif // !HAD_TYPEINFO_OPERATOR_DEREFERENCE_HPP
