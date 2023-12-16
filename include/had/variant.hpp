#pragma once
#ifndef HAD_VARIANT_HPP
#define HAD_VARIANT_HPP

#include <had/type_traits/is_same.hpp>
#include <had/type_traits/index_of.hpp>
#include <had/type_traits/biggest_sizeof.hpp>
#include <had/type_traits/make_integer_sequence.hpp>
#include <had/utility/destroy_at.hpp>
#include <had/exceptions.hpp>
#include <had/type_traits/type_identity.hpp>
#include <had/type_traits/is_constructible.hpp>
#include <had/type_traits/conjunction.hpp>
#include <had/type_traits/is_default_constructible.hpp>
#include <typeinfo>
#include <had/internal/SMF_control.hpp>
#include <had/utility/unreachable.hpp>
#include <limits>
HAD_NAMESPACE_BEGIN

namespace details {
namespace variant {

template<typename Arg,typename T,typename... Ts>
struct get_constructible_type : HAD_NS conditional_t<
    HAD_NS is_constructible<T,Arg>::value,
    HAD_NS type_identity<T>,
    get_constructible_type<Arg,Ts...> > {
};

template<typename Arg,typename T>
struct get_constructible_type<Arg,T> : HAD_NS conditional_t<
    HAD_NS is_constructible<T,Arg>::value,
    HAD_NS type_identity<T>,
    //pack<HAD_NS type_identity<U>,HAD_NS integral_constant<HAD_NS size_t,I> >,
    HAD_NS type_identity<void> > {
};



// 
template <bool AllTriviallyDestructible,typename... Ts>
struct destruct_base { // trivially destructible
    using this_type = destruct_base;
    using biggest_type = typename HAD_NS biggest_sizeof<Ts...>::type;
    using first_type   = typename HAD_NS extract<0, Ts...>::type;
    using index_type = unsigned char;
    using size_type = size_t;
    constexpr static size_t max_size() noexcept {
        return sizeof(biggest_type);
    }
    constexpr static size_t types_count() noexcept {
        return sizeof...(Ts);
    }
    alignas(biggest_type) unsigned char mStorage[max_size()];
    index_type mIndex;
    
    destruct_base() noexcept(HAD_NS is_nothrow_default_constructible<first_type>::value) 
        : mIndex(0) { 
        ::new (mStorage) first_type{};
    } 

    size_type index() const noexcept { return mIndex; }

    void reset() noexcept {
        mIndex = UCHAR_MAX;
    }
    void constexpr inline static destroy() noexcept {}
};

template <typename... Ts>
struct destruct_base<false,Ts...> {
    using this_type = destruct_base;
    using biggest_type = typename HAD_NS biggest_sizeof<Ts...>::type;
    using first_type   = typename HAD_NS extract<0, Ts...>::type;
    using size_type = size_t;
    using index_type = unsigned char;
    constexpr static unsigned char InvalidIndex = UCHAR_MAX;

    HAD_NODISCARD constexpr static size_type max_size() noexcept {
        return sizeof(biggest_type);
    }
    HAD_NODISCARD constexpr static size_type types_count() noexcept {
        return sizeof...(Ts);
    }
    
    HAD_NODISCARD constexpr bool valueless() noexcept {
        return mIndex == InvalidIndex;
    }

    alignas(biggest_type) unsigned char mStorage[max_size()];
    index_type mIndex;
    size_type index() const noexcept { return mIndex; }

    template<typename U>
    static void XDestructor(this_type& self) noexcept {
        HAD_NS destroy_at(reinterpret_cast<U*>(self.mStorage));
    }
    using destructorFunctionPtr = void(*)(this_type&);
    
    constexpr static destructorFunctionPtr mDestructorFunctions[] = {
        &XDestructor<Ts>...
    };

    void inline destroy() noexcept {
        mDestructorFunctions[this->mIndex](*this);
    }

    // non-trivial destructor
    ~destruct_base() noexcept {
        if (!valueless()) {
            this->destroy();
        }
    }

    destruct_base() noexcept(HAD_NS is_nothrow_default_constructible<first_type>::value) 
        : mIndex(0) { 
        ::new (mStorage) first_type{};
    } 

    destruct_base(const destruct_base&)            = default;
    destruct_base(destruct_base&&)                 = default;
    destruct_base& operator=(const destruct_base&) = default;
    destruct_base& operator=(destruct_base&&)      = default;

    void reset() noexcept {
        if (!valueless()) {
            destroy();
            mIndex = InvalidIndex;
        }
    }
};

template <typename... Ts>
struct construct_base : destruct_base<conjunction<is_trivially_destructible<Ts>...>::value
    ,Ts...> {

    using Base = destruct_base<conjunction<is_trivially_destructible<Ts>...>::value
        , Ts...>;

    using Base::Base;
    using Base::reset;
    using Base::max_size;
    using Base::types_count;
    using Base::index;

    using this_type = construct_base;
    using moveAssigmentPtr   = this_type&(*)(this_type&,this_type&);
    using copyAssigmentPtr   = this_type&(*)(this_type&,const this_type&);
    using moveConstructorPtr = void*(*)(this_type&,this_type&);
    using copyConstructorPtr = void*(*)(this_type&,const this_type&);

    template<typename U>
    static this_type& XMoveAssign(this_type& self, this_type& that) {
        reinterpret_cast<U&>(self.mStorage) = HAD_NS move(reinterpret_cast<U&>(that.mStorage));
        return self;
    }
    template<typename U>
    static this_type& XCopyAssign(this_type& self, const this_type& that) {
        reinterpret_cast<U&>(self.mStorage) = reinterpret_cast<const U&>(that.mStorage);
        return self;
    }
    
    template<typename U>
    static void* XMoveConstructor(this_type& self, this_type& that) {
        return ::new (self.mStorage) U(HAD_NS move(reinterpret_cast<U&>(that.mStorage)));
    }
    template<typename U>
    static void* XCopyConstructor(this_type& self, const this_type& that) {
        return ::new (self.mStorage) U(reinterpret_cast<const U&>(that.mStorage));
    }

    constexpr static moveAssigmentPtr mMoveAssignmentFunctions[] = {
        &XMoveAssign<Ts>...
    };
    constexpr static copyAssigmentPtr mCopyAssignmentFunctions[] = {
        &XCopyAssign<Ts>...
    };

    constexpr static moveConstructorPtr mMoveConstructorFunctions[] = {
        &XMoveConstructor<Ts>...
    };

    constexpr static copyConstructorPtr mCopyConstructorFunctions[] = {
        &XCopyConstructor<Ts>...
    };
    
    template <size_t Index,typename... Args>
    void* smf_construct(Args&&... args) {
        using T = typename extract<Index, Ts... >::type;
        this->mIndex = Index;
        return ::new (this->mStorage) T(HAD_NS forward<Args>(args)...);
    }

    void smf_assign(const this_type& that) {
        if (this->mIndex == that.mIndex) {
            return mCopyAssignmentFunctions[this->mIndex](*this, that);
        }
        else {
            this->destroy();
            this->mIndex = that.mIndex;
            mCopyConstructorFunctions[this->mIndex](*this, that);
        }
    }

    this_type& smf_assign(this_type&& that) {
        if (this->mIndex == that.mIndex) {
            return mMoveAssignmentFunctions[this->mIndex](*this, that);
        }
        else {
            this->destroy();
            this->mIndex = that.mIndex;
            return mMoveConstructorFunctions[this->mIndex](*this, that);
        }
    }

    void* smf_construct_from(const this_type& that) {
        this->mIndex = that.mIndex;
        return this->mCopyConstructorFunctions[this->mIndex](*this, that);
    }

    void* smf_construct_from(this_type&& that) {
        this->mIndex = that.mIndex;
        return this->mMoveConstructorFunctions[this->mIndex](*this, that);
    }

    template <typename Self>
    this_type& smf_assign_from(Self&& that) {
        return smf_assign(HAD_NS forward<Self>(that));
    }

};



}
}



template<typename... Ts>
class variant : private SMF_control<details::variant::construct_base<Ts...>,Ts...> {
    static_assert(sizeof...(Ts) > 1,"Sorry had::variant must contain atleast 2 types");
    static_assert(sizeof...(Ts) < 256,"Sorry had::variant cannot accept more than 256 types");
public:
    using Base = SMF_control<details::variant::construct_base<Ts...>, Ts...>;
    using this_type = variant;
    //using first_type = typename HAD_NS extract<0, Ts...>::type;
    //using size_type = size_t;
    //using index_type = unsigned char;
    using biggest_type = typename biggest_sizeof<Ts...>::type;

    constexpr static bool is_all_nothrow_copy_constructible = HAD_NS conjunction<
        is_nothrow_copy_constructible<Ts>...
    >::value;

    constexpr static bool is_all_nothrow_move_constructible = HAD_NS conjunction<
        is_nothrow_move_constructible<Ts>...
    >::value;

    using Base::reset;
    using Base::types_count;
    using Base::max_size;
    using Base::first_type;
    using Base::index_type;
    using Base::size_type;
    using Base::index;
private:
    using Base::mIndex;
    using Base::mStorage;
    using Base::smf_construct;
    using Base::smf_assign;
    using Base::smf_construct_from;
    using Base::smf_assign_from;
    using Base::destroy;
    using Base::mCopyAssignmentFunctions;
    using Base::mMoveAssignmentFunctions;
    using Base::mCopyConstructorFunctions;
    using Base::mMoveConstructorFunctions;

public:

    constexpr variant() noexcept {}
    template<typename U,
        typename enable_if <
        !is_same<remove_cvref_t<U>, this_type>::value
    >::type = 0 >
    variant(U&& u)
        noexcept(is_nothrow_constructible<
        typename details::variant::get_constructible_type<U, Ts...>::type
        ,U>::value) {

        using constructible_t = typename details::variant::get_constructible_type<U, Ts...>::type;
        
        static_assert(!is_void<constructible_t>::value, "had::variant<Types...>::variant(U&&). one of Types must be constructible with U");
        
        constexpr size_t Index = 
            (index_of<U, Ts...>::value != -1)
            ? index_of<U, Ts...>::value
            : index_of<constructible_t, Ts...>::value;
        using ChosenConstructor = typename extract<Index, Ts...>::type;
        //this->template smf_construct<Index>(HAD_NS forward<U>(u));
        ::new (this->mStorage) ChosenConstructor(HAD_NS forward<U>(u));
        this->mIndex = Index;
    }


    template<typename U,
        typename enable_if<!is_same<remove_cvref_t<U>, this_type>::value>::type = 0 >
    variant& operator=(U&& u) {
        using constructible_t = typename details::variant::get_constructible_type<U, Ts...>::type;
        
        static_assert(!is_void<constructible_t>::value, "had::variant<Types...>::operator=(U&&). one of Types must be constructible with U");
        
        this->destroy();
        this->mIndex = (index_of<U, Ts...>::value != -1)
            ? index_of<U, Ts...>::value
            : index_of<constructible_t, Ts...>::value;

        using ChosenConstructor = typename conditional<
            (index_of<U, Ts...>::value != -1),
            U,
            constructible_t
        >::type;

        ::new (&this->mStorage) ChosenConstructor(HAD_NS forward<U>(u));
        return *this;
    }

    void* x_get_void_1_401928() noexcept { return static_cast<void*>(&mStorage);}
    const void* x_get_void_1_401928() const noexcept {return static_cast<const void*>(&mStorage);}
    volatile void* x_get_void_1_401928() volatile noexcept {return static_cast<volatile void*>(&mStorage);}
    const volatile void* x_get_void_1_401928() const volatile noexcept {return static_cast<const volatile void*>(&mStorage);}

    void swap(variant& that) noexcept(is_all_nothrow_move_constructible) {
        variant copy = HAD_NS move(that);
        that.destroy();
        this->mMoveConstructorFunctions[this->index()](that, *this);
        this->destroy();
        this->mMoveConstructorFunctions[that.index()](*this, copy);
        HAD_NS swap(this->mIndex, that.mIndex);
    }

    friend void swap(variant& a, variant& b) noexcept(noexcept(a.swap(b))) {
        a.swap(b);
    }


    template<size_t Index,typename... Args>
    auto emplace(Args&&... args)
        noexcept(is_nothrow_constructible<typename extract<Index, Ts...>::type>::value)
        -> typename extract<Index, Ts...>::type& {

        static_assert(Index < types_count(), "had::variant::emplace<Index>(Args...). Index out of range.");
        this->destroy();
        mIndex = Index;
        using T = typename extract<Index, Ts...>::type;
        return *static_cast<T*>(::new (this->mStorage) T(HAD_NS forward<Args>(args)...));
    }

    template<typename T,typename... Args>
    T& emplace(Args&&... args) noexcept(is_nothrow_constructible<T,Args...>::value) {

        static_assert(index_of<T, Ts...>::value != -1, "had::variant<Types...>::emplace<T>(Args...). T must be a type in the variant's Types");
        this->destroy();
        this->mIndex = index_of<T, Ts...>::value;
        return *static_cast<T*>(::new (this->mStorage) T(HAD_NS forward<Args>(args)...));
    }


};

namespace details {
namespace variant {
template<size_t I>
struct visit_switch_case {
    static_assert(I != -1, "invalid case");
};

template<>
struct visit_switch_case<0> {

    template<typename... Ts,typename Callable,typename Ptr>
    static auto run(Callable&& callable, Ptr p, unsigned char index)
        -> decltype(callable(HAD_NS declval<typename extract<0, Ts...>::type>())) {
        

        constexpr auto count = sizeof...(Ts);
        switch (index) {
            case 0:  return callable(*static_cast<typename HAD_NS extract<0 < count ? 0 : 0, Ts... > ::type*>(p));
            case 1:  return callable(*static_cast<typename HAD_NS extract<1 < count ? 1 : 0, Ts... > ::type*>(p));
            case 2:  return callable(*static_cast<typename HAD_NS extract<2 < count ? 2 : 0, Ts... > ::type*>(p));
            case 3:  return callable(*static_cast<typename HAD_NS extract<3 < count ? 3 : 0, Ts... > ::type*>(p));
        }
        HAD_NS unreachable();
    }

};

template<>
struct visit_switch_case<1> {

    template<typename... Ts,typename Callable,typename Ptr>
    static auto run(Callable&& callable, Ptr p, unsigned char index)
        -> decltype(callable(HAD_NS declval<typename extract<0, Ts...>::type>())) {


        constexpr auto count = sizeof...(Ts);
        switch (index) {
            case 0:  return callable(*static_cast<typename HAD_NS extract<0 < count ? 0 : 0, Ts... > ::type*>(p));
            case 1:  return callable(*static_cast<typename HAD_NS extract<1 < count ? 1 : 0, Ts... > ::type*>(p));
            case 2:  return callable(*static_cast<typename HAD_NS extract<2 < count ? 2 : 0, Ts... > ::type*>(p));
            case 3:  return callable(*static_cast<typename HAD_NS extract<3 < count ? 3 : 0, Ts... > ::type*>(p));
            case 4:  return callable(*static_cast<typename HAD_NS extract<4 < count ? 4 : 0, Ts... > ::type*>(p));
            case 5:  return callable(*static_cast<typename HAD_NS extract<5 < count ? 5 : 0, Ts... > ::type*>(p));
            case 6:  return callable(*static_cast<typename HAD_NS extract<6 < count ? 6 : 0, Ts... > ::type*>(p));
            case 7:  return callable(*static_cast<typename HAD_NS extract<7 < count ? 7 : 0, Ts... > ::type*>(p));
        }
        HAD_NS unreachable();
    }

};

template<>
struct visit_switch_case<2> {

    template<typename... Ts,typename Callable,typename Ptr>
    static auto run(Callable&& callable, Ptr p, unsigned char index)
        -> decltype(callable(HAD_NS declval<typename extract<0, Ts...>::type>())) {

        constexpr auto count = sizeof...(Ts);
        switch (index) {
            case 0:  return callable(*static_cast<typename HAD_NS extract<0 < count ? 0 : 0, Ts... > ::type*>(p));
            case 1:  return callable(*static_cast<typename HAD_NS extract<1 < count ? 1 : 0, Ts... > ::type*>(p));
            case 2:  return callable(*static_cast<typename HAD_NS extract<2 < count ? 2 : 0, Ts... > ::type*>(p));
            case 3:  return callable(*static_cast<typename HAD_NS extract<3 < count ? 3 : 0, Ts... > ::type*>(p));
            case 4:  return callable(*static_cast<typename HAD_NS extract<4 < count ? 4 : 0, Ts... > ::type*>(p));
            case 5:  return callable(*static_cast<typename HAD_NS extract<5 < count ? 5 : 0, Ts... > ::type*>(p));
            case 6:  return callable(*static_cast<typename HAD_NS extract<6 < count ? 6 : 0, Ts... > ::type*>(p));
            case 7:  return callable(*static_cast<typename HAD_NS extract<7 < count ? 7 : 0, Ts... > ::type*>(p));
            case 8:  return callable(*static_cast<typename HAD_NS extract<8 < count ? 8 : 0, Ts... > ::type*>(p));
            case 9:  return callable(*static_cast<typename HAD_NS extract<9 < count ? 9 : 0, Ts... > ::type*>(p));
            case 10: return callable(*static_cast<typename HAD_NS extract<10 < count ? 10 : 0, Ts... > ::type*>(p));
            case 11: return callable(*static_cast<typename HAD_NS extract<11 < count ? 11 : 0, Ts... > ::type*>(p));
            case 12: return callable(*static_cast<typename HAD_NS extract<12 < count ? 12 : 0, Ts... > ::type*>(p));
            case 13: return callable(*static_cast<typename HAD_NS extract<13 < count ? 13 : 0, Ts... > ::type*>(p));
            case 14: return callable(*static_cast<typename HAD_NS extract<14 < count ? 14 : 0, Ts... > ::type*>(p));
            case 15: return callable(*static_cast<typename HAD_NS extract<15 < count ? 15 : 0, Ts... > ::type*>(p));
        }
        HAD_NS unreachable();
    }

};


template<>
struct visit_switch_case<3> {
    template<typename... Ts,typename Callable,typename Ptr>
    static auto run(Callable&& callable, Ptr p, unsigned char index)
        -> decltype(callable(HAD_NS declval<typename extract<0, Ts...>::type>())) {

        constexpr auto count = sizeof...(Ts);
        switch (index) {
            case 0:  return callable(*static_cast<typename HAD_NS extract<0 < count ? 0 : 0, Ts... > ::type*>(p));
            case 1:  return callable(*static_cast<typename HAD_NS extract<1 < count ? 1 : 0, Ts... > ::type*>(p));
            case 2:  return callable(*static_cast<typename HAD_NS extract<2 < count ? 2 : 0, Ts... > ::type*>(p));
            case 3:  return callable(*static_cast<typename HAD_NS extract<3 < count ? 3 : 0, Ts... > ::type*>(p));
            case 4:  return callable(*static_cast<typename HAD_NS extract<4 < count ? 4 : 0, Ts... > ::type*>(p));
            case 5:  return callable(*static_cast<typename HAD_NS extract<5 < count ? 5 : 0, Ts... > ::type*>(p));
            case 6:  return callable(*static_cast<typename HAD_NS extract<6 < count ? 6 : 0, Ts... > ::type*>(p));
            case 7:  return callable(*static_cast<typename HAD_NS extract<7 < count ? 7 : 0, Ts... > ::type*>(p));
            case 8:  return callable(*static_cast<typename HAD_NS extract<8 < count ? 8 : 0, Ts... > ::type*>(p));
            case 9:  return callable(*static_cast<typename HAD_NS extract<9 < count ? 9 : 0, Ts... > ::type*>(p));
            case 10: return callable(*static_cast<typename HAD_NS extract<10 < count ? 10 : 0, Ts... > ::type*>(p));
            case 11: return callable(*static_cast<typename HAD_NS extract<11 < count ? 11 : 0, Ts... > ::type*>(p));
            case 12: return callable(*static_cast<typename HAD_NS extract<12 < count ? 12 : 0, Ts... > ::type*>(p));
            case 13: return callable(*static_cast<typename HAD_NS extract<13 < count ? 13 : 0, Ts... > ::type*>(p));
            case 14: return callable(*static_cast<typename HAD_NS extract<14 < count ? 14 : 0, Ts... > ::type*>(p));
            case 15: return callable(*static_cast<typename HAD_NS extract<15 < count ? 15 : 0, Ts... > ::type*>(p));
            case 16: return callable(*static_cast<typename HAD_NS extract<16 < count ? 16 : 0, Ts... > ::type*>(p));
            case 17: return callable(*static_cast<typename HAD_NS extract<17 < count ? 17 : 0, Ts... > ::type*>(p));
            case 18: return callable(*static_cast<typename HAD_NS extract<18 < count ? 18 : 0, Ts... > ::type*>(p));
            case 19: return callable(*static_cast<typename HAD_NS extract<19 < count ? 19 : 0, Ts... > ::type*>(p));
            case 20: return callable(*static_cast<typename HAD_NS extract<20 < count ? 20 : 0, Ts... > ::type*>(p));
            case 21: return callable(*static_cast<typename HAD_NS extract<21 < count ? 21 : 0, Ts... > ::type*>(p));
            case 22: return callable(*static_cast<typename HAD_NS extract<22 < count ? 22 : 0, Ts... > ::type*>(p));
            case 23: return callable(*static_cast<typename HAD_NS extract<23 < count ? 23 : 0, Ts... > ::type*>(p));
            case 24: return callable(*static_cast<typename HAD_NS extract<24 < count ? 24 : 0, Ts... > ::type*>(p));
            case 25: return callable(*static_cast<typename HAD_NS extract<25 < count ? 25 : 0, Ts... > ::type*>(p));
            case 26: return callable(*static_cast<typename HAD_NS extract<26 < count ? 26 : 0, Ts... > ::type*>(p));
            case 27: return callable(*static_cast<typename HAD_NS extract<27 < count ? 27 : 0, Ts... > ::type*>(p));
            case 28: return callable(*static_cast<typename HAD_NS extract<28 < count ? 28 : 0, Ts... > ::type*>(p));
            case 29: return callable(*static_cast<typename HAD_NS extract<29 < count ? 29 : 0, Ts... > ::type*>(p));
            case 30: return callable(*static_cast<typename HAD_NS extract<30 < count ? 30 : 0, Ts... > ::type*>(p));
            case 31: return callable(*static_cast<typename HAD_NS extract<31 < count ? 31 : 0, Ts... > ::type*>(p));
        }
        HAD_NS unreachable();
    }

};


template<>
struct visit_switch_case<4> {
    template<typename... Ts,typename Callable,typename Ptr>
    static auto run(Callable&& callable, Ptr p, unsigned char index)
        -> decltype(callable(HAD_NS declval<typename extract<0, Ts...>::type>())) {

        constexpr auto count = sizeof...(Ts);
        switch (index) {
            case 0:  return callable(*static_cast<typename HAD_NS extract<0 < count ? 0 : 0, Ts...>::type*>(p));
            case 1:  return callable(*static_cast<typename HAD_NS extract<1 < count ? 1 : 0 ,Ts...>::type*>(p));
            case 2:  return callable(*static_cast<typename HAD_NS extract<2 < count ? 2 : 0 ,Ts...>::type*>(p));
            case 3:  return callable(*static_cast<typename HAD_NS extract<3 < count ? 3 : 0 ,Ts...>::type*>(p));
            case 4:  return callable(*static_cast<typename HAD_NS extract<4 < count ? 4 : 0 ,Ts...>::type*>(p));
            case 5:  return callable(*static_cast<typename HAD_NS extract<5 < count ? 5 : 0 ,Ts...>::type*>(p));
            case 6:  return callable(*static_cast<typename HAD_NS extract<6 < count ? 6 : 0 ,Ts...>::type*>(p));
            case 7:  return callable(*static_cast<typename HAD_NS extract<7 < count ? 7 : 0 ,Ts...>::type*>(p));
            case 8:  return callable(*static_cast<typename HAD_NS extract<8 < count ? 8 : 0 ,Ts...>::type*>(p));
            case 9:  return callable(*static_cast<typename HAD_NS extract<9 < count ? 9 : 0 ,Ts...>::type*>(p));
            case 10: return callable(*static_cast<typename HAD_NS extract<10 < count ? 10 : 0 ,Ts...>::type*>(p));
            case 11: return callable(*static_cast<typename HAD_NS extract<11 < count ? 11 : 0 ,Ts...>::type*>(p));
            case 12: return callable(*static_cast<typename HAD_NS extract<12 < count ? 12 : 0 ,Ts...>::type*>(p));
            case 13: return callable(*static_cast<typename HAD_NS extract<13 < count ? 13 : 0 ,Ts...>::type*>(p));
            case 14: return callable(*static_cast<typename HAD_NS extract<14 < count ? 14 : 0 ,Ts...>::type*>(p));
            case 15: return callable(*static_cast<typename HAD_NS extract<15 < count ? 15 : 0 ,Ts...>::type*>(p));
            case 16: return callable(*static_cast<typename HAD_NS extract<16 < count ? 16 : 0 ,Ts...>::type*>(p));
            case 17: return callable(*static_cast<typename HAD_NS extract<17 < count ? 17 : 0 ,Ts...>::type*>(p));
            case 18: return callable(*static_cast<typename HAD_NS extract<18 < count ? 18 : 0 ,Ts...>::type*>(p));
            case 19: return callable(*static_cast<typename HAD_NS extract<19 < count ? 19 : 0 ,Ts...>::type*>(p));
            case 20: return callable(*static_cast<typename HAD_NS extract<20 < count ? 20 : 0 ,Ts...>::type*>(p));
            case 21: return callable(*static_cast<typename HAD_NS extract<21 < count ? 21 : 0 ,Ts...>::type*>(p));
            case 22: return callable(*static_cast<typename HAD_NS extract<22 < count ? 22 : 0 ,Ts...>::type*>(p));
            case 23: return callable(*static_cast<typename HAD_NS extract<23 < count ? 23 : 0 ,Ts...>::type*>(p));
            case 24: return callable(*static_cast<typename HAD_NS extract<24 < count ? 24 : 0 ,Ts...>::type*>(p));
            case 25: return callable(*static_cast<typename HAD_NS extract<25 < count ? 25 : 0 ,Ts...>::type*>(p));
            case 26: return callable(*static_cast<typename HAD_NS extract<26 < count ? 26 : 0 ,Ts...>::type*>(p));
            case 27: return callable(*static_cast<typename HAD_NS extract<27 < count ? 27 : 0 ,Ts...>::type*>(p));
            case 28: return callable(*static_cast<typename HAD_NS extract<28 < count ? 28 : 0 ,Ts...>::type*>(p));
            case 29: return callable(*static_cast<typename HAD_NS extract<29 < count ? 29 : 0 ,Ts...>::type*>(p));
            case 30: return callable(*static_cast<typename HAD_NS extract<30 < count ? 30 : 0 ,Ts...>::type*>(p));
            case 31: return callable(*static_cast<typename HAD_NS extract<31 < count ? 31 : 0 ,Ts...>::type*>(p));
            case 32: return callable(*static_cast<typename HAD_NS extract<32 < count ? 32 : 0 ,Ts...>::type*>(p));
            case 33: return callable(*static_cast<typename HAD_NS extract<33 < count ? 33 : 0 ,Ts...>::type*>(p));
            case 34: return callable(*static_cast<typename HAD_NS extract<34 < count ? 34 : 0 ,Ts...>::type*>(p));
            case 35: return callable(*static_cast<typename HAD_NS extract<35 < count ? 35 : 0 ,Ts...>::type*>(p));
            case 36: return callable(*static_cast<typename HAD_NS extract<36 < count ? 36 : 0 ,Ts...>::type*>(p));
            case 37: return callable(*static_cast<typename HAD_NS extract<37 < count ? 37 : 0 ,Ts...>::type*>(p));
            case 38: return callable(*static_cast<typename HAD_NS extract<38 < count ? 38 : 0 ,Ts...>::type*>(p));
            case 39: return callable(*static_cast<typename HAD_NS extract<39 < count ? 39 : 0 ,Ts...>::type*>(p));
            case 40: return callable(*static_cast<typename HAD_NS extract<40 < count ? 40 : 0 ,Ts...>::type*>(p));
            case 41: return callable(*static_cast<typename HAD_NS extract<41 < count ? 41 : 0 ,Ts...>::type*>(p));
            case 42: return callable(*static_cast<typename HAD_NS extract<42 < count ? 42 : 0 ,Ts...>::type*>(p));
            case 43: return callable(*static_cast<typename HAD_NS extract<43 < count ? 43 : 0 ,Ts...>::type*>(p));
            case 44: return callable(*static_cast<typename HAD_NS extract<44 < count ? 44 : 0 ,Ts...>::type*>(p));
            case 45: return callable(*static_cast<typename HAD_NS extract<45 < count ? 45 : 0 ,Ts...>::type*>(p));
            case 46: return callable(*static_cast<typename HAD_NS extract<46 < count ? 46 : 0 ,Ts...>::type*>(p));
            case 47: return callable(*static_cast<typename HAD_NS extract<47 < count ? 47 : 0 ,Ts...>::type*>(p));
            case 48: return callable(*static_cast<typename HAD_NS extract<48 < count ? 48 : 0 ,Ts...>::type*>(p));
            case 49: return callable(*static_cast<typename HAD_NS extract<49 < count ? 49 : 0 ,Ts...>::type*>(p));
            case 50: return callable(*static_cast<typename HAD_NS extract<50 < count ? 50 : 0 ,Ts...>::type*>(p));
            case 51: return callable(*static_cast<typename HAD_NS extract<51 < count ? 51 : 0 ,Ts...>::type*>(p));
            case 52: return callable(*static_cast<typename HAD_NS extract<52 < count ? 52 : 0 ,Ts...>::type*>(p));
            case 53: return callable(*static_cast<typename HAD_NS extract<53 < count ? 53 : 0 ,Ts...>::type*>(p));
            case 54: return callable(*static_cast<typename HAD_NS extract<54 < count ? 54 : 0 ,Ts...>::type*>(p));
            case 55: return callable(*static_cast<typename HAD_NS extract<55 < count ? 55 : 0 ,Ts...>::type*>(p));
            case 56: return callable(*static_cast<typename HAD_NS extract<56 < count ? 56 : 0 ,Ts...>::type*>(p));
            case 57: return callable(*static_cast<typename HAD_NS extract<57 < count ? 57 : 0 ,Ts...>::type*>(p));
            case 58: return callable(*static_cast<typename HAD_NS extract<58 < count ? 58 : 0 ,Ts...>::type*>(p));
            case 59: return callable(*static_cast<typename HAD_NS extract<59 < count ? 59 : 0 ,Ts...>::type*>(p));
            case 60: return callable(*static_cast<typename HAD_NS extract<60 < count ? 60 : 0 ,Ts...>::type*>(p));
            case 61: return callable(*static_cast<typename HAD_NS extract<61 < count ? 61 : 0 ,Ts...>::type*>(p));
            case 62: return callable(*static_cast<typename HAD_NS extract<62 < count ? 62 : 0 ,Ts...>::type*>(p));
            case 63: return callable(*static_cast<typename HAD_NS extract<63 < count ? 63 : 0 ,Ts...>::type*>(p));
        }
        HAD_NS unreachable();
    }

};


template<>
struct visit_switch_case<5> {
    template<typename... Ts,typename Callable,typename Ptr>
    static auto run(Callable&& callable, Ptr p, unsigned char index)
        -> decltype(callable(HAD_NS declval<typename extract<0, Ts...>::type>())) {

        constexpr auto count = sizeof...(Ts);
        switch (index) {
            case 0:  return callable(*static_cast<typename HAD_NS extract<0 < count ? 0 : 0, Ts...>::type*>(p));
            case 1:  return callable(*static_cast<typename HAD_NS extract<1 < count ? 1 : 0 ,Ts...>::type*>(p));
            case 2:  return callable(*static_cast<typename HAD_NS extract<2 < count ? 2 : 0 ,Ts...>::type*>(p));
            case 3:  return callable(*static_cast<typename HAD_NS extract<3 < count ? 3 : 0 ,Ts...>::type*>(p));
            case 4:  return callable(*static_cast<typename HAD_NS extract<4 < count ? 4 : 0 ,Ts...>::type*>(p));
            case 5:  return callable(*static_cast<typename HAD_NS extract<5 < count ? 5 : 0 ,Ts...>::type*>(p));
            case 6:  return callable(*static_cast<typename HAD_NS extract<6 < count ? 6 : 0 ,Ts...>::type*>(p));
            case 7:  return callable(*static_cast<typename HAD_NS extract<7 < count ? 7 : 0 ,Ts...>::type*>(p));
            case 8:  return callable(*static_cast<typename HAD_NS extract<8 < count ? 8 : 0 ,Ts...>::type*>(p));
            case 9:  return callable(*static_cast<typename HAD_NS extract<9 < count ? 9 : 0 ,Ts...>::type*>(p));
            case 10: return callable(*static_cast<typename HAD_NS extract<10 < count ? 10 : 0 ,Ts...>::type*>(p));
            case 11: return callable(*static_cast<typename HAD_NS extract<11 < count ? 11 : 0 ,Ts...>::type*>(p));
            case 12: return callable(*static_cast<typename HAD_NS extract<12 < count ? 12 : 0 ,Ts...>::type*>(p));
            case 13: return callable(*static_cast<typename HAD_NS extract<13 < count ? 13 : 0 ,Ts...>::type*>(p));
            case 14: return callable(*static_cast<typename HAD_NS extract<14 < count ? 14 : 0 ,Ts...>::type*>(p));
            case 15: return callable(*static_cast<typename HAD_NS extract<15 < count ? 15 : 0 ,Ts...>::type*>(p));
            case 16: return callable(*static_cast<typename HAD_NS extract<16 < count ? 16 : 0 ,Ts...>::type*>(p));
            case 17: return callable(*static_cast<typename HAD_NS extract<17 < count ? 17 : 0 ,Ts...>::type*>(p));
            case 18: return callable(*static_cast<typename HAD_NS extract<18 < count ? 18 : 0 ,Ts...>::type*>(p));
            case 19: return callable(*static_cast<typename HAD_NS extract<19 < count ? 19 : 0 ,Ts...>::type*>(p));
            case 20: return callable(*static_cast<typename HAD_NS extract<20 < count ? 20 : 0 ,Ts...>::type*>(p));
            case 21: return callable(*static_cast<typename HAD_NS extract<21 < count ? 21 : 0 ,Ts...>::type*>(p));
            case 22: return callable(*static_cast<typename HAD_NS extract<22 < count ? 22 : 0 ,Ts...>::type*>(p));
            case 23: return callable(*static_cast<typename HAD_NS extract<23 < count ? 23 : 0 ,Ts...>::type*>(p));
            case 24: return callable(*static_cast<typename HAD_NS extract<24 < count ? 24 : 0 ,Ts...>::type*>(p));
            case 25: return callable(*static_cast<typename HAD_NS extract<25 < count ? 25 : 0 ,Ts...>::type*>(p));
            case 26: return callable(*static_cast<typename HAD_NS extract<26 < count ? 26 : 0 ,Ts...>::type*>(p));
            case 27: return callable(*static_cast<typename HAD_NS extract<27 < count ? 27 : 0 ,Ts...>::type*>(p));
            case 28: return callable(*static_cast<typename HAD_NS extract<28 < count ? 28 : 0 ,Ts...>::type*>(p));
            case 29: return callable(*static_cast<typename HAD_NS extract<29 < count ? 29 : 0 ,Ts...>::type*>(p));
            case 30: return callable(*static_cast<typename HAD_NS extract<30 < count ? 30 : 0 ,Ts...>::type*>(p));
            case 31: return callable(*static_cast<typename HAD_NS extract<31 < count ? 31 : 0 ,Ts...>::type*>(p));
            case 32: return callable(*static_cast<typename HAD_NS extract<32 < count ? 32 : 0 ,Ts...>::type*>(p));
            case 33: return callable(*static_cast<typename HAD_NS extract<33 < count ? 33 : 0 ,Ts...>::type*>(p));
            case 34: return callable(*static_cast<typename HAD_NS extract<34 < count ? 34 : 0 ,Ts...>::type*>(p));
            case 35: return callable(*static_cast<typename HAD_NS extract<35 < count ? 35 : 0 ,Ts...>::type*>(p));
            case 36: return callable(*static_cast<typename HAD_NS extract<36 < count ? 36 : 0 ,Ts...>::type*>(p));
            case 37: return callable(*static_cast<typename HAD_NS extract<37 < count ? 37 : 0 ,Ts...>::type*>(p));
            case 38: return callable(*static_cast<typename HAD_NS extract<38 < count ? 38 : 0 ,Ts...>::type*>(p));
            case 39: return callable(*static_cast<typename HAD_NS extract<39 < count ? 39 : 0 ,Ts...>::type*>(p));
            case 40: return callable(*static_cast<typename HAD_NS extract<40 < count ? 40 : 0 ,Ts...>::type*>(p));
            case 41: return callable(*static_cast<typename HAD_NS extract<41 < count ? 41 : 0 ,Ts...>::type*>(p));
            case 42: return callable(*static_cast<typename HAD_NS extract<42 < count ? 42 : 0 ,Ts...>::type*>(p));
            case 43: return callable(*static_cast<typename HAD_NS extract<43 < count ? 43 : 0 ,Ts...>::type*>(p));
            case 44: return callable(*static_cast<typename HAD_NS extract<44 < count ? 44 : 0 ,Ts...>::type*>(p));
            case 45: return callable(*static_cast<typename HAD_NS extract<45 < count ? 45 : 0 ,Ts...>::type*>(p));
            case 46: return callable(*static_cast<typename HAD_NS extract<46 < count ? 46 : 0 ,Ts...>::type*>(p));
            case 47: return callable(*static_cast<typename HAD_NS extract<47 < count ? 47 : 0 ,Ts...>::type*>(p));
            case 48: return callable(*static_cast<typename HAD_NS extract<48 < count ? 48 : 0 ,Ts...>::type*>(p));
            case 49: return callable(*static_cast<typename HAD_NS extract<49 < count ? 49 : 0 ,Ts...>::type*>(p));
            case 50: return callable(*static_cast<typename HAD_NS extract<50 < count ? 50 : 0 ,Ts...>::type*>(p));
            case 51: return callable(*static_cast<typename HAD_NS extract<51 < count ? 51 : 0 ,Ts...>::type*>(p));
            case 52: return callable(*static_cast<typename HAD_NS extract<52 < count ? 52 : 0 ,Ts...>::type*>(p));
            case 53: return callable(*static_cast<typename HAD_NS extract<53 < count ? 53 : 0 ,Ts...>::type*>(p));
            case 54: return callable(*static_cast<typename HAD_NS extract<54 < count ? 54 : 0 ,Ts...>::type*>(p));
            case 55: return callable(*static_cast<typename HAD_NS extract<55 < count ? 55 : 0 ,Ts...>::type*>(p));
            case 56: return callable(*static_cast<typename HAD_NS extract<56 < count ? 56 : 0 ,Ts...>::type*>(p));
            case 57: return callable(*static_cast<typename HAD_NS extract<57 < count ? 57 : 0 ,Ts...>::type*>(p));
            case 58: return callable(*static_cast<typename HAD_NS extract<58 < count ? 58 : 0 ,Ts...>::type*>(p));
            case 59: return callable(*static_cast<typename HAD_NS extract<59 < count ? 59 : 0 ,Ts...>::type*>(p));
            case 60: return callable(*static_cast<typename HAD_NS extract<60 < count ? 60 : 0 ,Ts...>::type*>(p));
            case 61: return callable(*static_cast<typename HAD_NS extract<61 < count ? 61 : 0 ,Ts...>::type*>(p));
            case 62: return callable(*static_cast<typename HAD_NS extract<62 < count ? 62 : 0 ,Ts...>::type*>(p));
            case 63: return callable(*static_cast<typename HAD_NS extract<63 < count ? 63 : 0 ,Ts...>::type*>(p));
            case 64: return callable(*static_cast<typename HAD_NS extract<64 < count ? 64 : 0 ,Ts...>::type*>(p));
            case 65: return callable(*static_cast<typename HAD_NS extract<65 < count ? 65 : 0 ,Ts...>::type*>(p));
            case 66: return callable(*static_cast<typename HAD_NS extract<66 < count ? 66 : 0 ,Ts...>::type*>(p));
            case 67: return callable(*static_cast<typename HAD_NS extract<67 < count ? 67 : 0 ,Ts...>::type*>(p));
            case 68: return callable(*static_cast<typename HAD_NS extract<68 < count ? 68 : 0 ,Ts...>::type*>(p));
            case 69: return callable(*static_cast<typename HAD_NS extract<69 < count ? 69 : 0 ,Ts...>::type*>(p));
            case 70: return callable(*static_cast<typename HAD_NS extract<70 < count ? 70 : 0 ,Ts...>::type*>(p));
            case 71: return callable(*static_cast<typename HAD_NS extract<71 < count ? 71 : 0 ,Ts...>::type*>(p));
            case 72: return callable(*static_cast<typename HAD_NS extract<72 < count ? 72 : 0 ,Ts...>::type*>(p));
            case 73: return callable(*static_cast<typename HAD_NS extract<73 < count ? 73 : 0 ,Ts...>::type*>(p));
            case 74: return callable(*static_cast<typename HAD_NS extract<74 < count ? 74 : 0 ,Ts...>::type*>(p));
            case 75: return callable(*static_cast<typename HAD_NS extract<75 < count ? 75 : 0 ,Ts...>::type*>(p));
            case 76: return callable(*static_cast<typename HAD_NS extract<76 < count ? 76 : 0 ,Ts...>::type*>(p));
            case 77: return callable(*static_cast<typename HAD_NS extract<77 < count ? 77 : 0 ,Ts...>::type*>(p));
            case 78: return callable(*static_cast<typename HAD_NS extract<78 < count ? 78 : 0 ,Ts...>::type*>(p));
            case 79: return callable(*static_cast<typename HAD_NS extract<79 < count ? 79 : 0 ,Ts...>::type*>(p));
            case 80: return callable(*static_cast<typename HAD_NS extract<80 < count ? 80 : 0 ,Ts...>::type*>(p));
            case 81: return callable(*static_cast<typename HAD_NS extract<81 < count ? 81 : 0 ,Ts...>::type*>(p));
            case 82: return callable(*static_cast<typename HAD_NS extract<82 < count ? 82 : 0 ,Ts...>::type*>(p));
            case 83: return callable(*static_cast<typename HAD_NS extract<83 < count ? 83 : 0 ,Ts...>::type*>(p));
            case 84: return callable(*static_cast<typename HAD_NS extract<84 < count ? 84 : 0 ,Ts...>::type*>(p));
            case 85: return callable(*static_cast<typename HAD_NS extract<85 < count ? 85 : 0 ,Ts...>::type*>(p));
            case 86: return callable(*static_cast<typename HAD_NS extract<86 < count ? 86 : 0 ,Ts...>::type*>(p));
            case 87: return callable(*static_cast<typename HAD_NS extract<87 < count ? 87 : 0 ,Ts...>::type*>(p));
            case 88: return callable(*static_cast<typename HAD_NS extract<88 < count ? 88 : 0 ,Ts...>::type*>(p));
            case 89: return callable(*static_cast<typename HAD_NS extract<89 < count ? 89 : 0 ,Ts...>::type*>(p));
            case 90: return callable(*static_cast<typename HAD_NS extract<90 < count ? 90 : 0 ,Ts...>::type*>(p));
            case 91: return callable(*static_cast<typename HAD_NS extract<91 < count ? 91 : 0 ,Ts...>::type*>(p));
            case 92: return callable(*static_cast<typename HAD_NS extract<92 < count ? 92 : 0 ,Ts...>::type*>(p));
            case 93: return callable(*static_cast<typename HAD_NS extract<93 < count ? 93 : 0 ,Ts...>::type*>(p));
            case 94: return callable(*static_cast<typename HAD_NS extract<94 < count ? 94 : 0 ,Ts...>::type*>(p));
            case 95: return callable(*static_cast<typename HAD_NS extract<95 < count ? 95 : 0 ,Ts...>::type*>(p));
            case 96: return callable(*static_cast<typename HAD_NS extract<96 < count ? 96 : 0 ,Ts...>::type*>(p));
            case 97: return callable(*static_cast<typename HAD_NS extract<97 < count ? 97 : 0 ,Ts...>::type*>(p));
            case 98: return callable(*static_cast<typename HAD_NS extract<98 < count ? 98 : 0 ,Ts...>::type*>(p));
            case 99: return callable(*static_cast<typename HAD_NS extract<99 < count ? 99 : 0 ,Ts...>::type*>(p));
            case 100:return callable(*static_cast<typename HAD_NS extract<100 < count ? 100 : 0 ,Ts...>::type*>(p));
            case 101:return callable(*static_cast<typename HAD_NS extract<101 < count ? 101 : 0 ,Ts...>::type*>(p));
            case 102:return callable(*static_cast<typename HAD_NS extract<102 < count ? 102 : 0 ,Ts...>::type*>(p));
            case 103:return callable(*static_cast<typename HAD_NS extract<103 < count ? 103 : 0 ,Ts...>::type*>(p));
            case 104:return callable(*static_cast<typename HAD_NS extract<104 < count ? 104 : 0 ,Ts...>::type*>(p));
            case 105:return callable(*static_cast<typename HAD_NS extract<105 < count ? 105 : 0 ,Ts...>::type*>(p));
            case 106:return callable(*static_cast<typename HAD_NS extract<106 < count ? 106 : 0 ,Ts...>::type*>(p));
            case 107:return callable(*static_cast<typename HAD_NS extract<107 < count ? 107 : 0 ,Ts...>::type*>(p));
            case 108:return callable(*static_cast<typename HAD_NS extract<108 < count ? 108 : 0 ,Ts...>::type*>(p));
            case 109:return callable(*static_cast<typename HAD_NS extract<109 < count ? 109 : 0 ,Ts...>::type*>(p));
            case 110:return callable(*static_cast<typename HAD_NS extract<110 < count ? 110 : 0 ,Ts...>::type*>(p));
            case 111:return callable(*static_cast<typename HAD_NS extract<111 < count ? 111 : 0 ,Ts...>::type*>(p));
            case 112:return callable(*static_cast<typename HAD_NS extract<112 < count ? 112 : 0 ,Ts...>::type*>(p));
            case 113:return callable(*static_cast<typename HAD_NS extract<113 < count ? 113 : 0 ,Ts...>::type*>(p));
            case 114:return callable(*static_cast<typename HAD_NS extract<114 < count ? 114 : 0 ,Ts...>::type*>(p));
            case 115:return callable(*static_cast<typename HAD_NS extract<115 < count ? 115 : 0 ,Ts...>::type*>(p));
            case 116:return callable(*static_cast<typename HAD_NS extract<116 < count ? 116 : 0 ,Ts...>::type*>(p));
            case 117:return callable(*static_cast<typename HAD_NS extract<117 < count ? 117 : 0 ,Ts...>::type*>(p));
            case 118:return callable(*static_cast<typename HAD_NS extract<118 < count ? 118 : 0 ,Ts...>::type*>(p));
            case 119:return callable(*static_cast<typename HAD_NS extract<119 < count ? 119 : 0 ,Ts...>::type*>(p));
            case 120:return callable(*static_cast<typename HAD_NS extract<120 < count ? 120 : 0 ,Ts...>::type*>(p));
            case 121:return callable(*static_cast<typename HAD_NS extract<121 < count ? 121 : 0 ,Ts...>::type*>(p));
            case 122:return callable(*static_cast<typename HAD_NS extract<122 < count ? 122 : 0 ,Ts...>::type*>(p));
            case 123:return callable(*static_cast<typename HAD_NS extract<123 < count ? 123 : 0 ,Ts...>::type*>(p));
            case 124:return callable(*static_cast<typename HAD_NS extract<124 < count ? 124 : 0 ,Ts...>::type*>(p));
            case 125:return callable(*static_cast<typename HAD_NS extract<125 < count ? 125 : 0 ,Ts...>::type*>(p));
            case 126:return callable(*static_cast<typename HAD_NS extract<126 < count ? 126 : 0 ,Ts...>::type*>(p));
            case 127:return callable(*static_cast<typename HAD_NS extract<127 < count ? 127 : 0 ,Ts...>::type*>(p));
        }
        HAD_NS unreachable();
    }

};


template<>
struct visit_switch_case<6> {
    template<typename... Ts,typename Callable,typename Ptr>
    static auto run(Callable&& callable, Ptr p, unsigned char index)
        -> decltype(callable(HAD_NS declval<typename extract<0, Ts...>::type>())) {
        
        constexpr auto count = sizeof...(Ts);
        switch (index) {
            case 0:  return callable(*static_cast<typename HAD_NS extract<0 < count ? 0 : 0, Ts... > ::type*>(p));
            case 1:  return callable(*static_cast<typename HAD_NS extract<1 < count ? 1 : 0, Ts... > ::type*>(p));
            case 2:  return callable(*static_cast<typename HAD_NS extract<2 < count ? 2 : 0, Ts... > ::type*>(p));
            case 3:  return callable(*static_cast<typename HAD_NS extract<3 < count ? 3 : 0, Ts... > ::type*>(p));
            case 4:  return callable(*static_cast<typename HAD_NS extract<4 < count ? 4 : 0, Ts... > ::type*>(p));
            case 5:  return callable(*static_cast<typename HAD_NS extract<5 < count ? 5 : 0, Ts... > ::type*>(p));
            case 6:  return callable(*static_cast<typename HAD_NS extract<6 < count ? 6 : 0, Ts... > ::type*>(p));
            case 7:  return callable(*static_cast<typename HAD_NS extract<7 < count ? 7 : 0, Ts... > ::type*>(p));
            case 8:  return callable(*static_cast<typename HAD_NS extract<8 < count ? 8 : 0, Ts... > ::type*>(p));
            case 9:  return callable(*static_cast<typename HAD_NS extract<9 < count ? 9 : 0, Ts... > ::type*>(p));
            case 10: return callable(*static_cast<typename HAD_NS extract<10 < count ? 10 : 0, Ts... > ::type*>(p));
            case 11: return callable(*static_cast<typename HAD_NS extract<11 < count ? 11 : 0, Ts... > ::type*>(p));
            case 12: return callable(*static_cast<typename HAD_NS extract<12 < count ? 12 : 0, Ts... > ::type*>(p));
            case 13: return callable(*static_cast<typename HAD_NS extract<13 < count ? 13 : 0, Ts... > ::type*>(p));
            case 14: return callable(*static_cast<typename HAD_NS extract<14 < count ? 14 : 0, Ts... > ::type*>(p));
            case 15: return callable(*static_cast<typename HAD_NS extract<15 < count ? 15 : 0, Ts... > ::type*>(p));
            case 16: return callable(*static_cast<typename HAD_NS extract<16 < count ? 16 : 0, Ts... > ::type*>(p));
            case 17: return callable(*static_cast<typename HAD_NS extract<17 < count ? 17 : 0, Ts... > ::type*>(p));
            case 18: return callable(*static_cast<typename HAD_NS extract<18 < count ? 18 : 0, Ts... > ::type*>(p));
            case 19: return callable(*static_cast<typename HAD_NS extract<19 < count ? 19 : 0, Ts... > ::type*>(p));
            case 20: return callable(*static_cast<typename HAD_NS extract<20 < count ? 20 : 0, Ts... > ::type*>(p));
            case 21: return callable(*static_cast<typename HAD_NS extract<21 < count ? 21 : 0, Ts... > ::type*>(p));
            case 22: return callable(*static_cast<typename HAD_NS extract<22 < count ? 22 : 0, Ts... > ::type*>(p));
            case 23: return callable(*static_cast<typename HAD_NS extract<23 < count ? 23 : 0, Ts... > ::type*>(p));
            case 24: return callable(*static_cast<typename HAD_NS extract<24 < count ? 24 : 0, Ts... > ::type*>(p));
            case 25: return callable(*static_cast<typename HAD_NS extract<25 < count ? 25 : 0, Ts... > ::type*>(p));
            case 26: return callable(*static_cast<typename HAD_NS extract<26 < count ? 26 : 0, Ts... > ::type*>(p));
            case 27: return callable(*static_cast<typename HAD_NS extract<27 < count ? 27 : 0, Ts... > ::type*>(p));
            case 28: return callable(*static_cast<typename HAD_NS extract<28 < count ? 28 : 0, Ts... > ::type*>(p));
            case 29: return callable(*static_cast<typename HAD_NS extract<29 < count ? 29 : 0, Ts... > ::type*>(p));
            case 30: return callable(*static_cast<typename HAD_NS extract<30 < count ? 30 : 0, Ts... > ::type*>(p));
            case 31: return callable(*static_cast<typename HAD_NS extract<31 < count ? 31 : 0, Ts... > ::type*>(p));
            case 32: return callable(*static_cast<typename HAD_NS extract<32 < count ? 32 : 0, Ts... > ::type*>(p));
            case 33: return callable(*static_cast<typename HAD_NS extract<33 < count ? 33 : 0, Ts... > ::type*>(p));
            case 34: return callable(*static_cast<typename HAD_NS extract<34 < count ? 34 : 0, Ts... > ::type*>(p));
            case 35: return callable(*static_cast<typename HAD_NS extract<35 < count ? 35 : 0, Ts... > ::type*>(p));
            case 36: return callable(*static_cast<typename HAD_NS extract<36 < count ? 36 : 0, Ts... > ::type*>(p));
            case 37: return callable(*static_cast<typename HAD_NS extract<37 < count ? 37 : 0, Ts... > ::type*>(p));
            case 38: return callable(*static_cast<typename HAD_NS extract<38 < count ? 38 : 0, Ts... > ::type*>(p));
            case 39: return callable(*static_cast<typename HAD_NS extract<39 < count ? 39 : 0, Ts... > ::type*>(p));
            case 40: return callable(*static_cast<typename HAD_NS extract<40 < count ? 40 : 0, Ts... > ::type*>(p));
            case 41: return callable(*static_cast<typename HAD_NS extract<41 < count ? 41 : 0, Ts... > ::type*>(p));
            case 42: return callable(*static_cast<typename HAD_NS extract<42 < count ? 42 : 0, Ts... > ::type*>(p));
            case 43: return callable(*static_cast<typename HAD_NS extract<43 < count ? 43 : 0, Ts... > ::type*>(p));
            case 44: return callable(*static_cast<typename HAD_NS extract<44 < count ? 44 : 0, Ts... > ::type*>(p));
            case 45: return callable(*static_cast<typename HAD_NS extract<45 < count ? 45 : 0, Ts... > ::type*>(p));
            case 46: return callable(*static_cast<typename HAD_NS extract<46 < count ? 46 : 0, Ts... > ::type*>(p));
            case 47: return callable(*static_cast<typename HAD_NS extract<47 < count ? 47 : 0, Ts... > ::type*>(p));
            case 48: return callable(*static_cast<typename HAD_NS extract<48 < count ? 48 : 0, Ts... > ::type*>(p));
            case 49: return callable(*static_cast<typename HAD_NS extract<49 < count ? 49 : 0, Ts... > ::type*>(p));
            case 50: return callable(*static_cast<typename HAD_NS extract<50 < count ? 50 : 0, Ts... > ::type*>(p));
            case 51: return callable(*static_cast<typename HAD_NS extract<51 < count ? 51 : 0, Ts... > ::type*>(p));
            case 52: return callable(*static_cast<typename HAD_NS extract<52 < count ? 52 : 0, Ts... > ::type*>(p));
            case 53: return callable(*static_cast<typename HAD_NS extract<53 < count ? 53 : 0, Ts... > ::type*>(p));
            case 54: return callable(*static_cast<typename HAD_NS extract<54 < count ? 54 : 0, Ts... > ::type*>(p));
            case 55: return callable(*static_cast<typename HAD_NS extract<55 < count ? 55 : 0, Ts... > ::type*>(p));
            case 56: return callable(*static_cast<typename HAD_NS extract<56 < count ? 56 : 0, Ts... > ::type*>(p));
            case 57: return callable(*static_cast<typename HAD_NS extract<57 < count ? 57 : 0, Ts... > ::type*>(p));
            case 58: return callable(*static_cast<typename HAD_NS extract<58 < count ? 58 : 0, Ts... > ::type*>(p));
            case 59: return callable(*static_cast<typename HAD_NS extract<59 < count ? 59 : 0, Ts... > ::type*>(p));
            case 60: return callable(*static_cast<typename HAD_NS extract<60 < count ? 60 : 0, Ts... > ::type*>(p));
            case 61: return callable(*static_cast<typename HAD_NS extract<61 < count ? 61 : 0, Ts... > ::type*>(p));
            case 62: return callable(*static_cast<typename HAD_NS extract<62 < count ? 62 : 0, Ts... > ::type*>(p));
            case 63: return callable(*static_cast<typename HAD_NS extract<63 < count ? 63 : 0, Ts... > ::type*>(p));
            case 64: return callable(*static_cast<typename HAD_NS extract<64 < count ? 64 : 0, Ts... > ::type*>(p));
            case 65: return callable(*static_cast<typename HAD_NS extract<65 < count ? 65 : 0, Ts... > ::type*>(p));
            case 66: return callable(*static_cast<typename HAD_NS extract<66 < count ? 66 : 0, Ts... > ::type*>(p));
            case 67: return callable(*static_cast<typename HAD_NS extract<67 < count ? 67 : 0, Ts... > ::type*>(p));
            case 68: return callable(*static_cast<typename HAD_NS extract<68 < count ? 68 : 0, Ts... > ::type*>(p));
            case 69: return callable(*static_cast<typename HAD_NS extract<69 < count ? 69 : 0, Ts... > ::type*>(p));
            case 70: return callable(*static_cast<typename HAD_NS extract<70 < count ? 70 : 0, Ts... > ::type*>(p));
            case 71: return callable(*static_cast<typename HAD_NS extract<71 < count ? 71 : 0, Ts... > ::type*>(p));
            case 72: return callable(*static_cast<typename HAD_NS extract<72 < count ? 72 : 0, Ts... > ::type*>(p));
            case 73: return callable(*static_cast<typename HAD_NS extract<73 < count ? 73 : 0, Ts... > ::type*>(p));
            case 74: return callable(*static_cast<typename HAD_NS extract<74 < count ? 74 : 0, Ts... > ::type*>(p));
            case 75: return callable(*static_cast<typename HAD_NS extract<75 < count ? 75 : 0, Ts... > ::type*>(p));
            case 76: return callable(*static_cast<typename HAD_NS extract<76 < count ? 76 : 0, Ts... > ::type*>(p));
            case 77: return callable(*static_cast<typename HAD_NS extract<77 < count ? 77 : 0, Ts... > ::type*>(p));
            case 78: return callable(*static_cast<typename HAD_NS extract<78 < count ? 78 : 0, Ts... > ::type*>(p));
            case 79: return callable(*static_cast<typename HAD_NS extract<79 < count ? 79 : 0, Ts... > ::type*>(p));
            case 80: return callable(*static_cast<typename HAD_NS extract<80 < count ? 80 : 0, Ts... > ::type*>(p));
            case 81: return callable(*static_cast<typename HAD_NS extract<81 < count ? 81 : 0, Ts... > ::type*>(p));
            case 82: return callable(*static_cast<typename HAD_NS extract<82 < count ? 82 : 0, Ts... > ::type*>(p));
            case 83: return callable(*static_cast<typename HAD_NS extract<83 < count ? 83 : 0, Ts... > ::type*>(p));
            case 84: return callable(*static_cast<typename HAD_NS extract<84 < count ? 84 : 0, Ts... > ::type*>(p));
            case 85: return callable(*static_cast<typename HAD_NS extract<85 < count ? 85 : 0, Ts... > ::type*>(p));
            case 86: return callable(*static_cast<typename HAD_NS extract<86 < count ? 86 : 0, Ts... > ::type*>(p));
            case 87: return callable(*static_cast<typename HAD_NS extract<87 < count ? 87 : 0, Ts... > ::type*>(p));
            case 88: return callable(*static_cast<typename HAD_NS extract<88 < count ? 88 : 0, Ts... > ::type*>(p));
            case 89: return callable(*static_cast<typename HAD_NS extract<89 < count ? 89 : 0, Ts... > ::type*>(p));
            case 90: return callable(*static_cast<typename HAD_NS extract<90 < count ? 90 : 0, Ts... > ::type*>(p));
            case 91: return callable(*static_cast<typename HAD_NS extract<91 < count ? 91 : 0, Ts... > ::type*>(p));
            case 92: return callable(*static_cast<typename HAD_NS extract<92 < count ? 92 : 0, Ts... > ::type*>(p));
            case 93: return callable(*static_cast<typename HAD_NS extract<93 < count ? 93 : 0, Ts... > ::type*>(p));
            case 94: return callable(*static_cast<typename HAD_NS extract<94 < count ? 94 : 0, Ts... > ::type*>(p));
            case 95: return callable(*static_cast<typename HAD_NS extract<95 < count ? 95 : 0, Ts... > ::type*>(p));
            case 96: return callable(*static_cast<typename HAD_NS extract<96 < count ? 96 : 0, Ts... > ::type*>(p));
            case 97: return callable(*static_cast<typename HAD_NS extract<97 < count ? 97 : 0, Ts... > ::type*>(p));
            case 98: return callable(*static_cast<typename HAD_NS extract<98 < count ? 98 : 0, Ts... > ::type*>(p));
            case 99: return callable(*static_cast<typename HAD_NS extract<99 < count ? 99 : 0, Ts... > ::type*>(p));
            case 100:return callable(*static_cast<typename HAD_NS extract<100 < count ? 100 : 0, Ts... > ::type*>(p));
            case 101:return callable(*static_cast<typename HAD_NS extract<101 < count ? 101 : 0, Ts... > ::type*>(p));
            case 102:return callable(*static_cast<typename HAD_NS extract<102 < count ? 102 : 0, Ts... > ::type*>(p));
            case 103:return callable(*static_cast<typename HAD_NS extract<103 < count ? 103 : 0, Ts... > ::type*>(p));
            case 104:return callable(*static_cast<typename HAD_NS extract<104 < count ? 104 : 0, Ts... > ::type*>(p));
            case 105:return callable(*static_cast<typename HAD_NS extract<105 < count ? 105 : 0, Ts... > ::type*>(p));
            case 106:return callable(*static_cast<typename HAD_NS extract<106 < count ? 106 : 0, Ts... > ::type*>(p));
            case 107:return callable(*static_cast<typename HAD_NS extract<107 < count ? 107 : 0, Ts... > ::type*>(p));
            case 108:return callable(*static_cast<typename HAD_NS extract<108 < count ? 108 : 0, Ts... > ::type*>(p));
            case 109:return callable(*static_cast<typename HAD_NS extract<109 < count ? 109 : 0, Ts... > ::type*>(p));
            case 110:return callable(*static_cast<typename HAD_NS extract<110 < count ? 110 : 0, Ts... > ::type*>(p));
            case 111:return callable(*static_cast<typename HAD_NS extract<111 < count ? 111 : 0, Ts... > ::type*>(p));
            case 112:return callable(*static_cast<typename HAD_NS extract<112 < count ? 112 : 0, Ts... > ::type*>(p));
            case 113:return callable(*static_cast<typename HAD_NS extract<113 < count ? 113 : 0, Ts... > ::type*>(p));
            case 114:return callable(*static_cast<typename HAD_NS extract<114 < count ? 114 : 0, Ts... > ::type*>(p));
            case 115:return callable(*static_cast<typename HAD_NS extract<115 < count ? 115 : 0, Ts... > ::type*>(p));
            case 116:return callable(*static_cast<typename HAD_NS extract<116 < count ? 116 : 0, Ts... > ::type*>(p));
            case 117:return callable(*static_cast<typename HAD_NS extract<117 < count ? 117 : 0, Ts... > ::type*>(p));
            case 118:return callable(*static_cast<typename HAD_NS extract<118 < count ? 118 : 0, Ts... > ::type*>(p));
            case 119:return callable(*static_cast<typename HAD_NS extract<119 < count ? 119 : 0, Ts... > ::type*>(p));
            case 120:return callable(*static_cast<typename HAD_NS extract<120 < count ? 120 : 0, Ts... > ::type*>(p));
            case 121:return callable(*static_cast<typename HAD_NS extract<121 < count ? 121 : 0, Ts... > ::type*>(p));
            case 122:return callable(*static_cast<typename HAD_NS extract<122 < count ? 122 : 0, Ts... > ::type*>(p));
            case 123:return callable(*static_cast<typename HAD_NS extract<123 < count ? 123 : 0, Ts... > ::type*>(p));
            case 124:return callable(*static_cast<typename HAD_NS extract<124 < count ? 124 : 0, Ts... > ::type*>(p));
            case 125:return callable(*static_cast<typename HAD_NS extract<125 < count ? 125 : 0, Ts... > ::type*>(p));
            case 126:return callable(*static_cast<typename HAD_NS extract<126 < count ? 126 : 0, Ts... > ::type*>(p));
            case 127:return callable(*static_cast<typename HAD_NS extract<127 < count ? 127 : 0, Ts... > ::type*>(p));
            case 128:return callable(*static_cast<typename HAD_NS extract<128 < count ? 128 : 0, Ts... > ::type*>(p));
            case 129:return callable(*static_cast<typename HAD_NS extract<129 < count ? 129 : 0, Ts... > ::type*>(p));
            case 130:return callable(*static_cast<typename HAD_NS extract<130 < count ? 130 : 0, Ts... > ::type*>(p));
            case 131:return callable(*static_cast<typename HAD_NS extract<131 < count ? 131 : 0, Ts... > ::type*>(p));
            case 132:return callable(*static_cast<typename HAD_NS extract<132 < count ? 132 : 0, Ts... > ::type*>(p));
            case 133:return callable(*static_cast<typename HAD_NS extract<133 < count ? 133 : 0, Ts... > ::type*>(p));
            case 134:return callable(*static_cast<typename HAD_NS extract<134 < count ? 134 : 0, Ts... > ::type*>(p));
            case 135:return callable(*static_cast<typename HAD_NS extract<135 < count ? 135 : 0, Ts... > ::type*>(p));
            case 136:return callable(*static_cast<typename HAD_NS extract<136 < count ? 136 : 0, Ts... > ::type*>(p));
            case 137:return callable(*static_cast<typename HAD_NS extract<137 < count ? 137 : 0, Ts... > ::type*>(p));
            case 138:return callable(*static_cast<typename HAD_NS extract<138 < count ? 138 : 0, Ts... > ::type*>(p));
            case 139:return callable(*static_cast<typename HAD_NS extract<139 < count ? 139 : 0, Ts... > ::type*>(p));
            case 140:return callable(*static_cast<typename HAD_NS extract<140 < count ? 140 : 0, Ts... > ::type*>(p));
            case 141:return callable(*static_cast<typename HAD_NS extract<141 < count ? 141 : 0, Ts... > ::type*>(p));
            case 142:return callable(*static_cast<typename HAD_NS extract<142 < count ? 142 : 0, Ts... > ::type*>(p));
            case 143:return callable(*static_cast<typename HAD_NS extract<143 < count ? 143 : 0, Ts... > ::type*>(p));
            case 144:return callable(*static_cast<typename HAD_NS extract<144 < count ? 144 : 0, Ts... > ::type*>(p));
            case 145:return callable(*static_cast<typename HAD_NS extract<145 < count ? 145 : 0, Ts... > ::type*>(p));
            case 146:return callable(*static_cast<typename HAD_NS extract<146 < count ? 146 : 0, Ts... > ::type*>(p));
            case 147:return callable(*static_cast<typename HAD_NS extract<147 < count ? 147 : 0, Ts... > ::type*>(p));
            case 148:return callable(*static_cast<typename HAD_NS extract<148 < count ? 148 : 0, Ts... > ::type*>(p));
            case 149:return callable(*static_cast<typename HAD_NS extract<149 < count ? 149 : 0, Ts... > ::type*>(p));
            case 150:return callable(*static_cast<typename HAD_NS extract<150 < count ? 150 : 0, Ts... > ::type*>(p));
            case 151:return callable(*static_cast<typename HAD_NS extract<151 < count ? 151 : 0, Ts... > ::type*>(p));
            case 152:return callable(*static_cast<typename HAD_NS extract<152 < count ? 152 : 0, Ts... > ::type*>(p));
            case 153:return callable(*static_cast<typename HAD_NS extract<153 < count ? 153 : 0, Ts... > ::type*>(p));
            case 154:return callable(*static_cast<typename HAD_NS extract<154 < count ? 154 : 0, Ts... > ::type*>(p));
            case 155:return callable(*static_cast<typename HAD_NS extract<155 < count ? 155 : 0, Ts... > ::type*>(p));
            case 156:return callable(*static_cast<typename HAD_NS extract<156 < count ? 156 : 0, Ts... > ::type*>(p));
            case 157:return callable(*static_cast<typename HAD_NS extract<157 < count ? 157 : 0, Ts... > ::type*>(p));
            case 158:return callable(*static_cast<typename HAD_NS extract<158 < count ? 158 : 0, Ts... > ::type*>(p));
            case 159:return callable(*static_cast<typename HAD_NS extract<159 < count ? 159 : 0, Ts... > ::type*>(p));
            case 160:return callable(*static_cast<typename HAD_NS extract<160 < count ? 160 : 0, Ts... > ::type*>(p));
            case 161:return callable(*static_cast<typename HAD_NS extract<161 < count ? 161 : 0, Ts... > ::type*>(p));
            case 162:return callable(*static_cast<typename HAD_NS extract<162 < count ? 162 : 0, Ts... > ::type*>(p));
            case 163:return callable(*static_cast<typename HAD_NS extract<163 < count ? 163 : 0, Ts... > ::type*>(p));
            case 164:return callable(*static_cast<typename HAD_NS extract<164 < count ? 164 : 0, Ts... > ::type*>(p));
            case 165:return callable(*static_cast<typename HAD_NS extract<165 < count ? 165 : 0, Ts... > ::type*>(p));
            case 166:return callable(*static_cast<typename HAD_NS extract<166 < count ? 166 : 0, Ts... > ::type*>(p));
            case 167:return callable(*static_cast<typename HAD_NS extract<167 < count ? 167 : 0, Ts... > ::type*>(p));
            case 168:return callable(*static_cast<typename HAD_NS extract<168 < count ? 168 : 0, Ts... > ::type*>(p));
            case 169:return callable(*static_cast<typename HAD_NS extract<169 < count ? 169 : 0, Ts... > ::type*>(p));
            case 170:return callable(*static_cast<typename HAD_NS extract<170 < count ? 170 : 0, Ts... > ::type*>(p));
            case 171:return callable(*static_cast<typename HAD_NS extract<171 < count ? 171 : 0, Ts... > ::type*>(p));
            case 172:return callable(*static_cast<typename HAD_NS extract<172 < count ? 172 : 0, Ts... > ::type*>(p));
            case 173:return callable(*static_cast<typename HAD_NS extract<173 < count ? 173 : 0, Ts... > ::type*>(p));
            case 174:return callable(*static_cast<typename HAD_NS extract<174 < count ? 174 : 0, Ts... > ::type*>(p));
            case 175:return callable(*static_cast<typename HAD_NS extract<175 < count ? 175 : 0, Ts... > ::type*>(p));
            case 176:return callable(*static_cast<typename HAD_NS extract<176 < count ? 176 : 0, Ts... > ::type*>(p));
            case 177:return callable(*static_cast<typename HAD_NS extract<177 < count ? 177 : 0, Ts... > ::type*>(p));
            case 178:return callable(*static_cast<typename HAD_NS extract<178 < count ? 178 : 0, Ts... > ::type*>(p));
            case 179:return callable(*static_cast<typename HAD_NS extract<179 < count ? 179 : 0, Ts... > ::type*>(p));
            case 180:return callable(*static_cast<typename HAD_NS extract<180 < count ? 180 : 0, Ts... > ::type*>(p));
            case 181:return callable(*static_cast<typename HAD_NS extract<181 < count ? 181 : 0, Ts... > ::type*>(p));
            case 182:return callable(*static_cast<typename HAD_NS extract<182 < count ? 182 : 0, Ts... > ::type*>(p));
            case 183:return callable(*static_cast<typename HAD_NS extract<183 < count ? 183 : 0, Ts... > ::type*>(p));
            case 184:return callable(*static_cast<typename HAD_NS extract<184 < count ? 184 : 0, Ts... > ::type*>(p));
            case 185:return callable(*static_cast<typename HAD_NS extract<185 < count ? 185 : 0, Ts... > ::type*>(p));
            case 186:return callable(*static_cast<typename HAD_NS extract<186 < count ? 186 : 0, Ts... > ::type*>(p));
            case 187:return callable(*static_cast<typename HAD_NS extract<187 < count ? 187 : 0, Ts... > ::type*>(p));
            case 188:return callable(*static_cast<typename HAD_NS extract<188 < count ? 188 : 0, Ts... > ::type*>(p));
            case 189:return callable(*static_cast<typename HAD_NS extract<189 < count ? 189 : 0, Ts... > ::type*>(p));
            case 190:return callable(*static_cast<typename HAD_NS extract<190 < count ? 190 : 0, Ts... > ::type*>(p));
            case 191:return callable(*static_cast<typename HAD_NS extract<191 < count ? 191 : 0, Ts... > ::type*>(p));
            case 192:return callable(*static_cast<typename HAD_NS extract<192 < count ? 192 : 0, Ts... > ::type*>(p));
            case 193:return callable(*static_cast<typename HAD_NS extract<193 < count ? 193 : 0, Ts... > ::type*>(p));
            case 194:return callable(*static_cast<typename HAD_NS extract<194 < count ? 194 : 0, Ts... > ::type*>(p));
            case 195:return callable(*static_cast<typename HAD_NS extract<195 < count ? 195 : 0, Ts... > ::type*>(p));
            case 196:return callable(*static_cast<typename HAD_NS extract<196 < count ? 196 : 0, Ts... > ::type*>(p));
            case 197:return callable(*static_cast<typename HAD_NS extract<197 < count ? 197 : 0, Ts... > ::type*>(p));
            case 198:return callable(*static_cast<typename HAD_NS extract<198 < count ? 198 : 0, Ts... > ::type*>(p));
            case 199:return callable(*static_cast<typename HAD_NS extract<199 < count ? 199 : 0, Ts... > ::type*>(p));
            case 200:return callable(*static_cast<typename HAD_NS extract<200 < count ? 200 : 0, Ts... > ::type*>(p));
            case 201:return callable(*static_cast<typename HAD_NS extract<201 < count ? 201 : 0, Ts... > ::type*>(p));
            case 202:return callable(*static_cast<typename HAD_NS extract<202 < count ? 202 : 0, Ts... > ::type*>(p));
            case 203:return callable(*static_cast<typename HAD_NS extract<203 < count ? 203 : 0, Ts... > ::type*>(p));
            case 204:return callable(*static_cast<typename HAD_NS extract<204 < count ? 204 : 0, Ts... > ::type*>(p));
            case 205:return callable(*static_cast<typename HAD_NS extract<205 < count ? 205 : 0, Ts... > ::type*>(p));
            case 206:return callable(*static_cast<typename HAD_NS extract<206 < count ? 206 : 0, Ts... > ::type*>(p));
            case 207:return callable(*static_cast<typename HAD_NS extract<207 < count ? 207 : 0, Ts... > ::type*>(p));
            case 208:return callable(*static_cast<typename HAD_NS extract<208 < count ? 208 : 0, Ts... > ::type*>(p));
            case 209:return callable(*static_cast<typename HAD_NS extract<209 < count ? 209 : 0, Ts... > ::type*>(p));
            case 210:return callable(*static_cast<typename HAD_NS extract<210 < count ? 210 : 0, Ts... > ::type*>(p));
            case 211:return callable(*static_cast<typename HAD_NS extract<211 < count ? 211 : 0, Ts... > ::type*>(p));
            case 212:return callable(*static_cast<typename HAD_NS extract<212 < count ? 212 : 0, Ts... > ::type*>(p));
            case 213:return callable(*static_cast<typename HAD_NS extract<213 < count ? 213 : 0, Ts... > ::type*>(p));
            case 214:return callable(*static_cast<typename HAD_NS extract<214 < count ? 214 : 0, Ts... > ::type*>(p));
            case 215:return callable(*static_cast<typename HAD_NS extract<215 < count ? 215 : 0, Ts... > ::type*>(p));
            case 216:return callable(*static_cast<typename HAD_NS extract<216 < count ? 216 : 0, Ts... > ::type*>(p));
            case 217:return callable(*static_cast<typename HAD_NS extract<217 < count ? 217 : 0, Ts... > ::type*>(p));
            case 218:return callable(*static_cast<typename HAD_NS extract<218 < count ? 218 : 0, Ts... > ::type*>(p));
            case 219:return callable(*static_cast<typename HAD_NS extract<219 < count ? 219 : 0, Ts... > ::type*>(p));
            case 220:return callable(*static_cast<typename HAD_NS extract<220 < count ? 220 : 0, Ts... > ::type*>(p));
            case 221:return callable(*static_cast<typename HAD_NS extract<221 < count ? 221 : 0, Ts... > ::type*>(p));
            case 222:return callable(*static_cast<typename HAD_NS extract<222 < count ? 222 : 0, Ts... > ::type*>(p));
            case 223:return callable(*static_cast<typename HAD_NS extract<223 < count ? 223 : 0, Ts... > ::type*>(p));
            case 224:return callable(*static_cast<typename HAD_NS extract<224 < count ? 224 : 0, Ts... > ::type*>(p));
            case 225:return callable(*static_cast<typename HAD_NS extract<225 < count ? 225 : 0, Ts... > ::type*>(p));
            case 226:return callable(*static_cast<typename HAD_NS extract<226 < count ? 226 : 0, Ts... > ::type*>(p));
            case 227:return callable(*static_cast<typename HAD_NS extract<227 < count ? 227 : 0, Ts... > ::type*>(p));
            case 228:return callable(*static_cast<typename HAD_NS extract<228 < count ? 228 : 0, Ts... > ::type*>(p));
            case 229:return callable(*static_cast<typename HAD_NS extract<229 < count ? 229 : 0, Ts... > ::type*>(p));
            case 230:return callable(*static_cast<typename HAD_NS extract<230 < count ? 230 : 0, Ts... > ::type*>(p));
            case 231:return callable(*static_cast<typename HAD_NS extract<231 < count ? 231 : 0, Ts... > ::type*>(p));
            case 232:return callable(*static_cast<typename HAD_NS extract<232 < count ? 232 : 0, Ts... > ::type*>(p));
            case 233:return callable(*static_cast<typename HAD_NS extract<233 < count ? 233 : 0, Ts... > ::type*>(p));
            case 234:return callable(*static_cast<typename HAD_NS extract<234 < count ? 234 : 0, Ts... > ::type*>(p));
            case 235:return callable(*static_cast<typename HAD_NS extract<235 < count ? 235 : 0, Ts... > ::type*>(p));
            case 236:return callable(*static_cast<typename HAD_NS extract<236 < count ? 236 : 0, Ts... > ::type*>(p));
            case 237:return callable(*static_cast<typename HAD_NS extract<237 < count ? 237 : 0, Ts... > ::type*>(p));
            case 238:return callable(*static_cast<typename HAD_NS extract<238 < count ? 238 : 0, Ts... > ::type*>(p));
            case 239:return callable(*static_cast<typename HAD_NS extract<239 < count ? 239 : 0, Ts... > ::type*>(p));
            case 240:return callable(*static_cast<typename HAD_NS extract<240 < count ? 240 : 0, Ts... > ::type*>(p));
            case 241:return callable(*static_cast<typename HAD_NS extract<241 < count ? 241 : 0, Ts... > ::type*>(p));
            case 242:return callable(*static_cast<typename HAD_NS extract<242 < count ? 242 : 0, Ts... > ::type*>(p));
            case 243:return callable(*static_cast<typename HAD_NS extract<243 < count ? 243 : 0, Ts... > ::type*>(p));
            case 244:return callable(*static_cast<typename HAD_NS extract<244 < count ? 244 : 0, Ts... > ::type*>(p));
            case 245:return callable(*static_cast<typename HAD_NS extract<245 < count ? 245 : 0, Ts... > ::type*>(p));
            case 246:return callable(*static_cast<typename HAD_NS extract<246 < count ? 246 : 0, Ts... > ::type*>(p));
            case 247:return callable(*static_cast<typename HAD_NS extract<247 < count ? 247 : 0, Ts... > ::type*>(p));
            case 248:return callable(*static_cast<typename HAD_NS extract<248 < count ? 248 : 0, Ts... > ::type*>(p));
            case 249:return callable(*static_cast<typename HAD_NS extract<249 < count ? 249 : 0, Ts... > ::type*>(p));
            case 250:return callable(*static_cast<typename HAD_NS extract<250 < count ? 250 : 0, Ts... > ::type*>(p));
            case 251:return callable(*static_cast<typename HAD_NS extract<251 < count ? 251 : 0, Ts... > ::type*>(p));
            case 252:return callable(*static_cast<typename HAD_NS extract<252 < count ? 252 : 0, Ts... > ::type*>(p));
            case 253:return callable(*static_cast<typename HAD_NS extract<253 < count ? 253 : 0, Ts... > ::type*>(p));
            case 254:return callable(*static_cast<typename HAD_NS extract<254 < count ? 254 : 0, Ts... > ::type*>(p));
            case 255:return callable(*static_cast<typename HAD_NS extract<255 < count ? 255 : 0, Ts... > ::type*>(p));
        }
        unreachable();
    }
};

}
}

template<typename Callable,typename... Ts>
auto visit(Callable&& callable, variant<Ts...>& variant) 
-> decltype(callable( HAD_NS declval<extract_t<0,Ts...>>() )) {
    void* p = variant.x_get_void_1_401928();
    constexpr size_t case_level =   sizeof...(Ts) < 4   ? 
                                0 : sizeof...(Ts) < 8   ? 
                                1 : sizeof...(Ts) < 16  ?
                                2 : sizeof...(Ts) < 32  ?
                                3 : sizeof...(Ts) < 64  ?
                                4 : sizeof...(Ts) < 128 ?
                                5 : sizeof...(Ts) < 256 ?
                                6 : -1;
    // using levels for better inlining
    // better inlining!
    return details::variant::visit_switch_case<case_level>::template run<Ts...>(callable,p,static_cast<unsigned char>(variant.index()));
}


template<typename Callable,typename... Ts>
auto visit(Callable&& callable, const variant<Ts...>& variant) 
-> decltype(callable( HAD_NS declval<typename extract<0,Ts...>::type>() )) {
    const void* p = variant.x_get_void_1_401928();
    constexpr size_t case_level =   sizeof...(Ts) < 4   ? 
                                0 : sizeof...(Ts) < 8   ? 
                                1 : sizeof...(Ts) < 16  ?
                                2 : sizeof...(Ts) < 32  ?
                                3 : sizeof...(Ts) < 64  ?
                                4 : sizeof...(Ts) < 128 ?
                                5 : sizeof...(Ts) < 256 ?
                                6 : -1;
    // using levels for better inlining
    // better inlining!
    return details::variant::visit_switch_case<case_level>::template run<Ts...>(callable,p,static_cast<unsigned char>(variant.index()));
}

struct bad_variant_access : exception {
    using base = exception;

    bad_variant_access(const char* msg = "bad_variant_access") : base(msg) {}
};

#define HAD_VARIANT_CAST_(CV_REF)                                   \
template<typename T,typename... Ts>                                 \
T CV_REF variant_cast(variant<Ts...> CV_REF variant) {              \
    if (index_of<T, Ts...>::value != variant.index()) {             \
        throw bad_variant_access{ "had::variant_cast<T> threw a bad_varaint_access" }; \
    }                                                               \
    return *reinterpret_cast<T*>(variant.x_get_void_1_401928());    \
}

HAD_VARIANT_CAST_(&)
HAD_VARIANT_CAST_(const&)
HAD_VARIANT_CAST_(volatile&)
HAD_VARIANT_CAST_(const volatile&)
HAD_VARIANT_CAST_(&&)
HAD_VARIANT_CAST_(const&&)
HAD_VARIANT_CAST_(volatile&&)
HAD_VARIANT_CAST_(const volatile&&);

#undef HAD_VARIANT_CAST_

#define HAD_VARIANT_CAST_PTR_(CVPTR)                                  \
template<typename T,typename...Ts>                                    \
T CVPTR variant_cast(variant<Ts...> CVPTR variant) {                  \
    if (index_of<T, Ts...>::value != variant->index()) {              \
        return nullptr;                                               \
    }                                                                 \
    return reinterpret_cast<T CVPTR>(variant->x_get_void_1_401928()); \
}

HAD_VARIANT_CAST_PTR_(*)
HAD_VARIANT_CAST_PTR_(const*)
HAD_VARIANT_CAST_PTR_(volatile*)
HAD_VARIANT_CAST_PTR_(const volatile*)


#undef HAD_VARIANT_CAST_PTR_



#define HAD_GET_V(THIS_QUALIFIERS) \
template<size_t I ,typename... Ts> \
auto get(HAD_NS variant<Ts...> THIS_QUALIFIERS v) -> typename HAD_NS extract<I,Ts...>::type THIS_QUALIFIERS { \
    using T = HAD_NS extract_t<I, Ts...>; \
    return static_cast<T THIS_QUALIFIERS>(*reinterpret_cast<T*>(v.x_get_void_1_401928())); \
}

HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(HAD_GET_V);

#undef HAD_GET_V

HAD_NAMESPACE_END


#endif // !HAD_VARIANT_HPP
