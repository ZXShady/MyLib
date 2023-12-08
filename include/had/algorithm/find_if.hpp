#pragma once
#ifndef HAD_ALGORITHM_FIND_IF_HPP
#define HAD_ALGORITHM_FIND_IF_HPP 1
#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/iterator/utilities.hpp>
HAD_NAMESPACE_BEGIN



template<typename Iterator,typename Predicate>
HAD_NODISCARD HAD_CONSTEXPR14 Iterator find_if(iterator_pair<Iterator> range, Predicate&& predicate)
noexcept(noexcept(predicate(*range.begin))) {
    for (; range.begin != range.end; (void)++range.begin) {
        if (predicate(*range.begin)) {
            return range.begin;
        }
    }
    return range.end;
}
HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_FIND_IF_HPP

