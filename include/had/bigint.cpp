#include "bigint.hpp"
#include <had/format.hpp>
#include <cctype>

using namespace had;

bigint::bigint(u64 num,bool is_negative) 
    :mIsNegative(is_negative)
{
    while (num > 0) {
        mNumbers.push_back(num % kMaxDigitsInNumber);
        num /= kMaxDigitsInNumber;
    }
    fixInvalid();
}

bigint::bigint(string_view numStr) 
    :mIsNegative(numStr.front() == '-')
{
    const auto begin = numStr.cbegin();
    const auto end   = numStr.cend();
    auto it = begin + mIsNegative;
    //auto iter = numStr.crbegin();
    while (it != end && *it == '0') {
        ++it;
    }

    while (it != end) {
        number_type num = 0;
        for (size_t i = 0; i < 6; i++) {
            if (*it == '\'') 
                it[0];
            else if (std::isdigit(*it)) {
                num = num * 10 + (*it - '0');
                ++it;
            }
            else if (it == end) {
                break;
            }
            else {
                throw std::runtime_error(
                    HAD_NS 
                    format("Invalid character in number string at position {}", distance(begin, it)).c_str());
                break;
            }
        }
        mNumbers.insert(mNumbers.begin(), num);

    }
    fixInvalid();
}


void bigint::fixInvalid() {
    // fixing when -0, or when size of numbers is 0
    // in both cases, set to 0 and positive

    // start with -0
    if (mNumbers.size() == 1 && mNumbers[0] == 0 && mIsNegative) {
        mIsNegative = false;
        return;
    }

    // now check if size is 0
    if (mNumbers.size() == 0) {
        mNumbers.push_back(0);
        mIsNegative = false;
        return;
    }

    // remove leading zeros
    while (mNumbers.size() > 1 && mNumbers.front() == 0) {
        mNumbers.pop_back();
    }

    // handle case where all digits were zeros
    if (mNumbers.is_empty()) {
        mNumbers.push_back(0);
        mIsNegative = false;
    }
}

bigint bigint::add(const bigint& lhs, const bigint& rhs, bool lhs_negative, bool rhs_negative) {
    if (!lhs_negative && !rhs_negative ) {
        bigint result = bigint::null_init{};

        number_type carry = 0;
        const auto lhs_size = lhs.mNumbers.size();
        const auto rhs_size = rhs.mNumbers.size();
        auto max = HAD_NS max(lhs_size,rhs_size);
        // 124
        //   9 +
        for (int i = 0; i < max; i++) {
            number_type sum = carry;
            if (i < lhs_size) {
                sum += lhs.mNumbers[i];
            }
            if (i < rhs_size) {
                sum += rhs.mNumbers[i];
            }
            result.mNumbers.push_back(sum % kMaxDigitsInNumber);
            carry = sum / kMaxDigitsInNumber;
        }
        if (carry > 0) {
            result.mNumbers.insert(result.mNumbers.begin(), carry);
        }
        result.fixInvalid();
        return result;
    }

    // case 2
    //if (!lhs.mIsNegative && rhs.mIsNegative) {
    //    return *this - (-rhs);
    //}

    // case 3
    //if (lhs.mIsNegative && !rhs.mIsNegative) {
    //    return rhs - -(*this);
    //}

    // case 4
    //if (lhs.mIsNegative && rhs.mIsNegative) {
    //    return -(-*this + -rhs);
    //}
    return lhs;

}
