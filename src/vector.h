#pragma once

#include "iterator.h"
#include "mem_tools.h"

// Used for std::allocator.
#include <memory>

// Used for is_integer<>().
#include <limits>

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

        using Base::get_allocator;

        iterator begin();
        iterator end();
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

/*
template <typename ItemType, typename Allocator>
template <typename InputIterator>
Vector<ItemType, Allocator>::Vector(InputIterator first, InputIterator last, const allocator_type& allocator) : Base(allocator) {
    if (std::numeric_limits<InputIterator>::is_integer == true) {
        // first -> size
        this->memory.start = this->memory.allocate(first);
        this->memory.storage_end = this->memory.start + first;

        // last -> value
        uninitialized_fill(this->memory.start, first, last, this->get_allocator());
    } else {
        const typename IteratorTraits<InputIterator>::difference_type size = distance(first, last);
    }
}
*/

template <typename ItemType, typename Allocator>
Vector<ItemType, Allocator>::~Vector() {


template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::iterator Vector<ItemType, Allocator>::begin() {
    return iterator(this->memory.start);
}

template <typename ItemType, typename Allocator>
typename Vector<ItemType, Allocator>::iterator Vector<ItemType, Allocator>::end() {
    return iterator(this->memory.finish);
}
