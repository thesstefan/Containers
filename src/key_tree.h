// Contains
//      ptrdiff_t distance(IteratorType, IteratorType)
//      default Compare object
#include "misc.h"

#pragma once

template <typename KeyType, typename ValueType, typename Compare = NodeCompare<KeyType>>
class KeyTree {
    private:
        class Node {
            public:
                KeyType key;
                ValueType value;

                Node *parent;

                Node *left;
                Node *right;

                Node();
                Node(const KeyType& key, const ValueType& value);
                Node(const KeyType& key, const ValueType& value, Node *parent);
        };

        Node *root;
        Compare compare;

        void real_delete(Node *leaf);

        Node *real_insert(Node *leaf, Node *parent, const KeyType& key, const ValueType& value);
        Node *real_erase(Node *leaf, const KeyType& key);

        Node *real_search(Node *leaf, const KeyType& key) const;

        Node *min_helper(Node *leaf) const;
        Node *max_helper(Node *leaf) const;

        void real_print(Node *leaf) const;

    public:
        class Iterator {
            private:
                Node *current;
                Node *max;

                void increment();
                void decrement();

            public:
                Iterator();
                Iterator(Node *node, Node *max);
                Iterator(const Iterator& iterator);

                Iterator& operator=(const Iterator& iterator);

                Iterator operator++();
                Iterator operator++(int);

                Iterator operator--();
                Iterator operator--(int);

                Node &operator*();
                Node *operator->();

                bool operator==(const Iterator& iterator) const;
                bool operator!=(const Iterator& iterator) const;
        };

        class ConstIterator {
            private:
                Node *current;
                Node *max;

                void increment();
                void decrement();

            public:
                ConstIterator();
                ConstIterator(Node *node, Node *max);
                ConstIterator(const ConstIterator& iterator);

                ConstIterator& operator=(const ConstIterator& iterator);

                ConstIterator operator++();
                ConstIterator operator++(int);

                ConstIterator operator--();
                ConstIterator operator--(int);

                const Node &operator*() const;
                const Node *operator->() const;

                bool operator==(const ConstIterator& iterator) const;
                bool operator!=(const ConstIterator& iterator) const;
        };

        KeyTree();
        KeyTree(const KeyTree& other);
        
        KeyTree& operator=(const KeyTree& other);

        ~KeyTree();

        void insert(const KeyType& key, const ValueType& value);
        void erase(const KeyType& key);

        Node *search(const KeyType& key) const;

        void print() const;

        Iterator begin();
        Iterator end();

        ConstIterator cbegin() const;
        ConstIterator cend() const;

        Node *min() const;
        Node *max() const;

        bool operator==(const KeyTree& other) const;
        bool operator!=(const KeyTree& other) const;

        bool operator<(const KeyTree& other) const;
        bool operator>(const KeyTree& other) const;
        bool operator<=(const KeyTree& other) const;
        bool operator>=(const KeyTree& other) const;
};

template <typename KeyType, typename ValueType, typename Compare>
KeyTree<KeyType, ValueType, Compare>::Node::Node() {
    this->key = KeyType();
    this->value = ValueType();

    this->parent = nullptr;

    this->left = nullptr;
    this->right = nullptr;
}

template <typename KeyType, typename ValueType, typename Compare>
KeyTree<KeyType, ValueType, Compare>::Node::Node(const KeyType& key, const ValueType& value) {
    this->key = key;
    this->value = value;

    this->parent = nullptr;

    this->left = nullptr;
    this->right = nullptr;
}

template <typename KeyType, typename ValueType, typename Compare>
KeyTree<KeyType, ValueType, Compare>::Node::Node(const KeyType& key, const ValueType& value, Node* parent) {
    this->key = key;
    this->value = value;

    this->parent = parent;

    this->left = nullptr;
    this->right = nullptr;
}

template <typename KeyType, typename ValueType, typename Compare>
KeyTree<KeyType, ValueType, Compare>::KeyTree() {
    this->root = nullptr;
}

template <typename KeyType, typename ValueType, typename Compare>
KeyTree<KeyType, ValueType, Compare>::KeyTree(const KeyTree &other) {
    this->root = nullptr;

    for (auto it = other.cbegin(); it != other.cend(); it++)
        this->insert(it->key, it->value);

}

template <typename KeyType, typename ValueType, typename Compare>
KeyTree<KeyType, ValueType, Compare>& KeyTree<KeyType, ValueType, Compare>::operator=(const KeyTree &other) {
    this->real_delete(this->root);

    this->root = nullptr;

    for (auto it = other.cbegin(); it != other.cend(); it++)
        this->insert(it->key, it->value);
}

template <typename KeyType, typename ValueType, typename Compare>
void KeyTree<KeyType, ValueType, Compare>::real_delete(KeyTree<KeyType, ValueType, Compare>::Node *leaf) {
    if (leaf != nullptr) {
        this->real_delete(leaf->left);
        this->real_delete(leaf->right);

        delete leaf;
    }
}

template <typename KeyType, typename ValueType, typename Compare>
KeyTree<KeyType, ValueType, Compare>::~KeyTree() {
    this->real_delete(this->root);
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Node* KeyTree<KeyType, ValueType, Compare>::real_insert(KeyTree<KeyType, ValueType, Compare>::Node *leaf, KeyTree<KeyType, ValueType, Compare>::Node *parent, const KeyType& key, const ValueType& value) {
    if (leaf == nullptr)
        leaf = new Node(key, value, parent);
    else if (key < leaf->key)
        leaf->left = real_insert(leaf->left, leaf, key, value);
    else if (key > leaf->key)
        leaf->right = real_insert(leaf->right, leaf, key, value);

    return leaf;
}

template <typename KeyType, typename ValueType, typename Compare>
void KeyTree<KeyType, ValueType, Compare>::insert(const KeyType& key, const ValueType& value) {
    this->root = this->real_insert(this->root, this->root, key, value);
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Node* KeyTree<KeyType, ValueType, Compare>::min_helper(typename KeyTree<KeyType, ValueType, Compare>::Node *leaf) const {
    if (leaf == nullptr)
        return nullptr;
    else if (leaf->left == nullptr)
        return leaf;
    else 
        return min_helper(leaf->left);
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Node* KeyTree<KeyType, ValueType, Compare>::max_helper(typename KeyTree<KeyType, ValueType, Compare>::Node *leaf) const {
    if (leaf == nullptr)
        return nullptr;
    else if (leaf->right == nullptr)
        return leaf;
    else
        return max_helper(leaf->right);
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Node* KeyTree<KeyType, ValueType, Compare>::real_erase(KeyTree<KeyType, ValueType, Compare>::Node *leaf, const KeyType& key) {
    if (leaf == nullptr)
        return nullptr;
    else if (key < leaf->key)
        leaf->left = real_erase(leaf->left, key);
    else if (key > leaf->key)
        leaf->right = real_erase(leaf->right, key);
    else if (leaf->left == nullptr && leaf->right == nullptr) {
        delete leaf;

        leaf = nullptr;
    } else if (leaf->right == nullptr) {
        auto parent = leaf->parent;
        auto left = leaf->left;

        delete leaf;

        leaf = left;
        leaf->parent = parent;
    } else if (leaf->left == nullptr) {
        auto parent = leaf->parent;
        auto right = leaf->right;

        delete leaf;

        leaf = right;
        leaf->parent = parent;
    } else {
        auto temp = this->max_helper(leaf->left);
        auto parent = temp->parent;

        leaf->key = temp->key;
        leaf->value = temp->value;

        if (parent != leaf)
            parent->right = temp->left;
        else
            leaf->left = temp->left;

        delete temp;
        temp = nullptr;
    }

    return leaf;
}

template <typename KeyType, typename ValueType, typename Compare>
void KeyTree<KeyType, ValueType, Compare>::erase(const KeyType& key) {
    this->root = this->real_erase(this->root, key);
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Node* KeyTree<KeyType, ValueType, Compare>::real_search(KeyTree<KeyType, ValueType, Compare>::Node *leaf, const KeyType& key) const {
    if (leaf == nullptr || leaf->key == key)
        return leaf;

    if (key < leaf->key)
        return real_search(leaf->left, key);

    return real_search(leaf->right, key);
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Node* KeyTree<KeyType, ValueType, Compare>::search(const KeyType& key) const {
    return this->real_search(this->root, key);
}

template <typename KeyType, typename ValueType, typename Compare>
void KeyTree<KeyType, ValueType, Compare>::real_print(KeyTree<KeyType, ValueType, Compare>::Node *leaf) const {
    if (leaf != nullptr) {
        real_print(leaf->left);

        std::cout << '[' << leaf->key << ',' << leaf->value << ']' << std::endl;

        if (leaf->parent != nullptr)
            std::cout << "PARENT -> " << leaf->parent->key << std::endl;
        else 
            std::cout << "ROOT" << std::endl;

        std::cout << "LEFT -> ";

        if (leaf->left != nullptr)
            std::cout << leaf->left->key << std::endl;
        else 
            std::cout << "NOTHING" << std::endl;

        std::cout << "RIGHT -> ";

        if (leaf->right != nullptr)
            std::cout << leaf->right->key << std::endl;
        else 
            std::cout << "NOTHING" << std::endl;

        std::cout << std::endl;

        real_print(leaf->right);
    }
}

template <typename KeyType, typename ValueType, typename Compare>
void KeyTree<KeyType, ValueType, Compare>::print() const {
    this->real_print(root);

    std::cout << std::endl;
}

template <typename KeyType, typename ValueType, typename Compare>
KeyTree<KeyType, ValueType, Compare>::Iterator::Iterator() {
    this->current = nullptr;
    this->max = nullptr;
}

template <typename KeyType, typename ValueType, typename Compare>
KeyTree<KeyType, ValueType, Compare>::Iterator::Iterator(typename KeyTree<KeyType, ValueType, Compare>::Node *node, typename KeyTree<KeyType, ValueType, Compare>::Node *max) {
    this->current = node;
    this->max = max;
}

template <typename KeyType, typename ValueType, typename Compare>
KeyTree<KeyType, ValueType, Compare>::Iterator::Iterator(const typename KeyTree<KeyType, ValueType, Compare>::Iterator &iterator) {
    this->current = iterator.current;
    this->max = iterator.max;
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Iterator& KeyTree<KeyType, ValueType, Compare>::Iterator::operator=(const Iterator &iterator) {
    this->current = iterator.current;
    this->max = iterator.max;
}

template <typename KeyType, typename ValueType, typename Compare>
void KeyTree<KeyType, ValueType, Compare>::Iterator::increment() {
    if (this->current->parent == nullptr && this->current->left == nullptr && this->current->right == nullptr)
        this->current = nullptr;
    else if (this->current->right != nullptr) {
        this->current = this->current->right;

        while (this->current->left != nullptr)
            this->current = this->current->left;
    } else {
        auto node = this->current->parent;

        while (node && this->current == node->right) {
            this->current = node;

            node = node->parent;
        }

        if (this->current->right != node)
            this->current = node;
    }
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Iterator KeyTree<KeyType, ValueType, Compare>::Iterator::operator++() {
    this->increment();

    return *this;
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Iterator KeyTree<KeyType, ValueType, Compare>::Iterator::operator++(int) {
    auto iterator = *this;

    this->increment();

    return iterator;
}

template <typename KeyType, typename ValueType, typename Compare>
void KeyTree<KeyType, ValueType, Compare>::Iterator::decrement() {
    if (this->current == nullptr)
        this->current = this->max;
    else if (this->current->parent->parent == this->current)
        this->current = this->current->right;
    else if (this->current->left != nullptr) {
        auto node = this->current->left;

        while (node->right != nullptr)
            node = node->right;

        this->current = node;
    } else {
        auto node = this->current->parent;

        while (this->current == node->left) {
            this->current = node;
            node = node->parent;
        }

        this->current = node;
    }
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Iterator KeyTree<KeyType, ValueType, Compare>::Iterator::operator--() {
    this->decrement();

    return *this;
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Iterator KeyTree<KeyType, ValueType, Compare>::Iterator::operator--(int) {
    auto iterator = *this;

    this->decrement();

    return iterator;
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Node& KeyTree<KeyType, ValueType, Compare>::Iterator::operator*() {
    return *this->current;
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Node* KeyTree<KeyType, ValueType, Compare>::Iterator::operator->() {
    return this->current;
}

template <typename KeyType, typename ValueType, typename Compare>
bool KeyTree<KeyType, ValueType, Compare>::Iterator::operator==(const Iterator& iterator) const {
    return this->current == iterator.current;
}

template <typename KeyType, typename ValueType, typename Compare>
bool KeyTree<KeyType, ValueType, Compare>::Iterator::operator!=(const Iterator& iterator) const {
    return this->current != iterator.current;;
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Iterator KeyTree<KeyType, ValueType, Compare>::begin() {
    return KeyTree<KeyType, ValueType, Compare>::Iterator(this->min(), this->max());
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Iterator KeyTree<KeyType, ValueType, Compare>::end() {
    return KeyTree<KeyType, ValueType, Compare>::Iterator(nullptr, this->max());
}

template <typename KeyType, typename ValueType, typename Compare>
KeyTree<KeyType, ValueType, Compare>::ConstIterator::ConstIterator() {
    this->current = nullptr;
    this->max = nullptr;
}

template <typename KeyType, typename ValueType, typename Compare>
KeyTree<KeyType, ValueType, Compare>::ConstIterator::ConstIterator(typename KeyTree<KeyType, ValueType, Compare>::Node *node, typename KeyTree<KeyType, ValueType, Compare>::Node *max) {
    this->current = node;
    this->max = max;
}

template <typename KeyType, typename ValueType, typename Compare>
KeyTree<KeyType, ValueType, Compare>::ConstIterator::ConstIterator(const typename KeyTree<KeyType, ValueType, Compare>::ConstIterator &iterator) {
    this->current = iterator.current;
    this->max = iterator.max;
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::ConstIterator& KeyTree<KeyType, ValueType, Compare>::ConstIterator::operator=(const typename KeyTree<KeyType, ValueType, Compare>::ConstIterator &iterator) {
    this->current = iterator.current;
    this->max = iterator.max;
}

template <typename KeyType, typename ValueType, typename Compare>
void KeyTree<KeyType, ValueType, Compare>::ConstIterator::increment() {
    if (this->current->parent == nullptr && this->current->left == nullptr && this->current->right == nullptr)
        this->current = nullptr;
    else if (this->current->right != nullptr) {
        this->current = this->current->right;

        while (this->current->left != nullptr)
            this->current = this->current->left;
    } else {
        auto node = this->current->parent;

        while (node && this->current == node->right) {
            this->current = node;

            node = node->parent;
        }

        if (this->current->right != node)
            this->current = node;
    }
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::ConstIterator KeyTree<KeyType, ValueType, Compare>::ConstIterator::operator++() {
    this->increment();

    return *this;
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::ConstIterator KeyTree<KeyType, ValueType, Compare>::ConstIterator::operator++(int) {
    auto iterator = *this;

    this->increment();

    return iterator;
}

template <typename KeyType, typename ValueType, typename Compare>
void KeyTree<KeyType, ValueType, Compare>::ConstIterator::decrement() {
    if (this->current == nullptr)
        this->current = this->max;
    else if (this->current->parent->parent == this->current)
        this->current = this->current->right;
    else if (this->current->left != nullptr) {
        auto node = this->current->left;

        while (node->right != nullptr)
            node = node->right;

        this->current = node;
    } else {
        auto node = this->current->parent;

        while (this->current == node->left) {
            this->current = node;
            node = node->parent;
        }

        this->current = node;
    }
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::ConstIterator KeyTree<KeyType, ValueType, Compare>::ConstIterator::operator--() {
    this->decrement();

    return *this;
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::ConstIterator KeyTree<KeyType, ValueType, Compare>::ConstIterator::operator--(int) {
    auto iterator = *this;

    this->decrement();

    return iterator;
}

template <typename KeyType, typename ValueType, typename Compare>
const typename KeyTree<KeyType, ValueType, Compare>::Node& KeyTree<KeyType, ValueType, Compare>::ConstIterator::operator*() const {
    return *this->current;
}

template <typename KeyType, typename ValueType, typename Compare>
const typename KeyTree<KeyType, ValueType, Compare>::Node* KeyTree<KeyType, ValueType, Compare>::ConstIterator::operator->() const {
    return this->current;
}

template <typename KeyType, typename ValueType, typename Compare>
bool KeyTree<KeyType, ValueType, Compare>::ConstIterator::operator==(const ConstIterator& iterator) const {
    return this->current == iterator.current;
}

template <typename KeyType, typename ValueType, typename Compare>
bool KeyTree<KeyType, ValueType, Compare>::ConstIterator::operator!=(const ConstIterator& iterator) const {
    return this->current != iterator.current;
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Node* KeyTree<KeyType, ValueType, Compare>::min() const {
    return min_helper(this->root);
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::Node* KeyTree<KeyType, ValueType, Compare>::max() const {
    return max_helper(this->root);
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::ConstIterator KeyTree<KeyType, ValueType, Compare>::cbegin() const {
    return KeyTree<KeyType, ValueType, Compare>::ConstIterator(this->min(), this->max());
}

template <typename KeyType, typename ValueType, typename Compare>
typename KeyTree<KeyType, ValueType, Compare>::ConstIterator KeyTree<KeyType, ValueType, Compare>::cend() const {
    return KeyTree<KeyType, ValueType, Compare>::ConstIterator(nullptr, this->max());
}

template <typename KeyType, typename ValueType, typename Compare>
bool KeyTree<KeyType, ValueType, Compare>::operator==(const KeyTree<KeyType, ValueType, Compare>& other) const {
    if (distance(this->cbegin(), this->cend()) != distance(other.cbegin(), other.cend()))
            return false;

    for (auto it_1 = this->cbegin(), it_2 = other.cbegin(); it_1 != this->cend(), it_2 != other.cend(); it_1++, it_2++)
        if (it_1->key != it_2->key || it_1->value != it_2->value)
            return false;

    return true;
}

template <typename KeyType, typename ValueType, typename Compare>
bool KeyTree<KeyType, ValueType, Compare>::operator!=(const KeyTree<KeyType, ValueType, Compare>& other) const {
    return !(*this == other);
}

template <typename KeyType, typename ValueType, typename Compare>
bool KeyTree<KeyType, ValueType, Compare>::operator<(const KeyTree<KeyType, ValueType, Compare>& other) const {
    if (Compare().execute(this->cbegin(), this->cend(), other.cbegin(), other.cend()) < 0)
        return true;

    return false;
}

template <typename KeyType, typename ValueType, typename Compare>
bool KeyTree<KeyType, ValueType, Compare>::operator>(const KeyTree<KeyType, ValueType, Compare>& other) const {
    if (Compare().execute(this->cbegin(), this->cend(), other.cbegin(), other.cend()) > 0)
        return true;

    return false;
}

template <typename KeyType, typename ValueType, typename Compare>
bool KeyTree<KeyType, ValueType, Compare>::operator<=(const KeyTree<KeyType, ValueType, Compare>& other) const {
    if (Compare().execute(this->cbegin(), this->cend(), other.cbegin(), other.cend()) < 0)
        return true;

    return false;
}

template <typename KeyType, typename ValueType, typename Compare>
bool KeyTree<KeyType, ValueType, Compare>::operator>=(const KeyTree<KeyType, ValueType, Compare>& other) const {
    if (Compare().execute(this->cbegin(), this->cend(), other.cbegin(), other.cend()) < 0)
        return true;

    return false;
}
