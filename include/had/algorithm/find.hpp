#pragma once
#ifndef HAD_ALGORITHM_FIND_HPP
#define HAD_ALGORITHM_FIND_HPP
#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/iterator/utilities.hpp>
HAD_NAMESPACE_BEGIN



template<typename Iterator,typename ValueType>
HAD_NODISCARD HAD_CONSTEXPR14 Iterator find_if(iterator_pair<Iterator> range, const ValueType& value)
noexcept(noexcept(*range.begin == value)) {
    for (; range.begin != range.end; (void)++range.begin)
        if (*range.begin == value)
            return range.begin;
    return range.end;
}

HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_FIND_HPP

