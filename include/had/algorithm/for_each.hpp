#pragma once
#ifndef HAD_ALGORITHM_FOR_EACH_HPP
#define HAD_ALGORITHM_FOR_EACH_HPP
#include <had/version/namespace.hpp>
#include <had/iterator/utilities.hpp>

HAD_NAMESPACE_BEGIN


template<typename Iterator,typename Function>
constexpr void for_each(Iterator begin, Iterator end, Function&& function) 
    noexcept(noexcept(function(*begin)))
{
    while(begin != end)
        function(*begin++);
}

template<typename Container,typename Function>
constexpr void for_each(Container&& container, Function&& function) 
noexcept(noexcept(function(*(begin(container)))))
{
    for_each(begin(container), end(container), static_cast<Function>(function));
}
HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_FOR_EACH_HPP

