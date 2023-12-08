#pragma once
#ifndef HAD_ALGORITHM_UNINITIALIZED_FILL_HPP
#define HAD_ALGORITHM_UNINITIALIZED_FILL_HPP 1
#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/iterator/iterator_traits.hpp>
#include <had/type_traits/is_trivially_copyable.hpp>
#include <had/utility/construct_at.hpp>
#include <had/type_traits/addressof.hpp>
#include <had/type_traits/is_character.hpp>
#include <had/utility/distance.hpp>
#include <had/utility/construct_at.hpp>
#include <cstring>
#include <had/iterator/utilities.hpp>
HAD_NAMESPACE_BEGIN


template<typename Iter,typename ValueType,typename enable_if<sizeof(Iter) != 1>::type = 0 >
Iter uninitialized_fill(iterator_pair<Iter> range, ValueType value) {
    using T = decltype(*range.begin);
    for (; range.begin != range.end; (void)++range.begin) {
        HAD_NS construct_at(range.get_addressof(), value);
    }
    return range.end;
}

template<typename Iter,typename ValueType,typename enable_if<sizeof(Iter) == 1>::type = 0 >
Iter* uninitialized_fill(iterator_pair<Iter*> range, ValueType value) {
    const auto distance = HAD_NS distance(range.begin, range.end);
    std::memset(range.begin, value, distance);
    return range.end;
}

HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_UNINITIALIZED_FILL_HPP

