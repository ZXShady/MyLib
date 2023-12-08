#pragma once
#ifndef HAD_ALGORITHM_FIND_IF_NOT_HPP
#define HAD_ALGORITHM_FIND_IF_NOT_HPP
#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/iterator/utilities.hpp>

HAD_NAMESPACE_BEGIN



template<typename Iterator,typename Predicate>
constexpr HAD_NODISCARD inline Iterator find_if_not(Iterator begin, Iterator end, Predicate predicate) 
noexcept(noexcept(predicate(*begin))) {
    for (auto it = begin; it != end; ++it)
        if (!predicate(*it))
            return it;
    return end;
}

template<typename Container,typename Predicate>
constexpr HAD_NODISCARD inline auto find_if_not(const Container& container, Predicate predicate) 
noexcept(noexcept(predicate(*container.begin()))) 
-> decltype(container.begin())
{
    return find_if_not(cbegin(container), cend(container), predicate);
}
HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_FIND_IF_NOT_HPP

