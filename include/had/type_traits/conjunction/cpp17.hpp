#define HAD_TYPE_TRAITS_CONJUNCTION_HPP 17

#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
HAD_NAMESPACE_BEGIN

template <typename... Traits>
struct conjunction : boolean_constant<(bool(Traits::value) && ...)> {};

template <typename... Traits>
inline constexpr bool conjunction_v = (bool(Traits::value) && ...);

HAD_NAMESPACE_END


