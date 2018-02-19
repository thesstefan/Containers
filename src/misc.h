#pragma once

template <typename ItemType>
class VectorCompare {
    public:
        template <typename IteratorType>
        int execute(IteratorType first_1, IteratorType last_1, IteratorType first_2, IteratorType last_2) const;
};

template <typename ItemType>
template <typename IteratorType>
int VectorCompare<ItemType>::execute(IteratorType first_1, IteratorType end_1, IteratorType first_2, IteratorType end_2) const {
    auto it_1 = first_1;
    auto it_2 = first_2;

    for ( ; it_1 != end_1 && it_2 != end_2; it_1++, it_2++) {
        if (*it_1 < *it_2)
            return -1;
        else if (*it_1 > *it_2)
            return 1;
    }

    if (it_1 == end_1 && it_2 != end_2)
        return -1;
    else if (it_1 != end_1 && it_2 == end_2)
        return 1;

    return 0;
}

template <typename ItemType>
class NodeCompare {
    public:
        template <typename IteratorType>
        int execute(IteratorType first_1, IteratorType last_1, IteratorType first_2, IteratorType last_2) const;
};

template <typename ItemType>
template <typename IteratorType> 
int NodeCompare<ItemType>::execute(IteratorType first_1, IteratorType last_1, IteratorType first_2, IteratorType last_2) const {
    auto it_1 = first_1;
    auto it_2 = first_2;

    for ( ; it_1 != last_1 && it_2 != last_2; it_1++, it_2++) {
        if (it_1->key < it_2->key)
            return -1;
        else if (it_1->key > it_2->key)
            return 1;
    }

    if (it_1 == last_1 && it_2 != last_2)
        return -1;
    else if (it_1 != last_1 && it_2 == last_2)
        return 1;

    return 0;
}

template <typename IteratorType>
ptrdiff_t distance(IteratorType first, IteratorType second) {
    ptrdiff_t distance = 0;

    for (auto it = first; it != second; it++, distance++);

    return distance;
}
