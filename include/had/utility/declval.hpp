#pragma once

#ifndef HAD_UTILITY_DECLVAL_HPP
#define HAD_UTILITY_DECLVAL_HPP 1

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/type_traits/add_reference.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/always_false.hpp>
HAD_NAMESPACE_BEGIN

template<typename T>
typename add_rvalue_reference<T>::type declval();

HAD_NAMESPACE_END

#endif // !HAD_UTILITY_DECLVAL_HPP
