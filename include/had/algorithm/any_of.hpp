#pragma once
#ifndef HAD_ALGORITHM_ANY_OF_HPP
#define HAD_ALGORITHM_ANY_OF_HPP

#include <had/version/namespace.hpp>
#include <had/version/attributes/constexpr.hpp>
#include <had/initializer_list.hpp>
#include <had/iterator/utilities.hpp>

HAD_NAMESPACE_BEGIN


template<typename Iterator,typename Predicate>
HAD_CONSTEXPR14 inline bool any_of(Iterator begin, Iterator end, Predicate predicate)
noexcept(noexcept(predicate(*begin))) {
    for (; begin != end; ++begin)
        if (predicate(*begin))
            return true;
    return false;
}

template<typename Container,typename Predicate>
HAD_CONSTEXPR14 inline bool any_of(const Container& container,Predicate&& predicate)
noexcept(noexcept(predicate(*container.begin() ) ) ) {
    return any_of(cbegin(container), cend(container), predicate);
}

template<typename Container,typename Predicate>
HAD_CONSTEXPR14 inline bool any_of(had::initializer_list<Container> ilist,Predicate&& predicate)
noexcept(noexcept(any_of(begin(ilist), end(ilist), predicate))) {
    return any_of(begin(ilist), end(ilist), predicate);
}

HAD_CONSTEXPR14 inline bool any_of(std::initializer_list<bool> ilist) noexcept {
    for (bool b : ilist)
        if (b)
            return true;
    return false;
}

HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_ANY_OF_HPP

