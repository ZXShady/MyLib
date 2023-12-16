#pragma once
#ifndef HAD_VECTOR_HPP
#define HAD_VECTOR_HPP 1

#include <had/exceptions.hpp>

#include <had/stdint.hpp>

#include <had/config.hpp>

#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>

#include <had/algorithm/destroy.hpp>
#include <had/utility/destroy_at.hpp>

#include <had/utility/exchange.hpp>
#include <had/utility/swap.hpp>

#include <had/iterator/reverse_iterator.hpp>
#include <had/iterator/iterator_traits.hpp>

#include <had/initializer_list.hpp>

#include <had/memory/unique_ptr.hpp>
#include <had/algorithm/fill.hpp>
#include <had/algorithm/uninitialized_fill.hpp>
#include <had/algorithm/uninitialized_move_if_noexcept.hpp>
#include <had/algorithm/uninitialized_copy.hpp>
#include <had/type_traits/move.hpp>
#include <had/type_traits/forward.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/void_t.hpp>
#include <had/type_traits/special_member_functions.hpp>
#include <had/utility/distance.hpp>
#include <had/allocator.hpp>
#include <had/exceptions.hpp>
#include <had/type_traits/is_integral.hpp>
#include <had/algorithm/equal.hpp>

#include <had/format/formatter.hpp>

/// C Libs
#include <cstring>

HAD_NAMESPACE_BEGIN


template<typename ValueType,typename Allocator = default_allocator<ValueType> >
class vector final {
public:
    using this_type = vector;
    using size_type = size_t;
    using value_type = ValueType;
    using pointer_type = value_type*;
    using reference_type = value_type&;
    using const_pointer_type = const value_type*;
    using const_reference_type = const value_type&;
    using allocator_type = Allocator;

    using iterator = value_type*;
    using const_iterator = const value_type*;
    using const_reverse_iterator = reverse_iterator<const value_type>;
    using reverse_iterator = reverse_iterator<value_type>;
    using iterator_pair_type = HAD_NS iterator_pair<iterator>;
    using reverse_iterator_pair_type = HAD_NS iterator_pair<reverse_iterator>;
    using const_iterator_pair_type = HAD_NS iterator_pair<const_iterator>;
    using const_reverse_iterator_pair_type = HAD_NS iterator_pair<const_reverse_iterator>;
    using ilist_type = std::initializer_list<value_type>;
public:
    constexpr static bool nothrow_constructible = is_nothrow_constructible<value_type>::value;
    constexpr static bool nothrow_move_constructible = is_nothrow_move_constructible<value_type>::value;
    constexpr static bool nothrow_copy_constructible = is_nothrow_copy_constructible<value_type>::value;

public:
    vector() noexcept 
    : mSize(0)
    , mCapacity(0)
    , mPair(nullptr) 
    {}

    vector(std::initializer_list<value_type> ilist)
        : mSize(ilist.size() )
        , mCapacity(calcGrowth(ilist.size()))
        , mPair(get_allocator().allocate(mCapacity)) 
    {
        HAD_NS uninitialized_copy(HAD_NS make_iterator_pair(ilist), begin());
    }
        template<typename NumType,typename enable_if<
            is_integral<NumType>::value>::type = 0 >
    explicit vector(NumType N ,const value_type& value = value_type())
        : mSize(N)
        , mCapacity(calcGrowth(N))
        , mPair(get_allocator().allocate(mCapacity)) 
    {
        HAD_NS uninitialized_fill(get_iterator_pair(), value);
    }

    template<typename Iterator,
        typename enable_if<is_iterator<Iterator>::value>::type = 0 >
    vector(Iterator begin,Iterator end) 
        : mSize(HAD_NS distance(begin,end))
        , mCapacity(mSize * 2)
        , mPair(get_allocator().allocate(mCapacity))
    {
        HAD_NS uninitialized_copy_no_overlap(begin, end, this->begin());
    }
    
    vector(const vector& v) 
        : mSize(v.mSize)
        , mCapacity(v.mCapacity)
    {
        mPair.get_first() = v.mPair.get_first();

        getMyPtr() = get_allocator().allocate(mCapacity);

        auto pair = make_iterator_pair(v);
        HAD_NS uninitialized_copy(pair, begin());
    }
    
    vector(vector&& v) noexcept 
        : mSize(v.mSize)
        , mCapacity(v.mCapacity)
        , mPair(HAD_NS exchange(v.getMyPtr(), nullptr))
    {
    }

    this_type& operator=(const vector& other) & {
        auto copy_pair = HAD_NS make_iterator_pair(other);
        if HAD_CONSTEXPR17(nothrow_copy_constructible) {
            if (other.size() <= capacity()) {
                const auto diff = (other.size() >= size())
                    ? other.size() - size()
                    : size() - other.size();

                auto destroy_begin = begin() + other.size();
                auto destroy_end = destroy_begin + diff;

                auto destroy_pair = HAD_NS make_iterator_pair(destroy_begin, destroy_end);
                HAD_NS destroy(destroy_pair);
                HAD_NS copy(copy_pair, begin());
                mSize = other.size();
                return *this;
            }
        }

        vector copy{ other };
        this->swap(copy);
        return *this;
    }

    void shrink_to_fit() {
        HAD_NS unique_ptr<value_type[]> buffer(get_allocator().allocate(mSize));
        const auto my_begin = begin();
        const auto my_end   = end();
        auto pair = make_iterator_pair(my_begin, my_end);

        HAD_NS uninitialized_move_if_noexcept(pair, buffer.data());
        
        HAD_NS destroy(my_begin, my_end);
        
        get_allocator().deallocate(data(), mCapacity);
        getMyPtr() = buffer.release(nullptr);
        mCapacity = mSize;
    }

    void resize(size_type count, const value_type& value = value_type{}) {
        if (count == size()) {
            return;
        }
        // If the current size is greater than count, the container is reduced to its first count elements.
        if (size() > count) {
            auto pair = HAD_NS make_iterator_pair(begin() + count, end());
            HAD_NS destroy(pair);
            mSize = count;
        }
        else {
            growIfNeeded(count - size());
            auto pair = make_iterator_pair(begin() + size(), begin() + count);
            HAD_NS uninitialized_fill(pair, value);
        }
        mSize = count;
    }

    this_type& operator=(vector&& v) & {
        HAD_NS destroy(get_iterator_pair());
        mSize = v.mSize;
        mCapacity = v.mCapacity;
        getMyPtr() = HAD_NS exchange(v.getMyPtr(),nullptr);
        return *this;
    }
    
    this_type& operator=(ilist_type ilist) & {
        auto copy_pair = HAD_NS make_iterator_pair(ilist);
        if (ilist.size() <= mCapacity) {
            const auto diff = (ilist.size() >= mSize)
                ? ilist.size() - mSize
                : mSize - ilist.size();

            auto destroy_begin = begin() + ilist.size();
            auto destroy_end = destroy_begin + diff;

            auto destroy_pair = HAD_NS make_iterator_pair(destroy_begin, destroy_end);
            HAD_NS destroy(destroy_pair);
            HAD_NS copy(copy_pair, begin());
        }
        else {
            reAlloc(ilist.size());
            HAD_NS copy(copy_pair, begin());
        }
        mSize = ilist.size();
        return *this;
    }

    ~vector() noexcept {
        
        HAD_NS destroy( begin(),end() );

        get_allocator().deallocate(data(),mCapacity);
    }

    
public:
    HAD_NODISCARD bool is_empty() const noexcept { return size() == 0; }

    HAD_NODISCARD pointer_type data() noexcept { return mPair.get_second(); }
    HAD_NODISCARD const_pointer_type data() const noexcept { return mPair.get_second(); }

    HAD_NODISCARD size_type size() const noexcept { return mSize; }
    HAD_NODISCARD size_type capacity() const noexcept { return mCapacity; }

    HAD_NODISCARD reference_type front() noexcept { return *begin(); }
    HAD_NODISCARD const_reference_type front() const noexcept { return *begin(); }

    HAD_NODISCARD reference_type back() noexcept { return *rbegin(); }
    HAD_NODISCARD const_reference_type back() const noexcept { return *rbegin(); }

    HAD_NODISCARD iterator_pair_type get_iterator_pair() noexcept { return make_iterator_pair(begin(), end()); }
    HAD_NODISCARD const_iterator_pair_type get_iterator_pair() const noexcept { return make_iterator_pair(begin(), end()); }
    HAD_NODISCARD reverse_iterator_pair_type get_riterator_pair() noexcept { return make_iterator_pair(rbegin(), rend()); }
    HAD_NODISCARD const_reverse_iterator_pair_type get_riterator_pair() const noexcept { return make_iterator_pair(rbegin(), rend()); }
    HAD_NODISCARD const_iterator_pair_type get_citerator_pair() const noexcept { return make_iterator_pair(begin(), end()); }
    HAD_NODISCARD const_reverse_iterator_pair_type get_criterator_pair() const noexcept { return make_iterator_pair(rbegin(), rend()); }

    HAD_NODISCARD iterator begin() noexcept { return data();}
    HAD_NODISCARD const_iterator begin() const noexcept { return data();}
    HAD_NODISCARD iterator end() noexcept { return data() + mSize;}
    HAD_NODISCARD const_iterator end() const noexcept { return data() + mSize;}



    HAD_NODISCARD reverse_iterator rbegin() noexcept { return end() - 1; }
    HAD_NODISCARD const_reverse_iterator rbegin() const noexcept { return end() - 1; }

    HAD_NODISCARD reverse_iterator rend() noexcept { return begin() - 1; }
    HAD_NODISCARD const_reverse_iterator rend() const noexcept { return begin() - 1; }

    HAD_NODISCARD const_iterator cbegin() const noexcept {return begin();}
    HAD_NODISCARD const_iterator cend() const noexcept {return end();}

    HAD_NODISCARD const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    HAD_NODISCARD const_reverse_iterator crend() const noexcept { return rend(); }
    
    allocator_type& get_allocator() noexcept { return mPair.get_first(); }
    const allocator_type& get_allocator() const noexcept { return mPair.get_first(); }
    
    template<typename... Args>
    iterator emplace_back(Args&&... args) {
        growIfNeeded(1);
        pointer_type ptr = data() + mSize;
        ++mSize;
        return HAD_NS construct_at(ptr, HAD_NS forward<Args>(args)...);
    }

    iterator push_back(const value_type& value) {
        return emplace_back(value);
    }

    iterator push_back(value_type&& value) {
        return emplace_back(HAD_NS move(value));
    }

    template<typename... Args>
    iterator emplace(const_iterator where,Args&&... args) {
        const auto old_begin = cbegin();
        const auto old_end   = cend();
        
        growIfNeeded(1);
        if (where == old_end) {
            auto ptr = HAD_NS construct_at(begin() + mSize,
                HAD_NS forward<Args>(args)...);
            ++mSize;
            return const_cast<iterator>(ptr);
        }
        const auto my_begin = begin();
        const auto distance = HAD_NS distance(old_begin, where);
        auto new_where = my_begin + distance;
        // 1 2 3 4 5 ^
        // 1 2 0 3 4 5
        HAD_NS construct_at(end(), HAD_NS move_if_noexcept(back()));
        for (iterator iter = HAD_NS addressof(back()); iter != new_where; --iter)
            *iter = *(iter - 1);
        auto ptr = HAD_NS construct_at(new_where, HAD_NS forward<Args>(args)...);
        ++mSize;
        return const_cast<iterator>(ptr);
    }

    iterator insert(const_iterator where,const value_type& value) {
        return emplace(where,value);
    }
    iterator insert(const_iterator where,value_type&& value) {
        return emplace(where,HAD_NS move(value));
    }
    HAD_CONSTEXPR20 void pop_back() noexcept(noexcept(HAD_NS destroy_at(data()))) {
        assert(mSize != 0);
        HAD_NS destroy_at(HAD_NS addressof(back()));
        --mSize;
    }


    void erase(const_iterator where) {
        iterator where_ = const_cast<iterator>(where); // safe
        HAD_NS destroy_at(where);
        if (where == end() - 1)
            return;

        const auto my_end = this->end();
        // 1 2 3 4 5
        // 1 2 4 5
        HAD_NS construct_at(where_,HAD_NS move(*(where_+1)));
        for (auto iter = where_ + 2; iter != my_end; ++iter) {
            *(iter-1) = HAD_NS move(*iter);
        }

        mSize -= 1;
    }

    void erase(const_iterator begin,const_iterator end) 
        noexcept(nothrow_move_constructible)
    {
        HAD_NS destroy({ begin,end });

        const auto my_end = this->end();
        // 1 2 3 4 5
        // 3
        const auto distance = HAD_NS distance(begin, end);
        auto non_const_begin = const_cast<iterator>(begin);
        for (auto iter = 1 + distance + non_const_begin; iter != my_end; ++iter) {
            *(iter - distance) = HAD_NS move(*iter);
        }

        mSize -= distance;
    }
    void clear() noexcept {
        destroy(get_iterator_pair());

        mSize = 0;
    }

    template<typename Iter>
    void assign(Iter begin, Iter end) {
        clear();
        auto pair = HAD_NS make_iterator_pair(begin, end);
        HAD_NS uninitialized_copy(pair, this->begin());
        mSize = HAD_NS distance(begin, end);
    }

    void assign(std::initializer_list<value_type> ilist) {
        clear();
        auto pair = HAD_NS make_iterator_pair(ilist);
        HAD_NS uninitialized_copy(pair, begin());
        mSize = ilist.size();
    }

    void assign(size_t N, const value_type& value = value_type()) {
        clear();
        HAD_NS uninitialized_fill(get_iterator_pair(), value);
        mSize = N;
    }

    void reserve(size_type capacity) {
        if (capacity > mSize) {
            reAlloc(capacity);
        }
    }

    void swap(this_type& other) noexcept {
        using ::had::swap;
        swap(mPair, other.mPair);
        swap(mCapacity, other.mCapacity);
        swap(mSize, other.mSize);
    }

    friend void swap(this_type& a,this_type& b) noexcept {
        a.swap(b);
    }
public:

    reference_type at(size_type index) {
        if (index >= mSize) {
            throw std::logic_error("");
        }
        return data()[index];
    }
    const_reference_type at(size_type index) const {
        if (index >= mSize) {
            throw std::logic_error("");
        }
        return data()[index];
    }

    reference_type operator[](size_type index) noexcept {
        return data()[index];
    }
    const_reference_type operator[](size_type index) const noexcept {
        return data()[index];
    }
private:
    void growIfNeeded(size_type added_elements) {
        if (added_elements + mSize >= mCapacity) {
            reAlloc(calcGrowth(added_elements));
        }
    }
    void reAlloc(size_type new_capacity) {
        using Deleter = decltype([&](void* ptr) {
            get_allocator().deallocate(ptr, mCapacity);
            });
        HAD_NS unique_ptr<value_type[],Deleter> buffer(get_allocator().allocate(new_capacity));

        auto pair = make_iterator_pair(begin(), end());
        HAD_NS uninitialized_copy(pair, buffer.data());
        
        HAD_NS destroy(pair.begin, pair.end);

        get_allocator().deallocate(data(),mCapacity);
        getMyPtr() = buffer.release(nullptr);
        mCapacity = new_capacity;

    }
    pointer_type& getMyPtr() noexcept {
        return mPair.get_second();
    }
    const pointer_type& getMyPtr() const noexcept {
        return mPair.get_second();
    }


    constexpr size_type calcGrowth(size_type num) const noexcept {
        return ((mCapacity * 3) / 2) + num + 1;
    }
    

    HAD_NODISCARD friend bool operator==(const this_type& a, const this_type& b) noexcept {
        if (a.size() != b.size())
            return false;
        const auto pair = make_iterator_pair(a);
        return had::equal(pair, b.begin());
    }

#ifndef HAD_CPP20
    HAD_NODISCARD friend bool operator!=(const this_type& a, const this_type& b) noexcept {
        return !(a == b);
    }
#endif // !HAD_CPP20


private:
    size_type mSize;
    size_type mCapacity;
    compressed_pair<allocator_type,pointer_type> mPair;
};


#ifdef __cpp_deduction_guides

template<typename T>
vector(std::initializer_list<T>) -> vector<T>;

template<typename NumType,typename ValueType,enable_if_t<is_integral_v<NumType>> = 0 >
vector(NumType, const ValueType&) -> vector<ValueType>;

template<typename Iterator, enable_if_t < is_iterator_v<Iterator> > = 0 >
vector(Iterator, Iterator) -> vector<typename iterator_traits<Iterator>::value_type>;
#endif

template<typename T>
struct formatter<vector<T>> {
    static ::had::string to_string(const vector<T>& input) {
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

namespace details {

template<typename>
struct is_vector : false_type {};

template<typename T>
struct is_vector<vector<T>> : true_type {};


}

template<typename T>
struct is_vector : details::is_vector<typename remove_cv<T>::type> {

};

HAD_NAMESPACE_END



#endif // !HAD_VECTOR_HPP