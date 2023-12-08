#pragma once

#ifndef HAD_TYPE_TRAITS_CONDITIONAL_HPP
#define HAD_TYPE_TRAITS_CONDITIONAL_HPP 1

#include <had/version/namespace.hpp>

HAD_NAMESPACE_BEGIN

template<bool Boolean,typename IfTrueType,typename IfFalseType>
struct conditional {
    using type = IfTrueType;
};

template<typename IfTrueType,typename IfFalseType>
struct conditional<false,IfTrueType,IfFalseType> {
    using type = IfFalseType;
};

template<bool Boolean,typename IfTrueType,typename IfFalseType>
using conditional_t = typename conditional<Boolean, IfTrueType, IfFalseType>::type;
HAD_NAMESPACE_END

#endif // !HAD_TYPE_TRAITS_CONDITIONAL_HPP
