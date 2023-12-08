#pragma once
#ifndef HAD_STDINT_HPP
#define HAD_STDINT_HPP
#include <cstdint>
#include <had/version/namespace.hpp>
#include <had/type_traits/make_signed.hpp>

HAD_NAMESPACE_BEGIN
enum class byte : unsigned char {};
using byte_t    = unsigned char;
using size_t    = decltype(sizeof(void*));
using ssize_t   = std::ptrdiff_t;
using i8        = std::int8_t;
using i16       = std::int16_t;
using i32       = std::int32_t;
using i64       = std::int64_t;
using u8        = std::uint8_t;
using u16       = std::uint16_t;
using u32       = std::uint32_t;
using u64       = std::uint64_t;
using least_i8  = std::int_least8_t;
using least_i16 = std::int_least16_t;
using least_i32 = std::int_least32_t;
using least_i64 = std::int_least64_t;
using least_u8  = std::uint_least8_t;
using least_u16 = std::uint_least16_t;
using least_u32 = std::uint_least32_t;
using least_u64 = std::uint_least64_t;
using fast_i8   = std::int_fast8_t;
using fast_i16  = std::int_fast16_t;
using fast_i32  = std::int_fast32_t;
using fast_i64  = std::int_fast64_t;
using fast_u8   = std::uint_fast8_t;
using fast_u16  = std::uint_fast16_t;
using fast_u32  = std::uint_fast32_t;
using fast_u64  = std::uint_fast64_t;
using f32       = float;
using f64       = double;
using f80       = long double;

enum class no_init : unsigned char {};

#ifdef __cpp_user_defined_literals

inline namespace literals {

inline ssize_t operator""_z(unsigned long long N) { return static_cast<ssize_t>(N); }

}
#endif // __cpp_user_defined_literals
using namespace literals;
HAD_NAMESPACE_END

#endif // !HAD_STDINT_HPP
