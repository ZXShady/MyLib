#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
HAD_NAMESPACE_BEGIN

namespace details {
template <bool FirstType, typename First, typename... Rest>
struct conjunction { // handle false trait or last trait
    using type = First;
};

template <typename True, typename Next, typename... Rest>
struct conjunction<true, True, Next, Rest...> {
    using type = typename conjunction<bool(Next::value), Next, Rest...>::type;
};

} // namespace details


template <typename...> // if empty
struct conjunction : true_type {};

template <typename First, typename... Rest>
struct conjunction<First, Rest...> : details::conjunction<First::value, First, Rest...>::type {

};

HAD_NAMESPACE_END

