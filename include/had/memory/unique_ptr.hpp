#pragma once
#ifndef HAD_MEMORY_UNIQUE_PTR_HPP
#define HAD_MEMORY_UNIQUE_PTR_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/is_const.hpp>
#include <had/type_traits/is_volatile.hpp>
#include <had/type_traits/is_reference.hpp>
#include <had/utility/compressed_pair.hpp>
#include <had/stdint.hpp>
#include <had/utility/swap.hpp>
#include <had/utility/exchange.hpp>
#include <had/memory/default_deleter.hpp>
#include <had/type_traits/decay.hpp>
#include <had/type_traits/remove_pointer.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/extent.hpp>
#include <had/type_traits/is_array.hpp>
#include <had/type_traits/is_unbounded_array.hpp>
#include <had/type_traits/is_bounded_array.hpp>
#include <had/type_traits/is_same.hpp>
#include <had/type_traits/is_convertible.hpp>
#include <had/type_traits/remove_extent.hpp>

HAD_NAMESPACE_BEGIN

template<typename ValueType, typename Deleter = default_deleter<ValueType> >
class unique_ptr {
public:
    using this_type = unique_ptr;
    using size_type = size_t;
    using value_type = ValueType;
    using deleter_type = Deleter;

    using noextent_type = typename remove_extent<value_type>::type;

    using pointer_type = noextent_type*;
    using const_pointer_type = const noextent_type*;
    using reference_type = value_type&;
    using const_reference_type = const value_type&;

    constexpr static bool is_array = HAD_NS is_array<value_type>::value;

    explicit unique_ptr(pointer_type ptr) noexcept
        :pair_(ptr)
    {
    }

    explicit unique_ptr(std::nullptr_t) noexcept
        :pair_(nullptr)
    {
    }

    template <typename U, typename DX,typename enable_if<
           (!is_array_v<U>) 
        && is_convertible_v<typename unique_ptr<U, DX>::pointer_type, pointer_type>
        && ((is_reference_v<DX>) ? is_same_v<DX, deleter_type> : is_convertible_v<DX, deleter_type>)
    >::type = 0>
    _CONSTEXPR23 unique_ptr(unique_ptr<U, DX>&& other) noexcept
        : pair_(one_then_variadic_args_t{}, HAD_NS forward<DX>(other.get_deleter()), other.release()) {}

    // uncopyable
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;
    // uncopyable

    unique_ptr(unique_ptr&& other) noexcept 
        : pair_(other.release())
    {
    }

    template <typename U, typename DX,typename enable_if<
        (!is_array_v<U>) 
        && is_convertible_v<typename unique_ptr<U, DX>::pointer_type, pointer_type>
        && ((is_reference_v<DX>) ? is_same_v<DX, deleter_type> : is_convertible_v<DX, deleter_type>)
    >::type = 0>
    unique_ptr& operator=(unique_ptr<U,DX>&& other) noexcept {
        reset(other.release());
        get_deleter() = other.get_deleter();
        return *this;
    }

    unique_ptr& operator=(std::nullptr_t) noexcept {
        deallocate(pair_.get_second());
        pair_.get_second() = nullptr;
        return *this;
    }


    ~unique_ptr() noexcept {
        deallocate(get_ptr());
    }
    
    HAD_NODISCARD deleter_type& get_deleter() noexcept {
        return pair_.get_first();
    }

    HAD_NODISCARD const deleter_type& get_deleter() const noexcept {
        return pair_.get_first();
    }

    HAD_NODISCARD pointer_type get_ptr() noexcept {
        return pair_.get_second();
    }

    HAD_NODISCARD const_pointer_type get_ptr() const noexcept {
        return pair_.get_second();
    }


    template<typename OtherT, typename OtherDeleter>
    HAD_NODISCARD bool operator==(const unique_ptr<OtherT, OtherDeleter>& o) const noexcept {
        return get_ptr() == o.get_ptr();
    }

    template<typename OtherT, typename OtherDeleter>
    HAD_NODISCARD bool operator!=(const unique_ptr<OtherT,OtherDeleter>& o) const noexcept {
        return get_ptr() == o.get_ptr();
    }

    template<typename OtherT, typename OtherDeleter>
    HAD_NODISCARD bool operator<(const unique_ptr<OtherT,OtherDeleter>& o) const noexcept {
        return get_ptr() < o.get_ptr();
    }

    template<typename OtherT, typename OtherDeleter>
    HAD_NODISCARD bool operator>(const unique_ptr<OtherT,OtherDeleter>& o) const noexcept {
        return get_ptr() > o.get_ptr();
    }

    template<typename OtherT, typename OtherDeleter>
    HAD_NODISCARD bool operator>=(const unique_ptr<OtherT,OtherDeleter>& o) const noexcept {
        return get_ptr() >= o.get_ptr();
    }
    
    template<typename OtherT, typename OtherDeleter>
    HAD_NODISCARD bool operator<=(const unique_ptr<OtherT,OtherDeleter>& o) const noexcept {
        return get_ptr() <= o.get_ptr();
    }

    HAD_NODISCARD bool operator==(std::nullptr_t) const noexcept {
        return get_ptr() == nullptr;
    }
    
    HAD_NODISCARD bool operator!=(std::nullptr_t) const noexcept {
        return get_ptr() != nullptr;
    }

    HAD_NODISCARD bool operator<(std::nullptr_t) const noexcept {
        return get_ptr() < nullptr;
    }

    HAD_NODISCARD bool operator>(std::nullptr_t) const noexcept {
        return get_ptr() > nullptr;
    }

    HAD_NODISCARD bool operator>=(std::nullptr_t) const noexcept {
        return get_ptr() >= nullptr;
    }

    HAD_NODISCARD bool operator<=(std::nullptr_t) const noexcept {
        return get_ptr() <= nullptr;
    }

    HAD_NODISCARD constexpr explicit operator bool() const noexcept {
        return get_ptr() != nullptr;
    }

    pointer_type release(pointer_type ptr = nullptr) noexcept {
        return HAD_NS exchange(pair_.get_second(), ptr);
    }

    void reset(pointer_type ptr = nullptr) noexcept {
        pointer_type old_ptr = HAD_NS exchange(pair_.get_second(), ptr);
        deallocate(old_ptr);
    }

    pointer_type data() const noexcept {
        return pair_.get_second();
    }
    
    HAD_CONSTEXPR14 void swap(unique_ptr& other) noexcept {
        using ::had::swap;
        swap(pair_.get_second(), other.pair_.get_second());
        swap(pair_.get_first(), other.pair_.get_first());
    }

    friend HAD_CONSTEXPR14 void swap(unique_ptr& a,unique_ptr& b) noexcept {
        a.swap(b);
    }

    HAD_NODISCARD reference_type operator*() const noexcept {
        return *(pair_.get_second());
    }
    
    HAD_NODISCARD pointer_type operator->() const noexcept {
        return pair_.get_second();
    }

    HAD_NODISCARD reference_type operator[](size_t index) const noexcept {
        static_assert(is_array," had::unique_ptr<T,Deleter>::operator[] cannot be used unless T is an unbounded array type.");
        return data()[index];
    }

private:
#ifdef __cpp_if_constexpr
    
    void deallocate(pointer_type ptr) noexcept {
        if constexpr (!is_same_v<deleter_type,default_deleter<ValueType> >) {
            if (ptr == nullptr) {
                return;
            }
        }

        get_deleter()(ptr);
    }

#else

    void deallocate(pointer_type ptr) noexcept {
        dealloc_impl<deleter_type>(ptr);
    }

    template<typename DeleterT, typename enable_if<is_same<DeleterT,default_deleter<ValueType> >::value >::type = 0 >
    void dealloc_impl(pointer_type ptr) noexcept {
        get_deleter()(ptr);
    }


    template<typename DeleterT,typename enable_if<!is_same<DeleterT,default_deleter<ValueType> >::value >::type = 0 >
    void dealloc_impl(pointer_type ptr) noexcept {

        if (ptr == nullptr) return;

        get_deleter()(ptr);
    }

#endif
private:

    HAD_NODISCARD pointer_type& get_ref_ptr() noexcept {
        return pair_.get_second();
    }

    HAD_NODISCARD const pointer_type& get_ref_ptr() const noexcept {
        return pair_.get_second();
    }

    compressed_pair<Deleter,pointer_type> pair_; // pair_.get_second() = my pointer
                                                 // get_first() is the deleter
};


template <typename T, typename... Args, typename enable_if<!is_array<T>::value>::type = 0>
unique_ptr<T> make_unique(Args&&... args) { // make a unique_ptr
    return unique_ptr<T>(new T(HAD_NS forward<Args>(args)...));
}

// T[](100) -> checks if T is array and the extent is 0 == unbounded array
template <typename T, typename enable_if<is_unbounded_array<T>::value>::type = 0>
unique_ptr<T> make_unique(size_t count) { // make a unique_ptr
    using Element = typename remove_extent<T>::type;
    return unique_ptr<T>(new Element[count]() );
}

_EXPORT_STD template <typename T, typename... Args, enable_if_t<is_bounded_array_v<T>> = 0>
void make_unique(Args...) = delete;






HAD_NAMESPACE_END

#endif // !HAD_MEMORY_UNIQUE_PTR_HPP