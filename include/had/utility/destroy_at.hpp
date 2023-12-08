#pragma once
#ifndef HAD_UTILTIY_DESTROY_AT_HPP
#define HAD_UTILTIY_DESTROY_AT_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/is_trivially_destructible.hpp>
#include <had/version/attributes/constexpr.hpp>

HAD_NAMESPACE_BEGIN


template<typename T>
HAD_CONSTEXPR20 inline void destroy_at(T* object) 
noexcept(noexcept(object->~T())) {
    if HAD_CONSTEXPR17 (!is_trivially_destructible<T>::value) {
        object->~T();
    }
}

HAD_NAMESPACE_END
#endif // !HAD_UTILTIY_DESTROY_AT_HPP

