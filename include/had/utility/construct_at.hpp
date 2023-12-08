#pragma once
#ifndef HAD_UTILTIY_CONSTUCT_AT_HPP
#define HAD_UTILTIY_CONSTUCT_AT_HPP
#include <had/version/namespace.hpp>
#include <had/version/attributes/constexpr.hpp>
#include <had/type_traits/forward.hpp>
#include <new>

HAD_NAMESPACE_BEGIN


template<typename T,typename... Args>
HAD_CONSTEXPR20 T* construct_at(T* where, Args&&... args) 
    noexcept(noexcept(::new (static_cast<void*>(where)) T(HAD_NS forward<Args>(args)...) ))
{
    return ::new (static_cast<void*>(where)) T(HAD_NS forward<Args>(args)...);
}


template<typename... Args>
void construct_at(std::nullptr_t,Args...) = delete;

HAD_NAMESPACE_END
#endif // !HAD_UTILTIY_CONSTUCT_AT_HPP

