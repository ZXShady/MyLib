#pragma once
#ifndef HAD_STRING_HPP
#define HAD_STRING_HPP 1

#include <iosfwd>
#include <cstring>

#include <had/string_view.hpp>
#include <had/stdint.hpp>
#include <had/utility/exchange.hpp>
#include <had/version/attributes.hpp>
#include <had/utility/max.hpp>
#include <had/version/namespace.hpp>
#include <had/iterator/reverse_iterator.hpp>
#include <had/cstring.h>
#include <had/allocator.hpp>
#include <had/char_traits.hpp>
#include <had/utility/compressed_pair.hpp>
#include <had/version/attributes/inline_namespace.hpp>
#include <had/algorithm/copy.hpp>
#include <had/algorithm/fill.hpp>
#include <had/algorithm/equal.hpp>
#include <limits>

HAD_NAMESPACE_BEGIN
template<
    typename CharType,
    typename Allocator = default_allocator<CharType>
>
class basic_string {
public:
    friend class basic_string_view<CharType>;
    static constexpr u64 npos     = static_cast<u64>(-1);
    using this_type               = basic_string;
    using size_type               = decltype(sizeof(void*));
    using value_type              = CharType;
    using allocator_type          = Allocator;
    using char_type               = value_type;
    using pointer_type            = value_type*;
    using reference_type          = value_type&;
    using string_view_type        = basic_string_view<value_type>;
    using const_pointer_type      = const value_type*;
    using const_reference_type    = const value_type&;
    using iterator                = value_type*;
    using const_iterator          = const value_type*;
    using reverse_iterator        = HAD_NS reverse_iterator<value_type>;
    using const_reverse_iterator  = HAD_NS reverse_iterator<const value_type>;
public:
    HAD_STATIC_ASSERT_IF(!is_trivially_copyable<value_type>::value, "had::string only allows memcpyable types mustbe trivially copyable!");
    constexpr allocator_type& get_allocator() noexcept {
        return mPair.get_first();
    }

    constexpr const allocator_type& get_allocator() const noexcept {
        return mPair.get_first();
    }

    HAD_NODISCARD size_type length() const noexcept {
        return is_small_string()
            ? string_length(mPair.get_second().small_buffer)
            : distance( begin(),end() );
    }

    HAD_NODISCARD size_type capacity() const noexcept {
        return is_small_string() 
            ? kSizeOfSSO 
            : mPair.get_second().big.capacity;
    }

    
    basic_string(std::nullptr_t) = delete;
    
    basic_string() : basic_string("",0) {}
    explicit basic_string(size_type repeat_times,value_type Char) 
        : basic_string(repeat_times)
    {

        auto pair = make_iterator_pair(mBegin, &mBegin[repeat_times]);
        HAD_NS uninitialized_fill(pair, Char);
    }

    basic_string(std::initializer_list<value_type> ilist) 
        : basic_string(ilist.size())
    {
        auto pair = HAD_NS make_iterator_pair(ilist);
        HAD_NS copy_no_overlap(pair, begin());
        data()[ilist.size()] = '\0';
    }


    explicit basic_string(size_type reserve_amount) {
        if (can_use_sso(reserve_amount + 1)) {
            mBegin = mPair.get_second().small_buffer;
        }
        else {
            const size_type cap = reserve_amount + 1;
            mBegin = get_allocator().allocate(cap);
            mPair.get_second().big.capacity = cap;
        }
    }
    HAD_NODISCARD20 basic_string(const_pointer_type str) {

        const size_type len = string_length(str);
        if (can_use_sso(len + 1)) {
            mBegin = mPair.get_second().small_buffer;
        }

        else {
            const size_type cap = len + (len / 2);
            mBegin = get_allocator().allocate(cap);
            mPair.get_second().big.capacity = cap;
            mPair.get_second().big.end = mBegin + len;
        }
        auto pair = HAD_NS make_iterator_pair(&str[0], &str[len + 1]);
        HAD_NS copy_no_overlap(pair, mBegin);
    }
    
    HAD_NODISCARD20 basic_string(const_pointer_type string, size_type length) {
        if (can_use_sso(length + 1)) {
            mBegin = mPair.get_second().small_buffer;
        }
        else {
            const size_type cap = length + (length / 2);
            mBegin = get_allocator().allocate(cap);
            mPair.get_second().big.capacity = cap;
            mPair.get_second().big.end = mBegin + length;
        }
        auto pair = HAD_NS make_iterator_pair(&string[0], &string[length + 1]);
        HAD_NS copy_no_overlap(pair, mBegin);
        //uninitialized_copy(begin_, string, length + 1); // null terminator
    }

    HAD_NODISCARD20 basic_string(const basic_string& other) {
        if (other.is_small_string()) {
            mBegin = mPair.get_second().small_buffer;
        }
        else {
            mBegin = get_allocator().allocate(other.mPair.get_second().big.capacity);
            mPair.get_second().big.capacity = other.mPair.get_second().big.capacity;
            mPair.get_second().big.end = mBegin + mPair.get_second().big.capacity;
        }
        auto pair = make_iterator_pair(other.begin(), other.end());
        copy_no_overlap(pair, mBegin); // null terminator
    }
    
    HAD_NODISCARD20 basic_string(basic_string&& other) noexcept {
        if (other.is_small_string()) {
            mBegin = mPair.get_second().small_buffer;
            auto pair = HAD_NS make_iterator_pair(other);
            copy_no_overlap(pair, mBegin); // null terminator
        }
        else {
            mBegin = HAD_NS exchange(other.mBegin, nullptr);
            mPair.get_second().big.end = HAD_NS exchange(other.mPair.get_second().big.end,nullptr);
            mPair.get_second().big.capacity = HAD_NS exchange(other.mPair.get_second().big.capacity, 0);
        }
    }
    
    basic_string& operator=(const basic_string& other) & {
        if (other.length() <= capacity()) {
            auto pair = make_iterator_pair(other);
            HAD_NS copy_no_overlap(pair, mBegin);
            //::memcpy(begin_, other.begin_, in_bytes(other.length()));
        }
        else {
            basic_string temp{ other };
            *this = HAD_NS move(temp);
        }
        return *this;
    }
    
    basic_string& operator=(basic_string&& other) & noexcept {
        if (this == &other) 
            return *this;
        
        if (other.is_small_string()) {
            mBegin = mPair.get_second().small_buffer;
            auto pair = HAD_NS make_iterator_pair(other.mBegin, other.mBegin + other.length() + 1);
            copy_no_overlap(pair, mBegin);
            //uninitialized_copy(begin_, other.begin_, other.length() + 1); // null terminator
        }
        else {
            mBegin = exchange(other.mBegin, nullptr);
            mPair.get_second().big.capacity = exchange(other.mPair.get_second().big.capacity, 0);
            mPair.get_second().big.end = exchange(other.mPair.get_second().big.end, nullptr);
        }
        return *this;
    }

    basic_string& operator=(std::initializer_list<value_type> ilist) & {
        if (capacity() > ilist.size()) {
            auto pair = make_iterator_pair(ilist);
            HAD_NS copy_no_overlap(pair, mBegin);
        }
        else {
            basic_string temp{ ilist };
            *this = HAD_NS move(temp);
        }
        return *this;
    }

    HAD_NODISCARD bool operator==(string_view_type other) const noexcept {
        return  length() == other.length()
             && other[0] == mBegin[0]
             && equal(&mBegin[0], &mBegin[other.length()], &other[0]);
    }

    HAD_NODISCARD bool operator!=(string_view_type other) const noexcept {
        return !(*this == other);
    }
    
    
    ~basic_string() noexcept {
        if (!is_small_string()) {
            get_allocator().deallocate(mBegin,mPair.get_second().big.capacity);
        }
    }
    HAD_NODISCARD operator string_view_type() const noexcept {
        return string_view_type(mBegin, length());
    }

    char_type* data() noexcept {
        return mBegin;
    }
    const char_type* data() const noexcept {
        return mBegin;
    }
    const char_type* c_str() const noexcept {
        return mBegin;
    }

    basic_string& operator+=(string_view_type str) {
        insert(size_type(-1), str);
        return *this;
    }

    friend basic_string operator+(basic_string a, const basic_string& b) {
        a += b;
        return a;
    }

    void pop_front() {
        erase(0, 1);
    }
    void pop_back() {
        erase(size_type(-1), 1);
    }

    void push_front(string_view_type str) {
        insert(0, str);
    }

    void push_back(value_type Char) {
        insert(size_type(-1), Char);
    }

    void push_back(string_view_type str) {
        insert(size_type(-1), str);
    }

    void insert(size_type offset, string_view_type str) {
        const size_type my_len = length();
        const size_type other_len = str.length();

        if (offset >= my_len)
            offset = my_len;
    
        const size_type sum = other_len + my_len;
        
        if (sum >= capacity())
            grow(calc_growth(sum));
    

        for(size_type i = sum;i >= (offset + other_len) ;--i)
             mBegin[i] = mBegin[i - other_len];
        
        for (size_type i = offset; i != ((offset + other_len)); i++)
            mBegin[i] = str[i - offset];
        
        if (!is_small_string())
            mPair.get_second().big.end = mBegin + sum;
        mBegin[sum] = '\0';
    }

    void insert(size_type offset, value_type Char) {
        const size_type my_len = length();
        const size_type other_len = 1;

        if (offset >= my_len)
            offset = my_len;

        const size_type sum = other_len + my_len;

        if (sum >= capacity())
            grow(calc_growth(sum));


        for (size_type i = sum; i != offset; i--)
            mBegin[i] = mBegin[i - other_len];
        mBegin[offset] = Char;

        if (!is_small_string()) {
            mPair.get_second().big.end = mBegin + sum;
        }
        mBegin[sum] = '\0';
    }
    void erase(size_type offset, size_type amount = -1) {
        const size_type len = length();
        if (len == 0 || amount == 0)
            return;

        if (offset > len)
            offset = len;

        if (amount > ((len - offset) + 1))
            amount = (len - offset);

        for (size_type i = offset; i < len; i++)
            mBegin[i] = mBegin[i + amount];

        if (!is_small_string())
            mPair.get_second().big.end -= amount;
        mBegin[len] = '\0';
    }


    void clear() noexcept {
        if(is_small_string())
            mBegin[0] = '\0';
        else
            mPair.get_second().big.end = mBegin;
    }

    HAD_NODISCARD size_type rfind(string_view_type needle) const noexcept {
        const auto needle_len = needle.length();
        const auto my_len = length();

        if (my_len < needle_len)
            return npos;

        const auto diff = (my_len - needle_len) + 1;
        // "st"
        // "Master"
        // 4
        for (size_type i = diff; i != 0; i--) {
            if (equal(&mBegin[i], &mBegin[i + needle_len], &needle[0])) {
                return i;
            }
        }
        return npos;
    }

    HAD_NODISCARD size_type find(string_view_type needle, size_type from_index = 0) const noexcept {
        const size_type my_len     = length();
        const size_type needle_len = needle.length();
        if (my_len < needle_len)
            return npos;
        
        const size_type diff = (my_len - needle_len) + 1;
        

        for (size_type i = from_index; i < diff; i++) {
            auto pair = HAD_NS make_iterator_pair(mBegin + i, mBegin + i + needle_len);
            if (HAD_NS equal(pair, needle.begin() )) {
                return i;
            }
        }

        return npos;
    }

    basic_string& replace(size_type pos, size_type amount, string_view_type with) {
        if (pos == npos)
            return *this;

        const size_type other_len = with.length();
        if (amount == other_len) {
            HAD_NS copy_no_overlap(with.iterator_pair(), mBegin + pos);
            return *this;
        }

        if (amount > with.length()) {
            const size_type erase_pos = pos + other_len;
            const size_type diff = amount - other_len;
            erase(erase_pos, diff);
            HAD_NS copy_no_overlap(with.iterator_pair(), mBegin + pos);
            //uninitialized_copy(begin_ + pos, with.data(),with.length());
            return *this;
        }

        insert(pos + amount, with.begin() + amount);

        auto pair = HAD_NS make_iterator_pair(with.begin(), with.begin() + amount);
        HAD_NS copy_no_overlap(pair, &mBegin[pos]);

        return *this;
    }
    basic_string& replace(string_view_type str, string_view_type with) {
        replace(find(str), str.length(), with);
        return *this;
    }

    bool starts_with(string_view_type str) const noexcept {
        const auto my_len = length();
        if (my_len < str.length()) {
            return false;
        }
        const auto my_begin = begin();
        auto pair = HAD_NS make_iterator_pair(my_begin, my_begin + str.length());
        return HAD_NS equal(pair, str.begin());
    }

    bool ends_with(string_view_type str) const noexcept {
        const auto my_len = length();
        if (my_len < str.length()) {
            return false;
        }

        const auto my_end = end();
        auto pair = HAD_NS make_iterator_pair(my_end - str.length(), my_end);
        return HAD_NS equal(pair, str.begin());
    }



    HAD_NODISCARD iterator               begin() noexcept { return mBegin; }
    HAD_NODISCARD const_iterator         begin() const noexcept { return mBegin; }
    HAD_NODISCARD iterator               end()   noexcept { 
        return is_small_string()
            ? mBegin + string_length(mPair.get_second().small_buffer)
            : mPair.get_second().big.end;
    }
    HAD_NODISCARD const_iterator         end()   const noexcept { 
        return is_small_string()
        ? mBegin + string_length(mPair.get_second().small_buffer) 
        : mPair.get_second().big.end;
    }
    HAD_NODISCARD const_iterator         cbegin() const noexcept { return begin(); }
    HAD_NODISCARD const_iterator         cend() const noexcept { return end(); }
    HAD_NODISCARD reverse_iterator       rbegin() noexcept { return end() - 1; }
    HAD_NODISCARD reverse_iterator       rend()   noexcept { return mBegin - 1; }
    HAD_NODISCARD const_reverse_iterator rbegin() const noexcept { return end() - 1; }
    HAD_NODISCARD const_reverse_iterator rend()   const noexcept { return mBegin - 1; }
    HAD_NODISCARD const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    HAD_NODISCARD const_reverse_iterator crend()   const noexcept { return rend(); }

    HAD_NODISCARD reference_type operator[](size_t index) noexcept {
        return mBegin[index];
    }

    HAD_NODISCARD const_reference_type operator[](size_t index) const noexcept {
        return mBegin[index];
    }

    basic_string& reverse() noexcept {
        // "Hello"
        // "olleH"
        const size_type len = length();
        const size_type mid = len / 2;

        for (size_t i = 0; i < mid; i++)
            HAD_NS swap(mBegin[i], mBegin[(len - i)-1]);

        return *this;
    }

    HAD_NODISCARD bool contains(string_view_type str) const noexcept {
        return find(str) != npos;
    }

    void reserve(size_type amount) noexcept {
        grow_if_needed(amount + 1);
    }
    HAD_NODISCARD reference_type back() noexcept {return mBegin[length() - 1];}
    HAD_NODISCARD const_reference_type back() const noexcept {return mBegin[length() - 1];}
    HAD_NODISCARD reference_type front() noexcept {return mBegin[0];}
    HAD_NODISCARD const_reference_type front() const noexcept {return mBegin[0];}
    HAD_NODISCARD bool is_empty() const noexcept {return length() == 0;}



    basic_string& assign(const basic_string& other) & {
        if (other.length() <= capacity()) {
            auto pair = make_iterator_pair(other);
            HAD_NS copy_no_overlap(pair, mBegin);
            //::memcpy(begin_, other.begin_, in_bytes(other.length()));
        }
        else {
            basic_string temp{ other };
            *this = HAD_NS move(temp);
        }
        return *this;
    }

    basic_string& assign(basic_string&& other) & noexcept {
        if (this == &other) 
            return *this;

        if (other.is_small_string()) {
            mBegin = mPair.get_second().small_buffer;
            auto pair = HAD_NS make_iterator_pair(other.mBegin, other.mBegin + other.length() + 1);
            copy_no_overlap(pair, mBegin);
            //uninitialized_copy(begin_, other.begin_, other.length() + 1); // null terminator
        }
        else {
            mBegin = exchange(other.mBegin, nullptr);
            mPair.get_second().big.capacity = exchange(other.mPair.get_second().big.capacity, 0);
            mPair.get_second().big.end = exchange(other.mPair.get_second().big.end, nullptr);
        }
        return *this;
    }

    basic_string& assign(std::initializer_list<value_type> ilist)& {
        if (capacity() > ilist.size()) {
            auto pair = make_iterator_pair(ilist);
            HAD_NS copy_no_overlap(pair, mBegin);
        }
        else {
            basic_string temp{ ilist };
            *this = HAD_NS move(temp);
        }
        return *this;
    }

private:
    void grow_if_needed(size_type new_capacity) {
        if (new_capacity >= capacity())
            grow(calc_growth(new_capacity));
    }

    void grow(size_type new_capacity) {

        pointer_type buffer = get_allocator().allocate(new_capacity);

        auto pair = make_iterator_pair(mBegin, end() + 1);
        copy_no_overlap(pair, buffer);

        if (!is_small_string()) {
            get_allocator().deallocate(mBegin, mPair.get_second().big.capacity);
        }
        mPair.get_second().big.capacity = new_capacity;
        mBegin = buffer;
    }

    HAD_NODISCARD size_type calc_growth(size_t min) const noexcept {
        const auto cap = capacity();
        return 1 + ::had::max(min, cap + (cap / 2));
    }

    HAD_NODISCARD bool is_small_string() const noexcept {
        const volatile void* begin = static_cast<const volatile void*>(mBegin);
        const volatile void* pair  = static_cast<const volatile void*>(&mPair);

        return begin == pair;
    }
    
    HAD_NODISCARD bool is_big_string() const noexcept {
        return !is_small_string();
    }
    HAD_NODISCARD constexpr static bool can_use_sso(size_type num) noexcept {
        return sizeof(value_type) * num < kSizeOfSSO;
    }

    basic_string& assign(basic_string& other) {

    }

private:
    // order matters
    pointer_type mBegin;
    struct big_t {
        pointer_type end;
        size_type capacity;
    };
    constexpr static size_type kSizeOfBigStorage = sizeof(big_t);
    constexpr static size_type kSizeOfSSO        = kSizeOfBigStorage / sizeof(value_type);
    union storage_t {
        big_t big;
        value_type small_buffer[kSizeOfSSO];
    };
    compressed_pair<allocator_type, storage_t> mPair;
    
};
template<typename CharType,typename Allocator = default_allocator<CharType>>
std::ostream& operator<<(std::ostream& o, const basic_string<CharType,Allocator>& s) {
    return o << s.data();
}


using string    = basic_string<char>;
using wstring   = basic_string<wchar_t>;
#ifdef __cpp_char8_t
using u8string  = basic_string<char8_t>;
#endif
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;



inline string to_string(const char* str) {
    return string{ str };
}

inline string to_string(bool boolean) {
    const static string map[] = {
        {"false",5},
        {"true",4}
    };
    return map[boolean];
}

inline string to_string(char ch) {
    return string(1,ch);
}

inline string to_string(std::nullptr_t) {
    return "0x0000000000000000";
}

inline string to_string(void* ptr) {
    string::value_type buffer[19];
    std::memset(&buffer[0], '0', sizeof(buffer));
    size_t i = 17;
    size_t num = (unsigned long long)(ptr);
    static constexpr char hex[] = "0123456789ABCDEF";
    do {
        char digit = num % 16;
        buffer[i] = hex[digit];
        i--;
        num /= 16;
    } while (num != 0);
    buffer[1] = 'x';
    buffer[0] = '0';
    buffer[sizeof(buffer) - 1] = '\0';
    return string(&buffer[0]);
}

inline had::string to_string(i64 num) {
    string::value_type buffer[16];
    //std::memset(buffer, 'A', 16);
    size_t i = 14;
    const bool is_negative = num < 0;
    if (is_negative)
        num *= -1;
    do {
        char digit = num % 10;
        buffer[i] = digit + '0';
        i--;
        num /= 10;
    } while (num != 0);
    if (is_negative) {
        buffer[i--] = '-';
    }
    buffer[15] = '\0';
    return string(&buffer[i+1]);
}


inline string to_string(u64 num) {
    string::value_type buffer[16];
    //std::memset(buffer, 'A', 16);
    size_t i = 14;
    do {
        char digit = num % 10;
        buffer[i] = digit + '0';
        i--;
        num /= 10;
    } while (num != 0);
    buffer[15] = '\0';
    return string(&buffer[i+1]);
}


inline string to_string(short num) noexcept {
    return to_string(i64(num));
}
inline string to_string(unsigned short num) noexcept {
    return to_string(u64(num));
}
inline string to_string(int num) noexcept {
    return to_string(i64(num));
}
inline string to_string(unsigned int num) noexcept {
    return to_string(u64(num));
}


inline string to_string(double num) {
    const auto len = static_cast<size_t>(::_scprintf("%f", num));
    string str(len, '\0');
    ::sprintf_s(&str[0], len + 1, "%f", num);
    return str;
}


inline string to_string(float num) {
    return to_string(static_cast<double>(num));
}

inline string to_string(long double num) {
    return to_string(static_cast<double>(num));
}
#pragma warning(push)
#pragma warning(disable : 5045) // "i" will be checked warning

inline string to_lowercase(string str) {
    constexpr static char table[] = {
        -128,-127,-126,-125,-124,-123,-122,-121,
        -120,-119,-118,-117,-116,-115,-114,-113,-112,-111,-110,-109,-108,-107,
        -106,-105,-104,-103,-102,-101,-100,-99,-98,-97,-96,-95,-94,-93,-92,-91,
        -90,-89,-88,-87,-86,-85,-84,-83,-82,-81,-80,-79,-78,-77,-76,-75,-74,-73,
        -72,-71,-70,-69,-68,-67,-66,-65,-64,-63,-62,-61,-60,-59,-58,-57,-56,-55,
        -54,-53,-52,-51,-50,-49,-48,-47,-46,-45,-44,-43,-42,-41,-40,-39,-38,-37,
        -36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,
        -18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,
        4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,
        30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,
        54,55,56,57,58,59,60,61,62,63,64,97,98,99,100,101,102,103,104,105,106,
        107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,91,92,
        93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
        112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127 };
    constexpr static bool is_signed = std::numeric_limits<string::value_type>::is_signed;
    for (auto& c : str) {
        c = table[c + (is_signed * 128)];
    }
    return str;
}

inline string to_uppercase(string str) {
    constexpr static char table[] = { 
        -128,-127,-126,-125,-124,-123,-122,-121,-120,-119,
        -118,-117,-116,-115,-114,-113,-112,-111,-110,-109,
        -108,-107,-106,-105,-104,-103,-102,-101,-100,-99,
        -98,-97,-96,-95,-94,-93,-92,-91,-90,-89,-88,-87,
        -86,-85,-84,-83,-82,-81,-80,-79,-78,-77,-76,-75,
        -74,-73,-72,-71,-70,-69,-68,-67,-66,-65,-64,-63,
        -62,-61,-60,-59,-58,-57,-56,-55,-54,-53,-52,-51,
        -50,-49,-48,-47,-46,-45,-44,-43,-42,-41,-40,-39,
        -38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,
        -26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,
        -14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,
        19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,
        35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,
        51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,
        67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,
        83,84,85,86,87,88,89,90,91,92,93,94,95,96,65,66,
        67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,
        83,84,85,86,87,88,89,90,123,124,125,126,127
    };
    constexpr static bool is_signed = std::numeric_limits<string::value_type>::is_signed;
    for (auto& c : str) {
        c = table[c + (is_signed * 128)];
    }
    return str;
}
#pragma warning(pop) // end of "i" will be checked warning
#ifdef __cpp_user_defined_literals

inline namespace literals {
inline namespace string_literals {

HAD_NODISCARD inline HAD_NS string operator""_s(const char* str, std::size_t len) {
    return HAD_NS string( str,len );
}

HAD_NODISCARD inline HAD_NS wstring operator""_s(const wchar_t* str, std::size_t len) {
    return HAD_NS wstring( str,len );
}
#ifdef __cpp_char8_t
HAD_NODISCARD inline HAD_NS u8string operator""_s(const char8_t* str, std::size_t len) {
    return HAD_NS u8string( str,len );
}
#endif // __cpp_char8_t

HAD_NODISCARD inline HAD_NS u16string operator""_s(const char16_t* str, std::size_t len) {
    return HAD_NS u16string( str,len );
}
HAD_NODISCARD inline HAD_NS u32string operator""_s(const char32_t* str, std::size_t len) {
    return HAD_NS u32string( str,len );
}

} // namespace string_literals 
} // namespace literals

#endif // __cpp_user_defined_literals
HAD_NAMESPACE_END



#endif // !HAD_STRING_HPP