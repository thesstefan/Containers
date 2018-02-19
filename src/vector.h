/**
 * @file vector.h
 *
 * This module provides an implementation of a Vector container
 *
 * The implementation is following the STL vector implementation.
 */

#pragma once

/**
 * @class Vector
 *
 * @tparam ItemType The type of the elements the Vector contains.
 */
template <typename ItemType>
class Vector {
    private:
        size_t size_;
        size_t capacity_;

        ItemType *buffer;

        bool alloc_memory_if_needed();

    public:
        class Iterator {
            private:
                /**
                 * @brief The buffer of the Vector the Iterator works on.
                 * 
                 * @details The internal buffer it's initialized to @b nullptr.
                 */
                ItemType *buffer;

            public:
                /** 
                 * @brief Constructs the Iterator (default).
                 *
                 * The internal buffer is initialized with @b nullptr.
                 */
                Iterator();

                /** 
                 * @brief Constructs the Iterator using a @b buffer.
                 *
                 * The internal @b buffer is initialized with @b buffer.
                 * 
                 * @param buffer The buffer of the Vector on which the Iterator works on.
                 */
                Iterator(ItemType *buffer);

                /**
                 * @brief Copy-constructs the Iterator, 
                 *
                 * The internal @b buffer is initialized with the one of @b iterator.
                 *
                 * @param iterator The Iterator to copy-construct with.
                 */
                Iterator(const Iterator &iterator);

                /**
                 * @brief Replaces the internal @b buffer of the Iterator with the one of @b iterator.
                 *
                 * @param iterator The Iterator to get the new @b buffer from.
                 *
                 * @return Reference to Iterator (Iterator&).
                 */
                Iterator& operator=(const Iterator &iterator);

                /**
                 * @brief Pre-increments the Iterator (++it).
                 *
                 * The Iterator is incremented and the incremented Iterator is returned.
                 *
                 * @return Pre-incremented Iterator.
                 */
                Iterator operator++();

                /**
                 * @brief Post-increments the Iterator (it++).
                 *
                 * The method creates a copy of the Iterator, increments it and returns the copy of the Iterator before the increment.
                 *
                 * @return Post-incremented Iterator.
                 */
                Iterator operator++(int);


                /**
                 * @brief Pre-decrements the Iterator (--it).
                 *
                 * The Iterator is decremented and the decremented Iterator is returned.
                 *
                 * @return Pre-decremented Iterator.
                 */
                Iterator operator--();

                /**
                 * @brief Post-decrements the Iterator (it--).
                 *
                 * The method creates a copy of the Iterator, decrements it and returns the copy of the Iterator before the decrement.
                 *
                 * @return Post-decremented Iterator.
                 */
                Iterator operator--(int);

                /**
                 * @brief Increments the Iterator by @p x.
                 *
                 * The method advances the position of the internal @b of the Iterator by @b x.
                 *
                 * @param x The amount of iterations to be made.
                 *
                 * @return The incremeneted Iterator.
                 */
                Iterator operator+(ptrdiff_t x);

                /**
                 * @brief Increments the Iterator by @p x.
                 *
                 * The method advances the position of the internal @b of the Iterator by @b x.
                 *
                 * @param x The amount of iterations to be made.
                 *
                 * @return The incremeneted Iterator.
                 */
                Iterator operator-(ptrdiff_t x);

                /**
                 * @brief Returns the difference between the Iterator and @b iterator.
                 *
                 * The method uses pointer arithmetic on the internal @b buffers of the Iterators, returning the so called "distance" = number of iterations needed to be made from the Iterator to reach @b iterator.
                 * @param iterator The Iterator to calculate the distance to.
                 *
                 * @return The difference between the Iterators.
                 */
                ptrdiff_t operator-(const Iterator &iterator) const;

                Iterator operator+=(ptrdiff_t x);
                Iterator operator-=(ptrdiff_t x);

                ItemType &operator*();
                ItemType *operator->();

                ItemType &operator[](ptrdiff_t index) const;

                bool operator==(const Iterator &iterator) const;
                bool operator!=(const Iterator &iterator) const;

                bool operator<(const Iterator &iterator) const;
                bool operator>(const Iterator &iterator) const;
                bool operator<=(const Iterator &iterator) const;
                bool operator>=(const Iterator &iterator) const;
        };

        class ConstIterator {
            private:
                ItemType *buffer;

            public:
                ConstIterator();
                ConstIterator(ItemType *buffer);
                ConstIterator(const ConstIterator &iterator);

                ConstIterator& operator=(const ConstIterator &iterator);

                ConstIterator operator++();
                ConstIterator operator++(int);

                ConstIterator operator--();
                ConstIterator operator--(int);

                ConstIterator operator+(ptrdiff_t x);
                ConstIterator operator-(ptrdiff_t x);

                ptrdiff_t operator-(const ConstIterator &iterator) const;

                ConstIterator operator+=(ptrdiff_t x);
                ConstIterator operator-=(ptrdiff_t x);

                const ItemType &operator*() const;
                const ItemType *operator->() const;

                const ItemType &operator[](ptrdiff_t index) const;

                bool operator==(const ConstIterator &iterator) const;
                bool operator!=(const ConstIterator &iterator) const;

                bool operator<(const ConstIterator &iterator) const;
                bool operator>(const ConstIterator &iterator) const;
                bool operator<=(const ConstIterator &iterator) const;
                bool operator>=(const ConstIterator &iterator) const;
        };

        class Compare {
            public:
                template <typename IteratorType>
                int execute(IteratorType first_1, IteratorType end_1, IteratorType first_2, IteratorType end_2);
        };

        /**
         * @brief Constructs the container.
         *
         * @b Default @b constructor. Constructs an empty Vector.
         *
         * @tparam ItemType The type of the elements the Vector contains.
         */
        Vector();

        /**
         * @brief Constructs the container with @b count copies of elements with value @b value.
         *
         * @tparam ItemType The type of the elements the Vector contains.
         *
         * @param count The number of elements to construct the Vector with.
         *
         * @param value The value which is assigned to the elements.
         *
         * @warning The behaviour is undefined if @b count is out of bounds (exceeds the limits of @b size_t)
         */
        Vector(size_t count, ItemType &value);

        /**
         * @brief Constructs the container with @b count default-inserted instances of ItemType.
         *
         * @tparam ItemType The type of the elements the Vector contains.
         *
         * @param count The number of elements to construct the Vector with.
         *
         * @warning The behaviour is undefined if @b count is out of bounds (exceeds the limits of @b size_t)
         */
        Vector(size_t count);

        /** 
         * @brief Constructs the container with the contents of range [@b first, @b last).
         *
         * @tparam ItemType The type of the elements the Vector contains.
         *
         * @tparam IteratorType The type of iterator used to define the range.
         *
         * @param first Iterator (of type IteratorType) to the first element of the range.
         *
         * @param last Iterator (of type IteratorType) to the element following the last element of the range.
         *
         * @warning The behaviour is undefined if the range is invalid e.g. @b first @b > @b last or @b last can't be reached from @b first.
         *
         * @warning The behaviour is undefined if @b last is dereferenced as the element it points to it's just a placeholder.
         */
        template <typename IteratorType>
        Vector(IteratorType first, IteratorType last);

        /**
         * @brief Copy-constructs the container with the contents of @b other.
         *
         * @tparam ItemType The type of the elements the Vector contains.
         *
         * @param other The Vector to be used for copy-construction.
         */
        Vector(const Vector &other);

        /**
         * @brief Destructs the container.
         *
         * The destructors of the elements are called and the storage used is deallocated.
         
         * @tparam ItemType The type of the elements the Vector contains.
         */
        ~Vector();

        /**
         * @brief Replaces the contents of the container with the ones of @b other.
         *
         * @tparam ItemType The type of the elements the Vector contains.
         *
         * @param other The container to be copied from.
         *
         * @return Vector with replaced content.
         */
        Vector& operator=(const Vector &other);

        /**
         * @brief Replaces the contents of the container with @b count copies of value @b value.
         *
         * @param count The number of elements to be in the final container.
         *
         * @param value The value which is assigned to the elements.
         *
         * @warning The behaviour is undefined if @b count is out of bounds (exceeds the limits of @b size_t)
         */
        void assign(size_t count, const ItemType &value);

        /**
         * @brief Replaces the contents of the container with copies of those in the range [@b first,@b last).
         *
         * @tparam ItemType The type of the elements the Vector contains.
         *
         * @tparam IteratorType The type of iterator used to define the range.
         *
         * @param first Iterator (of type IteratorType) to the first element of the range.
         *
         * @param last Iterator (of type IteratorType) to the element following the last element of the range.
         *
         * @warning The behaviour is undefined if the range is invalid e.g. @b first @b > @b last or @b last can't be reached from @b first.
         *
         * @warning The behaviour is undefined if @b last is dereferenced as the element it points to it's just a placeholder.
         */
        template <typename IteratorType>
        void assign(IteratorType first, IteratorType last);

        /**
         * @brief Returns a reference to the element at position @b index.
         *
         * No bounds checking if performed.
         *
         * @tparam ItemType The type of the elements the Vector contains.
         *
         * @param index The position of the element of which we return the reference.
         *
         * @return Reference (ItemType&) to the element at position @b index.
         *
         * @warning The behaviour is undefined if @b index is out of bounds e.g. @b index < 0 or @b index > size().
         */
        ItemType &operator[](size_t index);

        /**
         * @brief Returns a constant reference to the element at position @b index.
         *
         * No bounds checking if performed.
         *
         * @tparam ItemType The type of the elements the Vector contains.
         *
         * @param index The position of the element of which we return the reference.
         *
         * @return Constant reference (const ItemType&) to the element at position @b index.
         *
         * @warning The behaviour is undefined if @b index is out of bounds e.g. @b index < 0 or @b index > size().
         */
        const ItemType& operator[](size_t index) const;

        /**
         * @brief Returns a reference to the element at position @b index with bound checking.
         *
         * @tparam ItemType The type of the elements the Vector contains.
         *
         * @param index The position of the element of which we return the reference.
         *
         * @return Reference (ItemType&) to the element at position @b index.
         *
         * @warning If @b index is out of bounds e.g. @b index < 0 or @b index > size(), @b std::out_of_range is thrown.
         */
        ItemType& at(size_t index);

        /**
         * @brief Returns a constant reference to the element at position @b index with bound checking.
         *
         * @tparam ItemType The type of the elements the Vector contains.
         *
         * @tparam ItemType The type of the elements the Vector contains.
         *
         * @param index The position of the element of which we return the reference.
         *
         * @return Constant reference (const ItemType&) to the element at position @b index.
         *
         * @warning If @b index is out of bounds e.g. @b index < 0 or @b index > size(), @b std::out_of_range is thrown.
         */
        const ItemType& at(size_t index) const;

        /**
         * @brief Returns a reference to the first element in the container.
         *
         * @return Reference (ItemType&) to the first element in the container.
         *
         * @warning The behaviour is undefined if front() is called on an empty container.
         */
        ItemType& front();

        /**
         * @brief Returns a constant reference to the first element in the container.
         *
         * @return Constant reference (const ItemType&) to the first element in the container.
         *
         * @warning The behaviour is undefined if front() is called on an empty container.
         */
        const ItemType& front() const;

        /**
         * @brief Returns a reference to the last element in the container.
         *
         * @return Reference (ItemType&) to the last element in the container.
         *
         * @warning The behaviour is undefined if back() is called on an empty container.
         */
        ItemType& back();

        /**
         * @brief Returns a const reference to the last element in the container.
         *
         * @return Constant reference (const ItemType&) to the last element in the container.
         *
         * @warning The behaviour is undefined if const() is called on an empty container.
         */
        const ItemType& back() const;

        /**
         * @brief Checks if the container is empty.
         *
         * @return @b true if the container is empty, @b false otherwise.
         */
        bool empty() const;

        /**
         * @brief Returns the number of elements in the container.
         *
         * @return The number of elements in the container.
         */
        size_t size() const;

        /**
         * @brief Returns the number of elements that the container has currently allocated space for.
         *
         * @return The capacity of the currently allocated storage.
         */
        size_t capacity() const;

        /**
         * @brief Returns an Iterator to the first element of the container.
         *
         * If the container is empty, the returned Iterator will be equal to end().
         *
         * @return Iterator to the first element.
         */
        Iterator begin();

        /**
         * @brief Returns an Iterator to the element following the last element of the container.
         *
         * @return Iterator to the element following the last element.
         *
         * @warning The behaviour is undefined if end() is dereferenced, as the element it points to it's just a placeholder.
         */
        Iterator end();

        /**
         * @brief Returns an ConstantIterator to the first element of the container.
         *
         * If the container is empty, the returned ConstIterator will be equal to cend().
         *
         * @return ConstIterator to the first element.
         */
        ConstIterator cbegin() const;

        /**
         * @brief Returns an ConstIterator to the element following the last element of the container.
         *
         * @return ConstIterator to the element following the last element.
         *
         * @warning The behaviour is undefined if cend() is dereferenced, as the element it points to it's just a placeholder.
         */
        ConstIterator cend() const;

        /**
         * @brief Increases the capacity of the Vector to @b capacity.
         *
         * If @b capacity > capacity() the new storage is allocated and the elements from the old location are moved, otherwise the method does nothing.
         *
         * @param capacity The new capacity of the Vector.
         *
         * @warning If @b capacity is out of bounds or the new allocation simply fails, @b std::bad_alloc is thrown.
         */
        void reserve(size_t capacity);

        /**
         * @brief Removes all elements from the container.
         *
         * Leaves the capacity() of the Vector unchanged.
         */
        void clear();

        /**
         * @brief Inserts @b value before @b position.
         *
         * @param position Iterator to the element before which the element should be inserted.
         *
         * @param value The value of the element to be inserted.
         *
         * @return Iterator to the inserted value.
         *
         * @warning The behaviour is undefined if @b position doesn't point to a valid element of the container e.g. @b position < begin() or @b position > end().
         */
        Iterator insert(Iterator position, const ItemType &value);

        /**
         * @brief Inserts @b count copies of @b value before @b position.
         *
         * @param position Iterator to the element before which the elements should be inserted.
         *
         * @param count The number of elements to be inserted.
         *
         * @param value The value of the inserted elements.
         *
         * @warning The behaviour is undefined if @b position doesn't point to a valid element of the container e.g. @b position < begin() or @b position > end().
         *
         * @warning The behaviour is undefined if @b count is out of bounds (exceeds @b size_t limits).
         */
        void insert(Iterator position, size_t count, const ItemType& value);

        /**
         * @brief Inserts elements from range [@b first, @b last) before @b position.
         *
         * @tparam IteratorType The type of iterator used to define the range.
         *
         * @param position Iterator to the element before which the elements should be inserted.
         *
         * @param first Iterator (of type IteratorType) to the first element of the range.
         *
         * @param last Iterator (of type IteratorType) to the element following the last element of the range.
         *
         * @warning The behaviour is undefined if the range is invalid e.g. @b first @b > @b last or @b last can't be reached from @b first.
         *
         * @warning The behaviour is undefined if @b last is dereferenced as the element it points to it's just a placeholder.
         */
        template <typename IteratorType>
        void insert(Iterator position, IteratorType first, IteratorType last);


        /**
         * @brief Removes the element at @b position.
         *
         * @param position Iterator to the element which should be removed.
         *
         * @return Iterator following the removed element. If the element at @position was the last, then end() is returned.
         *
         * @warning The behaviour is undefined if @b position is invalid e.g. @b position < begin() or @b position > end() or can't be dereferenceable => end() can't be used as @b position, because it's not dereferenceable. 
         */
        Iterator erase(Iterator position);

        /**
         * @brief Removes the elements in the range [@b first, @b last).
         *
         * @param first Iterator to the first element to be removed.
         *
         * @param last Iterator to the element following the last element to be removed.
         *
         * @warning The behaviour is undefined if first and last are not valid / not pointing to the elements in the Vector.
         */
        void erase(Iterator first, Iterator last);

        /**
         * @brief Appends @b item to the end of the container.
         *
         * @param item The value to be appended.
         */
        void push_back(const ItemType &item);

        /**
         * @brief Removes the last element of the container.
         *
         * @warning The behaviour is undefined if pop_back() is called on a empty container.
         */
        void pop_back();

        /**
         * @brief Exchanges the contents of the container with those of @b other.
         *
         * @param @other The Vector to swap contents with.
         */
        void swap(Vector &other);

        /**
         * @brief Resizes the container to contain @b count elements with value @b value.
         *
         * If size() > @b count, the container is reduced to it's first @b count elements.
         *
         * If size() < @b count, additional elements are appended and initialized with copies of @b value. 
         *
         * @param count The number of elements to resize the container to.
         *
         * @param value The value of the elements to be appended if @b count > size()
         */
        void resize(size_t count, const ItemType &value);

        /**
         * @brief Checks if the contents of the container and the contents of @b other are equal.
         *
         * @param other The container to compare with.
         *
         * @return @b true if the contents are equal, @b false otherwise.
         */
        bool operator==(const Vector &other) const;

        /**
         * @brief Checks if the contents of the container and the contents of @b other are not equal.
         *
         * @param other The container to compare with.
         *
         * @return @b true if the contents are equal, @b false otherwise.
         */
        bool operator!=(const Vector &other) const;

        /**
         * @brief Compares the contents of the container and the contents of @b other lexicographically.
         *
         * @param other The container to compare with.
         *
         * @return @b true if the contents of the container are @b lexicographically @b less than the contents of @b other, @b false otherwise.
         */
        bool operator<(const Vector &other) const;

        /**
         * @brief Compares the contents of the container and the contents of @b other lexicographically.
         *
         * @param other The container to compare with.
         *
         * @return @b true if the contents of the container are @b lexicographically @b greater than the contents of @b other, @b false otherwise.
         */
        bool operator>(const Vector &other) const;

        /**
         * @brief Compares the contents of the container and the contents of @b other lexicographically.
         *
         * @param other The container to compare with.
         *
         * @return @b true if the contents of the container are @b lexicographically @b greater than or @b equal the contents of @b other, @b false otherwise.
         */
        bool operator>=(const Vector &other) const;

        /**
         * @brief Compares the contents of the container and the contents of @b other lexicographically.
         *
         * @param other The container to compare with.
         *
         * @return @b true if the contents of the container are @b lexicographically @b less than or @b equal the contents of @b other, @b false otherwise.
         */
        bool operator<=(const Vector &other) const;
};

template <typename ItemType>
Vector<ItemType>::Vector() {
    this->capacity_ = 0;
    this->size_ = 0;
    this->buffer = nullptr;
}

template <typename ItemType>
Vector<ItemType>::Vector(size_t count, ItemType& value) {
    this->capacity_ = count;
    this->size_ = count;

    this->buffer = new ItemType[count];

    for (auto it = this->begin(); it != this->end(); it++)
        *it = value;
}

template <typename ItemType>
Vector<ItemType>::Vector(size_t count) {
    this->capacity_ = count;
    this->size_ = count;

    this->buffer = new ItemType[count];

    for (auto it = this->begin(); it != this->end(); it++)
        *it = ItemType();
}

template <typename ItemType> 
template <typename IteratorType> 
Vector<ItemType>::Vector(IteratorType first, IteratorType last) {
    ptrdiff_t count = last - first;

    this->size_ = count;
    this->capacity_ = count;

    this->buffer = new ItemType[count];

    for (auto it = this->begin(), it_ = first; it != this->end() && it_ != last; it++, it_++)
        *it = *it_;
}

template <typename ItemType>
Vector<ItemType>::Vector(const Vector<ItemType> &other) {
    this->size_ = other.size_;
    this->capacity_ = other.capacity_;

    this->buffer = new ItemType[this->size_];

    for (auto it = this->begin(), it_ = other.cbegin(); it != this->end() && it_ != other.cend(); it++, it_++)
        *it = *it_;
}

template <typename ItemType>
Vector<ItemType>::~Vector() {
    delete[] this->buffer;
}

template <typename ItemType>
Vector<ItemType>& Vector<ItemType>::operator=(const Vector<ItemType> &other) {
    delete[] this->buffer;

    this->size_ = other.size_;
    this->capacity_ = other.capacity_;

    this->buffer = new ItemType[this->size_];

    for (auto it = this->begin(), it_ = other.cbegin(); it != this->end() && it_ != other.cend(); it++, it_++)
        *it = *it_;
}

template <typename ItemType>
void Vector<ItemType>::assign(size_t count, const ItemType& value) {
    this->clear();

    while (this->size_ < count)
        this->push_back(value);
}

template <typename ItemType>
template <typename IteratorType>
void Vector<ItemType>::assign(IteratorType first, IteratorType last) {
    this->clear();

    for (auto it = first; it != last; it++)
        this->push_back(*it);
}

template <typename ItemType>
ItemType& Vector<ItemType>::at(size_t index) {
    if (index < 0 || index >= this->size_)
        throw std::out_of_range("std::out_of_range : Vector::at(size_t index)");

    return this->buffer[index];
}

template <typename ItemType>
const ItemType& Vector<ItemType>::at(size_t index) const {
    if (index < 0 || index >= this->size_)
        throw std::out_of_range("std::out_of_range : Vector::at(size_t index) const");

    return this->buffer[index];
}

template <typename ItemType>
ItemType& Vector<ItemType>::operator[](size_t index) {
    return this->buffer[index];
}

template <typename ItemType>
const ItemType& Vector<ItemType>::operator[](size_t index) const {
    return this->buffer[index];
}

template <typename ItemType>
ItemType& Vector<ItemType>::front() {
    return *this->begin();
}

template <typename ItemType>
const ItemType& Vector<ItemType>::front() const {
    return *this->cbegin();
}

template <typename ItemType>
ItemType& Vector<ItemType>::back() {
    return *(this->end() - 1);
}

template <typename ItemType>
const ItemType& Vector<ItemType>::back() const {
    return *(this->cend() - 1);
}

template <typename ItemType>
bool Vector<ItemType>::empty() const {
    return this->cbegin() == this->cend();
}

template <typename ItemType>
size_t Vector<ItemType>::size() const {
    return this->size_;
}

template <typename ItemType>
size_t Vector<ItemType>::capacity() const {
    return this->capacity_;
}

template <typename ItemType>
void Vector<ItemType>::reserve(size_t capacity) {
    if (capacity < this->capacity_)
        return;

    ItemType *new_buffer = new ItemType[capacity];

    if (new_buffer == nullptr) 
        throw std::bad_alloc();

    for (size_t index = 0; index < this->size_; index++)
        new_buffer[index] = this->buffer[index];

    delete[] this->buffer;

    this->buffer = new_buffer;
    this->capacity_ = capacity;
}


template <typename ItemType>
bool Vector<ItemType>::alloc_memory_if_needed()  {
    if (this->size_ == this->capacity_) {
        if (this->capacity_ == 0)
            this->reserve(1);
        else
            this->reserve(this->capacity_ * 2);

        return true;
    }

    return false;
}

template <typename ItemType>
void Vector<ItemType>::clear() {
    delete[] this->buffer;
    this->size_ = 0;

    this->buffer = new ItemType[this->capacity_];
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::insert(typename Vector<ItemType>::Iterator position, const ItemType& value) {
    ptrdiff_t relative_position = position - this->begin();

    if (this->alloc_memory_if_needed() == true)
        position = this->begin() + relative_position;

    this->size_++;

    for (auto it = this->end() - 1; it > position; it--) {
        *it = *(it - 1);
    }

    *position = value;

    return position;
}

template <typename ItemType>
void Vector<ItemType>::insert(typename Vector<ItemType>::Iterator position, size_t count, const ItemType& value) {
    while (count--)
        position = this->insert(position, value);
}

template <typename ItemType>
template <typename IteratorType>
void Vector<ItemType>::insert(typename Vector<ItemType>::Iterator position, IteratorType first, IteratorType last) {
    for (auto it = last - 1; it >= first; it--)
        position = this->insert(position, *it);
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::erase(typename Vector<ItemType>::Iterator position) {
    ptrdiff_t relative_position = position - this->begin();

    for (auto it = position; it < this->end() - 1; it++)
        *it = *(it + 1);

    this->size_--;

    if (this->alloc_memory_if_needed() == true)
        return position + relative_position;

    return position;
}

template <typename ItemType>
void Vector<ItemType>::erase(typename Vector<ItemType>::Iterator first, typename Vector<ItemType>::Iterator last) {
    auto position = first;

    for (auto it = first; it < last; it++)
        position = this->erase(position);
}

template <typename ItemType>
void Vector<ItemType>::push_back(const ItemType &item) {
    this->alloc_memory_if_needed();

    this->buffer[this->size_++] = item;
}

template <typename ItemType>
void Vector<ItemType>::pop_back() {
    this->alloc_memory_if_needed();

    if (this->size_ != 0)
        this->size_--;
}

template <typename ItemType>
void swap_(ItemType &item_1, ItemType &item_2) {
    ItemType temp;

    temp = item_1;
    item_1 = item_2;
    item_2 = temp;
}

template <typename ItemType>
void Vector<ItemType>::swap(Vector<ItemType> &other) {
    swap_(this->buffer, other.buffer);
    swap_(this->size_, other.size_);
    swap_(this->capacity_, other.capacity_);
}

template <typename ItemType>
void Vector<ItemType>::resize(size_t count, const ItemType &value) {
    while (count != this->size_) {
        if (count > this->size_) 
            this->push_back(value);
        else
            this->pop_back();
    }
}

template <typename ItemType>
bool Vector<ItemType>::operator==(const Vector &other) const {
    if (this->size_ != other.size_)
        return false;

    for (auto it = this->cbegin(), other_it = other.cbegin(); it != this->cend() && other_it != this->cend(); it++, other_it++)
       if (!(*it == *other_it))
          return false;

   return true;
} 

template <typename ItemType>
bool Vector<ItemType>::operator!=(const Vector &other) const {
    if (*this == other)
        return false;

    return true;
}

template <typename ItemType>
template <typename IteratorType>
int Vector<ItemType>::Compare::execute(IteratorType first_1, IteratorType end_1, IteratorType first_2, IteratorType end_2) {
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
bool Vector<ItemType>::operator<(const Vector &other) const {
    if (Compare().execute(this->cbegin(), this->cend(), other.cbegin(), other.cend()) < 0)
        return true;

    return false;
}

template <typename ItemType>
bool Vector<ItemType>::operator>(const Vector &other) const {
    if (Compare().execute(this->cbegin(), this->cend(), other.cbegin(), other.cend()) > 0)
        return true;

    return false;
}

template <typename ItemType>
bool Vector<ItemType>::operator<=(const Vector &other) const {
    if (Compare().execute(this->cbegin(), this->cend(), other.cbegin(), other.cend()) <= 0)
        return true;

    return false;
}

template <typename ItemType>
bool Vector<ItemType>::operator>=(const Vector &other) const {
    if (Compare().execute(this->cbegin(), this->cend(), other.cbegin(), other.cend()) >= 0)
        return true;

    return false;
}

template <typename ItemType>
Vector<ItemType>::Iterator::Iterator() {
    this->buffer = nullptr;
}

template <typename ItemType>
Vector<ItemType>::Iterator::Iterator(ItemType *buffer) {
    this->buffer = buffer;
}

template <typename ItemType>
Vector<ItemType>::Iterator::Iterator(const Vector<ItemType>::Iterator &iterator) {
    this->buffer = iterator.buffer;
}

template <typename ItemType>
typename Vector<ItemType>::Iterator& Vector<ItemType>::Iterator::operator=(const Vector<ItemType>::Iterator &iterator) {
    this->buffer = iterator.buffer;
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::begin() {
    return Vector<ItemType>::Iterator(this->buffer);
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::end() {
    return Vector<ItemType>::Iterator(this->buffer + this->size_);
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::Iterator::operator++(int) {
    auto iterator = *this;

    this->buffer++;

    return iterator;
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::Iterator::operator++() {
    this->buffer++;

    return *this;
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::Iterator::operator--(int) {
    auto iterator = *this;

    this->buffer--;

    return iterator;
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::Iterator::operator--() {
    this->buffer--;

    return *this;
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::Iterator::operator+(ptrdiff_t x) {
    auto iterator = *this;

    iterator.buffer += x;

    return iterator;
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::Iterator::operator-(ptrdiff_t x) {
    auto iterator = *this;

    iterator.buffer -= x;

    return iterator;
}

template <typename ItemType>
ptrdiff_t Vector<ItemType>::Iterator::operator-(const typename Vector<ItemType>::Iterator& iterator) const {
    return this->buffer - iterator.buffer;
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::Iterator::operator+=(ptrdiff_t x) {
    this->buffer += x;

    return *this;
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::Iterator::operator-=(ptrdiff_t x) {
    this->buffer -= x;

    return *this;
}

template <typename ItemType>
ItemType& Vector<ItemType>::Iterator::operator*() {
    return *this->buffer;
}

template <typename ItemType>
ItemType* Vector<ItemType>::Iterator::operator->() {
    return this->buffer;
}

template <typename ItemType>
ItemType& Vector<ItemType>::Iterator::operator[](ptrdiff_t index) const {
    return *(this->buffer + index);
}

template <typename ItemType>
bool Vector<ItemType>::Iterator::operator==(const Iterator &iterator) const {
    return this->buffer == iterator.buffer;
}

template <typename ItemType>
bool Vector<ItemType>::Iterator::operator!=(const Iterator &iterator) const {
    return this->buffer != iterator.buffer;
}

template <typename ItemType>
bool Vector<ItemType>::Iterator::operator<(const Iterator &iterator) const {
    return (*this - iterator < 0) ? true : false;
}

template <typename ItemType>
bool Vector<ItemType>::Iterator::operator>(const Iterator &iterator) const {
    return (*this - iterator > 0) ? true : false;
}

template <typename ItemType>
bool Vector<ItemType>::Iterator::operator<=(const Iterator &iterator) const {
    return (*this - iterator <= 0) ? true : false;
}
template <typename ItemType>
bool Vector<ItemType>::Iterator::operator>=(const Iterator &iterator) const {
    return (*this - iterator >= 0) ? true : false;
}

template <typename ItemType>
Vector<ItemType>::ConstIterator::ConstIterator() {
    this->buffer = nullptr;
}

template <typename ItemType>
Vector<ItemType>::ConstIterator::ConstIterator(ItemType *buffer) {
    this->buffer = buffer;
}

template <typename ItemType>
Vector<ItemType>::ConstIterator::ConstIterator(const Vector<ItemType>::ConstIterator &iterator) {
    this->buffer = iterator.buffer;
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator& Vector<ItemType>::ConstIterator::operator=(const Vector<ItemType>::ConstIterator &iterator) {
    this->buffer = iterator.buffer;
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator Vector<ItemType>::cbegin() const {
    return Vector<ItemType>::ConstIterator(this->buffer);
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator Vector<ItemType>::cend() const {
    return Vector<ItemType>::ConstIterator(this->buffer + this->size_);
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator Vector<ItemType>::ConstIterator::operator++(int) {
    ConstIterator iterator = *this;

    this->buffer++;

    return iterator;
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator Vector<ItemType>::ConstIterator::operator++() {
    this->buffer++;

    return *this;
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator Vector<ItemType>::ConstIterator::operator--(int) {
    auto iterator = *this;

    this->buffer--;

    return iterator;
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator Vector<ItemType>::ConstIterator::operator--() {
    this->buffer--;

    return *this;
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator Vector<ItemType>::ConstIterator::operator+(ptrdiff_t x) {
    auto iterator = *this;

    iterator.buffer += x;

    return iterator;
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator Vector<ItemType>::ConstIterator::operator-(ptrdiff_t x) {
    auto iterator = *this;

    iterator.buffer -= x;

    return iterator;
}

template <typename ItemType>
ptrdiff_t Vector<ItemType>::ConstIterator::operator-(const typename Vector<ItemType>::ConstIterator& iterator) const {
    return this->buffer - iterator.buffer;
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator Vector<ItemType>::ConstIterator::operator+=(ptrdiff_t x) {
    this->buffer += x;

    return *this;
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator Vector<ItemType>::ConstIterator::operator-=(ptrdiff_t x) {
    this->buffer -= x;

    return *this;
}

template <typename ItemType>
const ItemType& Vector<ItemType>::ConstIterator::operator*() const {
    return *this->buffer;
}

template <typename ItemType>
const ItemType& Vector<ItemType>::ConstIterator::operator[](ptrdiff_t index) const {
    return *(this->buffer + index);
}

template <typename ItemType>
const ItemType* Vector<ItemType>::ConstIterator::operator->() const {
    return this->buffer;
}

template <typename ItemType>
bool Vector<ItemType>::ConstIterator::operator==(const ConstIterator &iterator) const {
    return this->buffer == iterator.buffer;
}

template <typename ItemType>
bool Vector<ItemType>::ConstIterator::operator!=(const ConstIterator &iterator) const {
    return this->buffer != iterator.buffer;
}

template <typename ItemType>
bool Vector<ItemType>::ConstIterator::operator<(const ConstIterator &iterator) const {
    return (*this - iterator < 0) ? true : false;
}

template <typename ItemType>
bool Vector<ItemType>::ConstIterator::operator>(const ConstIterator &iterator) const {
    return (*this - iterator > 0) ? true : false;
}

template <typename ItemType>
bool Vector<ItemType>::ConstIterator::operator<=(const ConstIterator &iterator) const {
    return (*this - iterator <= 0) ? true : false;
}
template <typename ItemType>
bool Vector<ItemType>::ConstIterator::operator>=(const ConstIterator &iterator) const {
    return (*this - iterator >= 0) ? true : false;
}
