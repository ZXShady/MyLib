#pragma once
#ifndef HAD_TUPLE_HPP
#define HAD_TUPLE_HPP

#include <iostream>

// Contains the actual value for one item in the tuple. The 
// template parameter `i` allows the
// `Get` function to find the value in O(1) time
// Obtain a reference to i-th item in a tuple
// Contains the actual value for one item in the tuple. The 
// template parameter `i` allows the
// `Get` function to find the value in O(1) time
template<std::size_t i, typename Item>
struct tuple_leaf {
    Item value;
};

template<std::size_t i, typename... Items>
struct tuple_impl;

// Base case: empty tuple
template<std::size_t i>
struct tuple_impl<i>{};

// Recursive specialization
template<std::size_t i, typename T, typename... Ts>
struct tuple_impl<i, T, Ts...> :
    public tuple_leaf<i, T>, // This adds a `value` member of type T
    public tuple_impl<i + 1, Ts...> // This recurses
{};

template<typename... Ts>
using tuple = tuple_impl<0ull, Ts...>;

namespace std {
// Obtain a reference to i-th item in a tuple
template<std::size_t i, typename T, typename... Ts>
T& get(tuple_impl<i, T, Ts...>& tuple) {
    // Fully qualified name for the member, to find the right one 
    // (they are all called `value`).
    return tuple.tuple_leaf<i, T>::value;
}

template<std::size_t i, typename T, typename... Ts>
const T& get(const tuple_impl<i, T, Ts...>& tuple) {
    // Fully qualified name for the member, to find the right one 
    // (they are all called `value`).
    return tuple.tuple_leaf<i, T>::value;
}
}

#endif // !HAD_TUPLE_HPP
