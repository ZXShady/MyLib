#pragma once

#ifndef HAD_ALGORITHM_LEXICOGRAPHICAL_COMPARE_HPP
#define HAD_ALGORITHM_LEXICOGRAPHICAL_COMPARE_HPP

#include <had/version/namespace.hpp>
#include <had/version/attributes/constexpr.hpp>
#include <had/initializer_list.hpp>
#include <had/iterator/utilities.hpp>

HAD_NAMESPACE_BEGIN

enum lexicographical_comparison : signed char {
    less = -1,
    equal = 0,
    greater = 1
};

template<typename Iterator1,typename Iterator2>
lexicographical_comparison lexicographical_compare(Iterator1 iter1_begin, Iterator1 iter1_end, Iterator2 iter2_begin) {
    auto& it1 = iter1_begin;
    auto& it2 = iter2_begin;
    const auto& end = iter1_end;

    for (; it1 != end; ++it1, ++it2) {
        if (*it1 != *it2) {
            return (*it1 > *it2) 
                   ? lexicographical_comparison::greater 
                   : lexicographical_comparison::less;
        }
    }
    return lexicographical_comparison::equal;
}

HAD_NAMESPACE_END

#endif // !HAD_ALGORITHM_LEXICOGRAPHICAL_COMPARE_HPP
