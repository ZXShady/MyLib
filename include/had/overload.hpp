#pragma once

#ifndef HAD_OVERLOAD_HPP
#define HAD_OVERLOAD_HPP

#include <had/version/namespace.hpp>

HAD_NAMESPACE_BEGIN

template<typename... Functors>
struct overload : Functors... {
    using Functors::operator()...;
#ifndef __cpp_deduction_guides

    constexpr overload(const Functors&...) {}

#endif // !__cpp_deduction_guides

};

#ifdef __cpp_deduction_guides

template<typename... Functors>
overload(Functors...) -> overload<Functors...>;

#endif // !__cpp_deduction_guides


template<typename... Fns>
overload<Fns...> make_overload(const Fns&...) {
    return overload<Fns...>{};
}

HAD_NAMESPACE_END

#endif // !HAD_OVERLOAD_HPP