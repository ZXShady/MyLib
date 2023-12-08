#pragma once

#ifndef HAD_TYPE_TRAITS_LIKE_HPP
#define HAD_TYPE_TRAITS_LIKE_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/is_reference.hpp>
#include <had/type_traits/conditional.hpp>
#include <had/type_traits/add_reference.hpp>
#include <had/type_traits/add_volatile.hpp>
#include <had/type_traits/remove_reference.hpp>
#include <had/type_traits/is_const.hpp>
#include <had/type_traits/is_volatile.hpp>
#include <had/type_traits/apply_modifiers.hpp>
#include <had/type_traits/remove_cvref.hpp>
HAD_NAMESPACE_BEGIN


template<typename From,typename To>
struct like {
private:
    constexpr static bool is_lval = is_lvalue_reference<From>::value;
    constexpr static bool is_ref  = is_reference<From>::value;
    using noref_From = typename remove_reference<From>::type;
    using nocvref_To = typename remove_cvref<To>::type;
    using cv = typename apply_modifiers<noref_From, nocvref_To>::type;
    using cv_lref = typename add_lvalue_reference<cv>::type;
    using cv_rref = typename add_rvalue_reference<cv>::type;
    using if_lval = typename conditional<is_lval, cv_lref, cv_rref>::type;
public:
    using type = typename conditional<is_ref,if_lval,cv>::type;
};

#ifdef __cpp_alias_templates

template<typename From,typename To>
using like_t = typename like<From,To>::type;

#endif // __cpp_alias_templates
HAD_NAMESPACE_END


#endif // !HAD_TYPE_TRAITS_LIKE_HPP
