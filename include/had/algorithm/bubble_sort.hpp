#pragma once
#ifndef HAD_ALGORITHM_BUBBLE_SORT_HPP
#define HAD_ALGORITHM_BUBBLE_SORT_HPP 1
#include <had/version/namespace.hpp>
#include <had/utility/swap.hpp>
#include <had/iterator/utilities.hpp>
HAD_NAMESPACE_BEGIN


template<typename Iterator,typename Predicate>
constexpr void bubble_sort(Iterator begin,Iterator end, Predicate&& predicate) 
noexcept(noexcept(function(*begin,*begin)))
{
    using had::swap;
    for (auto x = begin; x != end; ++x) {
        for (auto y = begin; y != end; ++y) {
            if (predicate(*x,*y)) {
                swap(*x, *y);
            }
        }
    }
}

template<typename Container,typename Predicate>
constexpr void bubble_sort(Container& container, Predicate&& predicate) 
noexcept(noexcept(function(*container.begin(),*container.begin())))
{
    bubble_sort(begin(container), end(container), predicate);
}
HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_BUBBLE_SORT_HPP

