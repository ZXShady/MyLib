#pragma once

#ifndef HAD_TYPE_TRAITS_APPLY_MODIFIERS_HPP
#define HAD_TYPE_TRAITS_APPLY_MODIFIERS_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/remove_reference.hpp>
HAD_NAMESPACE_BEGIN

namespace details {
template<typename From, typename To>
struct apply_modifiers { using type = To; };

template<typename From, typename To>
struct apply_modifiers<const From, To> { using type = const To; };

template<typename From, typename To>
struct apply_modifiers<volatile From, To> { using type = volatile To; };

template<typename From, typename To>
struct apply_modifiers<const volatile From, To> { using type = const volatile To; };
}

template<typename From,typename To>
struct apply_modifiers : details::apply_modifiers<typename remove_reference<From>::type, To> {};

#ifdef __cpp_alias_templates
template<typename From,typename To>
using apply_modifiers_t = typename apply_modifiers<From,To>::type;

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_APPLY_MODIFIERS_HPP
