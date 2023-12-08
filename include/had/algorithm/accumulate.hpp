#pragma once
#ifndef HAD_ALGORITHM_ACCUMULATE_HPP
#define HAD_ALGORITHM_ACCUMULATE_HPP

#include <had/version/attributes.hpp>
#include <had/version/namespace.hpp>
#include <had/iterator/utilities.hpp>

HAD_NAMESPACE_BEGIN

template <typename Iterator, typename T>
HAD_CONSTEXPR20 T accumulate (Iterator begin, Iterator end, T init) 
        noexcept(noexcept(init += *begin))
{
    while (begin != end) {
        init += *begin;
        ++begin;
    }
    return init;
}

template <typename Container, typename T>
HAD_CONSTEXPR20 T accumulate(const Container& container, T init) 
    noexcept(noexcept(accumulate(begin(container), end(container), init)))
{
    return accumulate(begin(container), end(container), init);
}

HAD_NAMESPACE_END

#endif // !HAD_ALGORITHM_ACCUMULATE_HPP
