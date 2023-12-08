#pragma once
#ifndef HAD_ALGORITHM_DESTROY_HPP
#define HAD_ALGORITHM_DESTROY_HPP 1
#include <had/version/namespace.hpp>
#include <had/utility/destroy_at.hpp>
#include <had/iterator/iterator_traits.hpp>
#include <had/iterator/utilities.hpp>
#include <had/type_traits/addressof.hpp>
HAD_NAMESPACE_BEGIN


template<typename Iterator>
HAD_CONSTEXPR20 void destroy(iterator_pair<Iterator> range) {
    using T = typename iterator_traits<Iterator>::value_type;
    if HAD_CONSTEXPR17(!is_trivially_destructible<T>::value) {
        auto& begin = range.begin;
        auto& end   = range.end;

        while (begin != end) {
            destroy_at(HAD_NS addressof(*begin));
            ++begin;
        }
    }
}

HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_DESTROY_HPP

