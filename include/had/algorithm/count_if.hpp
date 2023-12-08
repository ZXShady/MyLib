#pragma once
#ifndef HAD_ALGORITHM_COUNT_IF_HPP
#define HAD_ALGORITHM_COUNT_IF_HPP 1
#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/iterator/utilities.hpp>
HAD_NAMESPACE_BEGIN

template<typename Iterator,typename Predicate>
constexpr inline HAD_NODISCARD size_t count_if(iterator_pair<Iterator> range, Predicate&& predicate)
noexcept(noexcept(predicate(*range.begin))) {
    size_t count = 0;
    for (; range.begin; range.begin != range.end; ++range.begin)
        if (predicate(*range.begin))
            count++;
    return count;
}

HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_COUNT_IF_HPP

