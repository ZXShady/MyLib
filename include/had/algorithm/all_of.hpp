#pragma once
#ifndef HAD_ALGORITHM_ALL_OF_HPP
#define HAD_ALGORITHM_ALL_OF_HPP

#include <had/version/namespace.hpp>
#include <had/version/attributes/constexpr.hpp>
#include <had/iterator/utilities.hpp>
#include <had/initializer_list.hpp>
HAD_NAMESPACE_BEGIN

template<typename Iterator,typename Predicate>
HAD_CONSTEXPR14 bool all_of(Iterator begin, Iterator end, Predicate&& predicate)
noexcept(noexcept(predicate(*begin))) {
    for (; begin != end; ++begin)
        if (!predicate(*begin))
            return false;
    return true;
}

template<typename Container,typename Predicate>
HAD_CONSTEXPR14 bool all_of(const Container& container,Predicate&& predicate)
noexcept(noexcept(predicate(*container.begin() ) ) ) {
    return all_of(cbegin(container), cend(container), predicate);
}

HAD_CONSTEXPR14 inline bool all_of(std::initializer_list<bool> ilist) {
    for (bool b : ilist)
        if (b)
            return true;
    return false;
}

HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_ALL_OF_HPP

