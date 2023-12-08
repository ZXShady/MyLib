// xsmf_control.h internal header

// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#ifndef HAD_SMF_CONTROL_HPP
#define HAD_SMF_CONTROL_HPP

#include <had/type_traits/special_member_functions.hpp>
#include <had/type_traits/conditional.hpp>
#include <had/type_traits/conjunction.hpp>
#include <had/version/attributes/constexpr.hpp>
HAD_NAMESPACE_BEGIN

namespace details {
namespace SMF {

template <typename Base>
struct non_trivial_copy : Base { // non-trivial copy construction facade
    using Base::Base;

    non_trivial_copy() = default;
    HAD_CONSTEXPR20 non_trivial_copy(const non_trivial_copy& that) noexcept(
        noexcept(Base::smf_construct_from(static_cast<const Base&>(that)))) {
        Base::smf_construct_from(static_cast<const Base&>(that));
    }
    non_trivial_copy(non_trivial_copy&&)                 = default;
    non_trivial_copy& operator=(const non_trivial_copy&) = default;
    non_trivial_copy& operator=(non_trivial_copy&&)      = default;
};

template <typename Base>
struct deleted_copy : Base { // deleted copy construction facade
    using Base::Base;

    deleted_copy()                               = default;
    deleted_copy(const deleted_copy&)            = delete;
    deleted_copy(deleted_copy&&)                 = default;
    deleted_copy& operator=(const deleted_copy&) = default;
    deleted_copy& operator=(deleted_copy&&)      = default;
};

template <typename Base, typename... Ts>
using control_copy = conditional_t<conjunction_v<is_trivially_copy_constructible<Ts>...>, Base,
    conditional_t<conjunction_v<is_copy_constructible<Ts>...>, non_trivial_copy<Base>, deleted_copy<Base>>>;

template <typename Base, typename... Ts>
struct non_trivial_move : control_copy<Base, Ts...> { // non-trivial move construction facade
    using MyBase = control_copy<Base, Ts...>;
    using MyBase::MyBase;

    non_trivial_move()                         = default;
    non_trivial_move(const non_trivial_move&) = default;
    HAD_CONSTEXPR20 non_trivial_move(non_trivial_move&& that) noexcept(
        noexcept(MyBase::smf_construct_from(static_cast<Base&&>(that)))) {
        MyBase::smf_construct_from(static_cast<Base&&>(that));
    }
    non_trivial_move& operator=(const non_trivial_move&) = default;
    non_trivial_move& operator=(non_trivial_move&&)      = default;
};

template <typename Base, typename... Ts>
struct deleted_move : control_copy<Base, Ts...> { // deleted move construction facade
    using MyBase = control_copy<Base, Ts...>;
    using MyBase::MyBase;

    deleted_move()                                = default;
    deleted_move(const deleted_move&)            = default;
    deleted_move(deleted_move&&)                 = delete;
    deleted_move& operator=(const deleted_move&) = default;
    deleted_move& operator=(deleted_move&&)      = default;
};

template <typename Base, typename... Ts>
using control_move = conditional_t<
    conjunction_v<is_trivially_move_constructible<Ts>...>, 
    control_copy<Base, Ts...>,
    conditional_t<conjunction_v<is_move_constructible<Ts>...>, non_trivial_move<Base, Ts...>,
    deleted_move<Base, Ts...>>>;

template <typename Base, typename... Ts>
struct non_trivial_copy_assign : control_move<Base, Ts...> { // non-trivial copy assignment facade
    using MyBase = control_move<Base, Ts...>;
    using MyBase::MyBase;

    non_trivial_copy_assign()                                = default;
    non_trivial_copy_assign(const non_trivial_copy_assign&) = default;
    non_trivial_copy_assign(non_trivial_copy_assign&&)      = default;

    HAD_CONSTEXPR20 non_trivial_copy_assign& operator=(const non_trivial_copy_assign& that) noexcept(
        noexcept(MyBase::smf_assign_from(static_cast<const Base&>(that)))) {
        MyBase::smf_assign_from(static_cast<const Base&>(that));
        return *this;
    }
    non_trivial_copy_assign& operator=(non_trivial_copy_assign&&) = default;
};

template <typename Base, typename... Ts>
struct deleted_copy_assign : control_move<Base, Ts...> { // deleted copy assignment facade
    using MyBase = control_move<Base, Ts...>;
    using MyBase::MyBase;

    deleted_copy_assign()                                       = default;
    deleted_copy_assign(const deleted_copy_assign&)            = default;
    deleted_copy_assign(deleted_copy_assign&&)                 = default;
    deleted_copy_assign& operator=(const deleted_copy_assign&) = delete;
    deleted_copy_assign& operator=(deleted_copy_assign&&)      = default;
};

template <typename Base, typename... Ts>
using control_copy_assign =
conditional_t<conjunction_v<is_trivially_destructible<Ts>..., is_trivially_copy_constructible<Ts>...,
    is_trivially_copy_assignable<Ts>...>,
    control_move<Base, Ts...>,
    conditional_t<conjunction_v<is_copy_constructible<Ts>..., is_copy_assignable<Ts>...>,
    non_trivial_copy_assign<Base, Ts...>, deleted_copy_assign<Base, Ts...>>>;

template <typename Base, typename... Ts>
struct non_trivial_move_assign : control_copy_assign<Base, Ts...> { // non-trivial move assignment facade
    using MyBase = control_copy_assign<Base, Ts...>;
    using MyBase::MyBase;

    non_trivial_move_assign()                                           = default;
    non_trivial_move_assign(const non_trivial_move_assign&)            = default;
    non_trivial_move_assign(non_trivial_move_assign&&)                 = default;
    non_trivial_move_assign& operator=(const non_trivial_move_assign&) = default;

    HAD_CONSTEXPR20 non_trivial_move_assign& operator=(non_trivial_move_assign&& that) noexcept(
        noexcept(MyBase::smf_assign_from(static_cast<Base&&>(that)))) {
        MyBase::smf_assign_from(static_cast<Base&&>(that));
        return *this;
    }
};

template <typename Base, typename... Ts>
struct deleted_move_assign : control_copy_assign<Base, Ts...> { // deleted move assignment facade
    using MyBase = control_copy_assign<Base, Ts...>;
    using MyBase::MyBase;

    deleted_move_assign()                                      = default;
    deleted_move_assign(const deleted_move_assign&)            = default;
    deleted_move_assign(deleted_move_assign&&)                 = default;
    deleted_move_assign& operator=(const deleted_move_assign&) = default;
    deleted_move_assign& operator=(deleted_move_assign&&)      = delete;
};

template <typename Base, typename... Ts>
using control_move_assign =
conditional_t<
    conjunction_v<
        is_trivially_destructible<Ts>...,
        is_trivially_move_constructible<Ts>...,
        is_trivially_move_assignable<Ts>...>,
    control_copy_assign<Base, Ts...>,
    conditional_t<conjunction_v<
    is_move_constructible<Ts>...,
    is_move_assignable<Ts>...>,
    non_trivial_move_assign<Base, Ts...>, deleted_move_assign<Base, Ts...>>>;

}
}
template <typename Base, typename... Ts>
using SMF_control = details::SMF::control_move_assign<Base, Ts...>;

HAD_NAMESPACE_END

#endif // !HAD_SMF_CONTROL_HPP