#pragma once
#ifndef HAD_MACROS_HPP
#define HAD_MACROS_HPP

#include <had/version/cpp_version.hpp>

#ifdef __cpp_noexcept_function_type

#define HAD_IMPLEMENT_ALL_POSSIBLE_FUNCTION_SIGS(FUNCTION_DEFINE)  \
FUNCTION_DEFINE(__cdecl, , , );                                    \
FUNCTION_DEFINE(__vectorcall, , , );                               \
FUNCTION_DEFINE(__cdecl, const, , );                               \
FUNCTION_DEFINE(__vectorcall, const, , );                          \
FUNCTION_DEFINE(__cdecl, volatile, , );                            \
FUNCTION_DEFINE(__vectorcall, volatile, , );                       \
FUNCTION_DEFINE(__cdecl, const volatile, , );                      \
FUNCTION_DEFINE(__vectorcall, const volatile, , );                 \
FUNCTION_DEFINE(__cdecl, , &, );                                   \
FUNCTION_DEFINE(__vectorcall, , &, );                              \
FUNCTION_DEFINE(__cdecl, const, &, );                              \
FUNCTION_DEFINE(__vectorcall, const, &, );                         \
FUNCTION_DEFINE(__cdecl, volatile, &, );                           \
FUNCTION_DEFINE(__vectorcall, volatile, &, );                      \
FUNCTION_DEFINE(__cdecl, const volatile, &, );                     \
FUNCTION_DEFINE(__vectorcall, const volatile, &, );                \
FUNCTION_DEFINE(__cdecl, , &&, );                                  \
FUNCTION_DEFINE(__vectorcall, , &&, );                             \
FUNCTION_DEFINE(__cdecl, const, &&, );                             \
FUNCTION_DEFINE(__vectorcall, const, &&, );                        \
FUNCTION_DEFINE(__cdecl, volatile, &&, );                          \
FUNCTION_DEFINE(__vectorcall, volatile, &&, );                     \
FUNCTION_DEFINE(__cdecl, const volatile, &&, );                    \
FUNCTION_DEFINE(__vectorcall, const volatile, &&, );               \
FUNCTION_DEFINE(__cdecl, , , noexcept);                            \
FUNCTION_DEFINE(__vectorcall, , , noexcept);                       \
FUNCTION_DEFINE(__cdecl, const, , noexcept);                       \
FUNCTION_DEFINE(__vectorcall, const, , noexcept);                  \
FUNCTION_DEFINE(__cdecl, volatile, , noexcept);                    \
FUNCTION_DEFINE(__vectorcall, volatile, , noexcept);               \
FUNCTION_DEFINE(__cdecl, const volatile, , noexcept);              \
FUNCTION_DEFINE(__vectorcall, const volatile, , noexcept);         \
FUNCTION_DEFINE(__cdecl, , &, noexcept);                           \
FUNCTION_DEFINE(__vectorcall, , &, noexcept);                      \
FUNCTION_DEFINE(__cdecl, const, &, noexcept);                      \
FUNCTION_DEFINE(__vectorcall, const, &, noexcept);                 \
FUNCTION_DEFINE(__cdecl, volatile, &, noexcept);                   \
FUNCTION_DEFINE(__vectorcall, volatile, &, noexcept);              \
FUNCTION_DEFINE(__cdecl, const volatile, &, noexcept);             \
FUNCTION_DEFINE(__vectorcall, const volatile, &, noexcept);        \
FUNCTION_DEFINE(__cdecl, , &&, noexcept);                          \
FUNCTION_DEFINE(__vectorcall, , &&, noexcept);                     \
FUNCTION_DEFINE(__cdecl, const, &&, noexcept);                     \
FUNCTION_DEFINE(__vectorcall, const, &&, noexcept);                \
FUNCTION_DEFINE(__cdecl, volatile, &&, noexcept);                  \
FUNCTION_DEFINE(__vectorcall, volatile, &&, noexcept);             \
FUNCTION_DEFINE(__cdecl, const volatile, &&, noexcept);            \
FUNCTION_DEFINE(__vectorcall, const volatile, &&, noexcept)        
#else 
#define HAD_IMPLEMENT_ALL_POSSIBLE_FUNCTION_SIGS(FUNCTION_DEFINE)  \
FUNCTION_DEFINE(__cdecl, , , );                                    \
FUNCTION_DEFINE(__vectorcall, , , );                               \
FUNCTION_DEFINE(__cdecl, const, , );                               \
FUNCTION_DEFINE(__vectorcall, const, , );                          \
FUNCTION_DEFINE(__cdecl, volatile, , );                            \
FUNCTION_DEFINE(__vectorcall, volatile, , );                       \
FUNCTION_DEFINE(__cdecl, const volatile, , );                      \
FUNCTION_DEFINE(__vectorcall, const volatile, , );                 \
FUNCTION_DEFINE(__cdecl, , &, );                                   \
FUNCTION_DEFINE(__vectorcall, , &, );                              \
FUNCTION_DEFINE(__cdecl, const, &, );                              \
FUNCTION_DEFINE(__vectorcall, const, &, );                         \
FUNCTION_DEFINE(__cdecl, volatile, &, );                           \
FUNCTION_DEFINE(__vectorcall, volatile, &, );                      \
FUNCTION_DEFINE(__cdecl, const volatile, &, );                     \
FUNCTION_DEFINE(__vectorcall, const volatile, &, );                \
FUNCTION_DEFINE(__cdecl, , &&, );                                  \
FUNCTION_DEFINE(__vectorcall, , &&, );                             \
FUNCTION_DEFINE(__cdecl, const, &&, );                             \
FUNCTION_DEFINE(__vectorcall, const, &&, );                        \
FUNCTION_DEFINE(__cdecl, volatile, &&, );                          \
FUNCTION_DEFINE(__vectorcall, volatile, &&, );                     \
FUNCTION_DEFINE(__cdecl, const volatile, &&, );                    \
FUNCTION_DEFINE(__vectorcall, const volatile, &&, );               

#endif 

#define HAD_IMPLEMENT_LOGICAL_COMPARISONS(T) \
    HAD_NODISCARD HAD_CONSTEXPR14 friend bool operator!=(const T& a,const T& b) noexcept(noexcept( a == b )) { return !(a == b); }        \
    HAD_NODISCARD HAD_CONSTEXPR14 friend bool operator>(const T& a,const T& b)  noexcept(noexcept( a < b ))  { return b < a; }             \
    HAD_NODISCARD HAD_CONSTEXPR14 friend bool operator<=(const T& a,const T& b) noexcept(noexcept( a < b ))  { return !(a > b); }          \
    HAD_NODISCARD HAD_CONSTEXPR14 friend bool operator>=(const T& a,const T& b) noexcept(noexcept( a < b ))  { return !(a < b); }
#define HAD_IMPLEMENT_NON_FRIEND_LOGICAL_COMPARISONS(T) \
    HAD_NODISCARD HAD_CONSTEXPR14 bool  operator!=(const T& other) noexcept(noexcept( operator==(other) )) { return !(a == b); }        \
    HAD_NODISCARD HAD_CONSTEXPR14 bool  operator>(const T& other)  noexcept(noexcept( operator<(other)  )) { return b < a; }             \
    HAD_NODISCARD HAD_CONSTEXPR14 bool  operator<=(const T& other) noexcept(noexcept( operator<(other) ))  { return !(a > b); }          \
    HAD_NODISCARD HAD_CONSTEXPR14 bool  operator>=(const T& other) noexcept(noexcept( operator<(other) ))  { return !(a < b); }


#define HAD_DISABLE_SPECIAL_MEMBER_FUNCTIONS(T) \
    T() = delete;                      \
    T(const T&) = delete;              \
    T(T&&) = delete;                   \
    T& operator=(const T&) = delete;   \
    T& operator=(T&&) = delete        

#define HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(FUNCTION_DEFINE)  \
FUNCTION_DEFINE(&)                                                       \
FUNCTION_DEFINE(const&)                                                  \
FUNCTION_DEFINE(volatile&)                                               \
FUNCTION_DEFINE(const volatile&)                                         \
FUNCTION_DEFINE(&&)                                                      \
FUNCTION_DEFINE(const&&)                                                 \
FUNCTION_DEFINE(volatile&&)                                              \
FUNCTION_DEFINE(const volatile&&)                                      

#define HAD_DECLTYPE_NOEXCEPT_RETURN(T) \
noexcept(noexcept(T))   \
-> decltype(T) {        \
return T;               \
}


#ifdef _MSC_VER

#define HAD_FUNCSIG_HELPER __FUNCSIG__
#define HAD_FUNCSIG HAD_FUNCSIG_HELPER
#define HAD_RESTRICT __restrict

#else // !defined(_MSC_VER)

#define HAD_FUNCSIG_HELPER __PRETTY_FUNCTION__
#define HAD_FUNCSIG HAD_FUNCSIG_HELPER
#define HAD_RESTRICT

#endif // _MSC_VER

#define HAD_STATIC_ASSERT_IF(CONDITION,MESSAGE) static_assert(!((CONDITION)),MESSAGE)
#endif // !HAD_MACROS_HPP

