#pragma once

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
                ItemType *buffer;

            public:
                Iterator();
                Iterator(ItemType *buffer);
                Iterator(const Iterator &iterator);

                Iterator& operator=(const Iterator &iterator);

                Iterator operator++();
                Iterator operator++(int);

                Iterator operator--();
                Iterator operator--(int);

                Iterator operator+(ptrdiff_t x);
                Iterator operator-(ptrdiff_t x);

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

        Vector();
        Vector(size_t count, ItemType &value);
        Vector(size_t count);

        template <typename IteratorType>
        Vector(IteratorType first, IteratorType last);

        Vector(const Vector &other);

        ~Vector();

        Vector& operator=(const Vector &other);
        void assign(size_t count, const ItemType &value);

        template <typename IteratorType>
        void assign(IteratorType first, IteratorType last);

        ItemType &operator[](size_t index);
        const ItemType& operator[](size_t index) const;

        ItemType& at(size_t index);
        const ItemType& at(size_t index) const;

        ItemType& front();
        const ItemType& front() const;

        ItemType& back();
        const ItemType& back() const;

        bool empty() const;

        size_t size() const;
        size_t capacity() const;

        Iterator begin();
        Iterator end();

        ConstIterator cbegin() const;
        ConstIterator cend() const;

        void reserve(size_t capacity);

        Iterator find(const ItemType &item);

        void clear();

        Iterator insert(Iterator position, const ItemType &value);
        void insert(Iterator position, size_t count, const ItemType& value);

        template <typename IteratorType>
        void insert(Iterator position, IteratorType first, IteratorType last);

        Iterator erase(Iterator position);
        void erase(Iterator first, Iterator last);

        void push_back(const ItemType &item);
        void pop_back();

        void swap(Vector &other);
        void resize(size_t count, const ItemType &value);

        bool operator==(const Vector &other) const;
        bool operator!=(const Vector &other) const;

        bool operator<(const Vector &other) const;
        bool operator>(const Vector &other) const;
        bool operator>=(const Vector &other) const;
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
        throw std::out_of_range("Index is out of range in Vector::at(size_t index)");

    return this->buffer[index];
}

template <typename ItemType>
const ItemType& Vector<ItemType>::at(size_t index) const {
    if (index < 0 || index >= this->size_)
        throw std::out_of_range("Index is out of range in Vector::at(size_t index)");

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
typename Vector<ItemType>::Iterator Vector<ItemType>::find(const ItemType &item) {
    auto it = this->begin();

    for ( ; it != this->end() && *it != item; it++)

    return it;
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
       if (*it != *other_it)
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
        std::cout << *it_1 << " " << *it_2 << std::endl;

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
typename Vector<ItemType>::Iterator Vector<ItemType>::Iterator::operator++() {
    auto iterator = *this;

    this->buffer++;

    return iterator;
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::Iterator::operator++(int) {
    this->buffer++;

    return *this;
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::Iterator::operator--() {
    auto iterator = *this;

    this->buffer--;

    return iterator;
}

template <typename ItemType>
typename Vector<ItemType>::Iterator Vector<ItemType>::Iterator::operator--(int) {
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
typename Vector<ItemType>::ConstIterator Vector<ItemType>::ConstIterator::operator++() {
    ConstIterator iterator = *this;

    this->buffer++;

    return iterator;
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator Vector<ItemType>::ConstIterator::operator++(int) {
    this->buffer++;

    return *this;
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator Vector<ItemType>::ConstIterator::operator--() {
    auto iterator = *this;

    this->buffer--;

    return iterator;
}

template <typename ItemType>
typename Vector<ItemType>::ConstIterator Vector<ItemType>::ConstIterator::operator--(int) {
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
