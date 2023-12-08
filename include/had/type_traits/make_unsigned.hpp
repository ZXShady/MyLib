#pragma once

#ifndef HAD_TYPE_TRAITS_MAKE_UNSIGNED_HPP
#define HAD_TYPE_TRAITS_MAKE_UNSIGNED_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/conditional.hpp>
#include <had/type_traits/apply_modifiers.hpp>
#include <had/type_traits/is_unsigned.hpp>
#include <had/type_traits/remove_cv.hpp>
HAD_NAMESPACE_BEGIN

namespace details {
template<typename Modifiers,typename>
struct make_unsigned {
    using type = Modifiers;
};

template<typename Modifiers>
struct make_unsigned<Modifiers,char> {
    using type = typename apply_modifiers<Modifiers,unsigned char>::type;
};

template<typename Modifiers>
struct make_unsigned<Modifiers,signed char> {
    using type = typename apply_modifiers<Modifiers,unsigned char>::type;
};

template<typename Modifiers>
struct make_unsigned<Modifiers,signed short> {
    using type = typename apply_modifiers<Modifiers,unsigned short>::type;
};

template<typename Modifiers>
struct make_unsigned<Modifiers,signed int> {
    using type = typename apply_modifiers<Modifiers,unsigned int>::type;
};

template<typename Modifiers>
struct make_unsigned<Modifiers,signed long long> {
    using type = typename apply_modifiers<Modifiers,unsigned long long>::type;
};

}// namespace details

template<typename T> 
struct make_unsigned {
    using type = typename details::make_unsigned<T,typename remove_cv<T>::type >::type;
};


#ifdef __cpp_alias_templates

template<typename T>
using make_unsigned_t = typename make_unsigned<T>::type;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_MAKE_SIGNED_HPP
