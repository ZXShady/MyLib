#define HAD_TYPE_TRAITS_DISJUNCTION_HPP 14

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/algorithm/any_of.hpp>
HAD_NAMESPACE_BEGIN

template <typename... Traits>
struct disjunction : boolean_constant < 
    any_of(std::initializer_list<bool>{Traits::value...})
> {};

template <typename... Traits>
constexpr bool disjunction_v = any_of(std::initializer_list<bool>{Traits::value...});

HAD_NAMESPACE_END

