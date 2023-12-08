#pragma once
#ifndef HAD_TYPE_TRAITS_REFERENCE_WRAPPER_HPP
#define HAD_TYPE_TRAITS_REFERENCE_WRAPPER_HPP
#include <had/utility/declval.hpp>
#include <had/type_traits/addressof.hpp>
#include <had/type_traits/remove_cvref.hpp>
#include <had/type_traits/is_same.hpp>
#include <had/type_traits/is_object.hpp>
#include <had/type_traits/is_function.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/version/attributes.hpp>
#include <had/utility/invoke.hpp>
HAD_NAMESPACE_BEGIN
namespace details {
template <typename T>
void refwrap_ctor_fun(T&) noexcept; // not defined
template <typename T>
void refwrap_ctor_fun(T&&) = delete;

template <typename T, typename U, typename = void>
struct refwrap_ctor_from : false_type {};

template <typename T, typename U>
struct refwrap_ctor_from<T, U,void_t<decltype(refwrap_ctor_fun<T>(::had::declval<U>()))>> 
    : true_type {};

} // namespace details

template <typename T>
class reference_wrapper {
public:
    static_assert(is_object<T>::value || is_function<T>::value,
        "reference_wrapper<T> requires T to be an object type or a function type.");

    using type = T;
    using this_type = reference_wrapper;


    template <typename U, enable_if_t<
          !is_same_v<remove_cvref_t<U>, this_type>
        && details::refwrap_ctor_from<T, U>::value> = 0>
    HAD_CONSTEXPR20 reference_wrapper(U&& val) 
        noexcept(noexcept(details::refwrap_ctor_fun<T>(::had::declval<U>()))) 
    { 
        // qualified: avoid ADL, handle incomplete types
        T& ref    = static_cast<U&&>(val);
        mPtr      = ::had::addressof(ref);
    }

    HAD_CONSTEXPR20 operator T&() const noexcept {
        return *mPtr;
    }

    HAD_NODISCARD HAD_CONSTEXPR20 T& get() const noexcept {
        return *mPtr;
    }

private:
    T* mPtr{};
public:
    //template <typename... Args>
    //HAD_CONSTEXPR20 auto operator()(Args&&... args) const
    //    noexcept(noexcept(::had::invoke(*mPtr, static_cast<Args&&>(args)...)))
    //    -> decltype(::had::invoke(*mPtr, static_cast<Args&&>(args)...)) 
    //{
    //    return ::had::invoke(*mPtr, static_cast<Args&&>(args)...);
    //}
};

HAD_NAMESPACE_END
#endif // !HAD_TYPE_TRAITS_REFERENCE_WRAPPER_HPP
