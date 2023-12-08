#pragma once
#ifndef HAD_CONSTEXPR_MAP_HPP
#define HAD_CONSTEXPR_MAP_HPP
#include <had/type_traits/move.hpp>
#include <had/type_traits/conditional.hpp>
#include <had/type_traits/is_convertible.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/remove_cv.hpp>
#include <had/type_traits/addressof.hpp>
#include <had/type_traits/special_member_functions.hpp>
#include <had/type_traits/is_nothrow_swappable.hpp>
#include <had/type_traits/as.hpp>
#include <had/type_traits/remove_cv.hpp>
#include <had/version/namespace.hpp>
#include <had/macros.hpp>
#include <had/smf_control.hpp>
#include <had/utility/destroy_at.hpp>
#include <had/utility/invoke.hpp>
#include <had/format.hpp>
#include <had/exceptions.hpp>
#include <had/array.hpp>
#include <had/algorithm/find_if.hpp>
HAD_NAMESPACE_BEGIN

template<typename KeyType,typename ValueType,size_t Size>
class constexpr_map {
public:
    struct KeyValuePair {
        KeyType key;
        ValueType value;
        operator pair<KeyType, ValueType>() const noexcept {
            return pair<KeyType, ValueType>(key, value);
        }
    };

    array<KeyValuePair, Size> data;

    using iterator = typename decltype(data)::iterator;
    using const_iterator = typename decltype(data)::const_iterator;

    iterator at(const KeyType& key) {
        auto pred = [&](const KeyValuePair& kv) -> bool {
            return (kv.key == key);
            };

        return HAD_NS find_if(data.iterator_pair(), pred);
    }

    const_iterator at(const KeyType& key) const {
        auto pred = [&](const KeyValuePair& kv) -> bool {
            return (kv.key == key);
            };
        return HAD_NS find_if(data.iterator_pair(), pred);
    }
};


HAD_NAMESPACE_END

#endif // !HAD_CONSTEXPR_MAP_HPP
