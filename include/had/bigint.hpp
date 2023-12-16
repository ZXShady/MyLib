#pragma once
#ifndef HAD_BIGINT_HPP
#define HAD_BIGINT_HPP


#include <had/vector.hpp>
#include <had/format/formatter.hpp>

HAD_NAMESPACE_BEGIN


class bigint {
private:
    friend formatter<bigint>;
public:
    struct null_init {};
    using number_type = u32;

    bigint(null_init) {};

    bigint() : bigint(0) {};

    bigint(u64 num,bool is_negative);

    bigint(i64 num) : bigint(num < 0 ? u64(-num) : u64(num), num < 0) {};
    bigint(u64 num) : bigint(num, false) {};

    bigint(i32 num) : bigint(i64(num)) {};
    bigint(i16 num) : bigint(i64(num)) {};
    bigint(i8 num)  : bigint(i64(num)) {};

    bigint(u32 num) : bigint(u64(num)) {};
    bigint(u16 num) : bigint(u64(num)) {};
    bigint(u8 num)  : bigint(u64(num)) {};


    bigint(string_view numStr);
    bigint(const char* numStr) : bigint(string_view(numStr)) {};

    bool is_negative() const noexcept { return mIsNegative; }
    bigint(const bigint&) = default;
    bigint(bigint&&) noexcept = default;
    bigint& operator=(const bigint&) = default;
    bigint& operator=(bigint&&) = default;
    ~bigint() noexcept = default;

    HAD_NODISCARD const vector<number_type>& get_numbers() const noexcept { 
        return mNumbers;
    }
    
    HAD_NODISCARD bigint operator+() const {
        bigint copy = *this;
        copy.mIsNegative = false;
        return copy;
    };
    HAD_NODISCARD bigint operator-() const {
        bigint copy = *this;
        copy.mIsNegative = !copy.mIsNegative;
        return copy;
    };

    bigint& operator+=(const bigint& rhs);

    friend bigint operator+(const bigint& lhs, const bigint& rhs) {
        return add(lhs, rhs,lhs.mIsNegative,rhs.mIsNegative);
    }
private:
    static bigint add(const bigint& lhs, const bigint& rhs, bool lhs_negative, bool rhs_negative);
    void fixInvalid();
private:
    constexpr static number_type kMaxDigitsInNumber = number_type(1e7);
    vector<number_type> mNumbers;
    bool mIsNegative = false;
};

template<>
struct formatter<bigint> {
    HAD_NODISCARD static string to_string(const bigint& bignum) {
        string str = bignum.mIsNegative
            ? "-"
            : "";
        const auto begin = bignum.mNumbers.crbegin();
        const auto end = bignum.mNumbers.crend();
        using valt = decltype(bignum.mNumbers)::value_type;
        using formatter_t = formatter<valt>;
        // Print the digits in reverse order
        auto digitcount = [](bigint::number_type x) -> int{
            int count = 0;
            do {
                x /= 10;
                ++count;
            } while (x != 0);
            return count;
        };
        
        auto iter = begin;
        if (iter != end) {
            str += formatter_t::to_string(*iter);
            ++iter;
        }
        if (iter == end)
            return str;
        //                     ----|
        // 135790, 753135, 390135, 13051, 91035, 610009, 102345
        // 
        const auto max_count = digitcount(bigint::kMaxDigitsInNumber);
        for (; iter != (end-1); ++iter) {
            auto count = max_count - digitcount(*iter);
            for (int i = 0; i < count - 1; i++) {
                str += "0";
            }
            str += formatter_t::to_string(*iter);
        }
        str += formatter_t::to_string(*iter);
        ++iter;
        return str;
    }

};
HAD_NAMESPACE_END
#endif // !HAD_BIGINT_HPP
