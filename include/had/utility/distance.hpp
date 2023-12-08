#pragma once

#ifndef HAD_UTILITY_DISTANCE_HPP
#define HAD_UTILITY_DISTANCE_HPP 1

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/initializer_list.hpp>
#include <had/type_traits/is_same.hpp>
#include <had/type_traits/add_pointer.hpp>
#include <had/type_traits/is_random_access_iterator.hpp>
#include <had/version/attributes/constexpr.hpp>

HAD_NAMESPACE_BEGIN

namespace details {

template<typename Iterator,bool = is_random_access_iterator<Iterator>::value>
struct distance {
    HAD_CONSTEXPR20 static inline ptrdiff_t calc(Iterator begin, Iterator end) noexcept {
        ptrdiff_t count = 0;
        while (begin != end) { 
            ++begin;
            ++count;
        }
        return count;
    }
};

template<typename Iterator>
struct distance<Iterator,true> {
    HAD_CONSTEXPR20 static inline ptrdiff_t calc(Iterator begin, Iterator end) noexcept {
        return static_cast<ptrdiff_t>(end - begin);
    }
};


} // namespace details

template<typename Iterator>
HAD_NODISCARD HAD_CONSTEXPR20 ptrdiff_t distance(Iterator begin, Iterator end) noexcept {
    return ::had::details::distance<Iterator>{}.calc(begin,end);
}

template<typename Iterator>
HAD_NODISCARD HAD_CONSTEXPR20 ptrdiff_t distance(iterator_pair<Iterator> range) noexcept {
    return ::had::details::distance<Iterator>{}.calc(range.begin,range.end);
}

HAD_NAMESPACE_END

#endif // !HAD_UTILITY_DISTANCE_HPP
