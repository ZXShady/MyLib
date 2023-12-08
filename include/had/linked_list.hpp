#pragma once
#ifndef HAD_LINKED_LIST_HPP
#define HAD_LINKED_LIST_HPP

#include <had/initializer_list.hpp>
#include <had/iterator/reverse_iterator.hpp>
#include <had/config.hpp>

HAD_NAMESPACE_BEGIN
template<typename ValueType>
class singly_linked_list;

namespace details {
template<typename T>
struct singly_linked_list_node_t {
    using this_type = singly_linked_list_node_t;
    T          value;
    this_type* next;
    };

template<typename ValueType>
class singly_linked_list_iterator {
public:
    friend class ::had::singly_linked_list<ValueType>;
    using this_type = singly_linked_list_iterator;
    using size_type = size_t;
    using node_type = singly_linked_list_node_t<ValueType>;
    using value_type = ValueType;
    using pointer_type = value_type*;
    using reference_type = value_type&;
    using const_pointer_type = const value_type*;
    using const_reference_type = const value_type&;
public:
    singly_linked_list_iterator() noexcept
        :mPtr(nullptr) {
    }

    singly_linked_list_iterator(node_type* ptr) noexcept
        :mPtr(ptr) {
    }

    this_type& operator++() noexcept {
        mPtr = mPtr->next;
        return *this;
    }
    this_type operator++(int) noexcept {
        auto temp{ *this };
        ++(*this);
        return temp;
    }

    this_type& operator+=(ptrdiff_t num) const noexcept {
        HAD_ASSERT_IF(num < 0,"from had::singly_linked_list_iterator::operator+(diff_t num) does not allow num to be negative since this is a singly linked list it cannot go backwards");
        for (ptrdiff_t i = 0; i < num;++i)
            ++(*this);
        return *this;
    }

    this_type operator+(ptrdiff_t num) const noexcept {
        HAD_ASSERT_IF(num < 0,"from had::singly_linked_list_iterator::operator+(diff_t num) does not allow num to be negative since this is a singly linked list it cannot go backwards");

        this_type copy{*this};
        for (ptrdiff_t i = 0; i < num;++i) {
            ++copy;
        }
        return copy;
    }
    HAD_NODISCARD bool operator==(this_type other) const noexcept {
        return mPtr == other.mPtr;
    }
    HAD_NODISCARD bool operator!=(this_type other) const noexcept {
        return !(*this == other);
    }
    HAD_NODISCARD reference_type operator*() noexcept { return mPtr->value; }
    HAD_NODISCARD const_reference_type operator*() const noexcept { return mPtr->value; }
    HAD_NODISCARD pointer_type operator->() noexcept { return &mPtr->value; }
    HAD_NODISCARD const_pointer_type operator->() const noexcept { return &mPtr->value; }
private:
    node_type* mPtr;
};
} // namesapce details

template<typename ValueType>
class singly_linked_list {
private:
public:
    friend class details::singly_linked_list_iterator<ValueType>;
    using this_type             = singly_linked_list;
    using size_type             = size_t;
    using node_type             = details::singly_linked_list_node_t<ValueType>;
    using value_type            = ValueType;
    using pointer_type          = value_type*;
    using reference_type        = value_type&;
    using const_pointer_type    = const value_type*;
    using const_reference_type  = const value_type&;
    using iterator              = details::singly_linked_list_iterator<value_type>;
    using const_iterator        = details::singly_linked_list_iterator<const value_type>;
public:
    singly_linked_list(std::initializer_list<ValueType> values) 
    : mSize(values.size())
    , mHead(new node_type(*values.begin()))
    , mTail(mHead)
    {
        const auto begin = values.begin();
        const auto end   = values.end();
        for (auto i = begin + 1; i != end - 1;++i) {
            mTail->next = new node_type(*i);
            mTail = mTail->next;
        }
        auto last_elem = values.end() - 1;
        mTail->next = new node_type(*last_elem);
        mTail = mTail->next;
        mTail->next->next = nullptr;
    }

    explicit singly_linked_list(size_type count,const value_type& value = value_type())
    : mSize(count)
    , mHead(new node_type(*values.begin(),nullptr))
    , mTail(mHead)
    {
        const auto begin = values.begin();
        const auto end   = values.end();
        
        for (size_type i = 0; i < count-1;++i) {
            mTail->next = new node_type(value);
            mTail = mTail->next;
        }
        mTail->next = new node_type(value);
        mTail->next->next = nullptr;
        //mTail->next->next = new node_type(value);
    }



    singly_linked_list()
    : mSize(0)
    , mHead(nullptr)
    , mTail(nullptr)
    {
    }

    
    ~singly_linked_list() {
        for (node_type* i = mHead; i != nullptr;) {
            auto copy = i;
            i = i->next;
            delete copy;
        }
    }

    template<typename... Args>
    iterator emplace_at(iterator where,Args&&... args) {
        // 1 -> 2 -> 3 -> 4
        // ______| insert 10
        // 1 -> 2 -> 10 -> 3 -> 4
        // "3" = "10"
        // 2 points to 3
        
        node_type* ptr      = where.mPtr;
        node_type* next     = where.mPtr->next;
        ptr->next           = new node_type(ptr->value, next);
        ptr->value          = value_type(forward<Args>(args)...);
        mSize++;
        return &(*ptr);
    }


    iterator insert(iterator where,const value_type& value ) {
        return emplace_at(where, value);
    }

    iterator insert(iterator where,value_type&& value ) {
        return emplace_at(where, move(value));
    }

    iterator push_front(const value_type& value) {
        return emplace_at(mHead, value);
    }
    iterator push_front(value_type&& value) {
        return emplace_at(mHead, move(value));
    }
    iterator push_back(const value_type& value) {
        emplace_at(mTail, value)
        return mTail;
    }
    iterator push_back(value_type&& value) {
        return emplace_at(mTail, move(value));
    }
    reference_type front() noexcept { return mHead->value; }
    const_reference_type front() const noexcept { return mHead->value; }
    reference_type back() noexcept { return mTail->value; }
    const_reference_type back() const noexcept { return mTail->value; }

    HAD_NODISCARD size_type size() const noexcept { return mSize; }
    HAD_NODISCARD iterator begin() noexcept { return mHead; }
    HAD_NODISCARD iterator end() noexcept { return mTail->next; }
    HAD_NODISCARD const_iterator begin() const noexcept { return mHead; }
    HAD_NODISCARD const_iterator end() const noexcept { return mTail->next; }
    HAD_NODISCARD const_iterator cbegin() const noexcept { return begin(); }
    HAD_NODISCARD const_iterator cend() const noexcept { return end(); }
public:
    size_type  mSize;
    node_type* mHead;
    node_type* mTail;
};
HAD_NAMESPACE_END
#endif // !HAD_LINKED_LIST_HPP
