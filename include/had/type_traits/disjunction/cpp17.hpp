#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>

HAD_NAMESPACE_BEGIN

template <typename... Traits>
struct disjunction : boolean_constant<(bool(Traits::value) || ...)> {};

template <typename... Traits>
inline constexpr bool disjunction_v = (bool(Traits::value) || ...);

HAD_NAMESPACE_END


