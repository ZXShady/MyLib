#pragma once
#ifndef HAD_UTILITY_INVOKE_HPP
#define HAD_UTILITY_INVOKE_HPP
#include <utility>
#include <had/type_traits/reference_wrapper.hpp>
#include <had/type_traits/forward.hpp>
#include <had/macros.hpp>

HAD_NAMESPACE_BEGIN

template<typename Function>
auto invoke(Function&& function) HAD_DECLTYPE_NOEXCEPT_RETURN(
    static_cast<Function&&>(function)()
);

template<typename Fn,typename... Args>
auto invoke(Fn&& fn,Args&&... args) HAD_DECLTYPE_NOEXCEPT_RETURN(
    static_cast<Fn&&>(fn)(HAD_NS forward<Args>(args)...)
);


template<typename R,typename O,typename... Args1,typename... Args>
auto invoke(R (*fn)(Args1...),Args&&... args) HAD_DECLTYPE_NOEXCEPT_RETURN(
    fn(HAD_NS forward<Args>(args)...)
);

template<typename C,typename R,typename O,typename... Args1,typename... Args>
auto invoke(R (C::*fn)(Args1...),O&& o,Args&&... args) HAD_DECLTYPE_NOEXCEPT_RETURN(
    (o.*fn)(HAD_NS forward<Args>(args)...)
);

template<typename C,typename R,typename O,typename... Args1,typename... Args>
auto invoke(R (C::*fn)(Args1...),O* o,Args&&... args) HAD_DECLTYPE_NOEXCEPT_RETURN(
    (o->*fn)(HAD_NS forward<Args>(args)...)
);

template<typename T,typename C,typename O>
auto invoke(T C::*fn,O&& o) HAD_DECLTYPE_NOEXCEPT_RETURN(
    static_cast<O&&>(o).*fn
);

template<typename T,typename C,typename O>
auto invoke(T C::*fn,O* o) HAD_DECLTYPE_NOEXCEPT_RETURN(
    o->*fn
);

template<typename...>
struct invoke_result;

template<typename Fn,typename... Args>
struct invoke_result<Fn,Args...> {
    using type = decltype(HAD_NS invoke(HAD_NS declval<Fn>(),
        static_cast<Args>(HAD_NS declval<Args>())...));
};

template<typename Fn>
struct invoke_result<Fn> {
    using type = decltype(HAD_NS declval<Fn>() ());
};

template<typename Fn,typename... Args>
using invoke_result_t = typename invoke_result<Fn,Args...>::type;

HAD_NAMESPACE_END

#endif // !HAD_UTILITY_INVOKE_HPP
