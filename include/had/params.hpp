#pragma once
#include <had/type_traits/special_member_functions.hpp>
#include <had/type_traits/enable_if.hpp>
#include <limits.h>
#include <had/type_traits/add_const.hpp>

HAD_NAMESPACE_BEGIN

template<
    typename T,
    bool = (sizeof(T) <= (sizeof(void*) * 4) && 
    is_trivially_copyable<T>::value) >
struct in {
    using type = T;
};

template<typename T>
struct in<T, false> {
    using type = typename add_lvalue_reference<typename add_const<T>::type>::type;
};

#ifdef __cpp_alias_templates


template<typename T>
using in_t = typename in<T>::type;

#endif // defined(__cpp_alias_templates)

template<typename T>
using out = typename add_lvalue_reference<T>::type;

HAD_NAMESPACE_END
