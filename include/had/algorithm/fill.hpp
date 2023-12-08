#pragma once
#ifndef HAD_ALGORITHM_FILL_HPP
#define HAD_ALGORITHM_FILL_HPP
#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/utility/declval.hpp>
#include <had/utility/distance.hpp>
#include <had/iterator/utilities.hpp>

HAD_NAMESPACE_BEGIN

template<typename Iter,typename ValueType,typename enable_if<sizeof(Iter) != 1>::type = 0 >
Iter fill(iterator_pair<Iter> range, ValueType value) {
    for (; range.begin != range.end; (void)++range.begin) {
        *range.begin = value;
    }
    return range.end;
}

template<typename Iter,typename ValueType,typename enable_if<sizeof(Iter) == 1>::type = 0 >
Iter* fill(iterator_pair<Iter*> range, ValueType value) {
    const auto distance = HAD_NS distance(range.begin, range.end);
    std::memset(range.begin, value, distance);
    return range.end;
}


HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_FILL_HPP

