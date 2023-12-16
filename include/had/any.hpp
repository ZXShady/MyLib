#pragma once
#ifndef HAD_ANY_HPP
#define HAD_ANY_HPP

#include <typeinfo>
#include <had/type_traits/move.hpp>
#include <stdexcept>
#include <had/version/attributes/constexpr.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/type_traits/addressof.hpp>
#include <had/type_traits/is_same.hpp>
#include <had/utility/swap.hpp>
#include <had/memory/unique_ptr.hpp>

class bad_any_cast : public std::exception {
public:
    using base = std::exception;
    bad_any_cast(const char* what = "bad_any_cast") : base(what) {}

    HAD_NODISCARD char const* what() const override {
        return "bad_any_cast";
    }

};
HAD_NAMESPACE_BEGIN 

class any {
private:
    [[noreturn]] void XBadCast() {
        throw bad_any_cast{};
    }
public:
    any() noexcept
        : mTypeInfo(&typeid(void))
        , mPtr(nullptr) {
    };

    template<typename T,enable_if_t<!is_same_v<T,any>> = 0 >
    any(T&& value) 
        : mTypeInfo(&typeid(value))
        , mPtr(HAD_NS make_unique<object<typename remove_cvref<T>::type>>(HAD_NS forward<T>(value)))
    {
    }

    any(const any& other) 
        : mTypeInfo(&other.type())
        , mPtr(static_cast<object_base*>(other.mPtr->clone()))
    {
    }

    any(any&& other) noexcept
        : mTypeInfo(&other.type())
        , mPtr(other.mPtr.release())
    {
    }

    template<typename T,typename enable_if<!is_same<T,any>::value>::type = 0 >
    any& operator=(T&& value)  {
        any copy(HAD_NS forward<T>(value));
        this->swap(copy);
        return *this;
    }

    any& operator=(const any& other) {
        any copy(other);
        this->swap(copy);
        return *this;
    }

    any& operator=(any&& other) noexcept {
        mTypeInfo = &other.type();
        mPtr = HAD_NS move(other.mPtr);
        return *this;
    }

    void swap(any& other) noexcept {
        using ::had::swap;
        swap(mTypeInfo, other.mTypeInfo);
        swap(mPtr, other.mPtr);
    }

    friend void swap(any& a, any& b) noexcept {
        a.swap(b);
    }
    template<typename T>
    T& cast() {
        if (has_value() && typeid(T).hash_code() != mTypeInfo->hash_code()) {
            XBadCast();
        }
        return *reinterpret_cast<T*>(mPtr->get_ptr());
    }
    
    HAD_NODISCARD explicit operator bool() const noexcept {
        return has_value();
    }
    HAD_NODISCARD bool has_value() const noexcept {
        return mPtr != nullptr;
    }

    void reset() noexcept {
        mTypeInfo = &typeid(void);
        mPtr.reset();
    }

    template<typename T,typename... Args>
    typename decay<T>::type& emplace(Args&&... args) {
        using decay_T = typename decay<T>::type;
        any copy{ decay_T{HAD_NS forward<Args>(args)...} };
        this->swap(copy);
        return *static_cast<decay_T*>(this->mPtr->get_ptr());
    }

    HAD_NODISCARD HAD_CONSTEXPR14 const std::type_info& type() const noexcept {
        return *mTypeInfo;
    }

    HAD_NODISCARD HAD_CONSTEXPR14 decltype(typeid(void).hash_code()) id() const noexcept {
        return mTypeInfo->hash_code();
    }


    struct object_base {
        virtual void* get_ptr() = 0;
        virtual object_base* clone() = 0;
        virtual ~object_base() {};
    };

    template<typename T>
    struct object : object_base {
        T obj;


        object(const object&) = default;
        object(object&&) = default;
        object(T t)      : obj(HAD_NS move(t)) {};
        void* get_ptr() override { return static_cast<void*>(HAD_NS addressof(obj)); }
        object* clone() override { return new object(*this); }
        ~object() override {};
    };
    
    const std::type_info* mTypeInfo;
    unique_ptr<object_base> mPtr;

};

template<typename T>
T& any_cast(any& any) {
    return any.template cast<T>();
}

template<typename T>
T* any_cast(any* any) {
    if (any->type() != typeid(T)) {
        return nullptr;
    }
    return HAD_NS addressof(any->template cast<T>());
}

template<typename T>
const T* any_cast(const any* any) {
    if (any->type() != typeid(T)) {
        return nullptr;
    }
    return HAD_NS addressof(any->template cast<T>());
}
HAD_NAMESPACE_END

#endif // !HAD_ANY_HPP
