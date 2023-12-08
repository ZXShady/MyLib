#pragma once
#ifndef HAD_LIMITS_HPP
#define HAD_LIMITS_HPP
#include <cfloat>
#include <climits>
#include <cwchar>
#include <had/version/attributes/nodiscard.hpp>

namespace details {
struct num_base { // base for all types, with common defaults
    static constexpr bool has_infinity             = false;
    static constexpr bool has_quiet_NaN            = false;
    static constexpr bool has_signaling_NaN        = false;
    static constexpr bool is_bounded               = false;
    static constexpr bool is_exact                 = false;
    static constexpr bool is_iec559                = false;
    static constexpr bool is_integer               = false;
    static constexpr bool is_modulo                = false;
    static constexpr bool is_signed                = false;
    static constexpr bool is_specialized           = false;
    static constexpr bool tinyness_before          = false;
    static constexpr bool traps                    = false;
    static constexpr int  digits                    = 0;
    static constexpr int  digits10                  = 0;
    static constexpr int  max_digits10              = 0;
    static constexpr int  max_exponent              = 0;
    static constexpr int  max_exponent10            = 0;
    static constexpr int  min_exponent              = 0;
    static constexpr int  min_exponent10            = 0;
    static constexpr int  radix                     = 0;
};
struct num_int_base : num_base { // base for integer types
    static constexpr bool is_bounded     = true;
    static constexpr bool is_exact       = true;
    static constexpr bool is_integer     = true;
    static constexpr bool is_specialized = true;
    static constexpr int  radix           = 2;
};
struct num_float_base : num_base { // base for floating-point types
    static constexpr bool has_infinity             = true;
    static constexpr bool has_quiet_NaN            = true;
    static constexpr bool has_signaling_NaN        = true;
    static constexpr bool is_bounded               = true;
    static constexpr bool is_iec559                = true;
    static constexpr bool is_signed                = true;
    static constexpr bool is_specialized           = true;
    static constexpr int  radix                    = FLT_RADIX;
};

} // namespace details

template <typename T>
struct numeric_limits : details::num_base {
    // numeric limits for arbitrary type T (say little or nothing)
    HAD_NODISCARD static constexpr T min() noexcept {
        return T{};
    }

    HAD_NODISCARD static constexpr T max() noexcept {
        return T{};
    }

    HAD_NODISCARD static constexpr T lowest() noexcept {
        return T{};
    }

    HAD_NODISCARD static constexpr T epsilon() noexcept {
        return T{};
    }

    HAD_NODISCARD static constexpr T round_error() noexcept {
        return T{};
    }

    HAD_NODISCARD static constexpr T denorm_min() noexcept {
        return T{};
    }

    HAD_NODISCARD static constexpr T infinity() noexcept {
        return T{};
    }

    HAD_NODISCARD static constexpr T quiet_NaN() noexcept {
        return T{};
    }

    HAD_NODISCARD static constexpr T signaling_NaN() noexcept {
        return T{};
    }
};

template <typename T> struct numeric_limits<const T> : numeric_limits<T> {}; // numeric limits for const types

template <typename T> struct numeric_limits<volatile T> : numeric_limits<T> {}; // numeric limits for volatile types

template <typename T> struct numeric_limits<const volatile T> : numeric_limits<T> {}; // numeric limits for const volatile types



template <>
struct numeric_limits<bool> : details::num_int_base {
    HAD_NODISCARD static constexpr bool(min)() noexcept {
        return false;
    }

    HAD_NODISCARD static constexpr bool(max)() noexcept {
        return true;
    }

    HAD_NODISCARD static constexpr bool lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr bool epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr bool round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr bool denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr bool infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr bool quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr bool signaling_NaN() noexcept {
        return 0;
    }
    static constexpr int digits = 1;
};

template <>
struct numeric_limits<char> : details::num_int_base {
    HAD_NODISCARD static constexpr char(min)() noexcept {
        return CHAR_MIN;
    }

    HAD_NODISCARD static constexpr char(max)() noexcept {
        return CHAR_MAX;
    }

    HAD_NODISCARD static constexpr char lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr char epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char signaling_NaN() noexcept {
        return 0;
    }

    static constexpr bool is_signed = CHAR_MIN != 0;
    static constexpr bool is_modulo = CHAR_MIN == 0;
    static constexpr int digits     = 8 - (CHAR_MIN != 0);
    static constexpr int digits10   = 2;
};

template <>
class numeric_limits<signed char> : public details::num_int_base {
public:
    HAD_NODISCARD static constexpr signed char(min)() noexcept {
        return SCHAR_MIN;
    }

    HAD_NODISCARD static constexpr signed char(max)() noexcept {
        return SCHAR_MAX;
    }

    HAD_NODISCARD static constexpr signed char lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr signed char epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr signed char round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr signed char denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr signed char infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr signed char quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr signed char signaling_NaN() noexcept {
        return 0;
    }

    static constexpr bool is_signed = true;
    static constexpr int digits     = 7;
    static constexpr int digits10   = 2;
};

template <>
struct numeric_limits<unsigned char> : details::num_int_base {
    HAD_NODISCARD static constexpr unsigned char(min)() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned char(max)() noexcept {
        return UCHAR_MAX;
    }

    HAD_NODISCARD static constexpr unsigned char lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr unsigned char epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned char round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned char denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned char infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned char quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned char signaling_NaN() noexcept {
        return 0;
    }

    static constexpr bool is_modulo = true;
    static constexpr int digits     = 8;
    static constexpr int digits10   = 2;
};

#ifdef __cpp_char8_t
template <>
struct numeric_limits<char8_t> : details::num_int_base {
    HAD_NODISCARD static constexpr char8_t(min)() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char8_t(max)() noexcept {
        return UCHAR_MAX;
    }

    HAD_NODISCARD static constexpr char8_t lowest() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char8_t epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char8_t round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char8_t denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char8_t infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char8_t quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char8_t signaling_NaN() noexcept {
        return 0;
    }

    static constexpr bool is_modulo = true;
    static constexpr int digits     = 8;
    static constexpr int digits10   = 2;
};
#endif // __cpp_char8_t

template <>
struct numeric_limits<char16_t> : details::num_int_base {
    HAD_NODISCARD static constexpr char16_t(min)() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char16_t(max)() noexcept {
        return USHRT_MAX;
    }

    HAD_NODISCARD static constexpr char16_t lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr char16_t epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char16_t round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char16_t denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char16_t infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char16_t quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char16_t signaling_NaN() noexcept {
        return 0;
    }

    static constexpr bool is_modulo = true;
    static constexpr int digits     = 16;
    static constexpr int digits10   = 4;
};

template <>
struct numeric_limits<char32_t> : details::num_int_base {
    HAD_NODISCARD static constexpr char32_t(min)() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char32_t(max)() noexcept {
        return UINT_MAX;
    }

    HAD_NODISCARD static constexpr char32_t lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr char32_t epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char32_t round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char32_t denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char32_t infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char32_t quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr char32_t signaling_NaN() noexcept {
        return 0;
    }

    static constexpr bool is_modulo = true;
    static constexpr int digits     = 32;
    static constexpr int digits10   = 9;
};

template <>
struct numeric_limits<wchar_t> : details::num_int_base {
    HAD_NODISCARD static constexpr wchar_t(min)() noexcept {
        return WCHAR_MIN;
    }

    HAD_NODISCARD static constexpr wchar_t(max)() noexcept {
        return WCHAR_MAX;
    }

    HAD_NODISCARD static constexpr wchar_t lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr wchar_t epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr wchar_t round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr wchar_t denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr wchar_t infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr wchar_t quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr wchar_t signaling_NaN() noexcept {
        return 0;
    }

    static constexpr bool is_modulo = true;
    static constexpr int digits     = 16;
    static constexpr int digits10   = 4;
};

template <>
struct numeric_limits<short> : details::num_int_base {
    HAD_NODISCARD static constexpr short(min)() noexcept {
        return SHRT_MIN;
    }

    HAD_NODISCARD static constexpr short(max)() noexcept {
        return SHRT_MAX;
    }

    HAD_NODISCARD static constexpr short lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr short epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr short round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr short denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr short infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr short quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr short signaling_NaN() noexcept {
        return 0;
    }

    static constexpr bool is_signed = true;
    static constexpr int digits     = 15;
    static constexpr int digits10   = 4;
};

template <>
struct numeric_limits<int> : details::num_int_base {
    HAD_NODISCARD static constexpr int(min)() noexcept {
        return INT_MIN;
    }

    HAD_NODISCARD static constexpr int(max)() noexcept {
        return INT_MAX;
    }

    HAD_NODISCARD static constexpr int lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr int epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr int round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr int denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr int infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr int quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr int signaling_NaN() noexcept {
        return 0;
    }

    static constexpr bool is_signed = true;
    static constexpr int digits     = 31;
    static constexpr int digits10   = 9;
};

template <>
struct numeric_limits<long> : details::num_int_base {
    HAD_NODISCARD static constexpr long(min)() noexcept {
        return LONG_MIN;
    }

    HAD_NODISCARD static constexpr long(max)() noexcept {
        return LONG_MAX;
    }

    HAD_NODISCARD static constexpr long lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr long epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr long round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr long denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr long infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr long quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr long signaling_NaN() noexcept {
        return 0;
    }

    static_assert(sizeof(int) == sizeof(long), "LLP64 assumption");
    static constexpr bool is_signed = true;
    static constexpr int digits     = 31;
    static constexpr int digits10   = 9;
};

template <>
struct numeric_limits<long long> : details::num_int_base {
    HAD_NODISCARD static constexpr long long(min)() noexcept {
        return LLONG_MIN;
    }

    HAD_NODISCARD static constexpr long long(max)() noexcept {
        return LLONG_MAX;
    }

    HAD_NODISCARD static constexpr long long lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr long long epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr long long round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr long long denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr long long infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr long long quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr long long signaling_NaN() noexcept {
        return 0;
    }

    static constexpr bool is_signed = true;
    static constexpr int digits     = 63;
    static constexpr int digits10   = 18;
};

#ifdef _NATIVE_WCHAR_T_DEFINED
template <>
struct numeric_limits<unsigned short> : details::num_int_base {
public:
    HAD_NODISCARD static constexpr unsigned short(min)() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned short(max)() noexcept {
        return USHRT_MAX;
    }

    HAD_NODISCARD static constexpr unsigned short lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr unsigned short epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned short round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned short denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned short infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned short quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned short signaling_NaN() noexcept {
        return 0;
    }

    static constexpr bool is_modulo = true;
    static constexpr int digits     = 16;
    static constexpr int digits10   = 4;
};
#endif // _NATIVE_WCHAR_T_DEFINED

template <>
struct numeric_limits<unsigned int> : details::num_int_base {
    HAD_NODISCARD static constexpr unsigned int(min)() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned int(max)() noexcept {
        return UINT_MAX;
    }

    HAD_NODISCARD static constexpr unsigned int lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr unsigned int epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned int round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned int denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned int infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned int quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned int signaling_NaN() noexcept {
        return 0;
    }

    static constexpr bool is_modulo = true;
    static constexpr int digits     = 32;
    static constexpr int digits10   = 9;
};

template <>
struct numeric_limits<unsigned long> : details::num_int_base {
    HAD_NODISCARD static constexpr unsigned long(min)() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned long(max)() noexcept {
        return ULONG_MAX;
    }

    HAD_NODISCARD static constexpr unsigned long lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr unsigned long epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned long round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned long denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned long infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned long quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned long signaling_NaN() noexcept {
        return 0;
    }

    static_assert(sizeof(unsigned int) == sizeof(unsigned long), "LLP64 assumption");
    static constexpr bool is_modulo = true;
    static constexpr int digits     = 32;
    static constexpr int digits10   = 9;
};

template <>
struct numeric_limits<unsigned long long> : details::num_int_base {
    HAD_NODISCARD static constexpr unsigned long long(min)() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned long long(max)() noexcept {
        return ULLONG_MAX;
    }

    HAD_NODISCARD static constexpr unsigned long long lowest() noexcept {
        return (min) ();
    }

    HAD_NODISCARD static constexpr unsigned long long epsilon() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned long long round_error() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned long long denorm_min() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned long long infinity() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned long long quiet_NaN() noexcept {
        return 0;
    }

    HAD_NODISCARD static constexpr unsigned long long signaling_NaN() noexcept {
        return 0;
    }

    static constexpr bool is_modulo = true;
    static constexpr int digits     = 64;
    static constexpr int digits10   = 19;
};
#if 0
template <>
struct numeric_limits<float> : details::num_float_base {
    HAD_NODISCARD static constexpr float(min)() noexcept {
        return FLT_MIN;
    }

    HAD_NODISCARD static constexpr float(max)() noexcept {
        return FLT_MAX;
    }

    HAD_NODISCARD static constexpr float lowest() noexcept {
        return -(max) ();
    }

    HAD_NODISCARD static constexpr float epsilon() noexcept {
        return FLT_EPSILON;
    }

    HAD_NODISCARD static constexpr float round_error() noexcept {
        return 0.5F;
    }

    HAD_NODISCARD static constexpr float denorm_min() noexcept {
        return FLT_TRUE_MIN;
    }

    HAD_NODISCARD static constexpr float infinity() noexcept {
        return __builtin_huge_valf();
    }

    HAD_NODISCARD static constexpr float quiet_NaN() noexcept {
        return __builtin_nanf("0");
    }

    HAD_NODISCARD static constexpr float signaling_NaN() noexcept {
        return __builtin_nansf("1");
    }

    static constexpr int digits         = FLT_MANT_DIG;
    static constexpr int digits10       = FLT_DIG;
    static constexpr int max_digits10   = 9;
    static constexpr int max_exponent   = FLT_MAX_EXP;
    static constexpr int max_exponent10 = FLT_MAX_10_EXP;
    static constexpr int min_exponent   = FLT_MIN_EXP;
    static constexpr int min_exponent10 = FLT_MIN_10_EXP;
};

template <>
class numeric_limits<double> : public _Num_float_base {
public:
    HAD_NODISCARD static constexpr double(min)() noexcept {
        return DBL_MIN;
    }

    HAD_NODISCARD static constexpr double(max)() noexcept {
        return DBL_MAX;
    }

    HAD_NODISCARD static constexpr double lowest() noexcept {
        return -(max) ();
    }

    HAD_NODISCARD static constexpr double epsilon() noexcept {
        return DBL_EPSILON;
    }

    HAD_NODISCARD static constexpr double round_error() noexcept {
        return 0.5;
    }

    HAD_NODISCARD static constexpr double denorm_min() noexcept {
        return DBL_TRUE_MIN;
    }

    HAD_NODISCARD static constexpr double infinity() noexcept {
        return __builtin_huge_val();
    }

    HAD_NODISCARD static constexpr double quiet_NaN() noexcept {
        return __builtin_nan("0");
    }

    HAD_NODISCARD static constexpr double signaling_NaN() noexcept {
        return __builtin_nans("1");
    }

    static constexpr int digits         = DBL_MANT_DIG;
    static constexpr int digits10       = DBL_DIG;
    static constexpr int max_digits10   = 17;
    static constexpr int max_exponent   = DBL_MAX_EXP;
    static constexpr int max_exponent10 = DBL_MAX_10_EXP;
    static constexpr int min_exponent   = DBL_MIN_EXP;
    static constexpr int min_exponent10 = DBL_MIN_10_EXP;
};

template <>
class numeric_limits<long double> : public _Num_float_base {
public:
    HAD_NODISCARD static constexpr long double(min)() noexcept {
        return LDBL_MIN;
    }

    HAD_NODISCARD static constexpr long double(max)() noexcept {
        return LDBL_MAX;
    }

    HAD_NODISCARD static constexpr long double lowest() noexcept {
        return -(max) ();
    }

    HAD_NODISCARD static constexpr long double epsilon() noexcept {
        return LDBL_EPSILON;
    }

    HAD_NODISCARD static constexpr long double round_error() noexcept {
        return 0.5L;
    }

    HAD_NODISCARD static constexpr long double denorm_min() noexcept {
        return LDBL_TRUE_MIN;
    }

    HAD_NODISCARD static constexpr long double infinity() noexcept {
        return __builtin_huge_val();
    }

    HAD_NODISCARD static constexpr long double quiet_NaN() noexcept {
        return __builtin_nan("0");
    }

    HAD_NODISCARD static constexpr long double signaling_NaN() noexcept {
        return __builtin_nans("1");
    }

    static constexpr int digits         = LDBL_MANT_DIG;
    static constexpr int digits10       = LDBL_DIG;
    static constexpr int max_digits10   = 2 + LDBL_MANT_DIG * 301L / 1000;
    static constexpr int max_exponent   = LDBL_MAX_EXP;
    static constexpr int max_exponent10 = LDBL_MAX_10_EXP;
    static constexpr int min_exponent   = LDBL_MIN_EXP;
    static constexpr int min_exponent10 = LDBL_MIN_10_EXP;
};
#endif // ! floating point support
#endif // HAD_LIMITS_HPP