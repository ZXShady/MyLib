#pragma once
#ifndef HAD_FUNCTIONAL_FUNCTION_TRAITS_HPP
#define HAD_FUNCTIONAL_FUNCTION_TRAITS_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/type_traits/always_false.hpp>
#include <had/macros.hpp>
HAD_NAMESPACE_BEGIN

template<typename T > 
struct function_traits { 
    static_assert(always_false<T>::value, "function_traits only accept functions!");
};

#define HAD_FN_T_(CALL,CV,REF,NOEXCEPT)                                       \
template<typename ReturnType,typename... Args>                                \
struct function_traits<ReturnType CALL (Args...) NOEXCEPT> {                  \
    using return_type = ReturnType;                                           \
    using class_type  = void;                                                 \
    constexpr static auto arity = sizeof...(Args);                            \
    constexpr static bool is_member_function = false;                         \
    constexpr static bool is_noexcept        = NOEXCEPT(false);               \
    constexpr static bool is_vectorcall = #CALL[2] == 'v';                    \
    constexpr static bool is_cdecl      = !is_vectorcall;                     \
}

#define HAD_FN_T_C_(CALL,CV,REF,NOEXCEPT)                                     \
template<typename ReturnType,typename Class,typename... Args>                 \
struct function_traits<ReturnType (CALL Class::*)(Args...) CV REF NOEXCEPT> { \
    using return_type = ReturnType;                                           \
    using class_type = Class;                                                 \
    constexpr static auto arity = sizeof...(Args);                            \
    constexpr static bool is_member_function = true;                          \
    constexpr static bool is_noexcept        = NOEXCEPT(false);               \
    constexpr static bool is_vectorcall = #CALL[2] == 'v';                    \
    constexpr static bool is_cdecl      = !is_vectorcall;                     \
}


HAD_FN_T_(__cdecl, , , );                              
HAD_FN_T_(__vectorcall, , , );                         
HAD_FN_T_C_(__cdecl, , , );
HAD_FN_T_C_(__vectorcall, , , );
HAD_FN_T_C_(__cdecl, const, , );
HAD_FN_T_C_(__vectorcall, const, , );
HAD_FN_T_C_(__cdecl, volatile, , );
HAD_FN_T_C_(__vectorcall, volatile, , );
HAD_FN_T_C_(__cdecl, const volatile, , );
HAD_FN_T_C_(__vectorcall, const volatile, , );
HAD_FN_T_C_(__cdecl, , &, );
HAD_FN_T_C_(__vectorcall, , &, );
HAD_FN_T_C_(__cdecl, const, &, );
HAD_FN_T_C_(__vectorcall, const, &, );
HAD_FN_T_C_(__cdecl, volatile, &, );
HAD_FN_T_C_(__vectorcall, volatile, &, );
HAD_FN_T_C_(__cdecl, const volatile, &, );
HAD_FN_T_C_(__vectorcall, const volatile, &, );
HAD_FN_T_C_(__cdecl, , &&, );
HAD_FN_T_C_(__vectorcall, , &&, );
HAD_FN_T_C_(__cdecl, const, &&, );
HAD_FN_T_C_(__vectorcall, const, &&, );
HAD_FN_T_C_(__cdecl, volatile, &&, );
HAD_FN_T_C_(__vectorcall, volatile, &&, );
HAD_FN_T_C_(__cdecl, const volatile, &&, );
HAD_FN_T_C_(__vectorcall, const volatile, &&, );
#ifdef __cpp_noexcept_function_type
HAD_FN_T_(__cdecl, , ,noexcept );                              
HAD_FN_T_(__vectorcall, , ,noexcept );                         

HAD_FN_T_C_(__cdecl, , , noexcept);
HAD_FN_T_C_(__vectorcall, , , noexcept);
HAD_FN_T_C_(__cdecl, const, , noexcept);
HAD_FN_T_C_(__vectorcall, const, , noexcept);
HAD_FN_T_C_(__cdecl, volatile, , noexcept);
HAD_FN_T_C_(__vectorcall, volatile, , noexcept);
HAD_FN_T_C_(__cdecl, const volatile, , noexcept);
HAD_FN_T_C_(__vectorcall, const volatile, , noexcept);
HAD_FN_T_C_(__cdecl, , &, noexcept);
HAD_FN_T_C_(__vectorcall, , &, noexcept);
HAD_FN_T_C_(__cdecl, const, &, noexcept);
HAD_FN_T_C_(__vectorcall, const, &, noexcept);
HAD_FN_T_C_(__cdecl, volatile, &, noexcept);
HAD_FN_T_C_(__vectorcall, volatile, &, noexcept);
HAD_FN_T_C_(__cdecl, const volatile, &, noexcept);
HAD_FN_T_C_(__vectorcall, const volatile, &, noexcept);
HAD_FN_T_C_(__cdecl, , &&, noexcept);
HAD_FN_T_C_(__vectorcall, , &&, noexcept);
HAD_FN_T_C_(__cdecl, const, &&, noexcept);
HAD_FN_T_C_(__vectorcall, const, &&, noexcept);
HAD_FN_T_C_(__cdecl, volatile, &&, noexcept);
HAD_FN_T_C_(__vectorcall, volatile, &&, noexcept);
HAD_FN_T_C_(__cdecl, const volatile, &&, noexcept);
HAD_FN_T_C_(__vectorcall, const volatile, &&, noexcept);


#endif // __cpp_noexcept_function_type

#undef HAD_FN_T_
#undef HAD_FN_T_C_


HAD_NAMESPACE_END
#endif // !HAD_FUNCTIONAL_FUNCTION_TRAITS_HPP
