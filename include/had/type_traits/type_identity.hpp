#pragma once

#ifndef HAD_TYPE_TRAITS_TYPE_IDENTITY_HPP
#define HAD_TYPE_TRAITS_TYPE_IDENTITY_HPP

#include <had/version/namespace.hpp>
HAD_NAMESPACE_BEGIN

template<typename T> struct type_identity { using type = T; };

HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_TYPE_IDENTITY_HPP
