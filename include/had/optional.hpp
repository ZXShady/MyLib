#pragma once
#ifndef HAD_OPTIONAL_HPP
#define HAD_OPTIONAL_HPP

#include <had/type_traits/move.hpp>
#include <had/type_traits/conditional.hpp>
#include <had/type_traits/is_convertible.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/remove_cv.hpp>
#include <had/type_traits/addressof.hpp>
#include <had/type_traits/special_member_functions.hpp>
#include <had/type_traits/is_nothrow_swappable.hpp>
#include <had/type_traits/as.hpp>
#include <had/type_traits/remove_cvref.hpp>
#include <had/version/namespace.hpp>
#include <had/macros.hpp>
#include <had/internal/SMF_control.hpp>
#include <had/utility/destroy_at.hpp>
#include <had/utility/invoke.hpp>
#include <had/format.hpp>
#include <had/type_traits/as.hpp>
#include <had/type_traits/is_object.hpp>
#include <had/type_traits/is_array.hpp>
#include <had/exceptions.hpp>
HAD_NAMESPACE_BEGIN

enum class in_place_t : unsigned char {};
#ifdef __cpp_inline_variables

constexpr inline in_place_t in_place = in_place_t{};

#endif // __cpp_inline_variables


template<typename T>
class optional;
struct bad_optional_access : exception {
    using base = exception;

    explicit bad_optional_access(const char* message = "bad optional access") : base(message) {}
};


namespace details {
namespace optional {
#ifdef HAD_CPP17

template<typename Self,typename Fn>
constexpr auto and_then(Self&& self, Fn&& fn)
-> decltype(HAD_NS invoke(fn,*self))
{
    using SelfType = typename remove_cvref<Self>::type;
    using InvokeResultType = decltype(HAD_NS invoke(fn,*self));
    using LikeT = typename like<Self, typename SelfType::value_type>::type;
    using noCvRef = typename remove_cvref<InvokeResultType>::type;
    //static_assert(HAD_NS is_optional<InvokeResultType>::value,
    //    "Function of optional<T>::and_then(Function) must return an optional");

    if (self) {
        return HAD_NS invoke(HAD_NS forward<Fn>(fn), *self);
    }
    return noCvRef{};
}

#endif // HAD_CPP17

template<typename Self>
HAD_CONSTEXPR14 auto value(Self&& self) 
-> typename HAD_NS like<Self&&,decltype(*self)>::type {
    if (self) {
        return *self;
    }
    throw bad_optional_access();
}

struct non_trivial_dummy_type {
    constexpr non_trivial_dummy_type() noexcept {

    }
};
template <class T, bool = is_trivially_destructible<T>::value>
struct destruct_base { // either contains a value of T or is empty (trivial destructor)
    union {
        non_trivial_dummy_type dummy;
        typename remove_cv<T>::type mValue;
    };
    bool mHasValue;

    constexpr destruct_base() noexcept : dummy{}, mHasValue{false} {} // initialize an empty optional

    void reset() noexcept {
        mHasValue = false;
    }
};

template <class T>
struct destruct_base<T, false> { // either contains a value of T or is empty (non-trivial destructor)
    union {
        non_trivial_dummy_type dummy;
        remove_cv_t<T> mValue;
    };
    bool mHasValue;

    ~destruct_base() noexcept {
        if (mHasValue) {
            destroy_at(HAD_NS addressof(mValue));
        }
    }

    constexpr destruct_base() noexcept : dummy{}, mHasValue{false} {} // initialize an empty optional

    destruct_base(const destruct_base&)            = default;
    destruct_base(destruct_base&&)                 = default;
    destruct_base& operator=(const destruct_base&) = default;
    destruct_base& operator=(destruct_base&&)      = default;

    void reset() noexcept {
        if (mHasValue) {
            destroy_at(HAD_NS addressof(mValue));
            mHasValue = false;
        }
    }
};

template <typename T>
struct construct_base : destruct_base<T> {
    using destruct_base<T>::destruct_base;
    template <typename... Args>
    void smf_construct(Args&&... args) {
        construct_at(&this->mValue, ::had::forward<Args>(args)...);
        this->mHasValue = true;
    }

    template <typename Self>
    void smf_assign(Self&& other) {
        if (this->mHasValue) {
            static_cast<T&>(this->mValue) = ::had::forward<Self>(other);
        } else {
            smf_construct(::had::forward<Self>(other));
        }
    }

    template <typename Self>
    void smf_construct_from(Self&& other) {
        smf_construct(forward<Self>(other));
    }

    template <typename Self>
    void smf_assign_from(Self&& other)  {
        if (other.mHasValue) {
            smf_assign(other);
        }
    }

};

}
}

enum class nullopt_t : unsigned char {};

template<typename ValueType>
class optional : private HAD_NS SMF_control<details::optional::construct_base<ValueType>,ValueType>
{
    static_assert(!is_same<remove_cv_t<ValueType>, nullopt_t>::value,
        "T in optional<T> must be a type other than nullopt_t or in_place_t (N4950 [optional.optional.general]/3).");
    static_assert(is_object<ValueType>::value && !is_array<ValueType>::value,
        "T in optional<T> must meet the Cpp17Destructible requirements (N4950 [optional.optional.general]/3).");
private:
    template<typename Self>
    friend HAD_NODISCARD HAD_CONSTEXPR14 auto details::optional::operatorDereference(Self&&)
        -> typename HAD_NS like<Self,typename remove_cvref<Self>::type::value_type>::type;
public:
    using this_type                    = optional;
    using value_type                   = ValueType;
    using pointer_type                 = value_type*;
    using reference_type               = value_type&;
    using rvalue_reference_type        = value_type&&;
    using const_rvalue_reference_type  = value_type&&;
    using const_pointer_type           = const value_type*;
    using const_reference_type         = const value_type&;
    
private:
    using no_cv_type = typename remove_cv<value_type>::type;
    using my_base = ::had::SMF_control<
        details::optional::construct_base<ValueType>
        , ValueType>;
    constexpr static bool nothrow_copy_construct = is_nothrow_copy_constructible<value_type>::value;
    constexpr static bool nothrow_move_construct = is_nothrow_move_constructible<value_type>::value;
    constexpr static bool nothrow_swappable = is_nothrow_swappable<value_type>::value;
    template<typename U>
    constexpr static bool allow_unwrap = 
        !is_same_v<value_type, U>,
        && is_assignable_v<value_type&, optional<U>&>,
        && is_assignable_v<value_type&, const optional<U>&>,
        && is_assignable_v<value_type&, const optional<U>>,
        && is_assignable_v<value_type&, optional<U> > >;

public:    
    using my_base::reset;
private:
    using my_base::mValue;
    using my_base::mHasValue;
    using my_base::smf_construct;
    using my_base::smf_assign;
    using my_base::smf_construct_from;
    using my_base::smf_assign_from;
public:
    constexpr optional() noexcept {}
    constexpr optional(nullopt_t) noexcept {}


    template<typename... Args>
    optional(in_place_t,Args&&... args) noexcept(is_nothrow_constructible<T,Args...>::value) {
        this->smf_construct(HAD_NS forward<Args>(args)...);
    }

    HAD_NODISCARD constexpr explicit operator bool() const noexcept {
        return has_value();
    }

    HAD_NODISCARD constexpr explicit operator bool() const volatile noexcept {
        return has_value();
    }

    HAD_NODISCARD constexpr bool has_value() const noexcept {
        return this->mHasValue;
    }

    HAD_NODISCARD constexpr bool has_value() const volatile noexcept {
        return this->mHasValue;
    }

#if HAD_CONTAINER_DEBUGGING_LEVEL > 0
#define HAD_OPERATOR_DEREFERENCE_(CVREF)                                         \
    HAD_NODISCARD HAD_CONSTEXPR14 value_type CVREF operator*() CVREF noexcept {  \
        if (!this->has_value()) {                                                \
            throw bad_optional_access("From optional<T>::operator*()");          \
        }                                                                        \
        return static_cast<value_type CVREF>(this->mValue);                      \
    }
#else // HAD_CONTAINER_DEBUGGING_LEVEL == 0  

#define HAD_OPERATOR_DEREFERENCE_(CVREF)                                          \
    HAD_NODISCARD HAD_CONSTEXPR14 value_type CVREF operator*() CVREF noexcept {   \
        return static_cast<value_type CVREF>(this->mValue);                       \
    }

#endif // HAD_CONTAINER_DEBUGGING_LEVEL > 0  

    HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(HAD_OPERATOR_DEREFERENCE_)


#undef HAD_OPERATOR_DEREFERENCE_

#define HAD_VALUE_(THIS_QUALIFIERS)                                                     \
    HAD_NODISCARD HAD_CONSTEXPR14 value_type THIS_QUALIFIERS value() THIS_QUALIFIERS {  \
        return details::optional::value(static_cast<this_type THIS_QUALIFIERS>(*this)); \
    }   

    HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(HAD_VALUE_)

#undef HAD_VALUE_

    HAD_NODISCARD HAD_CONSTEXPR14 value_type* operator->() noexcept { return HAD_NS addressof(**this);}
    HAD_NODISCARD HAD_CONSTEXPR14 const value_type* operator->() const noexcept { return HAD_NS addressof(**this);}
    HAD_NODISCARD HAD_CONSTEXPR14 volatile value_type* operator->() volatile noexcept { return HAD_NS addressof(**this);}
    HAD_NODISCARD HAD_CONSTEXPR14 const volatile value_type* operator->() const volatile noexcept { return HAD_NS addressof(**this);}

    template <typename... Args>
    HAD_CONSTEXPR20 reference_type emplace(Args&&... args) 
        noexcept(is_nothrow_constructible<value_type, Args...>::value) {
        this->reset();
        this->smf_construct(::had::forward<Args>(args)...);
        return this->mValue;
    }

    template <typename ElemType, typename... Args,
        typename enable_if<
        is_constructible<value_type, initializer_list<ElemType>&, Args...>::value
        >::type = 0>
    HAD_CONSTEXPR20 reference_type emplace(initializer_list<ElemType> ilist, Args&&... args) noexcept(
        is_nothrow_constructible_v<value_type, initializer_list<ElemType>&, Args...>)  {
        
        this->reset();
        this->smf_construct(ilist, ::had::forward<Args>(args)...);
        return this->mValue;
    }

    HAD_CONSTEXPR14 void swap(optional& other) 
        noexcept(nothrow_move_construct && nothrow_swappable) {
        using ::had::swap;
        const bool has_val = this->mHasValue;
        if (has_val == other.has_value()) {
            if (has_val) {
                swap(**this, *other ); // intentional ADL
            }
        }
        else {
            optional& source = has_val ? *this : other;
            optional& target = has_val ? other : *this;
            target.smf_construct(HAD_NS as_rvalue(*source));
            source.reset();
        }
    }

    HAD_CONSTEXPR14 friend void swap(optional& a, optional& b) noexcept(noexcept(a.swap(b))) {
        a.swap(b);
    }
//        static_assert(is_convertible_v<const_reference_type, no_cv_type>,
//    "The const overload of optional<T>::value_or requires const T& to be convertible to remove_cv_t<T> "
 //       "(N4950 [optional.observe]/15 as modified by LWG-3424).");
 //       static_assert(is_convertible_v<U, value_type>,
 //           "optional<T>::value_or(U) requires U to be convertible to T (N4950 [optional.observe]/15).");

    /*
#define HAD_VALUE_OR_(CVREF)                                                                   \
    template<typename U>                                                                       \
    HAD_NODISCARD HAD_CONSTEXPR14 value_type value_or(U&& defaultValue) CVREF {                \
        if (this->has_value()) return static_cast<typename remove_cvref<U>::type>(**this);     \
        return static_cast<typename remove_cvref<U>::type>(HAD_NS forward<U>(defaultValue));   \
    }
    */


#define HAD_VALUE_OR_(CVREF)                                                                                         \
    template<typename U>                                                                                             \
    HAD_NODISCARD HAD_CONSTEXPR14 auto value_or(U&& defaultValue) CVREF                                              \
        -> typename remove_cv<value_type>::type {                                                                    \
        static_assert(is_convertible<                                                                                \
            typename remove_rvalue_reference<value_type CVREF>::type,                                                \
            typename remove_cv<U>::type                                                                              \
        >::value,                                                                                                    \
            "The " #CVREF " overload of optional<T>::value_or(U&&) requires "                                                  \
            "T" #CVREF "to be convertible to remove_cv_t<T> "                                                        \
            "(N4950 [optional.observe]/15 as modified by LWG-3424).");                                               \
        static_assert(is_convertible<U, value_type>::value,                                                          \
            "optional<T>::value_or(U) requires U to be convertible to T (N4950 [optional.observe]/15).");            \
        if (this->has_value()) {                                                                                     \
            return static_cast<typename remove_rvalue_reference<value_type CVREF>::type>(**this);                    \
        }                                                                                                            \
        return static_cast<typename remove_cv<value_type>::type>(HAD_NS forward<U>(defaultValue));                   \
    }

    HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(HAD_VALUE_OR_)    

#undef HAD_VALUE_OR_

    HAD_CONSTEXPR20 optional& operator=(nullopt_t) noexcept {
        this->reset();
        return *this;
    }


    template <typename U,typename enable_if<
        !is_same<this_type,typename remove_cvref<U>::type>::value
        && is_constructible<value_type,U>::value >::type = 0>
    HAD_CONSTEXPR20 optional& operator=(U&& other) noexcept(
           is_nothrow_assignable<value_type&, U>::value
        && is_nothrow_constructible<value_type, U>::value)  {
        if (other) {
            this->smf_assign(other);
        } else {
            this->reset();
        }

        return *this;
    }

    template <typename U,typename enable_if<is_same<value_type,U>::value >::type = 0>
    HAD_CONSTEXPR20 optional& operator=(const optional<U>& other) noexcept(
        is_nothrow_assignable<value_type&, const U&>::value 
        && is_nothrow_constructible<value_type, const U&>::value)  {
        if (other) {
            this->smf_assign(*other);
        } else {
            this->reset();
        }

        return *this;
    }

    template <typename U,typename enable_if<is_same<value_type,U>::value >::type = 0>
    HAD_CONSTEXPR20 optional& operator=(optional<U>&& other) noexcept(
        is_nothrow_assignable<value_type&, U>::value 
        && is_nothrow_constructible<value_type, U>::value)  {
        if (other) {
            this->smf_assign(*other);
        } else {
            this->reset();
        }

        return *this;
    }

#ifdef HAD_CPP17


#define HAD_OR_ELSE_(THIS_QUALIFIERS)                                                               \
    template<typename Fn>                                                                           \
    HAD_CONSTEXPR14 optional or_else(Fn&& fn) THIS_QUALIFIERS                                       \
        noexcept(noexcept(HAD_NS forward<Fn>(fn)()))                                                \
        {                                                                                           \
            return *this ? static_cast<optional THIS_QUALIFIERS>(*this) : HAD_NS forward<Fn>(fn)(); \
        }                                                    

    HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(HAD_OR_ELSE_)    

#undef HAD_OR_ELSE_

#define HAD_AND_THEN_(THIS_QUALIFIERS)                                                                             \
    template<typename Fn>                                                                                          \
    HAD_CONSTEXPR14 auto and_then(Fn&& fn) THIS_QUALIFIERS                                                         \
        noexcept(noexcept(HAD_NS invoke(fn,**this)))                                                               \
        -> decltype(HAD_NS invoke(fn,**this))                                                                      \
    {                                                                                                              \
        return details::optional::and_then(static_cast<this_type THIS_QUALIFIERS>(*this),HAD_NS forward<Fn>(fn));  \
    }

    HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(HAD_AND_THEN_)

#undef HAD_AND_THEN_

#define HAD_TRANSFORM_(THIS_QUALIFIERS)                                                                             \
    template<typename Fn>                                                                                           \
    HAD_CONSTEXPR14 auto transform(Fn&& fn) THIS_QUALIFIERS                                                         \
        noexcept(noexcept(HAD_NS invoke(fn,**this)))                                                                \
        -> optional<typename remove_cv<decltype(HAD_NS invoke(fn,**this))>::type>                                   \
    {                                                                                                               \
        return this_type::transformImpl(static_cast<this_type THIS_QUALIFIERS>(*this),HAD_NS forward<Fn>(fn));  \
    }

    HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(HAD_TRANSFORM_)    

#undef HAD_TRANSFORM_
   
private:
    template<typename Self,typename Fn>
    HAD_CONSTEXPR14 static auto transformImpl(Self&& self, Fn&& fn)
        noexcept(noexcept(HAD_NS invoke(fn, *self)))
        -> optional<remove_cv_t<decltype(HAD_NS invoke(fn, *self))>> {
        using U = HAD_NS remove_cv_t<decltype(HAD_NS invoke(fn, *self))>;
        if (self) {
            return optional<U>{HAD_NS invoke(fn, *self)};
        }
        return optional<U>{};
    }
#endif // HAD_CPP17 (cause C++17 made sequence evaluation of dot (.) operator guranteed)
       // so using the moniadic functions and_then, or_else and transform
       // can result in unexcpted output prior to C++17

};


template<typename T> struct is_optional : false_type {};
template<typename T> struct is_optional<optional<T>> : true_type {};
template<typename T> struct is_optional<const optional<T>> : true_type {};
template<typename T> struct is_optional<volatile optional<T>> : true_type {};
template<typename T> struct is_optional<const volatile optional<T>> : true_type {};




template<typename ValueType>
class optional<ValueType&> {
public:

    using this_type                    = optional;
    using value_type                   = ValueType;
    using pointer_type                 = value_type*;
    using reference_type               = value_type&;
    using rvalue_reference_type        = value_type&&;
    using const_rvalue_reference_type  = value_type&&;
    using const_pointer_type           = const value_type*;
    using const_reference_type         = const value_type&;

    constexpr optional() noexcept : mPointer(nullptr) {};
    constexpr optional(nullopt_t) noexcept : mPointer(nullptr) {};
    
    template<typename U,typename enable_if<!is_same<U,this_type>::value>::type = 0>
    optional(U& other) noexcept : mPointer(HAD_NS addressof(other)) {};

    HAD_CONSTEXPR14 optional(const optional&) noexcept = default;
    HAD_CONSTEXPR14 optional(optional&&) noexcept = default;
    HAD_CONSTEXPR14 optional& operator=(const optional&) noexcept = default;
    HAD_CONSTEXPR14 optional& operator=(optional&&) noexcept = default;
    ~optional() noexcept = default;

    HAD_CONSTEXPR14 void swap(this_type& other) noexcept {
        ::had::swap(mPointer, other.mPointer);
    }

    friend HAD_CONSTEXPR14 void swap(this_type& a, this_type& b) noexcept {
        return a.swap(b);
    }

    void reset() {
        mPointer = nullptr;
    }

    template<typename Arg>
    void emplace(Arg& arg) {
        mPointer = HAD_NS addressof(arg);
    }

    HAD_NODISCARD constexpr operator bool() const noexcept {
        return has_value();
    }
    HAD_NODISCARD constexpr operator bool() const volatile noexcept {
        return has_value();
    }

    HAD_NODISCARD constexpr bool has_value() const noexcept {
        return mPointer != nullptr;
    }

    HAD_NODISCARD constexpr bool has_value() const volatile noexcept {
        return mPointer != nullptr;
    }


#define HAD_OPERATOR_DEREFERENCE_(CVREF)                                          \
    HAD_NODISCARD HAD_CONSTEXPR14 value_type CVREF operator*() CVREF noexcept {   \                                                                                     \
        return static_cast<value_type CVREF>(*mPointer);                          \
    }

    HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(HAD_OPERATOR_DEREFERENCE_)    

#undef HAD_OPERATOR_DEREFERENCE_

    HAD_NODISCARD HAD_CONSTEXPR14 value_type* operator->() noexcept { return mPointer; }
    HAD_NODISCARD HAD_CONSTEXPR14 const value_type* operator->() const noexcept { return mPointer; }
    HAD_NODISCARD HAD_CONSTEXPR14 volatile value_type* operator->() volatile noexcept { return mPointer; }
    HAD_NODISCARD HAD_CONSTEXPR14 const volatile value_type* operator->() const volatile noexcept { return mPointer; }

#define HAD_VALUE_(THIS)                                          \
    HAD_NODISCARD HAD_CONSTEXPR14 value_type THIS value() THIS {  \
        return static_cast<value_type THIS>(*mPointer);           \
    }                                             

HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(HAD_VALUE_)    

#undef HAD_VALUE_

#ifdef HAD_CPP17


#define HAD_OR_ELSE_(THIS)                                                               \
    template<typename Fn>                                                                \
    HAD_CONSTEXPR14 optional or_else(Fn&& fn) THIS                                       \
        noexcept(noexcept(HAD_NS forward<Fn>(fn)()))                                     \
        {                                                                                \
            return *this ? static_cast<optional THIS>(*this) : HAD_NS forward<Fn>(fn)(); \
        }                                                    

    HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(HAD_OR_ELSE_)    

#undef HAD_OR_ELSE_

#define HAD_AND_THEN_(THIS)                                                                             \
    template<typename Fn>                                                                               \
    HAD_CONSTEXPR14 auto and_then(Fn&& fn) THIS                                                         \
        noexcept(noexcept(HAD_NS invoke(fn,**this)))                                                    \
        -> decltype(HAD_NS invoke(fn,**this))                                                           \
    {                                                                                                   \
        return details::optional::and_then(static_cast<this_type THIS>(*this),HAD_NS forward<Fn>(fn));  \
    }

    HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(HAD_AND_THEN_)

#undef HAD_AND_THEN_

#define HAD_TRANSFORM_(THIS_QUALIFIERS)                                                                             \
    template<typename Fn>                                                                                           \
    HAD_CONSTEXPR14 auto transform(Fn&& fn) THIS_QUALIFIERS                                                         \
        noexcept(noexcept(HAD_NS invoke(fn,**this)))                                                                \
        -> optional<typename remove_cv<decltype(HAD_NS invoke(fn,**this))>::type>                                   \
    {                                                                                                               \
        return this_type::transformImpl(static_cast<this_type THIS_QUALIFIERS>(*this),HAD_NS forward<Fn>(fn));  \
    }

    HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(HAD_TRANSFORM_)    

#undef HAD_TRANSFORM_

private:
    template<typename Self,typename Fn>
    HAD_CONSTEXPR14 static auto transformImpl(Self&& self, Fn&& fn)
        noexcept(noexcept(HAD_NS invoke(fn,*self)))
        -> optional<remove_cv_t<decltype(HAD_NS invoke(fn, *self))>>
    {
        using U = HAD_NS remove_cv_t<decltype(HAD_NS invoke(fn, *self))>;
        if (self) {
            return optional<U>{HAD_NS invoke(fn, *self)};
        }
        return optional<U>{};
    }
#endif // HAD_CPP17 (cause C++17 made sequence evaluation of dot (.) operator guranteed)
    // so using the moniadic functions and_then, or_else and transform prior to C++17
    // can result in unexpected output and behavoir.

private:
    pointer_type mPointer;
};



/*
template<>
class optional<bool> {
public:
    struct bit_proxy {
        using value_type = unsigned char;

        bit_proxy& operator=(bool Bool) noexcept {
            if (Bool) {
                set(0);
            }
            else {
                unset(0);
            }
            return *this;
        }

        constexpr operator bool() const noexcept {
            return get();
        }

        HAD_CONSTEXPR14 void set(unsigned int position) noexcept {
            *ptr |= (1 << position);
        };

        HAD_CONSTEXPR14 void unset(unsigned int position) noexcept {
            *ptr &= ~(1 << position);
        };

        HAD_NODISCARD constexpr bool get(unsigned int position = 0) const noexcept {
            return *ptr & 0x1 << position;
        };

        value_type* ptr;
    };

    using this_type = optional;
    using value_type = bool;
    using pointer_type = value_type*;
    using reference_type = value_type&;
    using rvalue_reference_type = value_type&&;
    using const_rvalue_reference_type = value_type&&;
    using const_pointer_type = const value_type*;
    using const_reference_type = const value_type&;
    // first bit is the boolean itself
    // second bit is whether it is added
    // size optimization

    constexpr optional() noexcept : mValue(0) {};
    constexpr optional(nullopt_t) noexcept : mValue(0) {};
    constexpr optional(bool Bool) noexcept : mValue(Bool + 0x02) {};
    constexpr optional(const optional&) noexcept = default;
    constexpr optional(optional&&) noexcept = default;
    constexpr optional& operator=(const optional&) noexcept = default;
    constexpr optional& operator=(optional&&) noexcept = default;
    HAD_CONSTEXPR20 ~optional() noexcept = default;

    void swap(this_type& that) noexcept {
        using ::had::swap;
        swap(mValue, that.mValue);
    }

    friend void swap(this_type& a, this_type& b) noexcept {
        using ::had::swap;
        swap(a.mValue, b.mValue);
    }

    void reset() {
        mValue = 0;
    }

    constexpr void emplace(bool arg) noexcept {
        mValue = arg + 0x02;
    }

    HAD_NODISCARD constexpr explicit operator bool() const noexcept {
        return has_value();
    }
    HAD_NODISCARD constexpr explicit operator bool() const volatile noexcept {
        return has_value();
    }

    HAD_NODISCARD constexpr bool has_value() const noexcept {
        return mValue & 0x02;
    }

    HAD_NODISCARD constexpr bool has_value() const volatile noexcept {
        return mValue & 0x02;
    }

    bit_proxy operator*() noexcept {
        return bit_proxy{ &mValue };
    }

    bool operator*() const noexcept {
        return mValue & 0x01;
    }

    bool operator*() const volatile noexcept {
        return mValue & 0x01;
    }

    bit_proxy value() {
        if (!has_value()) {
            throw bad_optional_access{ "optional<bool> threw a bad_optional_access" };
        }
        return **this;
    }

    bool value() const {
        if (!has_value()) {
            throw bad_optional_access{ "optional<bool> threw a bad_optional_access" };
        }
        return **this;
    }

    bool value() const volatile {
        if (!has_value()) {
            throw bad_optional_access{ "optional<bool> threw a bad_optional_access" };
        }
        return **this;
    }


    template<typename Fn>
    optional or_else(Fn&& fn) const {
        return *this ? *this : HAD_NS forward<Fn>(fn)();
    }

    template<typename Fn>
    optional or_else(Fn&& fn) const volatile {
        return *this ? *this : HAD_NS forward<Fn>(fn)();
    }

    template <typename Fn>
    HAD_CONSTEXPR14 auto and_then(Fn&& fn) const
        noexcept(noexcept(HAD_NS invoke(fn, **this)))
        -> decltype(HAD_NS invoke(fn, **this)) {
        using InvokeResultType = decltype(HAD_NS invoke(fn, **this));
        using noCvRef = typename remove_cvref<InvokeResultType>::type;
        if (*this) {
            return HAD_NS invoke(HAD_NS forward<Fn>(fn), **this);
        }
        return noCvRef{};
    }


    template <typename Fn>
    HAD_CONSTEXPR14 auto and_then(Fn&& fn) const volatile
        noexcept(noexcept(HAD_NS invoke(fn, **this)))
        -> decltype(HAD_NS invoke(fn, **this)) {
        using InvokeResultType = decltype(HAD_NS invoke(fn, **this));
        using noCvRef = typename remove_cvref<InvokeResultType>::type;
        if (*this) {
            return HAD_NS invoke(HAD_NS forward<Fn>(fn), **this);
        }
        return noCvRef{};
    }


    template<typename Fn>
    HAD_CONSTEXPR14 auto transform(Fn&& fn) const
        noexcept(noexcept(HAD_NS invoke(fn, **this)))
        -> optional<remove_cv_t<decltype(HAD_NS invoke(fn, **this))>> {
        using U = HAD_NS remove_cv_t<decltype(HAD_NS invoke(fn, **this))>;
        if (*this) {
            return optional<U>{HAD_NS invoke(fn, **this)};
        }
        return optional<U>{};
    }

    template<typename Fn>
    HAD_CONSTEXPR14 auto transform(Fn&& fn) const volatile
        noexcept(noexcept(HAD_NS invoke(fn, **this)))
        -> optional<remove_cv_t<decltype(HAD_NS invoke(fn, **this))>> {
        using U = HAD_NS remove_cv_t<decltype(HAD_NS invoke(fn, **this))>;
        if (*this) {
            return optional<U>{HAD_NS invoke(fn, **this)};
        }
        return optional<U>{};
    }

    HAD_NODISCARD HAD_CONSTEXPR14 bool value_or(bool default_value) const noexcept {
        if (has_value()) {
            return **this;
        }
        return default_value;
    }

    HAD_NODISCARD HAD_CONSTEXPR14 bool value_or(bool default_value) const volatile noexcept {
        if (has_value()) {
            return **this;
        }
        return default_value;
    }
private:
    unsigned char mValue;
};
*/


template<typename T,typename U>
HAD_NODISCARD HAD_CONSTEXPR14 bool operator==(const optional<T>& a, const optional<U>& b) noexcept(noexcept(*a == *b)) {
    if (!a && !b)
        return true;
    if (!a || !b)
        return false;

    return *a == *b;
}

template<typename T,typename U>
HAD_NODISCARD HAD_CONSTEXPR14 bool operator==(const optional<T>& a, const U& b) noexcept(noexcept(a == *b)) {
    if (!a)
        return false;
    return *a == b;
}

template<typename T,typename U>
HAD_NODISCARD HAD_CONSTEXPR14 bool operator==(const T& a, const optional<U>& b) noexcept(noexcept(a == *b)) {
    if (!b)
        return false;
    return a == *b;
}

template<typename T> HAD_NODISCARD HAD_CONSTEXPR14 bool operator==(nullopt_t, const optional<T>& b) noexcept { return !b.has_value(); }
template<typename T> HAD_NODISCARD HAD_CONSTEXPR14 bool operator==(const optional<T>& a,nullopt_t) noexcept { return !a.has_value(); }

#ifndef HAD_CPP20
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator!=(const T& a, const optional<U>& b) noexcept(noexcept(a==b)) { return !(a == b); }
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator!=(const optional<T>& a, const U& b) noexcept(noexcept(a==b)) { return !(a == b); }

template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator!=(const optional<T>& a, const optional<U>& b) noexcept(noexcept(a==b)) { return !(a == b); }
template<typename T> HAD_NODISCARD HAD_CONSTEXPR14 bool operator!=(nullopt_t, const optional<T>& b) noexcept { return b.has_value(); }
template<typename T> HAD_NODISCARD HAD_CONSTEXPR14 bool operator!=(const optional<T>& a,nullopt_t) noexcept { return a.has_value(); }
#endif // !HAD_CPP20

template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator< (const optional<T>& a,const optional<U>& b) noexcept(noexcept(*a < *b)) { 
    if (a.has_value() && b.has_value())
        return *a < *b;
    return a.has_value() < b.has_value();
}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator> (const optional<T>& a,const optional<U>& b)  noexcept(noexcept(*a < *b)) {return b < a;}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator<=(const optional<T>& a,const optional<U>& b) noexcept(noexcept(*a < *b)) {return !(a > b);}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator>=(const optional<T>& a,const optional<U>& b) noexcept(noexcept(*a < *b)) {return !(a < b);}


template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator< (const optional<T>& a,const U& b) noexcept(noexcept(*a < *b)) { 
    if (a.has_value())
        return *a < b;
    return false;
}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator> (const optional<T>& a,const U& b)  noexcept(noexcept(*a < *b)) {return b < a;}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator<=(const optional<T>& a,const U& b) noexcept(noexcept(*a < *b)) {return !(a > b);}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator>=(const optional<T>& a,const U& b) noexcept(noexcept(*a < *b)) {return !(a < b);}

template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator< (const T& a,const optional<U>& b) noexcept(noexcept(*a < *b)) { 
    if (a.has_value())
        return *a < b;
    return false;
}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator> (const T& a,const optional<U>& b)  noexcept(noexcept(*a < *b)) {return b < a;}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator<=(const T& a,const optional<U>& b) noexcept(noexcept(*a < *b)) {return !(a > b);}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator>=(const T& a,const optional<U>& b) noexcept(noexcept(*a < *b)) {return !(a < b);}


#ifndef HAD_DROP_VOLATILE_COMPARISONS

template<typename T,typename U>

HAD_NODISCARD HAD_CONSTEXPR14 bool operator==(const volatile optional<T>& a, const volatile optional<U>& b) noexcept(noexcept(*a == *b)) {
    if (!a && !b)
        return true;
    if (!a || !b)
        return false;

    return *a == *b;
}

template<typename T,typename U>
HAD_NODISCARD HAD_CONSTEXPR14 bool operator==( const volatile optional<T>& a,const volatile U& b) noexcept(noexcept(a == *b)) {
    if (!a)
        return false;
    return *a == b;
}

template<typename T,typename U>
HAD_NODISCARD HAD_CONSTEXPR14 bool operator==( const volatile T& a, const volatile optional<U>& b) noexcept(noexcept(a == *b)) {
    if (!b)
        return false;
    return a == *b;
}

template<typename T> HAD_NODISCARD HAD_CONSTEXPR14 bool operator==(nullopt_t, const volatile optional<T>& b) noexcept { return !b.has_value(); }
template<typename T> HAD_NODISCARD HAD_CONSTEXPR14 bool operator==(const volatile optional<T>& a,nullopt_t) noexcept { return !a.has_value(); }

#ifndef HAD_CPP20
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator!=(const volatile T& a, const volatile optional<U>& b) noexcept(noexcept(a==b)) { return !(a == b); }
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator!=(const volatile optional<T>& a, const volatile U& b) noexcept(noexcept(a==b)) { return !(a == b); }

template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator!=(const volatile optional<T>& a, const volatile optional<U>& b) noexcept(noexcept(a==b)) { return !(a == b); }
template<typename T> HAD_NODISCARD HAD_CONSTEXPR14 bool operator!=(nullopt_t, const volatile optional<T>& b) noexcept { return b.has_value(); }
template<typename T> HAD_NODISCARD HAD_CONSTEXPR14 bool operator!=(const volatile optional<T>& a,nullopt_t) noexcept { return a.has_value(); }
#endif // !HAD_CPP20

template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator< (const volatile optional<T>& a,const volatile optional<U>& b) noexcept(noexcept(*a < *b)) { 
    if (a.has_value() && b.has_value())
        return *a < *b;
    return a.has_value() < b.has_value();
}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator> (const volatile optional<T>& a,const volatile optional<U>& b)  noexcept(noexcept(*a < *b)) {return b < a;}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator<=(const volatile optional<T>& a,const volatile optional<U>& b) noexcept(noexcept(*a < *b)) {return !(a > b);}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator>=(const volatile optional<T>& a,const volatile optional<U>& b) noexcept(noexcept(*a < *b)) {return !(a < b);}


template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator< (const volatile optional<T>& a,const volatile U& b) noexcept(noexcept(*a < *b)) { 
    if (a.has_value())
        return *a < b;
    return false;
}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator> (const volatile optional<T>& a,const volatile U& b)  noexcept(noexcept(*a < *b)) {return b < a;}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator<=(const volatile optional<T>& a,const volatile U& b) noexcept(noexcept(*a < *b)) {return !(a > b);}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator>=(const volatile optional<T>& a,const volatile U& b) noexcept(noexcept(*a < *b)) {return !(a < b);}

template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator< (const volatile T& a,const volatile optional<U>& b) noexcept(noexcept(*a < *b)) { 
    if (a.has_value())
        return *a < b;
    return false;
}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator> (const volatile T& a,const volatile optional<U>& b)  noexcept(noexcept(*a < *b)) {return b < a;}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator<=(const volatile T& a,const volatile optional<U>& b) noexcept(noexcept(*a < *b)) {return !(a > b);}
template<typename T,typename U> HAD_NODISCARD HAD_CONSTEXPR14 bool operator>=(const volatile T& a,const volatile optional<U>& b) noexcept(noexcept(*a < *b)) {return !(a < b);}


#endif

#ifdef __cpp_deduction_guides

template<typename T>
optional(T) -> optional<T>;

#endif // __cpp_deduction_guides


template <typename T, 
    typename enable_if<
        is_constructible<typename decay<T>::type, T>::value
    >::type = 0>
HAD_NODISCARD HAD_CONSTEXPR17 optional<typename decay<T>::type> make_optional(T&& value) noexcept(
    noexcept(optional<typename decay<T>::type>{HAD_NS forward<T>(value)})) /* strengthened */ {
    return optional<decay_t<T>>{HAD_NS forward<T>(value)};
}
template <typename T, typename... Args, enable_if_t<is_constructible_v<T, Args...>> = 0>
HAD_NODISCARD HAD_CONSTEXPR17 optional<T> make_optional(Args&&... args) noexcept(
    noexcept(optional<T>{in_place_t{}, HAD_NS forward<Args>(args)...})) /* strengthened */ {
    return optional<T>{in_place_t{}, HAD_NS forward<Args>(args)...};
}
template <typename T, typename IlistType, typename... Args,
    enable_if_t<is_constructible_v<T, initializer_list<IlistType>&, Args...>> = 0>
    HAD_NODISCARD HAD_CONSTEXPR17 optional<T> make_optional(initializer_list<IlistType> IList, Args&&... args) noexcept(
    noexcept(optional<T>{in_place_t{}, IList, HAD_NS forward<Args>(args)...})) /* strengthened */ {
    return optional<T>{in_place_t{}, IList, HAD_NS forward<Args>(args)...};
}



HAD_NAMESPACE_END

#endif // !HAD_OPTIONAL_HPP