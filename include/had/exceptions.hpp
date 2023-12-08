#pragma once
#ifndef HAD_EXCEPTIONS_HPP
#define HAD_EXCEPTIONS_HPP 1
#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/type_traits/is_trivially_copy_constructible.hpp>
#include <had/utility/declval.hpp>
#include <stdexcept>

HAD_NAMESPACE_BEGIN

struct exception : public std::exception {
    using base = std::exception;

    explicit exception(const char* message = "had::exception") : base(message) {}
};
struct logic_error : public exception {
    using base = exception;

    explicit logic_error(const char* message = "had::logic_error") : base(message) {}
};


struct out_of_range : public logic_error {
    using base = logic_error;

    explicit out_of_range(const char* message = "had::out_of_range") : base(message) {}
};


HAD_NAMESPACE_END

#endif // !HAD_EXCEPTIONS_HPP