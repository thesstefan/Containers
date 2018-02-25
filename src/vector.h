#pragma once

#include "iterator.h"
#include "mem_tools.h"
#include "misc.h"

// Used for true_type and false_type.
#include <type_traits>

// Used for std::allocator.
#include <memory>

// Used for is_integral<Type>().
#include <limits>

// Used for initialize_list constructor and assign.
#include <initializer_list>

template <typename ItemType, typename Allocator> 
class VectorBase {
    private:
        struct VectorMemory : public Allocator {
            ItemType* start;
            ItemType* finish;
            ItemType* storage_end;

            VectorMemory(const Allocator& allocator) : Allocator(allocator) {
                start = nullptr;
                finish = nullptr;
                storage_end = nullptr;
            }
        };

    public:
        typedef Allocator       allocator_type;

        VectorMemory memory; 

        allocator_type get_allocator() const {
            return *static_cast<const Allocator*>(&this->memory);
        }

        ItemType* memory_allocate(size_t size) {
            return this->memory.allocate(size);
        }

        void memory_deallocate(ItemType* start, size_t size) {
            if (start)
                this->memory.deallocate(start, size);
        }

        VectorBase(const allocator_type& allocator) : memory(VectorMemory(allocator)) {}

        VectorBase(size_t size, const allocator_type& allocator) : memory(VectorMemory(allocator)) {
            this->memory.start = this->memory_allocate(size);
            this->memory.finish = this->memory.start;
            this->memory.storage_end = this->memory.start + size;
        }

        ~VectorBase() {
            ptrdiff_t size = this->memory.storage_end - this->memory.start;

            this->memory_deallocate(this->memory.start, size);
        }
};

template <typename ItemType, typename Allocator = std::allocator<ItemType>>
class Vector : protected VectorBase<ItemType, Allocator> {
    private:
        typedef Vector<ItemType, Allocator>       vector_type;
        typedef VectorBase<ItemType, Allocator>   Base;

    public:
        typedef ItemType                                value_type;
        typedef typename Allocator::pointer             pointer;
        typedef typename Allocator::const_pointer       const_pointer;
        typedef typename Allocator::reference           reference;
        typedef typename Allocator::const_reference     const_reference;

        typedef size_t                                  size_type;
        typedef ptrdiff_t                               difference_type;

        typedef typename Base::allocator_type           allocator_type;

        typedef NormalIterator<pointer, vector_type>            iterator;
        typedef NormalIterator<const_pointer, vector_type>      const_iterator;

        Vector(const allocator_type& allocator = allocator_type());
        Vector(size_type size, const value_type& value, const allocator_type& allocator = allocator_type());
        Vector(size_type size);

        Vector(const Vector& other);

        template <typename InputIterator>
        Vector(InputIterator first, InputIterator last, const allocator_type& allocator = allocator_type());

        ~Vector();

        Vector& operator=(const Vector& other);

    protected:
        template <typename Type>
        void assign_choose(Type first, Type last, bool is_integral);

        void assign_fill(size_type count, const value_type& value);

        template <typename InputIterator>
        void assign_range_fill(InputIterator first, InputIterator last);

        template <typename IntegralType>
        void assign_choose(IntegralType first, IntegralType last, std::true_type);

        template <typename IteratorType>
        void assign_choose(IteratorType first, IteratorType last, std::false_type);

    public:
        void assign(size_type count, const value_type& value);

        template <typename IteratorType>
        void assign(IteratorType first, IteratorType last);

        void assign(std::initializer_list<ItemType> list);

        reference operator[](size_type offset);
        const_reference operator[](size_type offset) const;

        reference at(size_type offset);
        const_reference at(size_type offset) const;

        reference front();
        const_reference front() const;

        reference back();
        const_reference back() const;

        using Base::get_allocator;

        size_type size() const;
        size_type max_size() const;
        size_type capacity() const;

        iterator begin();
        const_iterator begin() const;

        iterator end();
        const_iterator end() const;

        bool empty() const;

        iterator erase(iterator position);
        void erase(iterator first, iterator last);

        iterator insert(iterator position, const value_type& value);

        void clear();

        void push_back(const value_type& value);

        void reserve(size_type capacity);
        bool alloc_memory_if_needed();

        void pop_back();

        bool operator==(const Vector& other);
        bool operator!=(const Vector& other);
};

template <typename ItemType, typename Allocator>
Vector<ItemType, Allocator>::Vector(const allocator_type& allocator) : Base(allocator) {}

template <typename ItemType, typename Allocator>
Vector<ItemType, Allocator>::Vector(size_type size, const value_type& value, const allocator_type& allocator) : Base(size, allocator) {
    uninitialized_fill(this->memory.start, size, value, this->get_allocator());

    this->memory.finish = this->memory.start + size;
}

template <typename ItemType, typename Allocator>
Vector<ItemType, Allocator>::Vector(size_type size) : Base(size, allocator_type()) {
    uninitialized_fill(this->memory.start, size, value_type(), this->get_allocator());

    this->memory.finish = this->memory.start + size;
}

template <typename ItemType, typename Allocator>
Vector<ItemType, Allocator>::Vector(const Vector<ItemType, Allocator>& other) : Base(other.size(), other.get_allocator()) {
    this->memory.finish = uninitialized_copy(other.begin(), other.end(), this->memory.start, this->get_allocator());
}

template <typename ItemType, typename Allocator>
Vector<ItemType, Allocator>& Vector<ItemType, Allocator>::operator=(const Vector<ItemType, Allocator>& other) {
    // Asssuming that the Allocator interface inherited by VectorMemory has a working operator=()
    this->memory = other.memory;
}

template <typename ItemType, typename Allocator>
Vector<ItemType, Allocator>::~Vector() {
    for (size_type index = 0; index < this->size(); index++)
        this->memory.destroy(this->memory.start + index);
}


template <typename ItemType, typename Allocator>
void Vector<ItemType, Allocator>::assign_fill(size_type count, const value_type& value) {
    this->clear();

    while (this->size() < count) 
        this->push_back(value);
}

template <typename ItemType, typename Allocator>
void Vector<ItemType, Allocator>::assign(size_type count, const value_type& value) {
    this->assign_fill(count, value);
}

template <typename ItemType, typename Allocator>
template <typename IteratorType>
void Vector<ItemType, Allocator>::assign_range_fill(IteratorType first, IteratorType last) {
    this->clear();

    for (auto it = first; it != last; it++)
        this->push_back(*it);
}

template <typename ItemType, typename Allocator>
template <typename IntegralType>
void Vector<ItemType, Allocator>::assign_choose(IntegralType first, IntegralType last, std::true_type) {
    this->assign_fill(static_cast<size_type>(first), static_cast<value_type>(last));
}

template <typename ItemType, typename Allocator>
template <typename InteratorType>
void Vector<ItemType, Allocator>::assign_choose(InteratorType first, InteratorType last, std::false_type) {
    this->assign_range_fill(first, last);
}

template <typename ItemType, typename Allocator>
template <typename IteratorType>
void Vector<ItemType, Allocator>::assign(IteratorType first, IteratorType last) {
    this->assign_choose(first, last, std::is_integral<IteratorType>());
}

template <typename ItemType, typename Allocator>
void Vector<ItemType, Allocator>::assign(std::initializer_list<ItemType> list) {
    this->clear();

    for (auto it = list.begin(); it != list.end(); it++)
        this->push_back(*it);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::iterator Vector<ItemType, Allocator>::begin() {
    return iterator(this->memory.start);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::const_iterator Vector<ItemType, Allocator>::begin() const {
    return const_iterator(this->memory.start);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::iterator Vector<ItemType, Allocator>::end() {
    return iterator(this->memory.finish);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::const_iterator Vector<ItemType, Allocator>::end() const {
    return const_iterator(this->memory.finish);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::size_type Vector<ItemType, Allocator>::size() const {
    return size_type(this->memory.finish - this->memory.start);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::size_type Vector<ItemType, Allocator>::max_size() const {
    return size_type(-1) / sizeof(value_type);
} 

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::size_type Vector<ItemType, Allocator>::capacity() const {
    return size_type(this->memory.storage_end - this->memory.start);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::iterator Vector<ItemType, Allocator>::erase(typename Vector<ItemType, Allocator>::iterator position) {
    difference_type relative_position = position - this->begin();

    for (auto it = position; it < this->end() - 1; it++)
        *it = *(it + 1);

    this->memory.finish--;

    if (this->alloc_memory_if_needed() == true)
        return position + relative_position;

    return position;
}

template <typename ItemType, typename Allocator>
void Vector<ItemType, Allocator>::erase(typename Vector<ItemType, Allocator>::iterator first, typename Vector<ItemType, Allocator>::iterator last) {
    auto position = first;

    for (auto it = first; it < last; it++)
        position = this->erase(position);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::iterator Vector<ItemType, Allocator>::insert(typename Vector<ItemType, Allocator>::iterator position, const value_type& value) {
    difference_type relative_position = position - this->begin();

    if (this->alloc_memory_if_needed() == true)
        position = this->begin() + relative_position;

    this->memory.finish++;

    for (auto it = this->end() - 1; it > position; it--)
        *it = *(it - 1);

    *position = value;

    return position;
}


template <typename ItemType, typename Allocator>
void Vector<ItemType, Allocator>::clear() {
    this->erase(this->begin(), this->end());
}

template <typename ItemType, typename Allocator>
bool Vector<ItemType, Allocator>::empty() const {
    return this->begin() == this->end();
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::reference Vector<ItemType, Allocator>::operator[](size_type offset) {
    return *(this->begin() + offset);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::const_reference Vector<ItemType, Allocator>::operator[](size_type offset) const {
    return *(this->begin() + offset);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::reference Vector<ItemType, Allocator>::at(size_type offset) {
    if (offset > this->size())
        throw std::out_of_range("std::out_of_range in Vector::at(size_type offset)");

    return *(this->begin() + offset);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::const_reference Vector<ItemType, Allocator>::at(size_type offset) const {
    if (offset > this->size())
        throw std::out_of_range("std::out_of_range in Vector::at(size_type offset) const");

    return *(this->begin() + offset);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::reference Vector<ItemType, Allocator>::front() {
    return *this->begin();
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::const_reference Vector<ItemType, Allocator>::front() const {
    return *this->begin();
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::reference Vector<ItemType, Allocator>::back() {
    return *(this->end() - 1);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::const_reference Vector<ItemType, Allocator>::back() const {
    return *(this->end() - 1);
}

template <typename ItemType, typename Allocator>
void Vector<ItemType, Allocator>::reserve(size_type capacity) {
    if (capacity > this->capacity()) {
        if (capacity > this->max_size())
            throw std::length_error("Parameter of Vector::reserve(size_type) exceeds Vector::max_size()");

        auto temp = this->memory.allocate(capacity);

        if (temp == nullptr)
            throw std::bad_alloc();

        size_type index = 0;
        for (auto it = this->memory.start; it != this->memory.finish; it++, index++)
            *(temp + index) = *it;

        size_type size = this->size();

        this->memory.deallocate(this->memory.start, this->capacity());
        this->memory.start = temp;

        this->memory.storage_end = this->memory.start + capacity;
        this->memory.finish = this->memory.start + size;
    }
}


template <typename ItemType, typename Allocator>
bool Vector<ItemType, Allocator>::alloc_memory_if_needed() {
    if (this->memory.finish == this->memory.storage_end) {
        if (this->size() == 0)
            this->reserve(1);
        else 
            this->reserve(this->capacity() * 2);

        return true;
    }

    return false;
}

template <typename ItemType, typename Allocator>
void Vector<ItemType, Allocator>::push_back(const value_type& value) {
    alloc_memory_if_needed();

    this->memory.construct(this->memory.finish++, value);
}

template <typename ItemType, typename Allocator>
void Vector<ItemType, Allocator>::pop_back() {
    this->memory.destroy(--this->memory.finish);
}

template <typename ItemType, typename Allocator>
bool Vector<ItemType, Allocator>::operator==(const Vector<ItemType, Allocator>& other) {
    if (this->size() != other.size())
        return false;

    for (auto it = this->begin(), it_other = other.begin(); it != this->end(); it++, it_other++)
        if (*it != *it_other)
            return false;

    return true;
}

template <typename ItemType, typename Allocator>
bool Vector<ItemType, Allocator>::operator!=(const Vector<ItemType, Allocator>& other) {
    return !(*this == other);
}
