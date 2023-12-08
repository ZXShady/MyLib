#pragma once
#ifndef HAD_MEMORY_NEW_ALIGNOF_HPP
#define HAD_MEMORY_NEW_ALIGNOF_HPP

#include <had/stdint.hpp>
#include <had/version/namespace.hpp>
#include <had/utility/max.hpp>

HAD_NAMESPACE_BEGIN

template<typename T>
constexpr static size_t new_alignof = (max)(alignof(T), __STDCPP_DEFAULT_NEW_ALIGNMENT__);

HAD_NAMESPACE_END
#endif // !HAD_MEMORY_NEW_ALIGNOF_HPP