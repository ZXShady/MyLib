#pragma once
#ifndef HAD_FUNCTIONAL_HPP
#define HAD_FUNCTIONAL_HPP
#include <had/memory/unique_ptr.hpp>
#include <had/type_traits/forward.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/is_same.hpp>
#include <had/utility/declval.hpp>
HAD_NAMESPACE_BEGIN

template<typename>
class function;


template<typename ReturnType,typename... Args>
class function<ReturnType(Args...)> {
public:
    using this_type = function;
    using function_type = ReturnType(Args...);
    using value_type = ReturnType(Args...);
    using return_type = ReturnType;
    function(function_type* f)
        : mCallable(HAD_NS make_unique<Callable<function_type*> >(f) ) {}

    template<typename Fn,enable_if_t<!is_same_v<this_type,Fn> > = 0 >
    function(Fn&& fn) : mCallable(HAD_NS make_unique<Callable<Fn>>(HAD_NS forward<Fn>(fn))) {
        static_assert(is_same_v<return_type, decltype(fn(HAD_NS declval<Args>()...))>," had::function(Fn&&) Fn must have the same return type of had::function<F>");
    }

    function(const function& other) 
        :mCallable(other.mCallable->clone())
    {
    }

    function(function&& other) 
        :mCallable(other.mCallable.release() )
    {
    }

    ReturnType operator()(Args... args) {
        return mCallable->call(args...);
    }

    template<typename F>
    this_type& operator=(F&& other) noexcept {
        function copy(HAD_NS forward<F>(other));
        this->swap(copy);
        return *this;
    }

    HAD_CONSTEXPR14 void swap(function& other) noexcept {
        using ::had::swap;
        swap(mCallable, other.mCallable);
    }
    friend HAD_CONSTEXPR14 void swap(function& a, function& b) noexcept {
        a.swap(b);
        
    }
    
    explicit operator bool() const noexcept {
        return mCallable != nullptr;
    }
    struct ICallable {
        virtual ReturnType call(Args...) = 0;
        virtual ICallable* clone() = 0;
        virtual ~ICallable() = default;
    };

    template<typename F>
    class Callable : ICallable {
    private:
        F functor;
    public:
        Callable(F f) : functor(HAD_NS move(f)) {}
        ReturnType call(Args... args) override {
            return functor(args...);
        };
        Callable* clone() override { return new Callable(*this); }
        ~Callable() override = default;

    };


    had::unique_ptr<ICallable> mCallable;
};

HAD_NAMESPACE_END

#endif // !HAD_FUNCTIONAL_HPP
