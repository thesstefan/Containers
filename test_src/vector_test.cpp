#include <iostream>
#include <cassert>
#include <cstdint>

// Used for range iterators
#include <vector>

#include "vector.h"

class Dummy {
    private:
        int id = 0;

    public:
        Dummy() : id(0) {}
        Dummy(int new_id) : id(new_id) {}

        int get_id() const {
            return this->id;
        }

        bool operator==(const Dummy &other) const {
            return this->id == other.id;
        }
};

std::ostream& operator<<(std::ostream& stream, const Dummy& dummy) {
    stream << "Dummy : " << dummy.get_id() << std::endl;

    return stream;
}

template <typename ItemType>
void default_constructor_test() {
    std::cout << "Vector() -> ";

    Vector<ItemType> vec;

    assert(vec.size() == 0);
    assert(vec.capacity() == 0);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void count_and_value_constructor_test() {
    std::cout << "Vector(size_t count, ItemType& value) -> ";

    ItemType item(10);

    Vector<ItemType> vec(size_t(10), item);

    assert(vec.size() == 10);
    assert(vec.capacity() == 10);

    for (auto it = vec.cbegin(); it != vec.cend(); it++)
        assert(*it == item);

    Vector<ItemType> vec_(size_t(0), item);

    assert(vec_.size() == 0);
    assert(vec_.capacity() == 0);

#ifndef MEMORY_CHECK
    try {
        Vector<ItemType> vec__(size_t(-1), item);
    } catch (std::exception &e) {
        const char *expected = "std::bad_array_new_length";

        assert(*e.what() == *expected);
    }

    try {
        Vector<ItemType> vec___(size_t(SIZE_MAX + 1), item);
    } catch (std::exception &e) {
        const char *expected = "std::bad_array_new_length";

        assert(*e.what() == *expected);
    }
#endif

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void count_constructor_test() {
    std::cout << "Vector(size_t count) -> ";

    Vector<ItemType> vec(10);

    assert(vec.size() == 10);
    assert(vec.capacity() == 10);

    for (auto it = vec.cbegin(); it != vec.cend(); it++)
        assert(*it == ItemType());

    Vector<ItemType> vec_(0);

    assert(vec_.size() == 0);
    assert(vec_.capacity() == 0);

#ifndef MEMORY_CHECK
    try {
        Vector<ItemType> vec__(-1);
    } catch (std::exception &e) {
        const char *expected = "std::bad_array_new_length";

        assert(*e.what() == *expected);
    }

    try {
        Vector<ItemType> vec___(SIZE_MAX + 1);
    } catch (std::exception &e) {
        const char *expected = "std::bad_array_new_length";

        assert(*e.what() == *expected);
    }
#endif

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void range_constructor_test() {
    std::cout << "Vector(IteratorType first, IteratorType last) -> ";

    std::vector<ItemType> vector;

    Vector<ItemType> vec(vector.cbegin(), vector.cend());

    assert(vec.size() == 0);
    assert(vec.capacity() == 0);

    vector.push_back(10);
    vector.push_back(20);

    Vector<ItemType> vec_(vector.cbegin(), vector.cend());

    assert(vec_.size() == vector.size());

    // Assuming that std::vector has the same capacity() grow scale.
    assert(vec_.capacity() == vector.capacity());

    for (auto it_ = vec_.cbegin(), it = vector.cbegin(); it_ != vec_.cend(), it != vector.cend(); it_++, it++)
        assert(*it_ == *it);

#ifndef MEMORY_CHECK
    try {
        // first > last => last can't be reached by iterating first.
        Vector<ItemType> vec__(vector.cend(), vector.cbegin());
    } catch (std::exception &e) {
        const char *expected = "std::bad_alloc";

        assert(*e.what() == *expected);
    }
#endif

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void copy_constructor_test() {
    std::cout << "Vector(const Vector& other) -> ";

    Vector<ItemType> vec(100);

    Vector<ItemType> copy(vec);

    assert(copy.size() == vec.size());
    assert(copy.capacity() == vec.capacity());

    for (auto it = vec.cbegin(), it_ = copy.cbegin(); it != vec.cend(), it_ != copy.cend(); it++, it_++)
        assert(*it == *it_);

    Vector<ItemType> vec_(0);

    Vector<ItemType> copy_(vec_);

    assert(copy_.size() == vec_.size() && copy_.size() == 0);
    assert(copy_.capacity() == vec_.capacity() && copy_.capacity() == 0);
    assert(copy_.cbegin() == copy_.cend());

    Vector<ItemType> vec__;

    Vector<ItemType> copy__(vec__);

    assert(copy__.size() == vec__.size() && copy__.size() == 0);
    assert(copy__.capacity() == vec__.capacity() && copy__.capacity() == 0);
    assert(copy__.cbegin() == copy__.cend());

    Vector<ItemType> vec___(vec.begin(), vec.end());

    Vector<ItemType> copy___(vec___);

    assert(copy___.size() == vec___.size());
    assert(copy___.capacity() == vec___.capacity());

    for (auto it = vec___.cbegin(), it_ = copy___.cbegin(); it != vec___.cend(), it_ != copy___.cend(); it++, it_++)
        assert(*it == *it_);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void run_constructor_tests() {
    default_constructor_test<ItemType>();
    count_and_value_constructor_test<ItemType>();
    count_constructor_test<ItemType>();
    range_constructor_test<ItemType>();
    copy_constructor_test<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void copy_assignment_operator_test() {
    std::cout << "Vector::operator=(const Vector& other) -> ";

    Vector<ItemType> vec(100);

    Vector<ItemType> copy = vec;

    assert(copy.size() == vec.size());
    assert(copy.capacity() == vec.capacity());

    for (auto it = vec.cbegin(), it_ = copy.cbegin(); it != vec.cend(), it_ != copy.cend(); it++, it_++)
        assert(*it == *it_);

    Vector<ItemType> vec_(0);

    Vector<ItemType> copy_ = vec_;

    assert(copy_.size() == vec_.size() && copy_.size() == 0);
    assert(copy_.capacity() == vec_.capacity() && copy_.capacity() == 0);
    assert(copy_.cbegin() == copy_.cend());

    Vector<ItemType> vec__;

    Vector<ItemType> copy__ = vec__;

    assert(copy__.size() == vec__.size() && copy__.size() == 0);
    assert(copy__.capacity() == vec__.capacity() && copy__.capacity() == 0);
    assert(copy__.cbegin() == copy__.cend());

    Vector<ItemType> vec___(vec.begin(), vec.end());

    Vector<ItemType> copy___ = vec___;

    assert(copy___.size() == vec___.size());
    assert(copy___.capacity() == vec___.capacity());

    for (auto it = vec___.cbegin(), it_ = copy___.cbegin(); it != vec___.cend(), it_ != copy___.cend(); it++, it_++)
        assert(*it == *it_);

    std::cout << "SUCCESS" << std::endl;
    std::cout << std::endl;
}

template <typename ItemType>
void count_and_value_assign_test() {
    std::cout << "Vector::assign(size_t count, const ItemType& value) -> ";

    Vector<ItemType> vec;

    ItemType item(10);

    vec.assign(size_t(0), item);

    assert(vec.size() == 0);
    assert(vec.capacity() == 0);

    vec.assign(size_t(10), item);

    assert(vec.size() == 10);
    assert(vec.capacity() > 10);

    for (auto it = vec.cbegin(); it != vec.cend(); it++)
        assert(*it == item);

#ifndef MEMORY_CHECK
    /*
    try {
        vec.assign(-1, item);
    } catch (std::exception &e) {
        const char *expected = "std::bad_array_new_length";

        assert(*e.what() == *expected);
    }
    */
#endif

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void range_assign_test() {
    std::cout << "Vector::assign(IteratorType first, IteratorType last) -> ";

    Vector<ItemType> vec;

    std::vector<ItemType> vector;

    vector.push_back(10);
    vector.push_back(20);
    vector.push_back(30);

    vec.assign(vector.cbegin(), vector.cend());

    assert(vec.size() == vector.size() && vec.size() == 3);

    // Assuming that std::vector::capacity() has the same scaling
    assert(vec.capacity() == vector.capacity() && vec.capacity() >= 3);

    for (auto it_ = vec.cbegin(), it = vector.cbegin(); it_ != vec.cend(), it != vector.cend(); it_++, it++)
        assert(*it_ == *it);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void assign_test() {
    count_and_value_assign_test<ItemType>();
    range_assign_test<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void run_tests() {
    std::cout << std::endl;

    run_constructor_tests<ItemType>();

    copy_assignment_operator_test<ItemType>();
//    assign_test<ItemType>();
}

int main() {
    run_tests<int>();
    run_tests<Dummy>();

    return 0;
}
