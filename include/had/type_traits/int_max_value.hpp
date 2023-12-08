#pragma once
#ifndef HAD_TYPE_TRAITS_INT_MAX_VALUE_HPP
#define HAD_TYPE_TRAITS_INT_MAX_VALUE_HPP

#include <had/stdint.hpp>
#include <climits>
#include <had/type_traits/conditional.hpp>
#include <had/type_traits/make_signed.hpp>
HAD_NAMESPACE_BEGIN



template<size_t N>
struct uint_max_value {
    using type = typename conditional< N < UINT_LEAST8_MAX
        , least_u8
        , typename conditional< N < UINT_LEAST16_MAX
        , least_u16
        , typename conditional< N < UINT_LEAST32_MAX
        , least_u32
        , typename conditional< N < UINT_LEAST64_MAX
        , least_u64
        , void>::type >::type >::type >::type;
};

template<size_t N>
struct int_max_value {
public:
    using type = typename make_signed<typename uint_max_value<N>::type >::type;
};

#ifdef __cpp_alias_templates

template<size_t N>
using int_max_value_t = typename int_max_value<N>::type;

template<size_t N>
using uint_max_value_t = typename uint_max_value<N>::type;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_INT_MAX_VALUE_HPP
