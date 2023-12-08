#pragma once

#ifndef HAD_ITERATOR_ITERATOR_TRAITS_HPP
#define HAD_ITERATOR_ITERATOR_TRAITS_HPP

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/utility/declval.hpp>
#include <had/type_traits/remove_cvref.hpp>
#include <had/type_traits/is_random_access_iterator.hpp>
#include <had/type_traits/is_pointer.hpp>
#include <had/typeinfo/operator_dereference.hpp>
#include <had/typeinfo/operator_increment.hpp>
HAD_NAMESPACE_BEGIN

template<typename IteratorType>
struct iterator_traits {
    using iterator_type = IteratorType;
    using dereference_type  = decltype(* HAD_NS declval<iterator_type>());
    using value_type        = typename remove_cvref<dereference_type>::type;
    using no_reference_type = typename remove_reference<dereference_type>::type;
    using pointer_type            = no_reference_type*;
    using const_pointer_type      = const no_reference_type*;

    constexpr static bool is_random_access = is_random_access_iterator<IteratorType>::value;

};


template<typename IteratorType>
struct iterator_traits<IteratorType*> {
    using iterator_type   = IteratorType*;
    using value_type      = typename remove_cv<IteratorType>::type;
    using dereference_type = value_type&;
    using no_reference_type = typename remove_reference<dereference_type>::type;
    using pointer_type            = no_reference_type*;
    using const_pointer_type      = const no_reference_type*;

    constexpr static bool is_random_access = true;

};

template<typename IteratorType>
struct iterator_traits<const IteratorType*> {
    using type            = const IteratorType*;
    using value_type      = typename remove_cv<IteratorType>::type;

    using dereference_type  = const value_type&;
    constexpr static bool is_random_access = true;

};

template<typename IteratorType>
struct iterator_traits<const volatile IteratorType*> {
    using type            = const volatile IteratorType*;
    using value_type      = typename remove_cv<IteratorType>::type;
    using derefence_type  = const volatile value_type&;
    constexpr static bool is_random_access = true;

};

template<typename IteratorType>
struct iterator_traits<volatile IteratorType*> {
    using type            = volatile IteratorType*;
    using value_type      = typename remove_cvref<IteratorType>::type;
    using derefence_type  = volatile value_type&;
    constexpr static bool is_random_access = true;

};

template<typename Iterator>
struct is_iterator {
    constexpr static bool value = is_pointer<Iterator>::value
        || operator_increment<Iterator>::is_symbol_function
        && operator_dereference<Iterator>::is_symbol_function;


};

#ifdef __cpp_variable_templates

template<typename Iterator>
constexpr bool is_iterator_v = is_iterator<Iterator>::value;

#endif // __cpp_variable_templates

HAD_NAMESPACE_END

#endif // !HAD_ITERATOR_ITERATOR_TRAITS_HPP
