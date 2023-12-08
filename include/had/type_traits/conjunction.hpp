#pragma once


#ifndef HAD_TYPE_TRAITS_CONJUNCTION_HPP

#include <had/version/namespace.hpp>

#if HAD_CPP_CURRENT_VERSION == 11
#define HAD_TYPE_TRAITS_CONJUNCTION_HPP 11
#include <had/type_traits/conjunction/cpp11.hpp>
#elif HAD_CPP_CURRENT_VERSION == 14
#define HAD_TYPE_TRAITS_CONJUNCTION_HPP 14
#include <had/type_traits/conjunction/cpp14.hpp>
#elif HAD_CPP_CURRENT_VERSION >= 17
#define HAD_TYPE_TRAITS_CONJUNCTION_HPP 17
#include <had/type_traits/conjunction/cpp17.hpp>
#endif // !HAD_CPP_CURRENT_VERSION(S)

#endif // !HAD_TYPE_TRAITS_CONJUNCTION_HPP