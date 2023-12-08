#pragma once
#ifndef HAD_ALGORITHM_UNINITIALIZED_COPY_HPP
#define HAD_ALGORITHM_UNINITIALIZED_COPY_HPP 1
#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/is_trivially_copy_constructible.hpp>
#include <had/type_traits/is_trivially_copyable.hpp>
#include <had/iterator/iterator_traits.hpp>
#include <had/iterator/utilities.hpp>
#include <had/utility/declval.hpp>
#include <had/macros.hpp>
#include <cstring>
HAD_NAMESPACE_BEGIN


template<typename Iterator,
    typename enable_if <
       is_trivially_copyable<Iterator>::value
    >::type = 0
>
Iterator* uninitialized_copy(
      iterator_pair<const Iterator*> source,
      Iterator* destination) {

    const auto size = sizeof(*destination) * HAD_NS distance(source.begin, source.end);
    std::memmove(destination, source.begin, size);
    return destination;
}


template<typename InputIterator,typename OutputIterator>
OutputIterator uninitialized_copy(
      iterator_pair<InputIterator> source,
      OutputIterator destination) {
    using T = typename iterator_traits<OutputIterator>::value_type;
    for (; source.begin != source.end; ++source.begin, ++destination) {
        ::new (HAD_NS addressof(*destination)) T(*source.begin);
    }
    return destination;
}

//
//template<typename InputIterator, typename OutputIterator,
//    typename enable_if <
//    is_trivially_copyable<InputIterator>::value
//    && is_trivially_copyable<OutputIterator>::value
//    && sizeof(InputIterator) == sizeof(OutputIterator)
//    >::type = 0
//>
//inline void unititialized_copy_no_overlap(
//    const InputIterator* HAD_RESTRICT in_begin,
//    const InputIterator* HAD_RESTRICT in_end,
//    OutputIterator* HAD_RESTRICT out) {
//
//    const auto count = sizeof(*out) * distance(in_begin, in_end);
//    std::memcpy(out, in_begin,count);
//}

template<typename InputIterator, typename OutputIterator>
OutputIterator uninitialized_copy_no_overlap(
    InputIterator in_begin,
    InputIterator in_end,
    OutputIterator out) {
    using T = typename iterator_traits<OutputIterator>::value_type;
    for (; in_begin != in_end; ++in_begin, ++out) {
        ::new (HAD_NS addressof(*out)) T(*in_begin);
    }
    return out;
}

HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_UNINITIALIZED_COPY_HPP

