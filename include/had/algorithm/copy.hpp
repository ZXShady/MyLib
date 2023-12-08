#pragma once
#ifndef HAD_ALGORITHM_COPY_HPP
#define HAD_ALGORITHM_COPY_HPP
#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/version/attributes/constexpr.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/is_trivially_copy_constructible.hpp>
#include <had/type_traits/is_trivially_copyable.hpp>
#include <had/type_traits/is_pointing_to_same_type.hpp>
#include <had/type_traits/is_random_access_iterator.hpp>
#include <had/utility/declval.hpp>
#include <had/utility/distance.hpp>
#include <had/iterator/iterator_traits.hpp>
#include <cstring>

HAD_NAMESPACE_BEGIN

template<typename InputIt,typename OutputIt>
OutputIt copy(iterator_pair<InputIt> source, OutputIt destination) {
    for(; source.begin != source.end; ++source.begin, ++destination) {
        *destination = *source.begin;
    }
    return destination;
}

//template<typename Iter,typename enable_if<is_trivially_copyable<Iter>::value>::type = 0 >
//Iter copy(iterator_pair<Iter*> source, Iter* destination) {
//    std::memmove(source.begin, destination, distance(source.begin,source.end));
//    return destination;
//}


template<typename InputIt,typename OutputIt>
OutputIt copy_no_overlap(iterator_pair<InputIt> source, OutputIt destination) {
    for(; source.begin != source.end; ++source.begin, ++destination) {
        *destination = *source.begin;
    }
    return destination;
}

//template<typename Iter,typename enable_if<is_trivially_copyable<Iter>::value>::type = 0 >
//Iter copy_no_overlap(iterator_pair<Iter*> source, Iter* destination) {
//    std::memcpy(source.begin, destination, distance(source.begin,source.end));
//    return destination;
//}



HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_COPY

