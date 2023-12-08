#pragma once
#ifndef HAD_BASIC_STRING_VIEW_HPP
#define HAD_BASIC_STRING_VIEW_HPP

#include <had/stdint.hpp>
#include <had/version/namespace.hpp>
#include <had/version/attributes/inline_namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/iterator/reverse_iterator.hpp>
#include <had/iterator/utilities.hpp>
#include <had/exceptions.hpp>
#include <had/utility/distance.hpp>
#include <had/cstring.h>

HAD_NAMESPACE_BEGIN

template<typename CharType>
class basic_string_view {
public:
    using this_type              = basic_string_view;
    using size_type              = size_t;
    using value_type             = CharType;
    using pointer_type           = value_type*;
    using reference_type         = value_type&;
    using const_reference_type   = const value_type&;
    using const_pointer_type     = const value_type*;
    
    using iterator               = const value_type*;
    using const_iterator         = const value_type*;

    using reverse_iterator       = HAD_NS reverse_iterator<const value_type>;
    using const_reverse_iterator = HAD_NS reverse_iterator<const value_type>;

    using iterator_pair_type   = HAD_NS iterator_pair<const_iterator>;
    using citerator_pair_type  = HAD_NS iterator_pair<const_iterator>;
    using riterator_pair_type  = HAD_NS iterator_pair<const_reverse_iterator>;
    using criterator_pair_type = HAD_NS iterator_pair<const_reverse_iterator>;

    constexpr static size_type npos = static_cast<size_type>(-1);
public:

    basic_string_view(std::nullptr_t) = delete;
    basic_string_view(std::nullptr_t,size_type) = delete;
    basic_string_view(std::nullptr_t,std::nullptr_t) = delete;


    basic_string_view() : mLength(0), mBegin(nullptr) {}

    HAD_CONSTEXPR20 basic_string_view(const_pointer_type str) noexcept
        : mLength(HAD_NS string_length(str))
        , mBegin(str) {
    }

    HAD_CONSTEXPR20 basic_string_view(const_pointer_type str, size_type length) noexcept
        : mLength(length)
        , mBegin(str) {
    }

    HAD_CONSTEXPR20 basic_string_view(const_pointer_type begin, const_pointer_type end) noexcept
        : mLength(HAD_NS distance(begin,end) )
        , mBegin(begin) {
    }

    HAD_NODISCARD size_t length() const noexcept { return mLength; }
    HAD_NODISCARD const_pointer_type data() const noexcept   { return begin(); }
    HAD_NODISCARD const_pointer_type c_str() const noexcept  { return begin(); }

    HAD_NODISCARD const_reference_type at(size_type index) const {
        if (index >= length())
            throw out_of_range(
            "Out of bounds access from -> function "
            __FUNCSIG__
            );

        return data()[index];
    }

    HAD_NODISCARD HAD_CONSTEXPR14 const_reference_type back() const noexcept {return *rbegin();}
    HAD_NODISCARD HAD_CONSTEXPR14 const_reference_type front() const noexcept {return *begin();}

    HAD_NODISCARD HAD_CONSTEXPR14 const_iterator begin() const noexcept { return mBegin; }
    HAD_NODISCARD HAD_CONSTEXPR14 const_iterator end() const noexcept   { return begin() + mLength; }
    HAD_NODISCARD HAD_CONSTEXPR14 const_reverse_iterator rbegin() const noexcept { return end() - 1; }
    HAD_NODISCARD HAD_CONSTEXPR14 const_reverse_iterator rend() const noexcept { return begin() - 1; }
    HAD_NODISCARD HAD_CONSTEXPR14 const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    HAD_NODISCARD HAD_CONSTEXPR14 const_reverse_iterator crend() const noexcept { return rend(); }
    HAD_NODISCARD HAD_CONSTEXPR14 const_iterator cbegin() const noexcept { return begin(); }
    HAD_NODISCARD HAD_CONSTEXPR14 const_iterator cend() const noexcept { return end(); }
    HAD_NODISCARD HAD_CONSTEXPR14 citerator_pair_type iterator_pair() const noexcept {
        return make_iterator_pair(cbegin(), cend());
    };
    HAD_NODISCARD HAD_CONSTEXPR14 criterator_pair_type riterator_pair() const noexcept {
        return make_iterator_pair(crbegin(), crend());
    };
    HAD_NODISCARD HAD_CONSTEXPR14 citerator_pair_type citerator_pair() const noexcept {
        return make_iterator_pair(cbegin(), cend());
    };
    HAD_NODISCARD HAD_CONSTEXPR14 criterator_pair_type criterator_pair() const noexcept {
        return make_iterator_pair(crbegin(), crend());
    };

    HAD_NODISCARD HAD_CONSTEXPR14 const_reference_type operator[](size_type index) const noexcept {return data()[index];}

    HAD_NODISCARD size_type rfind(this_type str) const noexcept {
        const size_type len = length();
        if (len < str.length())
            return npos;

        const size_type diff = (len - str.length());
        // "Hello" "ell"
        //  
        auto my_begin = begin();
        for (size_type i = diff; i != 0; i--) {
            auto pair = HAD_NS make_iterator_pair(my_begin + i, my_begin + i + str.length());
            if (HAD_NS equal(pair, str.data())) {
                return i;
            }
        }
        return npos;
    }

    HAD_NODISCARD size_type find(this_type str, size_type from_index = 0) const noexcept {
        const size_type len = length();
        if (len < str.length())
            return npos;

        const size_type diff = (len - str.length()) + 1;

        for (size_type i = from_index; i < diff; i++) {
            auto pair = HAD_NS make_iterator_pair(begin() + i, begin() + i + str.length());
            if (HAD_NS equal(pair,str.data())) {
                return i;
            }
        }
        return npos;
    }

    friend HAD_NODISCARD bool operator==(this_type a, this_type b) noexcept {
        return a.length() == b.length()
            && a[0] == b[0]
            && HAD_NS equal(HAD_NS make_iterator_pair(a.begin()+ 1,a.end()), b.begin() + 1);
    }
#ifndef HAD_CPP20
    friend HAD_NODISCARD bool operator!=(this_type a, this_type b) noexcept {
        return !(a == b);
    }
#endif // !HAD_CPP20
private:
    const_pointer_type mBegin;
    size_type mLength;
};

using string_view    = basic_string_view<char>;
using wstring_view   = basic_string_view<wchar_t>;
#ifdef __cpp_char8_t
using u8string_view  = basic_string_view<char8_t>;
#endif
using u16string_view = basic_string_view<char16_t>;
using u32string_view = basic_string_view<char32_t>;

HAD_INLINE_NAMESPACE namespace literals {
HAD_INLINE_NAMESPACE namespace string_view_literals {

HAD_NODISCARD inline ::had::string_view operator""_sv(const char* str, ::had::size_t len) noexcept {
    return { str,len };
}

HAD_NODISCARD inline ::had::wstring_view operator""_sv(const wchar_t* str, ::had::size_t len) noexcept {
    return { str,len };
}
#ifdef __cpp_char8_t
HAD_NODISCARD inline ::had::u8string_view operator""_sv(const char8_t* str, ::had::size_t len) noexcept {
    return { str,len };
}
#endif 

HAD_NODISCARD inline ::had::u16string_view operator""_sv(const char16_t* str, ::had::size_t len) noexcept {
    return { str,len };
}

HAD_NODISCARD inline ::had::u32string_view operator""_sv(const char32_t* str, ::had::size_t len) noexcept {
    return { str,len };
}

} // namespace string_literals 
} // namespace literals

HAD_NAMESPACE_END
#endif // !BASIC_STRING_VIEW_HPP