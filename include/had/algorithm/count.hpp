#pragma once
#ifndef HAD_ALGORITHM_COUNT_HPP
#define HAD_ALGORITHM_COUNT_HPP 1
#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/iterator/utilities.hpp>
HAD_NAMESPACE_BEGIN

template<typename Iterator,typename ValueType>
constexpr inline HAD_NODISCARD size_t count(iterator_pair<Iterator> range, const ValueType& value = ValueType())
noexcept(noexcept(*range.begin == value)) {
    size_t count = 0;
    for (; range.begin; range.begin != range.end; ++range.begin)
        count += *range.begin == value;
    return count;
}

HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_COUNT_HPP

