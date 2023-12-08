#pragma once
#ifndef HAD_ALGORITHM_COMPARE_HPP
#define HAD_ALGORITHM_COMPARE_HPP

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/is_random_access_iterator.hpp>
#include <had/utility/declval.hpp>
#include <had/utility/distance.hpp>
#include <had/iterator/iterator_traits.hpp>
#include <cstring>

HAD_NAMESPACE_BEGIN

enum struct comparison {
    less    = -1,
    equal   = 0,
    greater = 1
};
template<
    typename Iterator,
    typename enable_if<!is_random_access_iterator<Iterator>::value>::type = 0 >
constexpr inline comparison compare(
    Iterator begin
    , Iterator end
    , Iterator begin2) {
    for (; begin != end; ++begin, ++begin2) {
        if (*begin != *begin2) {
            return (*begin > *begin2) ? comparison::greater : comparison::less;
        }
    }
    return comparison::equal;
}
template<
    typename Iterator,
    typename enable_if<is_random_access_iterator<Iterator>::value>::type = 0 >
constexpr inline comparison compare(
    Iterator begin
    , Iterator end
    , Iterator begin2) {
    return comparison(std::memcmp(begin,begin2,sizeof(*begin) * distance(begin,end)));
}

HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_COMPARE

