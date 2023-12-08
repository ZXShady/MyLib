
#ifndef HAD_ALGORITHM_MERGE_SORT_HPP
#define HAD_ALGORITHM_MERGE_SORT_HPP 1

#include <had/version/namespace.hpp>
#include <had/type_traits/remove_reference.hpp>

HAD_NAMESPACE_BEGIN

namespace details {
template<typename Iterator>
void merge_impl(Iterator l, Iterator m, Iterator r) {
    size_t i, j;
    size_t k;
    size_t n1 = m - l + 1;
    size_t n2 = r - m;
    using T = remove_reference_t<decltype(*l)>;
    /* create temp arrays */
    T* LeftArray  = new T[n1];
    T* RightArray = new T[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        LeftArray[i] = *(l + i);
    for (j = 0; j < n2; j++)
        RightArray[j] = *(m + 1 + j);

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = 0; // Initial index of merged subarray 
    while (i < n1 && j < n2) {
        if (LeftArray[i] <= RightArray[j]) {
            *(l + k) = LeftArray[i];
            i++;
        }
        else {
            *(l + k) = RightArray[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        *(l + k) = LeftArray[i];
        i++;
        k++;
    }
    while (j < n2) {
        *(l + k) = RightArray[j];
        j++;
        k++;
    }
}
} // namespace details
template<typename Container>
Container::iterator get_mid(Container& container) {
    const auto sz = container.size() / 2;
    auto iterator = container.begin();
    return iterator + sz;
}

template<typename Iterator>
Iterator get_mid(Iterator iterator,size_t size) {
    const auto sz = size / 2;
    return iterator + sz;
}

template<typename Iterator>
void merge_sort(Iterator begin,Iterator end) {
    
    if (begin == end)
        return;
    const auto middle = get_mid(begin,[&]{
        size_t cnt = 0;
        auto it = begin;
        for (; it != end; ++it) {
            cnt++;
        }
        for (auto i = 0, it = begin; i < cnt; ++it,++i);
        return cnt;
        }() );

    merge_sort(begin, middle);
    merge_sort(middle + 1, end);

    details::merge_impl(begin, middle, end);
}

HAD_NAMESPACE_END

#endif // !HAD_ALGORITHM_MERGE_SORT_HPP