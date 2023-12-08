#pragma once
#ifndef HAD_ALGORITHM_COMPARE_HPP
#define HAD_ALGORITHM_COMPARE_HPP

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/is_trivially_copy_constructible.hpp>
#include <had/type_traits/is_trivially_copyable.hpp>
#include <had/type_traits/is_random_access_iterator.hpp>
#include <had/type_traits/is_fundamental.hpp>
#include <had/utility/declval.hpp>
#include <had/utility/distance.hpp>
#include <had/iterator/iterator_traits.hpp>
#include <cstring>
#include <had/typeinfo/operator_equals.hpp>
HAD_NAMESPACE_BEGIN

template<typename Iter1,typename Iter2,
    enable_if_t<!(
    is_fundamental<typename iterator_traits<Iter1>::value_type>::value 
    && is_same_no_qualifiers_v<Iter1,Iter2>
)> = 0 >
HAD_NODISCARD HAD_CONSTEXPR14 bool equal(iterator_pair<Iter1> range, Iter2 begin) {
    for (; range.begin != range.end; ++range.begin, ++begin)
        if (*range.begin != *begin)
            return false;
    return true;
}

template<typename Iter,typename enable_if<
    is_fundamental<Iter>::value 
    >::type = 0 >
HAD_NODISCARD HAD_CONSTEXPR14 bool equal(iterator_pair<Iter*> range, Iter* begin) noexcept {
    return std::memcmp(range.begin, begin, HAD_NS distance(range.begin, range.end)) == 0;
}

HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_COMPARE

