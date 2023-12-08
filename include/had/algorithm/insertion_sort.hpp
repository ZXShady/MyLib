#pragma once

#include <had/version/namespace.hpp>

HAD_NAMESPACE_BEGIN

template<typename Container>
void insertion_sort(Container& container) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        // Move elements of arr[0..i-1],
        // that are greater than key,
        // to one position ahead of their
        // current position
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    
    const auto begin = container.begin();
    const auto end = container.end();
    auto it = container.begin() + 1;
    decltype(it) it2;
    decltype(it) temp;

    //signed long long i, j, temp;
    for (; it != end; ++it) {
        temp = it;  
        it2 = it - 1;  

        while(it2 >= begin && *temp <= *(it2))  /* Move the elements greater than temp to one position ahead from their current position*/  
        {    
            // container.begin() + j + 1
            *(it2 + 1) = *(it2);     
            --it2;    
        }    
        *(it2 + 1) = *temp;    
    }  
    //decltype(container.begin()) temp;
    //decltype(container.begin()) i;
    //decltype(container.begin()) j;
    //for (i = begin + 1; i != end; ++i) {
    //    temp = i;
    //    j = i - 1;
    //    while (j != begin-1 && *temp <= *j) {
    //        *(j + 1) = *j;
    //        --j;
    //    }
    //    *(j + 1) = *temp;
    //}
}

HAD_NAMESPACE_END