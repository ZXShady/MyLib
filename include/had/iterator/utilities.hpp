#pragma once
#ifndef HAD_ITERATOR_UTILITIES_HPP
#define HAD_ITERATOR_UTILITIES_HPP
#include <had/version/namespace.hpp>
#include <had/version/attributes.hpp>
#include <had/iterator/iterator_traits.hpp>
#include <had/typeinfo/operator_dereference.hpp>
#include <had/iterator/reverse_iterator.hpp>
#include <had/type_traits/is_const.hpp>
#include <had/type_traits/is_pointing_to_same_type.hpp>
#include <had/type_traits/addressof.hpp>

HAD_NAMESPACE_BEGIN

template<typename T,size_t N>
HAD_NODISCARD HAD_CONSTEXPR20 T* begin(T (&array)[N]) noexcept {
    return array + 0;
}

template<typename T,size_t N>
HAD_NODISCARD HAD_CONSTEXPR20 T* end(T (&array)[N]) noexcept {
    return array + N;
}
template<typename T,size_t N>
HAD_NODISCARD HAD_CONSTEXPR20 reverse_iterator<T> rbegin(T (&array)[N]) noexcept {
    return &array[N-1];
}

template<typename T,size_t N>
HAD_NODISCARD HAD_CONSTEXPR20 reverse_iterator<T> rend(T (&array)[N]) noexcept {
    return &array[-1];
}

template<typename T,size_t N>
HAD_NODISCARD HAD_CONSTEXPR20 reverse_iterator<const T> crbegin(const T (&array)[N]) noexcept {
    return HAD_NS rbegin(array);
}

template<typename T,size_t N>
HAD_NODISCARD HAD_CONSTEXPR20 reverse_iterator<const T> crend(const T (&array)[N]) noexcept {
    return HAD_NS rend(array);
}

template<typename T,size_t N>
HAD_NODISCARD HAD_CONSTEXPR20 const T* cbegin(const T (&array)[N]) noexcept {
    return HAD_NS begin(array);
}

template<typename T,size_t N>
HAD_NODISCARD HAD_CONSTEXPR20 const T* cend(const T (&array)[N]) noexcept {
    return HAD_NS end(array);
}


template<typename Container>
HAD_NODISCARD HAD_CONSTEXPR20 auto begin(Container&& container) 
noexcept(noexcept(container.begin()))
-> decltype(container.begin())
{
    return container.begin();
}

template<typename Container>
HAD_NODISCARD HAD_CONSTEXPR20 auto end(Container&& container) 
noexcept(noexcept(container.end()))
-> decltype(container.end())
{
    return container.end();
}

template<typename Container>
HAD_NODISCARD HAD_CONSTEXPR20 auto rbegin(Container&& container) 
noexcept(noexcept(container.rbegin()))
-> decltype(container.rbegin())
{
    return container.rbegin();
}

template<typename Container>
HAD_NODISCARD HAD_CONSTEXPR20 auto rend(Container&& container) 
noexcept(noexcept(container.rend()))
-> decltype(container.rend())
{
    return container.rend();
}

template<typename Container>
HAD_NODISCARD HAD_CONSTEXPR20 auto cbegin(Container&& container) 
noexcept(noexcept(container.cbegin()))
-> decltype(container.cbegin())
{
    return container.cbegin();
}

template<typename Container>
HAD_NODISCARD HAD_CONSTEXPR20 auto cend(Container&& container) 
noexcept(noexcept(container.cend()))
-> decltype(container.cend())
{
    return container.cend();
}

template<typename Container>
HAD_NODISCARD HAD_CONSTEXPR20 auto crbegin(Container&& container) 
noexcept(noexcept(container.crbegin()))
-> decltype(container.crbegin())
{
    return container.crbegin();
}

template<typename Container>
HAD_NODISCARD HAD_CONSTEXPR20 auto crend(Container&& container) 
noexcept(noexcept(container.crend()))
-> decltype(container.crend())
{
    return container.crend();
}




template<typename T,bool = 
    (is_pointing_to_same_type_no_qualifiers<T,void*>::value) 
    || operator_dereference<T>::is_symbol_function >
struct iterator_pair {
    using type = T;
    using value_type             = typename iterator_traits<T>::value_type;
    using pointer_type           = value_type*;
    using const_pointer_type     = const value_type*;

    using dereference_type = typename iterator_traits<T>::dereference_type;
    using this_type = iterator_pair;
    type begin, end;
    decltype(HAD_NS addressof(*begin)) get_addressof() noexcept {
        return HAD_NS addressof(*begin);
    };

};


template<typename T>
struct iterator_pair<T,false> {
    static_assert(always_false<T>::value, "iterator_pair cannot be formed from types that don't have the '*' operator");
};

template<typename T>
HAD_NODISCARD constexpr iterator_pair<T> make_iterator_pair(T begin, T end) noexcept {
    return iterator_pair<T>{begin, end};
}

template<typename Container>
HAD_NODISCARD constexpr auto make_iterator_pair(Container&& container)
noexcept(noexcept(begin(container)))
-> iterator_pair<decltype(begin(container))>
{
    using T = decltype(begin(container));
    return iterator_pair<T>{begin(container), end(container)};
}
#ifdef __cpp_deduction_guides

template<typename T>
iterator_pair(T, T) -> iterator_pair<T>;

template<typename T>
iterator_pair(T) -> iterator_pair<decltype(HAD_NS begin(HAD_NS declval<T>() )) >;

#endif // __cpp_deduction_guides

HAD_NAMESPACE_END

#endif // !HAD_ITERATOR_UTILITIES_HPP
