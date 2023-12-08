#pragma once

#ifndef HAD_UTILITY_COMPRESSED_PAIR_HPP
#define HAD_UTILITY_COMPRESSED_PAIR_HPP

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>

#include <had/type_traits/move.hpp>

#include <had/type_traits/is_empty_class.hpp>
#include <had/type_traits/is_final.hpp>
#include <had/type_traits/forward.hpp>
#include <had/type_traits/enable_if.hpp>
HAD_NAMESPACE_BEGIN

enum class one_then_variadic_args_t : unsigned char {};

#ifdef __cpp_inline_variables

constexpr inline one_then_variadic_args_t one_then_variadic_args{};

#endif // __cpp_inline_variables

template<
    typename T1,
    typename T2,
    bool = is_empty_class<T1>::value && !is_final<T1>::value,
    bool = is_empty_class<T2>::value && !is_final<T2>::value
    >
class compressed_pair final : private T1 {
public:
    using first_type  = T1;
    using second_type = T2;
    using base        = T1;

    compressed_pair() : base(), mSecond() {};
    compressed_pair(const compressed_pair&) = default;
    compressed_pair(compressed_pair&&) = default;
    compressed_pair& operator=(const compressed_pair&) = default;
    compressed_pair& operator=(compressed_pair&&) = default;
    ~compressed_pair() = default;


    template<typename First,typename... Args>
    explicit compressed_pair(one_then_variadic_args_t, First&& first, Args&&... args)
        : base(HAD_NS forward<First>(first)), mSecond(HAD_NS forward<Args>(args)...) {}

    template<typename... Args>
    explicit compressed_pair(Args&&... args) 
        : mSecond(HAD_NS forward<Args>(args)...) {}
    // base slicing

    HAD_NODISCARD constexpr base& get_first() noexcept {
        return *this;
    }
    
    HAD_NODISCARD constexpr const base& get_first() const noexcept {
        return *this;
    }

    HAD_NODISCARD constexpr second_type& get_second() noexcept {
        return mSecond;
    }

    HAD_NODISCARD constexpr const second_type& get_second() const noexcept {
        return mSecond;
    }

private:
    T2 mSecond;
};

template<typename T1,typename T2>
class compressed_pair<T1,T2,false,true> final : private T2 {
public:
    using first_type  = T1;
    using second_type = T2;

    using base        = T2;
    compressed_pair() : base(), mFirst() {};
    compressed_pair(const compressed_pair&) = default;
    compressed_pair(compressed_pair&&) = default;
    compressed_pair& operator=(const compressed_pair&) = default;
    compressed_pair& operator=(compressed_pair&&) = default;

    ~compressed_pair() = default;

    template<typename First,typename... Args>
    explicit compressed_pair(one_then_variadic_args_t, First&& first, Args&&... args)
        : base(HAD_NS forward<First>(first)), mFirst(HAD_NS forward<Args>(args)...) {}

    template<typename... Args>
    explicit compressed_pair(Args&&... args) 
        : mFirst(forward<Args>(args)...) {}
    // base slicing
    HAD_NODISCARD constexpr first_type& get_first() noexcept {
        return mFirst;
    }

    HAD_NODISCARD constexpr const first_type& get_first() const noexcept {
        return mFirst;
    }

    HAD_NODISCARD constexpr base& get_second() noexcept {
        return *this;
    }

    HAD_NODISCARD constexpr const base& get_second() const noexcept {
        return *this;
    }
private:
    T1 mFirst;
};

template<typename T1,typename T2>
class compressed_pair<T1,T2,false,false> final {
public:
    using first_type  = T1;
    using second_type = T2;

    using base        = void;

    compressed_pair(const first_type& first,const second_type& second) 
        : mFirst(first),mSecond(second){};
    compressed_pair(const compressed_pair&) = default;
    compressed_pair(compressed_pair&&) = default;
    compressed_pair& operator=(const compressed_pair&) = default;
    compressed_pair& operator=(compressed_pair&&) = default;
    ~compressed_pair() = default;

    // base slicing
    HAD_NODISCARD constexpr first_type& get_first() noexcept {
        return mFirst;
    }

    HAD_NODISCARD constexpr const first_type& get_first() const noexcept {
        return mFirst;
    }

    HAD_NODISCARD constexpr second_type& get_second() noexcept {
        return mSecond;
    }

    HAD_NODISCARD constexpr const second_type& get_second() const noexcept {
        return mSecond;
    }
private:
    T1 mFirst;
    T2 mSecond;
};

HAD_NAMESPACE_END

#endif // !HAD_UTILITY_COMPRESSED_PAIR_HPP
