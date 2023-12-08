#pragma once

#ifndef HAD_TYPE_TRAITS_MAKE_SIGNED_HPP
#define HAD_TYPE_TRAITS_MAKE_SIGNED_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/conditional.hpp>
#include <had/type_traits/apply_modifiers.hpp>
#include <had/type_traits/is_unsigned.hpp>
#include <had/type_traits/remove_cv.hpp>
HAD_NAMESPACE_BEGIN

namespace details {
template<typename Modifiers,typename Type>
struct make_signed {
    using type = typename apply_modifiers<Modifiers,Type>::type;
};

template<typename Modifiers>
struct make_signed<Modifiers,char> {
    using type = typename apply_modifiers<Modifiers,signed char>::type;
};

template<typename Modifiers>
struct make_signed<Modifiers,unsigned char> {
    using type = typename apply_modifiers<Modifiers,signed char>::type;
};

template<typename Modifiers>
struct make_signed<Modifiers,unsigned short> {
    using type = typename apply_modifiers<Modifiers,signed short>::type;
};

template<typename Modifiers>
struct make_signed<Modifiers,unsigned int> {
    using type = typename apply_modifiers<Modifiers,signed int>::type;
};

template<typename Modifiers>
struct make_signed<Modifiers,unsigned long long> {
    using type = typename apply_modifiers<Modifiers,signed long long>::type;
};

}// namespace details

template<typename T> 
struct make_signed {
    using type = typename details::make_signed<T,typename remove_cv<T>::type >::type;
};


#ifdef __cpp_alias_templates

template<typename T>
using make_signed_t = typename make_signed<T>::type;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_MAKE_SIGNED_HPP
