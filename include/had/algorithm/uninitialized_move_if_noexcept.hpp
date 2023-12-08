#pragma once
#ifndef HAD_ALGORITHM_UNINITIALIZED_MOVE_IF_NOEXCEPT_HPP
#define HAD_ALGORITHM_UNINITIALIZED_MOVE_IF_NOEXCEPT_HPP 1
#include <had/version/namespace.hpp>
#include <had/version/attributes/nodiscard.hpp>
#include <had/type_traits/is_trivially_copyable.hpp>
#include <had/utility/declval.hpp>
#include <had/type_traits/move.hpp>
#include <had/type_traits/enable_if.hpp>
#include <had/type_traits/move_if_noexcept.hpp>
#include <had/utility/construct_at.hpp>
#include <had/type_traits/addressof.hpp>
#include <had/iterator/iterator_traits.hpp>
#include <cstring>

HAD_NAMESPACE_BEGIN

template<typename Iterator,
    typename enable_if <
    is_trivially_copyable<Iterator>::value
    >::type = 0
>
Iterator* uninitialized_move_if_noexcept(
    iterator_pair<const Iterator*> source,
    Iterator* destination) {

    const auto size = sizeof(*destination) * HAD_NS distance(source.begin, source.end);
    std::memmove(destination, source.begin, size);
    return destination;
}


template<typename InputIterator,typename OutputIterator>
OutputIterator uninitialized_move_if_noexcept(
    iterator_pair<InputIterator> source,
    OutputIterator destination) {
    for (; source.begin != source.end; ++source.begin, ++destination) {
        HAD_NS construct_at(HAD_NS addressof(*destination),HAD_NS move_if_noexcept(*source.begin));
    }
    return destination;
}


HAD_NAMESPACE_END
#endif // !HAD_ALGORITHM_UNINITIALIZED_MOVE_IF_NOEXCEPT_HPP

