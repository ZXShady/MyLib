#pragma once
#ifndef HAD_SPAN_HPP
#define HAD_SPAN_HPP


#include <had/stdint.hpp>
#include <had/version/namespace.hpp>
#include <had/version/attributes/inline_namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/iterator/reverse_iterator.hpp>
#include <had/exceptions.hpp>

HAD_NAMESPACE_BEGIN

template<typename ValueType>
class span {
public:
    using this_type              = span;
    using size_type              = size_t;
    using value_type             = ValueType;
    using pointer_type           = value_type*;
    using reference_type         = value_type&;
    using const_reference_type   = const value_type&;
    using const_pointer_type     = const value_type*;
    using const_iterator         = const value_type*;
    using const_reverse_iterator = reverse_iterator<const value_type>;

public:

    span(std::nullptr_t) = delete;
    span(std::nullptr_t,size_type) = delete;
    span(std::nullptr_t,std::nullptr_t) = delete;

    HAD_CONSTEXPR23 span() : begin_(nullptr), end_(nullptr) {}
    HAD_CONSTEXPR23 span(const span& other) noexcept
        : begin_(other.begin_)
        , end_(other.end_) {}
    HAD_CONSTEXPR23 span(span&& other) noexcept
        : begin_(exchange(other.begin_,nullptr))
        , end_(exchange(other.end_,nullptr)) {}
    HAD_CONSTEXPR23 span& operator=(const span& other) & noexcept {
        begin_ = other.begin_;
        end_   = other.end_;
    };

    HAD_CONSTEXPR23 span& operator=(span&& other) & noexcept {
    };

    HAD_CONSTEXPR23 span(const_pointer_type str, size_type length) noexcept
        : begin_(str)
        , end_(str + length) {
    }

    HAD_CONSTEXPR23 span(const_pointer_type begin, const_pointer_type end) noexcept
        : begin_(begin)
        , end_(end) {
    }
    HAD_NODISCARD size_type size_in_bytes() const noexcept { return size() * sizeof(value_type); }
    HAD_NODISCARD bool is_empty() const noexcept { return begin_ == end_; }
    HAD_NODISCARD size_type size() const noexcept { return static_cast<size_type>(end_ - begin_); }
    HAD_NODISCARD const_pointer_type data() const noexcept   { return begin_; }

    HAD_NODISCARD const_reference_type at(size_type index) const {
        if (index >= length())
            throw out_of_range("Out of bounds access. from span<CharT>::at method.");


        return begin_[index];
    }
    HAD_NODISCARD span subspan(size_type offset,size_type count) const noexcept {
        
        return span(&begin_[offset], &begin_[offset + count])
    }
    HAD_NODISCARD const_reference_type operator[](size_type index) const noexcept {return begin_[index];}
    HAD_NODISCARD const_reference_type back() const noexcept {return begin_[0];}
    HAD_NODISCARD const_reference_type front() const noexcept {return begin_[length() - 1];}
    HAD_NODISCARD const_iterator begin() const noexcept { return begin_; }
    HAD_NODISCARD const_iterator end() const noexcept   { return end_; }
    HAD_NODISCARD const_reverse_iterator rbegin() const noexcept { return end_ - 1; }
    HAD_NODISCARD const_reverse_iterator rend() const noexcept { return begin_ - 1; }
    HAD_NODISCARD const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    HAD_NODISCARD const_reverse_iterator crend() const noexcept { return rend(); }
    HAD_NODISCARD const_iterator cbegin() const noexcept { return begin(); }
    HAD_NODISCARD const_iterator cend() const noexcept { return end(); }

private:
    const_pointer_type begin_;
    const_pointer_type end_;

};

HAD_NAMESPACE_END

#endif // !HAD_SPAN_HPP
