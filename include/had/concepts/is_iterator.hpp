#pragma once

#ifndef HAD_CONCEPTS_IS_ITERATOR_HPP
#define HAD_CONCEPTS_IS_ITERATOR_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/is_class.hpp>

HAD_NAMESPACE_BEGIN

#ifndef __cpp_concept
#error <ERROR> HADSTD LIB file concepts/is_iterator is only in C++20
#endif // __cpp_concept
template<typename T>
concept is_iterator = requires (T x) {
    x++;
    ++x;
    x += 1;
    x -= 1;
    *x;
    requires is_class_v<T>;
};
HAD_NAMESPACE_END

#endif // !HAD_CONCEPTS_IS_ITERATOR_HPP
