/** 
 * @file stack.h
 *
 * This module provides an implementation of a Stack 
 */

#pragma once

#include "vector.h"

/**
 * @tparam ItemType the type of item the stack will contain
 */
template <typename ItemType, typename ContainerType = Vector<ItemType>>
/** 
 * @class Stack
 *
 * @brief A class implementing a Stack-type container
 */
class Stack {
    private:
        /** @brief The underlying container of the Stack */
        ContainerType container;

    public:
        /** @brief Default constructor */
        Stack();

        /** 
         * @brief Copy constructor
         *
         * The underlying container is copy-constructed with the content of @b other.container
         */
        Stack(const Stack& other);

        /**
         * @brief Default destructor
         *
         * Destructs the underlying container
         */
        ~Stack();

        /**
         * @brief Assignment operator
         *
         * @details Replaces the content of the underlying container with those of @b other
         */
        Stack& operator=(const Stack<ItemType, ContainerType>& other);

        /**
         * @brief Checks if the underlying container has no elements
         *
         * Calls @b container.empty()
         *
         * @return 
         *      true if the Stack is empty
         *
         *      false if the Stack is not empty
         */ 
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
