#include <had/version/namespace.hpp>
#include <had/type_traits/boolean_types.hpp>
#include <had/algorithm/all_of.hpp>
HAD_NAMESPACE_BEGIN

template <typename... Traits>
struct conjunction : boolean_constant <all_of(std::initializer_list<bool>{bool(Traits::value)...})> {};

template<typename... Traits>
constexpr bool conjunction_v = all_of(std::initializer_list<bool>{bool(Traits::value)...});

HAD_NAMESPACE_END
