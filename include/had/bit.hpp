#pragma once
#ifndef HAD_BIT_HPP
#define HAD_BIT_HPP

#include <had/version/namespace.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/remove_extent.hpp>
#include <had/type_traits/is_trivially_copyable.hpp>
#include <cstring>
#include <had/array.hpp>

HAD_NAMESPACE_BEGIN

#ifdef __has_builtin
#if __has_builtin(__builtin_bit_cast)

#define HAD_HAS_BUILTIN_BITCAST 1

#endif // __has_builtin(__builtin_bitcast)
#endif // defined(__has_builtin)

#if HAD_HAS_BUILTIN_BITCAST

template<typename To, typename From, typename enable_if<!is_array<To>::value>::type = 0>
HAD_NODISCARD constexpr To bit_cast(From from) noexcept {
	static_assert(sizeof(To) == sizeof(From), "had::bit_cast<To>(From) precondition failed (sizeof(To) == sizeof(From))");
	static_assert(is_trivially_copyable<To>::value, "had::bit_cast<To>(From) precondition failed (is_trivially_copyable<To>)");
	static_assert(is_trivially_copyable<From>::value, "had::bit_cast<To>(From) precondition failed (is_trivially_copyable<From>)");
	return __builtin_bit_cast(To,from);
}

#else 
	
template<typename To,typename From,typename enable_if<is_bounded_array<To>::value>::type = 0>
HAD_NODISCARD To bit_cast(From from) noexcept {
	static_assert(sizeof(To) == sizeof(From), "had::bit_cast<To>(From) precondition failed (sizeof(To) == sizeof(From))");
	static_assert(is_trivially_copyable<To>::value, "had::bit_cast<To>(From) precondition failed (is_trivially_copyable<To>)");
	static_assert(is_trivially_copyable<From>::value, "had::bit_cast<To>(From) precondition failed (is_trivially_copyable<From>)");

	To to;
	std::memcpy(&to, &from, sizeof(To));
	return to;
}

#endif // HAD_HAS_BUILTIN_BITCAST


template<typename To,typename From,typename enable_if<is_bounded_array<To>::value>::type = 0>
had::array<typename remove_extent<To>::type,
	(sizeof(To) / sizeof(typename remove_extent<To>::type))> bit_cast(From from) noexcept {
	
	static_assert(sizeof(To) == sizeof(From), "had::bit_cast<To>(From) sizeof(To) must be equal to sizeof(From)");
	static_assert(is_trivially_copyable<To>::value, "had::bit_cast<To>(From) To must be trivially copyable");
	static_assert(is_trivially_copyable<From>::value, "had::bit_cast<To>(From) From must be trivially copyable");
	
	had::array<typename remove_extent<To>::type,
		(sizeof(To) / sizeof(typename remove_extent<To>::type))> to;
	std::memcpy(&to[0], &from, sizeof(To));
	return to;
}
HAD_NAMESPACE_END

#endif // !HAD_BIT_HPP