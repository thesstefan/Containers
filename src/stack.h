#pragma once

#include "vector.h"

template <typename ItemType, typename ContainerType = Vector<ItemType>>
class Stack {
    private:
        ContainerType container;

    public:
        Stack();
        Stack(const Stack& other);

        ~Stack();

        Stack& operator=(const Stack<ItemType, ContainerType>& other);

        bool empty() const;
        size_t size() const;

        void push(const ItemType& value);
        void pop();

        ItemType& top();
        const ItemType& top() const;
};

template <typename ItemType, typename ContainerType>
Stack<ItemType, ContainerType>::Stack() {
    this->container = ContainerType();
}

template <typename ItemType, typename ContainerType>
Stack<ItemType, ContainerType>::Stack(const Stack& other) {
    this->container = ContainerType(other.container);
}

template <typename ItemType, typename ContainerType>
Stack<ItemType, ContainerType>::~Stack() {}

template <typename ItemType, typename ContainerType>
Stack<ItemType, ContainerType>& Stack<ItemType, ContainerType>::operator=(const Stack<ItemType, ContainerType>& other) {
    this->container = ContainerType(other.container);

    return *this;
}

template <typename ItemType, typename ContainerType>
bool Stack<ItemType, ContainerType>::empty() const {
    return this->container.empty();
}

template <typename ItemType, typename ContainerType>
size_t Stack<ItemType, ContainerType>::size() const {
    return this->container.size();
}

template <typename ItemType, typename ContainerType>
void Stack<ItemType, ContainerType>::push(const ItemType& value) {
    this->container.push_back(value);
}

template <typename ItemType, typename ContainerType>
void Stack<ItemType, ContainerType>::pop() {
    this->container.pop_back();
}

template <typename ItemType, typename ContainerType>
ItemType& Stack<ItemType, ContainerType>::top() {
    return this->container.back();
}

template <typename ItemType, typename ContainerType>
const ItemType& Stack<ItemType, ContainerType>::top() const {
    return this->container.back();
}
