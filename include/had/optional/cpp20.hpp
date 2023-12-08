#include <had/type_traits/special_member_functions.hpp>
#include <had/type_traits/addressof.hpp>
#include <had/utility/destroy_at.hpp>
#include <had/utility/construct_at.hpp>

HAD_NAMESPACE_BEGIN

template<typename ValueType>
class optional {
public:
    using this_type                    = optional;
    using value_type                   = ValueType;
    using pointer_type                 = value_type*;
    using reference_type               = value_type&;
    using rvalue_reference_type        = value_type&&;
    using const_rvalue_reference_type  = value_type&&;
    using const_pointer_type           = const value_type*;
    using const_reference_type         = const value_type&;

    bool mHasValue = false;
    union {
        value_type mValue;
    };

    constexpr static bool is_trivially_copy_constructible   = is_trivially_copy_assignable_v<value_type>;
    constexpr static bool is_trivially_move_constructible   = is_trivially_move_assignable_v<value_type>;
    constexpr static bool is_trivially_copy_assignable      = is_trivially_copy_constructible_v<value_type>;
    constexpr static bool is_trivially_move_assignable      = is_trivially_move_constructible_v<value_type>;
    constexpr static bool is_trivially_destructible         = is_trivially_destructible_v<value_type>;


    optional(value_type value) {
        HAD_NS construct_at(HAD_NS addressof(mValue), value);
    }

    template<typename... Args>
    reference_type emplace(Args&&... args) {
        if (has_value()) {
            destroy();
        }
        return *HAD_NS construct_at(HAD_NS addressof(mValue, HAD_NS forward<Args>(args)...);
    }


    constexpr explicit operator bool() const noexcept {
        return has_value();
    }
    constexpr explicit bool has_value() const noexcept {
        return mHasValue;
    }

    constexpr optional(const this_type&) 
        requires is_trivially_copy_assignable = default;
    constexpr optionaloperator=(this_type&&) 
        requires is_trivially_move_assignable = default;
    constexpr this_type& operator=(const this_type&) 
        requires is_trivially_copy_assignable = default;
    constexpr this_type& operator=(this_type&&)  
        requires is_trivially_move_assignable = default;
    constexpr ~optional() requires is_trivially_destructible = default;

    constexpr ~optional() {
        HAD_NS destroy_at(HAD_NS addressof(mValue));
    };

private:
    void destroy() noexcept {
        HAD_NS destroy_at(HAD_NS addressof(mValue));
    }
};

template<typename T>
optional(T) -> optional<T>;

int ma() {
    optional<int> l = 10;
    is_trivially_copy_constructible_v<decltype(l)>;

}
HAD_NAMESPACE_END