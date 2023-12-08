#pragma once

#ifndef HAD_TYPE_TRAITS_DECAY_HPP
#define HAD_TYPE_TRAITS_DECAY_HPP 1

#include <had/version/namespace.hpp>
#include <had/utility/declval.hpp>

HAD_NAMESPACE_BEGIN


namespace details {

template<typename T>
auto decay_f(T t) -> decltype(t);

}
template<typename T>
struct decay {
    using type = decltype(details::decay_f(HAD_NS declval<T>()) );
};

#ifdef __cpp_alias_templates

template<typename T>
using decay_t = decltype(details::decay_f(HAD_NS declval<T>()));

#endif // __cpp_alias_templates

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_DECAY_HPP
