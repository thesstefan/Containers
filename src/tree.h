#pragma once

template <typename ItemType>
class Tree {
    private:
        class Node {
            public:
                ItemType value;

                Node *parent;

                Node *left;
                Node *right;

                Node();
                Node(const ItemType& value);
                Node(const ItemType& value, Node *parent);
        };

        Node *root;

        void real_delete(Node *leaf);

        Node *real_insert(Node *leaf, Node *parent, const ItemType& value);
        Node *real_erase(Node *leaf, const ItemType& value);

        Node *real_search(Node *leaf, const ItemType& value) const;

        Node *min(Node *leaf) const;
        Node *max(Node *leaf) const;

        void real_print(Node *leaf) const;

    public:
        class Iterator {
            private:
                Node *current;

                void increment();
                void decrement();

            public:
                Iterator();
                Iterator(Node *node);
                Iterator(const Iterator &iterator);

                Iterator& operator=(const Iterator &iterator);

                Iterator operator++();
                Iterator operator++(int);

                Iterator operator--();
                Iterator operator--(int);

                Node &operator*();
                Node *operator->();

                bool operator==(const Iterator &iterator) const;
                bool operator!=(const Iterator &iterator) const;
        };

        class ConstIterator {
            private:
                Node *current;

                void increment();
                void decrement();

            public:
                ConstIterator();
                ConstIterator(Node *node);
                ConstIterator(const ConstIterator &iterator);

                ConstIterator& operator=(const ConstIterator &iterator);

                ConstIterator operator++();
                ConstIterator operator++(int);

                ConstIterator operator--();
                ConstIterator operator--(int);

                const Node &operator*() const;
                const Node *operator->() const;

                bool operator==(const ConstIterator &iterator) const;
                bool operator!=(const ConstIterator &iterator) const;
        };

        Tree();
        Tree(const Tree& other);
        
        Tree& operator=(const Tree& other);

        ~Tree();

        void insert(const ItemType& value);
        void erase(const ItemType& value);

        Node *search(const ItemType& value) const;

        void print() const;

        Iterator begin();
        Iterator end();

        ConstIterator cbegin() const;
        ConstIterator cend() const;
};

template <typename ItemType>
Tree<ItemType>::Node::Node() {
    this->value = ItemType();

    this->parent = nullptr;

    this->left = nullptr;
    this->right = nullptr;
}

template <typename ItemType>
Tree<ItemType>::Node::Node(const ItemType& value) {
    this->value = value;

    this->parent = nullptr;

    this->left = nullptr;
    this->right = nullptr;
}

template <typename ItemType>
Tree<ItemType>::Node::Node(const ItemType& value, Node* parent) {
    this->value = value;

    this->parent = parent;

    this->left = nullptr;
    this->right = nullptr;
}

template <typename ItemType>
Tree<ItemType>::Tree() {
    this->root = nullptr;
}

template <typename ItemType>
Tree<ItemType>::Tree(const Tree &other) {
    this->root = nullptr;

    for (auto it = other.cbegin(); it != other.cend(); it++)
        this->insert(it->value);

}

template <typename ItemType>
Tree<ItemType>& Tree<ItemType>::operator=(const Tree &other) {
    this->real_delete(this->root);

    this->root = nullptr;

    for (auto it = other.cbegin(); it != other.cend(); it++)
        this->insert(it->value);
}

template <typename ItemType>
void Tree<ItemType>::real_delete(Tree<ItemType>::Node *leaf) {
    if (leaf != nullptr) {
        this->real_delete(leaf->left);
        this->real_delete(leaf->right);

        delete leaf;
    }
}

template <typename ItemType>
Tree<ItemType>::~Tree() {
    this->real_delete(this->root);
}

template <typename ItemType>
typename Tree<ItemType>::Node* Tree<ItemType>::real_insert(Tree<ItemType>::Node *leaf, Tree<ItemType>::Node *parent, const ItemType& value) {
    if (leaf == nullptr)
        leaf = new Node(value, parent);
    else if (value < leaf->value)
        leaf->left = real_insert(leaf->left, leaf, value);
    else if (value > leaf->value)
        leaf->right = real_insert(leaf->right, leaf, value);

    return leaf;
}

template <typename ItemType>
void Tree<ItemType>::insert(const ItemType& value) {
    this->root = this->real_insert(this->root, this->root, value);
}

template <typename ItemType>
typename Tree<ItemType>::Node* Tree<ItemType>::min(Tree<ItemType>::Node *leaf) const {
    if (leaf == nullptr)
        return nullptr;
    else if (leaf->left == nullptr)
        return leaf;
    else 
        return min(leaf->left);
}

template <typename ItemType>
typename Tree<ItemType>::Node* Tree<ItemType>::max(Tree<ItemType>::Node *leaf) const {
    if (leaf == nullptr)
        return nullptr;
    else if (leaf->right == nullptr)
        return leaf;
    else
        return max(leaf->right);
}

template <typename ItemType>
typename Tree<ItemType>::Node* Tree<ItemType>::real_erase(Tree<ItemType>::Node *leaf, const ItemType &value) {
    if (leaf == nullptr)
        return nullptr;
    else if (value < leaf->value)
        leaf->left = real_erase(leaf->left, value);
    else if (value > leaf->value)
        leaf->right = real_erase(leaf->right, value);
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
        auto temp = max(leaf->left);
        auto parent = temp->parent;

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

template <typename ItemType>
void Tree<ItemType>::erase(const ItemType &value) {
    this->root = this->real_erase(this->root, value);
}

template <typename ItemType>
typename Tree<ItemType>::Node* Tree<ItemType>::real_search(Tree<ItemType>::Node *leaf, const ItemType &value) const {
    if (leaf == nullptr || leaf->value == value)
        return leaf;

    if (value < leaf->value)
        return real_search(leaf->left, value);

    return real_search(leaf->right, value);
}

template <typename ItemType>
typename Tree<ItemType>::Node* Tree<ItemType>::search(const ItemType &value) const {
    return this->real_search(this->root, value);
}

template <typename ItemType>
void Tree<ItemType>::real_print(Tree<ItemType>::Node *leaf) const {
    if (leaf != nullptr) {
        real_print(leaf->left);

        std::cout << leaf->value << std::endl;

        if (leaf->parent != nullptr)
            std::cout << "PARENT -> " << leaf->parent->value << std::endl;
        else 
            std::cout << "ROOT" << std::endl;

        std::cout << "LEFT -> ";

        if (leaf->left != nullptr)
            std::cout << leaf->left->value << std::endl;
        else 
            std::cout << "NOTHING" << std::endl;

        std::cout << "RIGHT -> ";

        if (leaf->right != nullptr)
            std::cout << leaf->right->value << std::endl;
        else 
            std::cout << "NOTHING" << std::endl;

        std::cout << std::endl;

        real_print(leaf->right);
    }
}

template <typename ItemType>
void Tree<ItemType>::print() const {
    this->real_print(root);

    std::cout << std::endl;
}

template <typename ItemType>
Tree<ItemType>::Iterator::Iterator() {
    this->current = nullptr;
}

template <typename ItemType>
Tree<ItemType>::Iterator::Iterator(typename Tree<ItemType>::Node *node) {
    this->current = node;
}

template <typename ItemType>
Tree<ItemType>::Iterator::Iterator(const typename Tree<ItemType>::Iterator &iterator) {
    this->current = iterator.current;
}

template <typename ItemType>
typename Tree<ItemType>::Iterator& Tree<ItemType>::Iterator::operator=(const Iterator &iterator) {
    this->current = iterator.current;
}

template <typename ItemType>
void Tree<ItemType>::Iterator::increment() {
    if (this->current->right != nullptr) {
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

template <typename ItemType>
typename Tree<ItemType>::Iterator Tree<ItemType>::Iterator::operator++() {
    this->increment();

    return *this;
}

template <typename ItemType>
typename Tree<ItemType>::Iterator Tree<ItemType>::Iterator::operator++(int) {
    auto iterator = *this;

    this->increment();

    return iterator;
}

template <typename ItemType>
void Tree<ItemType>::Iterator::decrement() {
    if (this->current->parent->parent == this->current)
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

template <typename ItemType>
typename Tree<ItemType>::Iterator Tree<ItemType>::Iterator::operator--() {
    this->decrement();

    return *this;
}

template <typename ItemType>
typename Tree<ItemType>::Iterator Tree<ItemType>::Iterator::operator--(int) {
    auto iterator = *this;

    this->decrement();

    return iterator;
}

template <typename ItemType>
typename Tree<ItemType>::Node& Tree<ItemType>::Iterator::operator*() {
    return *this->current;
}

template <typename ItemType>
typename Tree<ItemType>::Node* Tree<ItemType>::Iterator::operator->() {
    return this->current;
}

template <typename ItemType>
bool Tree<ItemType>::Iterator::operator==(const Iterator& iterator) const {
    return this->current == iterator.current;
}

template <typename ItemType>
bool Tree<ItemType>::Iterator::operator!=(const Iterator& iterator) const {
    return this->current != iterator.current;
}

template <typename ItemType>
typename Tree<ItemType>::Iterator Tree<ItemType>::begin() {
    return Tree<ItemType>::Iterator(this->min(this->root));
}

template <typename ItemType>
typename Tree<ItemType>::Iterator Tree<ItemType>::end() {
    return Tree<ItemType>::Iterator(nullptr);
}

template <typename ItemType>
Tree<ItemType>::ConstIterator::ConstIterator() {
    this->current = nullptr;
}

template <typename ItemType>
Tree<ItemType>::ConstIterator::ConstIterator(typename Tree<ItemType>::Node *node) {
    this->current = node;
}

template <typename ItemType>
Tree<ItemType>::ConstIterator::ConstIterator(const typename Tree<ItemType>::ConstIterator &iterator) {
    this->current = iterator.current;
}

template <typename ItemType>
typename Tree<ItemType>::ConstIterator& Tree<ItemType>::ConstIterator::operator=(const ConstIterator &iterator) {
    this->current = iterator.current;
}

template <typename ItemType>
void Tree<ItemType>::ConstIterator::increment() {
    if (this->current->right != nullptr) {
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

template <typename ItemType>
typename Tree<ItemType>::ConstIterator Tree<ItemType>::ConstIterator::operator++() {
    this->increment();

    return *this;
}

template <typename ItemType>
typename Tree<ItemType>::ConstIterator Tree<ItemType>::ConstIterator::operator++(int) {
    auto iterator = *this;

    this->increment();

    return iterator;
}

template <typename ItemType>
void Tree<ItemType>::ConstIterator::decrement() {
    if (this->current->parent->parent == this->current)
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

template <typename ItemType>
typename Tree<ItemType>::ConstIterator Tree<ItemType>::ConstIterator::operator--() {
    this->decrement();

    return *this;
}

template <typename ItemType>
typename Tree<ItemType>::ConstIterator Tree<ItemType>::ConstIterator::operator--(int) {
    auto iterator = *this;

    this->decrement();

    return iterator;
}

template <typename ItemType>
const typename Tree<ItemType>::Node& Tree<ItemType>::ConstIterator::operator*() const {
    return *this->current;
}

template <typename ItemType>
const typename Tree<ItemType>::Node* Tree<ItemType>::ConstIterator::operator->() const {
    return this->current;
}

template <typename ItemType>
bool Tree<ItemType>::ConstIterator::operator==(const ConstIterator& iterator) const {
    return this->current == iterator.current;
}

template <typename ItemType>
bool Tree<ItemType>::ConstIterator::operator!=(const ConstIterator& iterator) const {
    return this->current != iterator.current;
}

template <typename ItemType>
typename Tree<ItemType>::ConstIterator Tree<ItemType>::cbegin() const {
    return Tree<ItemType>::ConstIterator(this->min(this->root));
}

template <typename ItemType>
typename Tree<ItemType>::ConstIterator Tree<ItemType>::cend() const {
    return Tree<ItemType>::ConstIterator(nullptr);
}
