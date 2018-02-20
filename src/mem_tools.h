#pragma once

#include "iterator.h"

template <typename ForwardIterator, typename Size, typename ItemType, typename Allocator>
void uninitialized_fill(ForwardIterator first, Size size, const ItemType& value, const Allocator &allocator) {
    auto temp_allocator(allocator);

    for ( ; size--; first++)
        temp_allocator.construct(first, typename IteratorTraits<ForwardIterator>::value_type(value));
}

template <typename InputIterator, typename ForwardIterator, typename Allocator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator to_first, const Allocator &allocator) {
    auto temp_allocator(allocator);

    for ( ; first != last; first++, to_first++);
        temp_allocator.construct(to_first, typename IteratorTraits<ForwardIterator>::value_type(*first));

    return to_first;
}
