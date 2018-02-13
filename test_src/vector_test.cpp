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
void constructor_tests() {
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
void at_test() {
    std::cout << "Vector::at(size_t index) -> ";

    ItemType item(10);

    Vector<ItemType> vec(size_t(10), item);

    size_t index = 0;
    for (auto it = vec.cbegin(); it != vec.cend(); it++, index++)
        assert(vec.at(index) == *it);

#ifndef MEMORY_CHECK
    try {
        vec.at(-1);
    } catch (std::exception &e) {
        const char *expected = "std::out_of_range : Vector::at(size_t, index)";

        assert(*e.what() == *expected);
    }

    try {
        vec.at(11);
    } catch (std::exception &e) {
        const char *expected = "std::out_of_range : Vector::at(size_t, index)";

        assert(*e.what() == *expected);
    }

    Vector<ItemType> vec_;

    try {
        vec_.at(0);
    } catch (std::exception &e) {
        const char *expected = "std::out_of_range : Vector::at(size_t, index)";

        assert(*e.what() == *expected);
    }
#endif

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_at_test() {
    std::cout << "Vector::at(size_t index) const -> ";

    ItemType item(10);

    const Vector<ItemType> vec(size_t(10), item);

    size_t index = 0;
    for (auto it = vec.cbegin(); it != vec.cend(); it++, index++)
        assert(vec.at(index) == *it);

#ifndef MEMORY_CHECK
    try {
        vec.at(-1);
    } catch (std::exception &e) {
        const char *expected = "std::out_of_range : Vector::at(size_t, index) const";

        assert(*e.what() == *expected);
    }

    try {
        vec.at(10);
    } catch (std::exception &e) {
        const char *expected = "std::out_of_range : Vector::at(size_t, index) const";

        assert(*e.what() == *expected);
    }

    Vector<ItemType> vec_;

    try {
        vec_.at(0);
    } catch (std::exception &e) {
        const char *expected = "std::out_of_range : Vector::at(size_t, index) const";

        assert(*e.what() == *expected);
    }
#endif

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void at_tests() {
    at_test<ItemType>();
    const_at_test<ItemType>();
}

template <typename ItemType>
void access_element_operator_test() {
    std::cout << "Vector::operator[](size_t index) -> ";

    ItemType item(10);

    Vector<ItemType> vec(size_t(10), item);

    size_t index = 0;
    for (auto it = vec.cbegin(); it != vec.cend(); it++, index++)
        assert(vec.at(index) == *it);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_access_element_operator_test() {
    std::cout << "Vector::operator[](size_t index) const -> ";

    ItemType item(10);

    const Vector<ItemType> vec(size_t(10), item);

    size_t index = 0;
    for (auto it = vec.cbegin(); it != vec.cend(); it++, index++)
        assert(vec.at(index) == *it);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void access_element_operator_tests() {
    access_element_operator_test<ItemType>();
    const_access_element_operator_test<ItemType>();
    
    std::cout << std::endl;
}

template <typename ItemType>
void front_test() {
    std::cout << "Vector::front() -> ";

    ItemType item(10);

    Vector<ItemType> vec(size_t(10), item);

    assert(vec.front() == item);
    assert(vec.front() == *vec.begin());
    assert(vec.front() == vec[0]);
    assert(vec.front() == vec.at(0));

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_front_test() {
    std::cout << "Vector::front() const -> ";

    ItemType item(10);

    const Vector<ItemType> vec(size_t(10), item);

    assert(vec.front() == item);
    assert(vec.front() == *vec.cbegin());
    assert(vec.front() == vec[0]);
    assert(vec.front() == vec.at(0));

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void front_tests() {
    front_test<ItemType>();
    const_front_test<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void back_test() {
    std::cout << "Vector::back() -> ";

    ItemType item(10);

    Vector<ItemType> vec(size_t(10), item);

    assert(vec.back() == item);
    assert(vec.back() == vec[9]);
    assert(vec.back() == vec.at(9));

    assert(vec.back() == *(vec.end() - 1));

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_back_test() {
    std::cout << "Vector::back() const -> ";

    ItemType item(10);

    const Vector<ItemType> vec(size_t(10), item);

    assert(vec.back() == item);
    assert(vec.back() == vec[9]);
    assert(vec.back() == vec.at(9));

    assert(vec.back() == *(vec.cend() - 1));

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void back_tests() {
    back_test<ItemType>();
    const_back_test<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void element_access_tests() {
    at_tests<ItemType>();
    access_element_operator_tests<ItemType>();
    front_tests<ItemType>();
    back_tests<ItemType>();
}

template <typename ItemType>
void iterator_default_constructor_test() {
    std::cout << "Vector::Iterator() -> ";

    typename Vector<ItemType>::Iterator it;

    typename Vector<ItemType>::Iterator it_(nullptr);

    assert(it == it_);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_buffer_constructor_test() {
    std::cout << "Vector::Iterator(ItemType *buffer) -> ";

    typename Vector<ItemType>::Iterator it;
    typename Vector<ItemType>::Iterator it_(nullptr);

    assert(it == it_);

    ItemType item(10);
    size_t size = 10;

    ItemType *buffer = new ItemType[size];

    typename Vector<ItemType>::Iterator it__(buffer);
    
    for (size_t index = 0; index < size; index++)
        *(buffer + index) = item;

    typename Vector<ItemType>::Iterator it___(buffer);

    assert(it__ == it___);

    for (size_t index = 0; index < size; index++)
        assert(*it___ == item);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_copy_constructor_test() {
    std::cout << "Vector::Iterator(const Iterator& other) -> ";

    typename Vector<ItemType>::Iterator it;
    typename Vector<ItemType>::Iterator copy(it);

    assert(it == copy);

    typename Vector<ItemType>::Iterator it_(nullptr);
    typename Vector<ItemType>::Iterator copy_(it_);

    assert(it_ == copy_);
    assert(it == copy_);

    std::cout << "SUCCESS" << std::endl;
}


template <typename ItemType>
void iterator_constructor_tests() {
    iterator_default_constructor_test<ItemType>();
    iterator_buffer_constructor_test<ItemType>();
    iterator_copy_constructor_test<ItemType>();
}

template <typename ItemType>
void iterator_copy_assignment_operator_test() {
    std::cout << "Vector::Iterator::operator=(const Iterator& other) -> ";

    ItemType item(10);

    Vector<ItemType> vec(size_t(20), item);

    typename Vector<ItemType>::Iterator it = vec.begin();

    assert(it == vec.begin());
    assert(*it == item);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_prefix_increment_test() {
    std::cout << "Vector::Iterator::operator++(int) -> ";

    ItemType item(10);
    Vector<ItemType> vec(size_t(20), item);

    typename Vector<ItemType>::Iterator it = vec.begin();

    for ( ; it != vec.end(); ++it)
        assert(*it == item);

    it = vec.begin();

    assert(++it > vec.begin());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_postfix_increment_test() {
    std::cout << "Vector::Iterator::operator++() -> ";

    ItemType item(10);
    Vector<ItemType> vec(size_t(20), item);

    typename Vector<ItemType>::Iterator it = vec.begin();

    for ( ; it != vec.end(); it++)
        assert(*it == item);

    it = vec.begin();

    assert(it++ == vec.begin());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_increment_and_assign_test() {
    std::cout << "Vector::Iterator::operator+=(ptrdiff_t x) -> ";

    ItemType item(10);
    Vector<ItemType> vec(size_t(20), item);

    typename Vector<ItemType>::Iterator it = vec.begin();

    for ( ; it != vec.end(); it += 1)
        assert(*it == item);

    it = vec.begin();
    it += vec.size();

    assert(it == vec.end());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_increment_tests() {
    iterator_prefix_increment_test<ItemType>();
    iterator_postfix_increment_test<ItemType>();
    iterator_increment_and_assign_test<ItemType>();
}

/*
template <typename ItemType>
void iterator_decrement_tests() {
    iterator_prefix_decrement_test();
    iterator_postfix_decrement_test();
    iterator_return_decrement_test();
}

template <typename ItemType>
void iterator_access_tests<ItemType>() {
    iterator_return_pointer_operator_test();
    iterator_return_reference_operator_test();
    iterator_offset_dereference_operator_test();
}

template <typename ItemType>
void iterator_substraction_test() {}

template <typename ItemType>
void iterator_compare_tests() {
    iterator_equal_test<ItemType>;
    iterator_not_equal_test<ItemType>;

    iterator_less_test<ItemType>;
    iterator_greater_test<ItemType>;

    iterator_less_or_equal_test<ItemType>;
    iterator_greater_or_equal_test<ItemType>;
}
*/

template <typename ItemType>
void iterator_test() {
    iterator_constructor_tests<ItemType>();
    iterator_copy_assignment_operator_test<ItemType>();
    iterator_increment_tests<ItemType>();
    /*
    iterator_decrement_tests<ItemType>();
    iterator_access_tests<ItemType>();
    iterator_substraction_test<ItemType>();
    iterator_compare_tests<ItemType>();
*/
}

/*
template <typename ItemType>
void const_iterator_constructor_tests<ItemType>() {
    const_iterator_default_constructor_test<ItemType>();
    const_iterator_buffer_constructor_test<ItemType>();
    const_iterator_copy_constructor_test<ItemType>();
}

template <typename ItemType>
void const_iterator_increment_tests() {
    const_iterator_prefix_increment_test();
    const_iterator_postfix_increment_test();
    const_iterator_return_increment_test();
}

template <typename ItemType>
void const_iterator_decrement_tests() {
    const_iterator_prefix_decrement_test();
    const_iterator_postfix_decrement_test();
    const_iterator_return_decrement_test();
}

template <typename ItemType>
void const_iterator_access_tests<ItemType>() {
    const_iterator_return_pointer_operator_test();
    const_iterator_return_reference_operator_test();
    const_iterator_offset_dereference_operator_test();
}

template <typename ItemType>
void const_iterator_compare_tests() {
    const_iterator_equal_test<ItemType>;
    const_iterator_not_equal_test<ItemType>;

    const_iterator_less_test<ItemType>;
    const_iterator_greater_test<ItemType>;

    const_iterator_less_or_equal_test<ItemType>;
    const_iterator_greater_or_equal_test<ItemType>;
}

template <typename ItemType>
void const_iterator_test() {
    const_iterator_constructor_tests<ItemType>();
    const_iterator_copy_assignment_operator_test<ItemType>();
    const_iterator_increment_tests<ItemType>();
    const_iterator_decrement_tests<ItemType>();
    const_iterator_access_tests<ItemType>();
    const_iterator_substraction_test<ItemType>();
    const_iterator_compare_tests<ItemType>();
}
*/

template <typename ItemType>
void iterator_tests() {
    iterator_test<ItemType>();
//    const_iterator_test<ItemType>;
}

template <typename ItemType>
void run_tests() {
    std::cout << std::endl;

    constructor_tests<ItemType>();

    copy_assignment_operator_test<ItemType>();
    assign_test<ItemType>();
    
    element_access_tests<ItemType>();

    iterator_tests<ItemType>();
}


int main() {
    std::cout << "Vector<int> TESTS" << std::endl;

    run_tests<int>();

    /*
    std::cout << "Vector<Dummy> TESTS" << std::endl;

    run_tests<Dummy>();
    */

    return 0;
}
