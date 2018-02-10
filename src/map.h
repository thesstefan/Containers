#pragma once

#include "pair.h"
#include "key_tree.h"
#include "misc.h"

template <typename KeyType, typename ValueType, typename Compare = NodeCompare<KeyType>>
class Map {
    typedef typename KeyTree<KeyType, ValueType, Compare>::Iterator Iterator;
    typedef typename KeyTree<KeyType, ValueType, Compare>::ConstIterator ConstIterator;

    private:
        KeyTree<KeyType, ValueType, Compare> data;

    public:
        Map();
        Map(const Map& other);

        template <typename IteratorType>
        Map(IteratorType first, IteratorType last);

        ~Map();

        Map& operator=(const Map &other); 

        ValueType& at(const KeyType& key);
        const ValueType& at(const KeyType& key) const;

        ValueType& operator[](const KeyType& key);

        Iterator begin();
        Iterator end();

        ConstIterator cbegin() const;
        ConstIterator cend() const;

        bool empty() const;
        size_t size() const;

        void clear();

        Iterator insert(const Pair<KeyType, ValueType>& pair);
        Iterator insert(const KeyType& key, const ValueType& value);

        template <typename IteratorType>
        void insert(IteratorType first, IteratorType last);

        void erase(Iterator position);
        size_t erase(const KeyType& key);

        template <typename IteratorType>
        void erase(IteratorType first, IteratorType last);

        void swap(Map<KeyType, ValueType, Compare>& other);

        size_t count (const KeyType& key) const;

        Iterator find(const KeyType& key);
        ConstIterator find(const KeyType& key) const;

        Iterator lower_bound(const KeyType& key);
        ConstIterator lower_bound(const KeyType& key) const;

        Iterator upper_bound(const KeyType &key);
        ConstIterator upper_bound(const KeyType& key) const;

        Pair<Iterator, Iterator> equal_range(const KeyType& key);
        Pair<ConstIterator, ConstIterator> equal_range(const KeyType& key) const;

        bool operator==(const Map& other) const;
        bool operator!=(const Map& other) const;

        bool operator<(const Map& other) const;
        bool operator>(const Map& other) const;
        bool operator<=(const Map& other) const;
        bool operator>=(const Map& other) const;
};

template <typename KeyType, typename ValueType, typename Compare>
Map<KeyType, ValueType, Compare>::Map() {
    this->data = KeyTree<KeyType, ValueType>();
}

template <typename KeyType, typename ValueType, typename Compare>
template <typename IteratorType>
Map<KeyType, ValueType, Compare>::Map(IteratorType first, IteratorType last) {
    this->data = KeyTree<KeyType, ValueType>();

    for (auto it = first; it != last; it++)
        this->data.insert(*it);
}

template <typename KeyType, typename ValueType, typename Compare>
Map<KeyType, ValueType, Compare>::Map(const Map& other) {
    this->data = other.data;
}

template <typename KeyType, typename ValueType, typename Compare>
Map<KeyType, ValueType, Compare>::~Map() {}

template <typename KeyType, typename ValueType, typename Compare>
Map<KeyType, ValueType, Compare>& Map<KeyType, ValueType, Compare>::operator=(const Map& other) {
    this->data = other.data;
}

template <typename KeyType, typename ValueType, typename Compare>
ValueType& Map<KeyType, ValueType, Compare>::at(const KeyType& key) {
    auto result = this->data.search(key);

    if (result == nullptr)
        throw std::out_of_range("KeyType is not present in Map (Map::at(const KeyType& key))");

    return result->value;
}

template <typename KeyType, typename ValueType, typename Compare>
const ValueType& Map<KeyType, ValueType, Compare>::at(const KeyType& key) const {
    auto result = this->data.search(key);

    if (result == nullptr)
        throw std::out_of_range("KeyType is not present in Map (Map::at(const KeyType& key))");

    return result->value;
}

template <typename KeyType, typename ValueType, typename Compare>
ValueType& Map<KeyType, ValueType, Compare>::operator[](const KeyType& key) {
    auto find = this->data.search(key);

    if (find == nullptr)
        return this->insert(key, ValueType())->value;

    return find->value;
}

template <typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::begin() {
    return this->data.begin();
}

template <typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::end() {
    return this->data.end();
}

template <typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator Map<KeyType, ValueType, Compare>::cbegin() const {
    return this->data.cbegin();
}

template <typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator Map<KeyType, ValueType, Compare>::cend() const {
    return this->data.cend();
}

template <typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::empty() const {
    return this->data.cbegin() == this->data.cend();
}

template <typename KeyType, typename ValueType, typename Compare>
size_t Map<KeyType, ValueType, Compare>::size() const {
    return distance(this->data.cbegin(), this->data.cend());
}

template <typename KeyType, typename ValueType, typename Compare>
void Map<KeyType, ValueType, Compare>::clear() {
    this->erase(this->begin(), this->end());
}

template <typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::insert(const Pair<KeyType, ValueType>& pair) {
    this->data.insert(pair.first, pair.second);

    return Map<KeyType, ValueType, Compare>::Iterator(this->data.search(pair.first), this->data.max());
}

template <typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::insert(const KeyType& key, const ValueType& value) {
    this->data.insert(key, value);

    return Map<KeyType, ValueType, Compare>::Iterator(this->data.search(key), this->data.max());
}

template <typename KeyType, typename ValueType, typename Compare>
template <typename IteratorType>
void Map<KeyType, ValueType, Compare>::insert(IteratorType first, IteratorType last) {
    for (auto it = first; it != last; it++)
        this->data.insert(it->key, it->value);
}

template <typename KeyType, typename ValueType, typename Compare>
void Map<KeyType, ValueType, Compare>::erase(typename Map<KeyType, ValueType, Compare>::Iterator position) {
    this->data.erase(position->key);
}

template <typename KeyType, typename ValueType, typename Compare>
template <typename IteratorType>
void Map<KeyType, ValueType, Compare>::erase(IteratorType first, IteratorType last) {
    for (auto it = first; it != last; it++)
        this->data.erase(it->key);
}

template <typename KeyType, typename ValueType, typename Compare>
size_t Map<KeyType, ValueType, Compare>::erase(const KeyType& key) {
    size_t count = 0;

    while (this->data.search(key) != nullptr) {
        this->data.erase(key);

        count++;
    }

    return count;
}

template <typename KeyType, typename ValueType, typename Compare>
void Map<KeyType, ValueType, Compare>::swap(Map<KeyType, ValueType, Compare>& other) {
    auto temp = this->data;

    this->data = other.data;

    other.data = temp;
}

template <typename KeyType, typename ValueType, typename Compare>
size_t Map<KeyType, ValueType, Compare>::count(const KeyType& key) const {
    return (this->data.search(key) != nullptr) ? 1 : 0;
}

template <typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::find(const KeyType& key) {
    return Map<KeyType, ValueType, Compare>::Iterator(this->data.search(key), this->data.max());
}

template <typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator Map<KeyType, ValueType, Compare>::find(const KeyType& key) const {
    return Map<KeyType, ValueType, Compare>::ConstIterator(this->data.search(key), this->data.max());
}

template <typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::lower_bound(const KeyType& key) {
    auto it = this->begin();

    for ( ; it != this->end() && !(key < it->key); it++);

    return it;
}

template <typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator Map<KeyType, ValueType, Compare>::lower_bound(const KeyType& key) const {
    auto it = this->cbegin();

    for ( ; it != this->cend() && !(key < it->key); it++);

    return it;
}

template <typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::Iterator Map<KeyType, ValueType, Compare>::upper_bound(const KeyType& key) {
    auto it = this->begin();

    for ( ; it != this->end() && key > it->key ; it++);

    return it;
}

template <typename KeyType, typename ValueType, typename Compare>
typename Map<KeyType, ValueType, Compare>::ConstIterator Map<KeyType, ValueType, Compare>::upper_bound(const KeyType& key) const {
    auto it = this->cbegin();

    for ( ; it != this->cend() && key > it->key ; it++);

    return it;
}

template <typename KeyType, typename ValueType, typename Compare>
Pair<typename Map<KeyType, ValueType, Compare>::Iterator, typename Map<KeyType, ValueType, Compare>::Iterator> Map<KeyType, ValueType, Compare>::equal_range(const KeyType& key) {
    return Pair<typename Map<KeyType, ValueType, Compare>::Iterator, typename Map<KeyType, ValueType, Compare>::Iterator>(this->lower_bound(key), this->upper_bound(key));
}

template <typename KeyType, typename ValueType, typename Compare>
Pair<typename Map<KeyType, ValueType, Compare>::ConstIterator, typename Map<KeyType, ValueType, Compare>::ConstIterator> Map<KeyType, ValueType, Compare>::equal_range(const KeyType& key) const {
    return Pair<typename Map<KeyType, ValueType, Compare>::ConstIterator, typename Map<KeyType, ValueType, Compare>::ConstIterator>(this->lower_bound(key), this->upper_bound(key));
}

template <typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::operator==(const Map<KeyType, ValueType, Compare>& other) const {
    return this->data == other.data;
}

template <typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::operator!=(const Map<KeyType, ValueType, Compare>& other) const {
    return this->data != other.data;
}

template <typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::operator<(const Map<KeyType, ValueType, Compare>& other) const {
    return this->data < other.data;
}

template <typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::operator>(const Map<KeyType, ValueType, Compare>& other) const {
    return this->data > other.data;
}

template <typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::operator<=(const Map<KeyType, ValueType, Compare>& other) const {
    return this->data < other.data;
}

template <typename KeyType, typename ValueType, typename Compare>
bool Map<KeyType, ValueType, Compare>::operator>=(const Map<KeyType, ValueType, Compare>& other) const {
    return this->data < other.data;
}
