#pragma once
#ifndef HAD_VARIANT_HPP
#define HAD_VARIANT_HPP

#include <had/type_traits/is_same.hpp>
#include <had/type_traits/index_of.hpp>
#include <had/type_traits/biggest_sizeof.hpp>
#include <had/type_traits/make_integer_sequence.hpp>
#include <had/memory/aligned_storage.hpp>
#include <had/array.hpp>
#include <had/utility/destroy_at.hpp>
#include <had/exceptions.hpp>
#include <had/type_traits/type_identity.hpp>
#include <had/type_traits/is_constructible.hpp>
#include <had/type_traits/conjunction.hpp>
#include <typeinfo>
#include <had/utility/unreachable.hpp>
#include <limits>
HAD_NAMESPACE_BEGIN

namespace details {
namespace variant {

template<typename Arg,typename T,typename... Ts>
struct get_constructible_type : HAD_NS conditional_t<
    HAD_NS is_constructible_v<T,Arg>,
    HAD_NS type_identity<T>,
    get_constructible_type<Arg,Ts...> > {
};

template<typename Arg,typename T>
struct get_constructible_type<Arg,T> : HAD_NS conditional_t<
    HAD_NS is_constructible_v<T,Arg>,
    HAD_NS type_identity<T>,
    //pack<HAD_NS type_identity<U>,HAD_NS integral_constant<HAD_NS size_t,I> >,
    HAD_NS type_identity<void> > {
};

}
}
template<typename... Ts>
class variant {
public:
    static_assert(sizeof...(Ts) > 1,"Sorry had::variant cannot accept must contain atleast 2 types");
    static_assert(sizeof...(Ts) < 256,"Sorry had::variant cannot accept more than 256 types");

    using this_type = variant;
    using first_type = typename extract<0, Ts...>::type;
    using size_type = size_t;
    using index_type = unsigned char;
    using biggest_type = typename biggest_sizeof<Ts...>::type;
    HAD_NODISCARD constexpr static size_type max_size() noexcept { return sizeof(biggest_type); }
    HAD_NODISCARD constexpr static size_type types_count() noexcept { return sizeof...(Ts); }
    constexpr static size_type npos = std::numeric_limits<size_type>::max();

    constexpr static bool is_all_nothrow_copy_constructible = HAD_NS conjunction<
        is_nothrow_copy_constructible<Ts>...
    >::value;

    constexpr static bool is_all_nothrow_move_constructible = HAD_NS conjunction<
        is_nothrow_move_constructible<Ts>...
    >::value;

private:

    using storage_t = typename aligned_storage<max_size(), alignof(biggest_type)>::type;

    storage_t mStorage;
    index_type mIndex;
    using copy_construction_function_t = void(variant&, const void*);
    using move_construction_function_t = void(variant&, void*);
    using copy_assignment_function_t   = void(variant&, const void*);
    using move_assignment_function_t   = void(variant&, void*);
    using destructor_function_t        = void(variant&);

    template<typename U>
    static void XCopyConstruct(variant& self, const void* object) {
        ::new (&self.mStorage) U(*reinterpret_cast<const U*>(object));
    }

    template<typename U>
    static void XMoveConstruct(variant& self, void* object) {
        ::new (&self.mStorage) U(HAD_NS as_rvalue(*reinterpret_cast<U*>(object)));
    }

    template<typename U>
    static void XCopyAssign(variant& self, const void* object) {
        ::new (&self.mStorage) U(*reinterpret_cast<const U*>(object));
    }

    template<typename U>
    static void XMoveAssign(variant& self, void* object) {
        ::new (&self.mStorage) U(HAD_NS as_rvalue(*reinterpret_cast<U*>(object)));
    }

    template<typename U>
    static void XDestructor(variant& self) noexcept {
        auto ptr = reinterpret_cast<U*>(&self.mStorage);
        HAD_NS destroy_at(ptr);
    }

    static constexpr array<copy_construction_function_t*, max_size()> copyConstructorFunctions{
        &XCopyConstruct<Ts>...
    };
    static constexpr array<move_construction_function_t*, max_size()> moveConstructorFunctions{
        &XMoveConstruct<Ts>...
    };

    static constexpr array<copy_assignment_function_t*, max_size()> copyAssignmentFunctions{
        &XCopyAssign<Ts>...
    };
    static constexpr array<move_assignment_function_t*, max_size()> moveAssignmentFunctions{
        &XMoveAssign<Ts>...

    };

    static constexpr array<destructor_function_t*, max_size()> destructorFunctions{
        &XDestructor<Ts>...
    };

public:

    template<typename U,
        typename enable_if <
        !is_same<remove_cvref_t<U>, this_type>::value
    >::type = 0 >
    variant(U&& u)
        noexcept(is_nothrow_constructible<typename details::variant::get_constructible_type<U, Ts...>::type,
        U>::value) {

        using constructible_t = typename details::variant::get_constructible_type<U, Ts...>::type;
        
        static_assert(!is_void<constructible_t>::value, "had::variant<Types...>::variant(U&&). one of Types must be constructible with U");
        
        constexpr size_t Index = 
            (index_of<U, Ts...>::value != npos)
            ? index_of<U, Ts...>::value
            : index_of<constructible_t, Ts...>::value;
        mIndex = Index;
        using ChosenConstructor = typename extract<Index, Ts...>::type;
        ::new (&mStorage) ChosenConstructor(HAD_NS forward<U>(u));
    }


    template<typename U,
        typename enable_if<!is_same<remove_cvref_t<U>, this_type>::value>::type = 0 >
    variant& operator=(U&& u) {
        using constructible_t = typename details::variant::get_constructible_type<U, Ts...>::type;
        
        static_assert(!is_void<constructible_t>::value, "had::variant<Types...>::operator=(U&&). one of Types must be constructible with U");
        
        destructorFunctions[mIndex](*this);
        mIndex = (index_of<U, Ts...>::value != -1)
            ? index_of<U, Ts...>::value
            : index_of<constructible_t, Ts...>::value;

        using ChosenConstructor = typename conditional<
            (index_of<U, Ts...>::value != -1),
            U,
            constructible_t
        >::type;

        ::new (&mStorage) ChosenConstructor(HAD_NS forward<U>(u));
        return *this;
    }


    variant(const variant& that) noexcept(is_all_nothrow_copy_constructible)
        : mIndex(that.mIndex) {
        const auto& fn = copyConstructorFunctions[mIndex];
        fn(*this, &that.mStorage);
    }

    variant(variant&& that) noexcept(is_all_nothrow_move_constructible)
        : mIndex(that.mIndex) {
        const auto& fn = moveConstructorFunctions[mIndex];
        fn(*this, &that.mStorage);
    }

    variant& operator=(const variant& that) noexcept(is_all_nothrow_copy_constructible) 
    {
        const auto& destructor = destructorFunctions[mIndex];
        destructor(*this);

        const auto& fn = copyConstructorFunctions[that.mIndex];
        fn(*this, &that.mStorage);
        mIndex = that.mIndex;
        return *this;
    }

    variant& operator=(variant&& that) noexcept(is_all_nothrow_move_constructible) {
        const auto& destructor = destructorFunctions[mIndex];
        destructor(*this);

        const auto& fn = moveConstructorFunctions[that.mIndex];
        fn(*this, &that.mStorage);
        mIndex = that.mIndex;
        return *this;
    }

    ~variant() noexcept {
        const auto& fn = destructorFunctions[mIndex];
        fn(*this);
    }

    template<size_t Index,typename... Args>
     auto emplace(Args&&... args) 
         noexcept(is_nothrow_constructible<typename extract<Index,Ts...>::type>::value) 
         -> typename extract<Index,Ts...>::type& {

         static_assert(Index < types_count(), "had::variant::emplace<Index>(Args...). Index out of range.");
        destructorFunctions[mIndex](*this);
        mIndex = Index;
        using T = typename extract<Index, Ts...>::type;
        return *static_cast<T*>(::new (&mStorage) T(HAD_NS forward<Args>(args)...));
    }

    template<typename T,typename... Args>
    T& emplace(Args&&... args) noexcept(is_nothrow_constructible<T,Args...>::value) {
        
        static_assert(index_of<T, Ts...>::value != -1, "had::variant<Types...>::emplace<T>(Args...). T must be a type in the variant's Types");
        destructorFunctions[mIndex](*this);
        mIndex = index_of<T, Ts...>::value;
        return *static_cast<T*>(::new (&mStorage) T(HAD_NS forward<Args>(args)...));
    }

    size_type index() const noexcept { return mIndex; }

    void* x_get_void_1_401928() noexcept { return static_cast<void*>(&mStorage);}
    const void* x_get_void_1_401928() const noexcept {return static_cast<const void*>(&mStorage);}
    volatile void* x_get_void_1_401928() volatile noexcept {return static_cast<volatile void*>(&mStorage);}
    const volatile void* x_get_void_1_401928() const volatile noexcept {return static_cast<const volatile void*>(&mStorage);}

    void swap(variant& that) noexcept(is_all_nothrow_move_constructible) {
        variant copy = HAD_NS move(that);

        destructorFunctions[that.mIndex](that);
        moveConstructorFunctions[mIndex](that, &this->mStorage);

        destructorFunctions[mIndex](*this);
        moveConstructorFunctions[that.mIndex](*this, &that.mStorage);

        HAD_NS swap(this->mIndex, that.mIndex);
    }

    friend void swap(variant& a,variant& b) noexcept(noexcept(a.swap(b))) {
        a.swap(b);
    }


};


template<typename Callable,typename... Ts>
auto visit(Callable&& callable, variant<Ts...> variant) 
-> decltype(callable( HAD_NS declval<extract_t<0,Ts...>>() )) {
    void* p = variant.x_get_void_1_401928();



    switch (variant.index()) {
        case 0:  return callable(*static_cast<typename HAD_NS extract_t<0 < sizeof...(Ts) ? 0 : 0, Ts...>*>(p));
        case 1:  return callable(*static_cast<typename HAD_NS extract_t<1 < sizeof...(Ts) ? 1 : 0 ,Ts...>*>(p));    
        case 2:  return callable(*static_cast<typename HAD_NS extract_t<2 < sizeof...(Ts) ? 2 : 0 ,Ts...>*>(p));    
        case 3:  return callable(*static_cast<typename HAD_NS extract_t<3 < sizeof...(Ts) ? 3 : 0 ,Ts...>*>(p));    
        case 4:  return callable(*static_cast<typename HAD_NS extract_t<4 < sizeof...(Ts) ? 4 : 0 ,Ts...>*>(p));    
        case 5:  return callable(*static_cast<typename HAD_NS extract_t<5 < sizeof...(Ts) ? 5 : 0 ,Ts...>*>(p));    
        case 6:  return callable(*static_cast<typename HAD_NS extract_t<6 < sizeof...(Ts) ? 6 : 0 ,Ts...>*>(p));    
        case 7:  return callable(*static_cast<typename HAD_NS extract_t<7 < sizeof...(Ts) ? 7 : 0 ,Ts...>*>(p));    
        case 8:  return callable(*static_cast<typename HAD_NS extract_t<8 < sizeof...(Ts) ? 8 : 0 ,Ts...>*>(p));    
        case 9:  return callable(*static_cast<typename HAD_NS extract<9 < sizeof...(Ts) ? 9 : 0 ,Ts...>::type*>(p));    
        case 10: return callable(*static_cast<typename HAD_NS extract<10 < sizeof...(Ts) ? 10 : 0 ,Ts...>::type*>(p));    
        case 11: return callable(*static_cast<typename HAD_NS extract<11 < sizeof...(Ts) ? 11 : 0 ,Ts...>::type*>(p));    
        case 12: return callable(*static_cast<typename HAD_NS extract<12 < sizeof...(Ts) ? 12 : 0 ,Ts...>::type*>(p));    
        case 13: return callable(*static_cast<typename HAD_NS extract<13 < sizeof...(Ts) ? 13 : 0 ,Ts...>::type*>(p));    
        case 14: return callable(*static_cast<typename HAD_NS extract<14 < sizeof...(Ts) ? 14 : 0 ,Ts...>::type*>(p));    
        case 15: return callable(*static_cast<typename HAD_NS extract<15 < sizeof...(Ts) ? 15 : 0 ,Ts...>::type*>(p));    
        case 16: return callable(*static_cast<typename HAD_NS extract<16 < sizeof...(Ts) ? 16 : 0 ,Ts...>::type*>(p));    
        case 17: return callable(*static_cast<typename HAD_NS extract<17 < sizeof...(Ts) ? 17 : 0 ,Ts...>::type*>(p));    
        case 18: return callable(*static_cast<typename HAD_NS extract<18 < sizeof...(Ts) ? 18 : 0 ,Ts...>::type*>(p));    
        case 19: return callable(*static_cast<typename HAD_NS extract<19 < sizeof...(Ts) ? 19 : 0 ,Ts...>::type*>(p));    
        case 20: return callable(*static_cast<typename HAD_NS extract<20 < sizeof...(Ts) ? 20 : 0 ,Ts...>::type*>(p));    
        case 21: return callable(*static_cast<typename HAD_NS extract<21 < sizeof...(Ts) ? 21 : 0 ,Ts...>::type*>(p));    
        case 22: return callable(*static_cast<typename HAD_NS extract<22 < sizeof...(Ts) ? 22 : 0 ,Ts...>::type*>(p));    
        case 23: return callable(*static_cast<typename HAD_NS extract<23 < sizeof...(Ts) ? 23 : 0 ,Ts...>::type*>(p));    
        case 24: return callable(*static_cast<typename HAD_NS extract<24 < sizeof...(Ts) ? 24 : 0 ,Ts...>::type*>(p));    
        case 25: return callable(*static_cast<typename HAD_NS extract<25 < sizeof...(Ts) ? 25 : 0 ,Ts...>::type*>(p));    
        case 26: return callable(*static_cast<typename HAD_NS extract<26 < sizeof...(Ts) ? 26 : 0 ,Ts...>::type*>(p));    
        case 27: return callable(*static_cast<typename HAD_NS extract<27 < sizeof...(Ts) ? 27 : 0 ,Ts...>::type*>(p));    
        case 28: return callable(*static_cast<typename HAD_NS extract<28 < sizeof...(Ts) ? 28 : 0 ,Ts...>::type*>(p));    
        case 29: return callable(*static_cast<typename HAD_NS extract<29 < sizeof...(Ts) ? 29 : 0 ,Ts...>::type*>(p));    
        case 30: return callable(*static_cast<typename HAD_NS extract<30 < sizeof...(Ts) ? 30 : 0 ,Ts...>::type*>(p));    
        case 31: return callable(*static_cast<typename HAD_NS extract<31 < sizeof...(Ts) ? 31 : 0 ,Ts...>::type*>(p));    
        case 32: return callable(*static_cast<typename HAD_NS extract<32 < sizeof...(Ts) ? 32 : 0 ,Ts...>::type*>(p));    
        case 33: return callable(*static_cast<typename HAD_NS extract<33 < sizeof...(Ts) ? 33 : 0 ,Ts...>::type*>(p));    
        case 34: return callable(*static_cast<typename HAD_NS extract<34 < sizeof...(Ts) ? 34 : 0 ,Ts...>::type*>(p));    
        case 35: return callable(*static_cast<typename HAD_NS extract<35 < sizeof...(Ts) ? 35 : 0 ,Ts...>::type*>(p));    
        case 36: return callable(*static_cast<typename HAD_NS extract<36 < sizeof...(Ts) ? 36 : 0 ,Ts...>::type*>(p));    
        case 37: return callable(*static_cast<typename HAD_NS extract<37 < sizeof...(Ts) ? 37 : 0 ,Ts...>::type*>(p));    
        case 38: return callable(*static_cast<typename HAD_NS extract<38 < sizeof...(Ts) ? 38 : 0 ,Ts...>::type*>(p));    
        case 39: return callable(*static_cast<typename HAD_NS extract<39 < sizeof...(Ts) ? 39 : 0 ,Ts...>::type*>(p));    
        case 40: return callable(*static_cast<typename HAD_NS extract<40 < sizeof...(Ts) ? 40 : 0 ,Ts...>::type*>(p));    
        case 41: return callable(*static_cast<typename HAD_NS extract<41 < sizeof...(Ts) ? 41 : 0 ,Ts...>::type*>(p));    
        case 42: return callable(*static_cast<typename HAD_NS extract<42 < sizeof...(Ts) ? 42 : 0 ,Ts...>::type*>(p));    
        case 43: return callable(*static_cast<typename HAD_NS extract<43 < sizeof...(Ts) ? 43 : 0 ,Ts...>::type*>(p));    
        case 44: return callable(*static_cast<typename HAD_NS extract<44 < sizeof...(Ts) ? 44 : 0 ,Ts...>::type*>(p));    
        case 45: return callable(*static_cast<typename HAD_NS extract<45 < sizeof...(Ts) ? 45 : 0 ,Ts...>::type*>(p));    
        case 46: return callable(*static_cast<typename HAD_NS extract<46 < sizeof...(Ts) ? 46 : 0 ,Ts...>::type*>(p));    
        case 47: return callable(*static_cast<typename HAD_NS extract<47 < sizeof...(Ts) ? 47 : 0 ,Ts...>::type*>(p));    
        case 48: return callable(*static_cast<typename HAD_NS extract<48 < sizeof...(Ts) ? 48 : 0 ,Ts...>::type*>(p));    
        case 49: return callable(*static_cast<typename HAD_NS extract<49 < sizeof...(Ts) ? 49 : 0 ,Ts...>::type*>(p));    
        case 50: return callable(*static_cast<typename HAD_NS extract<50 < sizeof...(Ts) ? 50 : 0 ,Ts...>::type*>(p));    
        case 51: return callable(*static_cast<typename HAD_NS extract<51 < sizeof...(Ts) ? 51 : 0 ,Ts...>::type*>(p));    
        case 52: return callable(*static_cast<typename HAD_NS extract<52 < sizeof...(Ts) ? 52 : 0 ,Ts...>::type*>(p));    
        case 53: return callable(*static_cast<typename HAD_NS extract<53 < sizeof...(Ts) ? 53 : 0 ,Ts...>::type*>(p));    
        case 54: return callable(*static_cast<typename HAD_NS extract<54 < sizeof...(Ts) ? 54 : 0 ,Ts...>::type*>(p));    
        case 55: return callable(*static_cast<typename HAD_NS extract<55 < sizeof...(Ts) ? 55 : 0 ,Ts...>::type*>(p));    
        case 56: return callable(*static_cast<typename HAD_NS extract<56 < sizeof...(Ts) ? 56 : 0 ,Ts...>::type*>(p));    
        case 57: return callable(*static_cast<typename HAD_NS extract<57 < sizeof...(Ts) ? 57 : 0 ,Ts...>::type*>(p));    
        case 58: return callable(*static_cast<typename HAD_NS extract<58 < sizeof...(Ts) ? 58 : 0 ,Ts...>::type*>(p));    
        case 59: return callable(*static_cast<typename HAD_NS extract<59 < sizeof...(Ts) ? 59 : 0 ,Ts...>::type*>(p));    
        case 60: return callable(*static_cast<typename HAD_NS extract<60 < sizeof...(Ts) ? 60 : 0 ,Ts...>::type*>(p));    
        case 61: return callable(*static_cast<typename HAD_NS extract<61 < sizeof...(Ts) ? 61 : 0 ,Ts...>::type*>(p));    
        case 62: return callable(*static_cast<typename HAD_NS extract<62 < sizeof...(Ts) ? 62 : 0 ,Ts...>::type*>(p));    
        case 63: return callable(*static_cast<typename HAD_NS extract<63 < sizeof...(Ts) ? 63 : 0 ,Ts...>::type*>(p));    
        case 64: return callable(*static_cast<typename HAD_NS extract<64 < sizeof...(Ts) ? 64 : 0 ,Ts...>::type*>(p));    
        case 65: return callable(*static_cast<typename HAD_NS extract<65 < sizeof...(Ts) ? 65 : 0 ,Ts...>::type*>(p));    
        case 66: return callable(*static_cast<typename HAD_NS extract<66 < sizeof...(Ts) ? 66 : 0 ,Ts...>::type*>(p));    
        case 67: return callable(*static_cast<typename HAD_NS extract<67 < sizeof...(Ts) ? 67 : 0 ,Ts...>::type*>(p));    
        case 68: return callable(*static_cast<typename HAD_NS extract<68 < sizeof...(Ts) ? 68 : 0 ,Ts...>::type*>(p));    
        case 69: return callable(*static_cast<typename HAD_NS extract<69 < sizeof...(Ts) ? 69 : 0 ,Ts...>::type*>(p));    
        case 70: return callable(*static_cast<typename HAD_NS extract<70 < sizeof...(Ts) ? 70 : 0 ,Ts...>::type*>(p));    
        case 71: return callable(*static_cast<typename HAD_NS extract<71 < sizeof...(Ts) ? 71 : 0 ,Ts...>::type*>(p));    
        case 72: return callable(*static_cast<typename HAD_NS extract<72 < sizeof...(Ts) ? 72 : 0 ,Ts...>::type*>(p));    
        case 73: return callable(*static_cast<typename HAD_NS extract<73 < sizeof...(Ts) ? 73 : 0 ,Ts...>::type*>(p));    
        case 74: return callable(*static_cast<typename HAD_NS extract<74 < sizeof...(Ts) ? 74 : 0 ,Ts...>::type*>(p));    
        case 75: return callable(*static_cast<typename HAD_NS extract<75 < sizeof...(Ts) ? 75 : 0 ,Ts...>::type*>(p));    
        case 76: return callable(*static_cast<typename HAD_NS extract<76 < sizeof...(Ts) ? 76 : 0 ,Ts...>::type*>(p));    
        case 77: return callable(*static_cast<typename HAD_NS extract<77 < sizeof...(Ts) ? 77 : 0 ,Ts...>::type*>(p));    
        case 78: return callable(*static_cast<typename HAD_NS extract<78 < sizeof...(Ts) ? 78 : 0 ,Ts...>::type*>(p));    
        case 79: return callable(*static_cast<typename HAD_NS extract<79 < sizeof...(Ts) ? 79 : 0 ,Ts...>::type*>(p));    
        case 80: return callable(*static_cast<typename HAD_NS extract<80 < sizeof...(Ts) ? 80 : 0 ,Ts...>::type*>(p));    
        case 81: return callable(*static_cast<typename HAD_NS extract<81 < sizeof...(Ts) ? 81 : 0 ,Ts...>::type*>(p));    
        case 82: return callable(*static_cast<typename HAD_NS extract<82 < sizeof...(Ts) ? 82 : 0 ,Ts...>::type*>(p));    
        case 83: return callable(*static_cast<typename HAD_NS extract<83 < sizeof...(Ts) ? 83 : 0 ,Ts...>::type*>(p));    
        case 84: return callable(*static_cast<typename HAD_NS extract<84 < sizeof...(Ts) ? 84 : 0 ,Ts...>::type*>(p));    
        case 85: return callable(*static_cast<typename HAD_NS extract<85 < sizeof...(Ts) ? 85 : 0 ,Ts...>::type*>(p));    
        case 86: return callable(*static_cast<typename HAD_NS extract<86 < sizeof...(Ts) ? 86 : 0 ,Ts...>::type*>(p));    
        case 87: return callable(*static_cast<typename HAD_NS extract<87 < sizeof...(Ts) ? 87 : 0 ,Ts...>::type*>(p));    
        case 88: return callable(*static_cast<typename HAD_NS extract<88 < sizeof...(Ts) ? 88 : 0 ,Ts...>::type*>(p));    
        case 89: return callable(*static_cast<typename HAD_NS extract<89 < sizeof...(Ts) ? 89 : 0 ,Ts...>::type*>(p));    
        case 90: return callable(*static_cast<typename HAD_NS extract<90 < sizeof...(Ts) ? 90 : 0 ,Ts...>::type*>(p));    
        case 91: return callable(*static_cast<typename HAD_NS extract<91 < sizeof...(Ts) ? 91 : 0 ,Ts...>::type*>(p));    
        case 92: return callable(*static_cast<typename HAD_NS extract<92 < sizeof...(Ts) ? 92 : 0 ,Ts...>::type*>(p));    
        case 93: return callable(*static_cast<typename HAD_NS extract<93 < sizeof...(Ts) ? 93 : 0 ,Ts...>::type*>(p));    
        case 94: return callable(*static_cast<typename HAD_NS extract<94 < sizeof...(Ts) ? 94 : 0 ,Ts...>::type*>(p));    
        case 95: return callable(*static_cast<typename HAD_NS extract<95 < sizeof...(Ts) ? 95 : 0 ,Ts...>::type*>(p));    
        case 96: return callable(*static_cast<typename HAD_NS extract<96 < sizeof...(Ts) ? 96 : 0 ,Ts...>::type*>(p));    
        case 97: return callable(*static_cast<typename HAD_NS extract<97 < sizeof...(Ts) ? 97 : 0 ,Ts...>::type*>(p));    
        case 98: return callable(*static_cast<typename HAD_NS extract<98 < sizeof...(Ts) ? 98 : 0 ,Ts...>::type*>(p));    
        case 99: return callable(*static_cast<typename HAD_NS extract<99 < sizeof...(Ts) ? 99 : 0 ,Ts...>::type*>(p));    
        case 100:return callable(*static_cast<typename HAD_NS extract<100 < sizeof...(Ts) ? 100 : 0 ,Ts...>::type*>(p));    
        case 101:return callable(*static_cast<typename HAD_NS extract<101 < sizeof...(Ts) ? 101 : 0 ,Ts...>::type*>(p));    
        case 102:return callable(*static_cast<typename HAD_NS extract<102 < sizeof...(Ts) ? 102 : 0 ,Ts...>::type*>(p));    
        case 103:return callable(*static_cast<typename HAD_NS extract<103 < sizeof...(Ts) ? 103 : 0 ,Ts...>::type*>(p));    
        case 104:return callable(*static_cast<typename HAD_NS extract<104 < sizeof...(Ts) ? 104 : 0 ,Ts...>::type*>(p));    
        case 105:return callable(*static_cast<typename HAD_NS extract<105 < sizeof...(Ts) ? 105 : 0 ,Ts...>::type*>(p));    
        case 106:return callable(*static_cast<typename HAD_NS extract<106 < sizeof...(Ts) ? 106 : 0 ,Ts...>::type*>(p));    
        case 107:return callable(*static_cast<typename HAD_NS extract<107 < sizeof...(Ts) ? 107 : 0 ,Ts...>::type*>(p));    
        case 108:return callable(*static_cast<typename HAD_NS extract<108 < sizeof...(Ts) ? 108 : 0 ,Ts...>::type*>(p));    
        case 109:return callable(*static_cast<typename HAD_NS extract<109 < sizeof...(Ts) ? 109 : 0 ,Ts...>::type*>(p));    
        case 110:return callable(*static_cast<typename HAD_NS extract<110 < sizeof...(Ts) ? 110 : 0 ,Ts...>::type*>(p));    
        case 111:return callable(*static_cast<typename HAD_NS extract<111 < sizeof...(Ts) ? 111 : 0 ,Ts...>::type*>(p));    
        case 112:return callable(*static_cast<typename HAD_NS extract<112 < sizeof...(Ts) ? 112 : 0 ,Ts...>::type*>(p));    
        case 113:return callable(*static_cast<typename HAD_NS extract<113 < sizeof...(Ts) ? 113 : 0 ,Ts...>::type*>(p));    
        case 114:return callable(*static_cast<typename HAD_NS extract<114 < sizeof...(Ts) ? 114 : 0 ,Ts...>::type*>(p));    
        case 115:return callable(*static_cast<typename HAD_NS extract<115 < sizeof...(Ts) ? 115 : 0 ,Ts...>::type*>(p));    
        case 116:return callable(*static_cast<typename HAD_NS extract<116 < sizeof...(Ts) ? 116 : 0 ,Ts...>::type*>(p));    
        case 117:return callable(*static_cast<typename HAD_NS extract<117 < sizeof...(Ts) ? 117 : 0 ,Ts...>::type*>(p));    
        case 118:return callable(*static_cast<typename HAD_NS extract<118 < sizeof...(Ts) ? 118 : 0 ,Ts...>::type*>(p));    
        case 119:return callable(*static_cast<typename HAD_NS extract<119 < sizeof...(Ts) ? 119 : 0 ,Ts...>::type*>(p));    
        case 120:return callable(*static_cast<typename HAD_NS extract<120 < sizeof...(Ts) ? 120 : 0 ,Ts...>::type*>(p));    
        case 121:return callable(*static_cast<typename HAD_NS extract<121 < sizeof...(Ts) ? 121 : 0 ,Ts...>::type*>(p));    
        case 122:return callable(*static_cast<typename HAD_NS extract<122 < sizeof...(Ts) ? 122 : 0 ,Ts...>::type*>(p));    
        case 123:return callable(*static_cast<typename HAD_NS extract<123 < sizeof...(Ts) ? 123 : 0 ,Ts...>::type*>(p));    
        case 124:return callable(*static_cast<typename HAD_NS extract<124 < sizeof...(Ts) ? 124 : 0 ,Ts...>::type*>(p));    
        case 125:return callable(*static_cast<typename HAD_NS extract<125 < sizeof...(Ts) ? 125 : 0 ,Ts...>::type*>(p));    
        case 126:return callable(*static_cast<typename HAD_NS extract<126 < sizeof...(Ts) ? 126 : 0 ,Ts...>::type*>(p));    
        case 127:return callable(*static_cast<typename HAD_NS extract<127 < sizeof...(Ts) ? 127 : 0 ,Ts...>::type*>(p));    
        case 128:return callable(*static_cast<typename HAD_NS extract<128 < sizeof...(Ts) ? 128 : 0 ,Ts...>::type*>(p));    
        case 129:return callable(*static_cast<typename HAD_NS extract<129 < sizeof...(Ts) ? 129 : 0 ,Ts...>::type*>(p));    
        case 130:return callable(*static_cast<typename HAD_NS extract<130 < sizeof...(Ts) ? 130 : 0 ,Ts...>::type*>(p));    
        case 131:return callable(*static_cast<typename HAD_NS extract<131 < sizeof...(Ts) ? 131 : 0 ,Ts...>::type*>(p));    
        case 132:return callable(*static_cast<typename HAD_NS extract<132 < sizeof...(Ts) ? 132 : 0 ,Ts...>::type*>(p));    
        case 133:return callable(*static_cast<typename HAD_NS extract<133 < sizeof...(Ts) ? 133 : 0 ,Ts...>::type*>(p));    
        case 134:return callable(*static_cast<typename HAD_NS extract<134 < sizeof...(Ts) ? 134 : 0 ,Ts...>::type*>(p));    
        case 135:return callable(*static_cast<typename HAD_NS extract<135 < sizeof...(Ts) ? 135 : 0 ,Ts...>::type*>(p));    
        case 136:return callable(*static_cast<typename HAD_NS extract<136 < sizeof...(Ts) ? 136 : 0 ,Ts...>::type*>(p));    
        case 137:return callable(*static_cast<typename HAD_NS extract<137 < sizeof...(Ts) ? 137 : 0 ,Ts...>::type*>(p));    
        case 138:return callable(*static_cast<typename HAD_NS extract<138 < sizeof...(Ts) ? 138 : 0 ,Ts...>::type*>(p));    
        case 139:return callable(*static_cast<typename HAD_NS extract<139 < sizeof...(Ts) ? 139 : 0 ,Ts...>::type*>(p));    
        case 140:return callable(*static_cast<typename HAD_NS extract<140 < sizeof...(Ts) ? 140 : 0 ,Ts...>::type*>(p));    
        case 141:return callable(*static_cast<typename HAD_NS extract<141 < sizeof...(Ts) ? 141 : 0 ,Ts...>::type*>(p));    
        case 142:return callable(*static_cast<typename HAD_NS extract<142 < sizeof...(Ts) ? 142 : 0 ,Ts...>::type*>(p));    
        case 143:return callable(*static_cast<typename HAD_NS extract<143 < sizeof...(Ts) ? 143 : 0 ,Ts...>::type*>(p));    
        case 144:return callable(*static_cast<typename HAD_NS extract<144 < sizeof...(Ts) ? 144 : 0 ,Ts...>::type*>(p));    
        case 145:return callable(*static_cast<typename HAD_NS extract<145 < sizeof...(Ts) ? 145 : 0 ,Ts...>::type*>(p));    
        case 146:return callable(*static_cast<typename HAD_NS extract<146 < sizeof...(Ts) ? 146 : 0 ,Ts...>::type*>(p));    
        case 147:return callable(*static_cast<typename HAD_NS extract<147 < sizeof...(Ts) ? 147 : 0 ,Ts...>::type*>(p));    
        case 148:return callable(*static_cast<typename HAD_NS extract<148 < sizeof...(Ts) ? 148 : 0 ,Ts...>::type*>(p));    
        case 149:return callable(*static_cast<typename HAD_NS extract<149 < sizeof...(Ts) ? 149 : 0 ,Ts...>::type*>(p));    
        case 150:return callable(*static_cast<typename HAD_NS extract<150 < sizeof...(Ts) ? 150 : 0 ,Ts...>::type*>(p));    
        case 151:return callable(*static_cast<typename HAD_NS extract<151 < sizeof...(Ts) ? 151 : 0 ,Ts...>::type*>(p));    
        case 152:return callable(*static_cast<typename HAD_NS extract<152 < sizeof...(Ts) ? 152 : 0 ,Ts...>::type*>(p));    
        case 153:return callable(*static_cast<typename HAD_NS extract<153 < sizeof...(Ts) ? 153 : 0 ,Ts...>::type*>(p));    
        case 154:return callable(*static_cast<typename HAD_NS extract<154 < sizeof...(Ts) ? 154 : 0 ,Ts...>::type*>(p));    
        case 155:return callable(*static_cast<typename HAD_NS extract<155 < sizeof...(Ts) ? 155 : 0 ,Ts...>::type*>(p));    
        case 156:return callable(*static_cast<typename HAD_NS extract<156 < sizeof...(Ts) ? 156 : 0 ,Ts...>::type*>(p));    
        case 157:return callable(*static_cast<typename HAD_NS extract<157 < sizeof...(Ts) ? 157 : 0 ,Ts...>::type*>(p));    
        case 158:return callable(*static_cast<typename HAD_NS extract<158 < sizeof...(Ts) ? 158 : 0 ,Ts...>::type*>(p));    
        case 159:return callable(*static_cast<typename HAD_NS extract<159 < sizeof...(Ts) ? 159 : 0 ,Ts...>::type*>(p));    
        case 160:return callable(*static_cast<typename HAD_NS extract<160 < sizeof...(Ts) ? 160 : 0 ,Ts...>::type*>(p));    
        case 161:return callable(*static_cast<typename HAD_NS extract<161 < sizeof...(Ts) ? 161 : 0 ,Ts...>::type*>(p));    
        case 162:return callable(*static_cast<typename HAD_NS extract<162 < sizeof...(Ts) ? 162 : 0 ,Ts...>::type*>(p));    
        case 163:return callable(*static_cast<typename HAD_NS extract<163 < sizeof...(Ts) ? 163 : 0 ,Ts...>::type*>(p));    
        case 164:return callable(*static_cast<typename HAD_NS extract<164 < sizeof...(Ts) ? 164 : 0 ,Ts...>::type*>(p));    
        case 165:return callable(*static_cast<typename HAD_NS extract<165 < sizeof...(Ts) ? 165 : 0 ,Ts...>::type*>(p));    
        case 166:return callable(*static_cast<typename HAD_NS extract<166 < sizeof...(Ts) ? 166 : 0 ,Ts...>::type*>(p));    
        case 167:return callable(*static_cast<typename HAD_NS extract<167 < sizeof...(Ts) ? 167 : 0 ,Ts...>::type*>(p));    
        case 168:return callable(*static_cast<typename HAD_NS extract<168 < sizeof...(Ts) ? 168 : 0 ,Ts...>::type*>(p));    
        case 169:return callable(*static_cast<typename HAD_NS extract<169 < sizeof...(Ts) ? 169 : 0 ,Ts...>::type*>(p));    
        case 170:return callable(*static_cast<typename HAD_NS extract<170 < sizeof...(Ts) ? 170 : 0 ,Ts...>::type*>(p));    
        case 171:return callable(*static_cast<typename HAD_NS extract<171 < sizeof...(Ts) ? 171 : 0 ,Ts...>::type*>(p));    
        case 172:return callable(*static_cast<typename HAD_NS extract<172 < sizeof...(Ts) ? 172 : 0 ,Ts...>::type*>(p));    
        case 173:return callable(*static_cast<typename HAD_NS extract<173 < sizeof...(Ts) ? 173 : 0 ,Ts...>::type*>(p));    
        case 174:return callable(*static_cast<typename HAD_NS extract<174 < sizeof...(Ts) ? 174 : 0 ,Ts...>::type*>(p));    
        case 175:return callable(*static_cast<typename HAD_NS extract<175 < sizeof...(Ts) ? 175 : 0 ,Ts...>::type*>(p));    
        case 176:return callable(*static_cast<typename HAD_NS extract<176 < sizeof...(Ts) ? 176 : 0 ,Ts...>::type*>(p));    
        case 177:return callable(*static_cast<typename HAD_NS extract<177 < sizeof...(Ts) ? 177 : 0 ,Ts...>::type*>(p));    
        case 178:return callable(*static_cast<typename HAD_NS extract<178 < sizeof...(Ts) ? 178 : 0 ,Ts...>::type*>(p));    
        case 179:return callable(*static_cast<typename HAD_NS extract<179 < sizeof...(Ts) ? 179 : 0 ,Ts...>::type*>(p));    
        case 180:return callable(*static_cast<typename HAD_NS extract<180 < sizeof...(Ts) ? 180 : 0 ,Ts...>::type*>(p));    
        case 181:return callable(*static_cast<typename HAD_NS extract<181 < sizeof...(Ts) ? 181 : 0 ,Ts...>::type*>(p));    
        case 182:return callable(*static_cast<typename HAD_NS extract<182 < sizeof...(Ts) ? 182 : 0 ,Ts...>::type*>(p));    
        case 183:return callable(*static_cast<typename HAD_NS extract<183 < sizeof...(Ts) ? 183 : 0 ,Ts...>::type*>(p));    
        case 184:return callable(*static_cast<typename HAD_NS extract<184 < sizeof...(Ts) ? 184 : 0 ,Ts...>::type*>(p));    
        case 185:return callable(*static_cast<typename HAD_NS extract<185 < sizeof...(Ts) ? 185 : 0 ,Ts...>::type*>(p));    
        case 186:return callable(*static_cast<typename HAD_NS extract<186 < sizeof...(Ts) ? 186 : 0 ,Ts...>::type*>(p));    
        case 187:return callable(*static_cast<typename HAD_NS extract<187 < sizeof...(Ts) ? 187 : 0 ,Ts...>::type*>(p));    
        case 188:return callable(*static_cast<typename HAD_NS extract<188 < sizeof...(Ts) ? 188 : 0 ,Ts...>::type*>(p));    
        case 189:return callable(*static_cast<typename HAD_NS extract<189 < sizeof...(Ts) ? 189 : 0 ,Ts...>::type*>(p));    
        case 190:return callable(*static_cast<typename HAD_NS extract<190 < sizeof...(Ts) ? 190 : 0 ,Ts...>::type*>(p));    
        case 191:return callable(*static_cast<typename HAD_NS extract<191 < sizeof...(Ts) ? 191 : 0 ,Ts...>::type*>(p));    
        case 192:return callable(*static_cast<typename HAD_NS extract<192 < sizeof...(Ts) ? 192 : 0 ,Ts...>::type*>(p));    
        case 193:return callable(*static_cast<typename HAD_NS extract<193 < sizeof...(Ts) ? 193 : 0 ,Ts...>::type*>(p));    
        case 194:return callable(*static_cast<typename HAD_NS extract<194 < sizeof...(Ts) ? 194 : 0 ,Ts...>::type*>(p));    
        case 195:return callable(*static_cast<typename HAD_NS extract<195 < sizeof...(Ts) ? 195 : 0 ,Ts...>::type*>(p));    
        case 196:return callable(*static_cast<typename HAD_NS extract<196 < sizeof...(Ts) ? 196 : 0 ,Ts...>::type*>(p));    
        case 197:return callable(*static_cast<typename HAD_NS extract<197 < sizeof...(Ts) ? 197 : 0 ,Ts...>::type*>(p));    
        case 198:return callable(*static_cast<typename HAD_NS extract<198 < sizeof...(Ts) ? 198 : 0 ,Ts...>::type*>(p));    
        case 199:return callable(*static_cast<typename HAD_NS extract<199 < sizeof...(Ts) ? 199 : 0 ,Ts...>::type*>(p));    
        case 200:return callable(*static_cast<typename HAD_NS extract<200 < sizeof...(Ts) ? 200 : 0 ,Ts...>::type*>(p));    
        case 201:return callable(*static_cast<typename HAD_NS extract<201 < sizeof...(Ts) ? 201 : 0 ,Ts...>::type*>(p));    
        case 202:return callable(*static_cast<typename HAD_NS extract<202 < sizeof...(Ts) ? 202 : 0 ,Ts...>::type*>(p));    
        case 203:return callable(*static_cast<typename HAD_NS extract<203 < sizeof...(Ts) ? 203 : 0 ,Ts...>::type*>(p));    
        case 204:return callable(*static_cast<typename HAD_NS extract<204 < sizeof...(Ts) ? 204 : 0 ,Ts...>::type*>(p));    
        case 205:return callable(*static_cast<typename HAD_NS extract<205 < sizeof...(Ts) ? 205 : 0 ,Ts...>::type*>(p));    
        case 206:return callable(*static_cast<typename HAD_NS extract<206 < sizeof...(Ts) ? 206 : 0 ,Ts...>::type*>(p));    
        case 207:return callable(*static_cast<typename HAD_NS extract<207 < sizeof...(Ts) ? 207 : 0 ,Ts...>::type*>(p));    
        case 208:return callable(*static_cast<typename HAD_NS extract<208 < sizeof...(Ts) ? 208 : 0 ,Ts...>::type*>(p));    
        case 209:return callable(*static_cast<typename HAD_NS extract<209 < sizeof...(Ts) ? 209 : 0 ,Ts...>::type*>(p));    
        case 210:return callable(*static_cast<typename HAD_NS extract<210 < sizeof...(Ts) ? 210 : 0 ,Ts...>::type*>(p));    
        case 211:return callable(*static_cast<typename HAD_NS extract<211 < sizeof...(Ts) ? 211 : 0 ,Ts...>::type*>(p));    
        case 212:return callable(*static_cast<typename HAD_NS extract<212 < sizeof...(Ts) ? 212 : 0 ,Ts...>::type*>(p));    
        case 213:return callable(*static_cast<typename HAD_NS extract<213 < sizeof...(Ts) ? 213 : 0 ,Ts...>::type*>(p));    
        case 214:return callable(*static_cast<typename HAD_NS extract<214 < sizeof...(Ts) ? 214 : 0 ,Ts...>::type*>(p));    
        case 215:return callable(*static_cast<typename HAD_NS extract<215 < sizeof...(Ts) ? 215 : 0 ,Ts...>::type*>(p));    
        case 216:return callable(*static_cast<typename HAD_NS extract<216 < sizeof...(Ts) ? 216 : 0 ,Ts...>::type*>(p));    
        case 217:return callable(*static_cast<typename HAD_NS extract<217 < sizeof...(Ts) ? 217 : 0 ,Ts...>::type*>(p));    
        case 218:return callable(*static_cast<typename HAD_NS extract<218 < sizeof...(Ts) ? 218 : 0 ,Ts...>::type*>(p));    
        case 219:return callable(*static_cast<typename HAD_NS extract<219 < sizeof...(Ts) ? 219 : 0 ,Ts...>::type*>(p));    
        case 220:return callable(*static_cast<typename HAD_NS extract<220 < sizeof...(Ts) ? 220 : 0 ,Ts...>::type*>(p));    
        case 221:return callable(*static_cast<typename HAD_NS extract<221 < sizeof...(Ts) ? 221 : 0 ,Ts...>::type*>(p));    
        case 222:return callable(*static_cast<typename HAD_NS extract<222 < sizeof...(Ts) ? 222 : 0 ,Ts...>::type*>(p));    
        case 223:return callable(*static_cast<typename HAD_NS extract<223 < sizeof...(Ts) ? 223 : 0 ,Ts...>::type*>(p));    
        case 224:return callable(*static_cast<typename HAD_NS extract<224 < sizeof...(Ts) ? 224 : 0 ,Ts...>::type*>(p));    
        case 225:return callable(*static_cast<typename HAD_NS extract<225 < sizeof...(Ts) ? 225 : 0 ,Ts...>::type*>(p));    
        case 226:return callable(*static_cast<typename HAD_NS extract<226 < sizeof...(Ts) ? 226 : 0 ,Ts...>::type*>(p));    
        case 227:return callable(*static_cast<typename HAD_NS extract<227 < sizeof...(Ts) ? 227 : 0 ,Ts...>::type*>(p));    
        case 228:return callable(*static_cast<typename HAD_NS extract<228 < sizeof...(Ts) ? 228 : 0 ,Ts...>::type*>(p));    
        case 229:return callable(*static_cast<typename HAD_NS extract<229 < sizeof...(Ts) ? 229 : 0 ,Ts...>::type*>(p));    
        case 230:return callable(*static_cast<typename HAD_NS extract<230 < sizeof...(Ts) ? 230 : 0 ,Ts...>::type*>(p));    
        case 231:return callable(*static_cast<typename HAD_NS extract<231 < sizeof...(Ts) ? 231 : 0 ,Ts...>::type*>(p));    
        case 232:return callable(*static_cast<typename HAD_NS extract<232 < sizeof...(Ts) ? 232 : 0 ,Ts...>::type*>(p));    
        case 233:return callable(*static_cast<typename HAD_NS extract<233 < sizeof...(Ts) ? 233 : 0 ,Ts...>::type*>(p));    
        case 234:return callable(*static_cast<typename HAD_NS extract<234 < sizeof...(Ts) ? 234 : 0 ,Ts...>::type*>(p));    
        case 235:return callable(*static_cast<typename HAD_NS extract<235 < sizeof...(Ts) ? 235 : 0 ,Ts...>::type*>(p));    
        case 236:return callable(*static_cast<typename HAD_NS extract<236 < sizeof...(Ts) ? 236 : 0 ,Ts...>::type*>(p));    
        case 237:return callable(*static_cast<typename HAD_NS extract<237 < sizeof...(Ts) ? 237 : 0 ,Ts...>::type*>(p));    
        case 238:return callable(*static_cast<typename HAD_NS extract<238 < sizeof...(Ts) ? 238 : 0 ,Ts...>::type*>(p));    
        case 239:return callable(*static_cast<typename HAD_NS extract<239 < sizeof...(Ts) ? 239 : 0 ,Ts...>::type*>(p));    
        case 240:return callable(*static_cast<typename HAD_NS extract<240 < sizeof...(Ts) ? 240 : 0 ,Ts...>::type*>(p));    
        case 241:return callable(*static_cast<typename HAD_NS extract<241 < sizeof...(Ts) ? 241 : 0 ,Ts...>::type*>(p));    
        case 242:return callable(*static_cast<typename HAD_NS extract<242 < sizeof...(Ts) ? 242 : 0 ,Ts...>::type*>(p));    
        case 243:return callable(*static_cast<typename HAD_NS extract<243 < sizeof...(Ts) ? 243 : 0 ,Ts...>::type*>(p));    
        case 244:return callable(*static_cast<typename HAD_NS extract<244 < sizeof...(Ts) ? 244 : 0 ,Ts...>::type*>(p));    
        case 245:return callable(*static_cast<typename HAD_NS extract<245 < sizeof...(Ts) ? 245 : 0 ,Ts...>::type*>(p));    
        case 246:return callable(*static_cast<typename HAD_NS extract<246 < sizeof...(Ts) ? 246 : 0 ,Ts...>::type*>(p));    
        case 247:return callable(*static_cast<typename HAD_NS extract<247 < sizeof...(Ts) ? 247 : 0 ,Ts...>::type*>(p));    
        case 248:return callable(*static_cast<typename HAD_NS extract<248 < sizeof...(Ts) ? 248 : 0 ,Ts...>::type*>(p));    
        case 249:return callable(*static_cast<typename HAD_NS extract<249 < sizeof...(Ts) ? 249 : 0 ,Ts...>::type*>(p));    
        case 250:return callable(*static_cast<typename HAD_NS extract<250 < sizeof...(Ts) ? 250 : 0 ,Ts...>::type*>(p));    
        case 251:return callable(*static_cast<typename HAD_NS extract<251 < sizeof...(Ts) ? 251 : 0 ,Ts...>::type*>(p));    
        case 252:return callable(*static_cast<typename HAD_NS extract<252 < sizeof...(Ts) ? 252 : 0 ,Ts...>::type*>(p));    
        case 253:return callable(*static_cast<typename HAD_NS extract<253 < sizeof...(Ts) ? 253 : 0 ,Ts...>::type*>(p));    
        case 254:return callable(*static_cast<typename HAD_NS extract<254 < sizeof...(Ts) ? 254 : 0 ,Ts...>::type*>(p));    
        case 255:return callable(*static_cast<typename HAD_NS extract<255 < sizeof...(Ts) ? 255 : 0 ,Ts...>::type*>(p));    
    }
    unreachable();
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


template<typename T,typename...Ts>
T* variant_cast(variant<Ts...>* variant) {
    if (index_of<T, Ts...>::value != variant->index()) {
        return nullptr;
    }
    return reinterpret_cast<T*>(variant->x_get_void_1_401928());
}


template<typename T,typename...Ts>
const T* variant_cast(const variant<Ts...>* variant) {
    if (index_of<T, Ts...>::value != variant->index()) {
        return nullptr;
    }
    return reinterpret_cast<const T*>(variant->x_get_void_1_401928());
}

template<typename T,typename...Ts>
volatile T* variant_cast(volatile variant<Ts...>* variant) {
    if (index_of<T, Ts...>::value != variant->index()) {
        return nullptr;
    }
    return reinterpret_cast<volatile T*>(variant->x_get_void_1_401928());
}

template<typename T,typename...Ts>
const volatile T* variant_cast(const volatile variant<Ts...>* variant) {
    if (index_of<T, Ts...>::value != variant->index()) {
        return nullptr;
    }
    return reinterpret_cast<const T*>(variant->x_get_void_1_401928());
}




HAD_NAMESPACE_END


namespace std {

#define HAD_GET_V(THIS_QUALIFIERS) \
template<size_t I ,typename... Ts> \
auto get(HAD_NS variant<Ts...> THIS_QUALIFIERS v) -> typename HAD_NS extract<I,Ts...>::type THIS_QUALIFIERS { \
    using T = HAD_NS extract_t<I, Ts...>; \
    return static_cast<T THIS_QUALIFIERS>(*reinterpret_cast<T*>(v.x_get_void_1_401928())); \
}

HAD_DEFINE_ALL_MEMBER_FUNCTIONS_WITH_QUALIIERS(HAD_GET_V);

#undef HAD_GET_V
}



#endif // !HAD_VARIANT_HPP
