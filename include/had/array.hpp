#pragma once
#ifndef HAD_ARRAY_HPP
#define HAD_ARRAY_HPP 1

#include <had/stdint.hpp>

#include <had/iterator/reverse_iterator.hpp>

#include <cstring>
#include <exception>
#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/type_traits/is_object.hpp>
#include <had/algorithm/fill.hpp>
#include <had/pair.hpp>
#include <had/format/formatter.hpp>
#include <had/macros.hpp>
HAD_NAMESPACE_BEGIN

template<typename ValueType,std::size_t Size>
class array {
public:
    static_assert(Size != 0,"had::array<T,N> | Size cannot be zero!");
    //static_assert(is_object_v<ValueType>, "had::array prohibits non object types to \
    //                                    \n be used as value types.");

    using this_type = array;
    using size_type = size_t;
    using value_type = ValueType;
    using pointer_type = value_type*;
    using reference_type = value_type&;
    using const_pointer_type = const value_type*;
    using const_reference_type = const value_type&;
    using const_iterator = const value_type*;
    using iterator = value_type*;
    using reverse_iterator = HAD_NS reverse_iterator<value_type>;
    using const_reverse_iterator = HAD_NS reverse_iterator<const value_type>;

    constexpr static size_type npos = static_cast<size_type>(-1);
    
    HAD_NODISCARD static constexpr size_type size() noexcept { return Size; }
    HAD_NODISCARD constexpr pointer_type data() noexcept { return unused_private_data_; }
    HAD_NODISCARD constexpr const_pointer_type data() const noexcept { return unused_private_data_; }
    HAD_NODISCARD constexpr reference_type back() noexcept { return data()[size()-1]; }
    HAD_NODISCARD constexpr reference_type front() noexcept { return data()[0]; }
    HAD_NODISCARD constexpr const_reference_type back() const noexcept { return data()[size() - 1]; }
    HAD_NODISCARD constexpr const_reference_type front() const noexcept { return data()[0]; }
    HAD_NODISCARD constexpr iterator begin() noexcept { return data() + 0; }
    HAD_NODISCARD constexpr iterator end() noexcept { return data() + size(); }
    HAD_NODISCARD constexpr const_iterator begin() const noexcept { return data() + 0; }
    HAD_NODISCARD constexpr const_iterator end() const noexcept { return data() + size(); }
    HAD_NODISCARD constexpr reverse_iterator rbegin() noexcept { return end() - 1; }
    HAD_NODISCARD constexpr reverse_iterator rend() noexcept { return begin() - 1; }
    HAD_NODISCARD constexpr const_reverse_iterator rbegin() const noexcept { return end() - 1; }
    HAD_NODISCARD constexpr const_reverse_iterator rend() const noexcept { return begin() - 1; }
    HAD_NODISCARD constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    HAD_NODISCARD constexpr const_reverse_iterator crend() const noexcept { return rend(); }
    HAD_NODISCARD constexpr const_iterator cbegin() const noexcept { return begin(); }
    HAD_NODISCARD constexpr const_iterator cend() const noexcept { return end(); }

    HAD_NODISCARD constexpr HAD_NS iterator_pair<iterator> iterator_pair() noexcept { return make_iterator_pair(begin(), end()); }
    HAD_NODISCARD constexpr HAD_NS iterator_pair<const_iterator> iterator_pair() const noexcept { return make_iterator_pair(begin(), end()); }

    HAD_NODISCARD constexpr reference_type at(size_type index) {
        if (index >= Size)
            throw std::exception("had::array::at() out of bounds\n Index was greater or equal to Size");
        return data()[index];
    }
    
    HAD_NODISCARD constexpr const_reference_type at(size_type index) const {
        if (index >= Size)
            throw std::exception("had::array::at() out of bounds\n Index was greater or equal to Size");
        return data()[index];
    }
    
    HAD_NODISCARD constexpr reference_type operator[](size_type index) noexcept {
        return data()[index];
    }
    HAD_NODISCARD constexpr const_reference_type operator[](size_type index) const noexcept {
        return data()[index];
    }


    HAD_NODISCARD constexpr size_type rfind(const value_type& value, size_type from_index = (Size-1)) const noexcept {
        for (size_type i = from_index; i > 0; i--)
            if (data()[i] == value)
                return i;
        return npos;
    }

    HAD_NODISCARD constexpr size_type find(const value_type& value,size_type from_index = 0) const noexcept {
        for (size_type i = from_index; i < Size; i++)
            if (data()[i] == value)
                return i;
        return npos;
    }

    HAD_NODISCARD constexpr bool contains(const value_type& value) const noexcept { 
        return find(value) != npos;
    }


    void fill(size_t N, const value_type& value = value_type()) {
        HAD_NS fill(this->iterator_pair(), value);
    }

    HAD_NODISCARD HAD_CONSTEXPR14 friend bool operator==(const this_type& a, const this_type& b) noexcept {
        return HAD_NS equal(a.iterator_pair(), b.begin());
    }
#ifndef HAD_CPP20
    HAD_NODISCARD HAD_CONSTEXPR14 friend bool operator!=(const this_type& a, const this_type& b) noexcept {
        return !(a == b);
    }
#endif // !HAD_CPP20


public: // aggregate initialization
    value_type unused_private_data_[Size];
};

template<typename T,size_t N>
struct formatter<array<T,N>> {
    constexpr static size_t reserve_amount = 0;
    static ::had::string to_string(const array<T,N>& input) {
        using formatter_t = ::had::formatter<T>;
        ::had::string str = "[ ";
        //str.reserve(reserve_amount);
        for (const auto& item : input) {
            str += formatter_t::to_string(item);
            str += ", ";
        }
        str.back() = ']';
        *(&str.back() - 1) = ' ';

        return str;
    }
};



#ifdef __cpp_deduction_guides

template<typename First,typename... Rest>
array(First, Rest...) -> array<First, 1 + sizeof...(Rest)>;

#endif // __cpp_deduction_guides

template<typename>            struct is_std_array                             : false_type {};
template<typename T,size_t N> struct is_std_array<array<T,N> >                : true_type  {};
template<typename T,size_t N> struct is_std_array<const array<T,N> >          : true_type  {};
template<typename T,size_t N> struct is_std_array<volatile array<T,N> >       : true_type  {};
template<typename T,size_t N> struct is_std_array<const volatile array<T,N> > : true_type  {};

#ifdef __cpp_variable_templates

template<typename T>
constexpr bool is_std_array_v = is_std_array<T>::value;

#endif // __cpp_variable_templates



HAD_NAMESPACE_END

#endif // !HAD_ARRAY_HPP