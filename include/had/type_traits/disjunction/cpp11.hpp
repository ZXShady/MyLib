#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>

HAD_NAMESPACE_BEGIN

namespace details {

template <bool, typename First, typename... Rest>
struct disjunction { // handle false trait or last trait
    using type = First;
};

template <typename False, typename Next, typename... Rest>
struct disjunction<false, False, Next, Rest...> {
    using type = typename disjunction<bool(Next::value), Next, Rest...>::type;
};

} // namespace details


template <typename...> // if empty
struct disjunction : true_type {};

template <typename First, typename... Rest>
struct disjunction<First, Rest...> : details::disjunction<First, Rest...>::type {

};

HAD_NAMESPACE_END

