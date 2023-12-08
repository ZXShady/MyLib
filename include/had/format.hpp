#pragma once

#ifndef HAD_FORMAT_HPP
#define HAD_FORMAT_HPP

#include <had/vector.hpp>
#include <had/string.hpp>
#include <cstdio>
#include <had/type_traits/decay.hpp>
#include <had/format/formatter.hpp>
HAD_NAMESPACE_BEGIN


namespace details {
namespace format {

inline void to_str(const string&) {}

template<typename First>
inline void to_str(string& str, const First& first) {
    using format_t = typename remove_reference<First>::type;
//    using format_t = typename decay<First>::type;
    const auto& out = ::had::formatter<format_t>::to_string(first);
    str.replace("{}", out);
}

template<typename First, typename... Rest>
inline void to_str(string& str, const First& first, const Rest&... rest) {
    //using format_t = First;
    using format_t = typename remove_reference<First>::type;
    //    using format_t = typename decay<First>::type;
    const auto& out = ::had::formatter<format_t>::to_string(first);
    str.replace("{}", out);
    to_str(str, rest...);
}

} // namespace format
} // namespace details

template<typename... Args>
HAD_NODISCARD string format(string_view format, const Args&... args) {
    //constexpr size_t initial_size = details::format::get_reserve_amount<Args...>::value;
    string output = { format.c_str(),format.length() };
    //output.reserve(initial_size);
    //copy_no_overlap(format, output.begin());
    
    details::format::to_str(output,args...);
    return output;
}
template<typename... Args>
void print(string_view format, const Args&... args) {
    std::printf(::had::format(format,args...).c_str());
}

template<typename... Args>
void println(string_view format, const Args&... args) {
    std::puts(::had::format(format,args...).c_str());
}

template<typename Arg>
void print(const Arg& arg) {
    using format_t = typename remove_reference<Arg>::type;
    std::printf(formatter<format_t>::to_string(arg).c_str());
}

template<typename Arg>
void println(const Arg& arg) {
    using format_t = typename remove_reference<Arg>::type;
    std::puts(formatter<format_t>::to_string(arg).c_str());
}
HAD_NAMESPACE_END
#endif // !HAD_FORMAT_HPP
