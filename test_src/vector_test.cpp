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

        bool operator>(const Dummy &other) const {
            return this->id > other.id;
        }

        bool operator<(const Dummy &other) const {
            return this->id < other.id;
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
    //
    // TYPE DEPENDENT
    //
    // assert(vec_.capacity() == vector.capacity());

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
    //
    // TYPE DEPENDENT
    //
    //assert(vec.capacity() == vector.capacity() && vec.capacity() >= 3);

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

    typename Vector<ItemType>::Iterator it_;

    assert(it == it_);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_copy_constructor_test() {
    std::cout << "Vector::Iterator(const Iterator& other) -> ";

    typename Vector<ItemType>::Iterator it;
    typename Vector<ItemType>::Iterator copy(it);

    assert(it == copy);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_constructor_tests() {
    iterator_default_constructor_test<ItemType>();
    iterator_copy_constructor_test<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void iterator_copy_assignment_operator_test() {
    std::cout << "Vector::Iterator::operator=(const Iterator& other) -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::Iterator it = vec.begin();

    assert(it == vec.begin());
    assert(*it == vec[0]);

    std::cout << "SUCCESS" << std::endl;

    std::cout << std::endl;
}

template <typename ItemType>
void iterator_prefix_increment_test() {
    std::cout << "Vector::Iterator::operator++(int) -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::Iterator it = vec.begin();

    for (size_t index = 0; it != vec.end(); ++it, index++)
        assert(*it == vec[index]);

    it = vec.begin();

    assert(++it > vec.begin());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_postfix_increment_test() {
    std::cout << "Vector::Iterator::operator++() -> ";

    ItemType item(10);
    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::Iterator it = vec.begin();

    for (size_t index = 0; it != vec.end(); it++, index++)
        assert(*it == vec[index]);

    it = vec.begin();

    assert(it++ == vec.begin() && it > vec.begin());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_increment_and_assign_test() {
    std::cout << "Vector::Iterator::operator+=(ptrdiff_t x) -> ";

    ItemType item(10);
    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::Iterator it = vec.begin();

    for (size_t index = 0; it != vec.end(); it += 1, index++)
        assert(*it == vec[index]);

    it = vec.begin();
    it += vec.size();

    assert(it == vec.end());

    it = vec.begin();
    it += 0;

    assert(it == vec.begin());

    it += -1;

    assert(it < vec.begin());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_increment_tests() {
    iterator_prefix_increment_test<ItemType>();
    iterator_postfix_increment_test<ItemType>();
    iterator_increment_and_assign_test<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void iterator_prefix_decrement_test() {
    std::cout << "Vector::Iterator::operator--(int) -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::Iterator it = vec.end();

    --it;
    for (size_t index = vec.size() - 1; it != vec.begin(); --it, --index)
        assert(*it == vec[index]);

    it = vec.end();

    assert(--it < vec.end());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_postfix_decrement_test() {
    std::cout << "Vector::Iterator::operator--() -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::Iterator it = vec.end();

    it--;
    for (size_t index = vec.size() - 1; it != vec.begin(); it--, index--)
        assert(*it == vec[index]);

    it = vec.end();

    assert(it-- == vec.end() && it < vec.end());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_decrement_and_assign_test() {
    std::cout << "Vector::Iterator::operator-=(ptrdiff_t x) -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::Iterator it = vec.end();

    it -= 1;
    for (size_t index = vec.size() - 1; it != vec.begin(); it -= 1, index -= 1)
        assert(*it == vec[index]);

    it = vec.end();
    it -= vec.size();

    assert(it == vec.begin());

    it -= 0;

    assert(it == vec.begin());

    it -= -1;

    assert(it > vec.begin());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_decrement_tests() {
    iterator_prefix_decrement_test<ItemType>();
    iterator_postfix_decrement_test<ItemType>();
    iterator_decrement_and_assign_test<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void iterator_substraction_test() {
    std::cout << "Vector::Iterator::operator-(const Iterator &iterator) const ->";

    Vector<ItemType> vec;

    assert(vec.end() - vec.begin() == vec.size());
    assert(vec.begin() - vec.end() == -vec.size());
    assert(vec.end() - vec.size() == vec.begin());

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);
    vec.push_back(50);
    vec.push_back(60);
    vec.push_back(70);
    vec.push_back(80);

    assert(vec.end() - vec.begin() == vec.size());
    assert(vec.begin() - vec.end() == -vec.size());
    assert(vec.end() - vec.size() == vec.begin());

    std::cout << "SUCCESS" << std::endl;

    std::cout << std::endl;
}

// Run only for Dummy class.
void dummy_iterator_return_pointer_operator_test() {
    std::cout << "Vector::Iterator::operator->() -> ";

    Vector<Dummy> vec;

    vec.push_back(Dummy(10));
    vec.push_back(Dummy(20));
    vec.push_back(Dummy(30));
    vec.push_back(Dummy(40));
    vec.push_back(Dummy(50));

    auto it = vec.begin();
    for (size_t index = 0; it != vec.end(); it++, index++) 
        assert(it->get_id() == (vec[index]).get_id());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_return_reference_operator_test() {
    std::cout << "Vector::Iterator::operator*() -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    auto it = vec.begin();
    for (size_t index = 0; it != vec.end(); it++, index++)
        assert(*it == vec[index]);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_offset_dereference_operator_test() {
    std::cout << "Vector::Iterator::operator[](ptrdiff_t index) const -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    auto it = vec.begin();

    assert(it[0] == vec[0]);

    for (size_t index = 0; index < vec.size(); index++)
        assert(it[index] == vec[index]);

    it = vec.end();
    for (size_t index = vec.size() - 1; index > 0; index--) {
        ptrdiff_t offset = vec.size() - index;

        // Tests negative argument
        assert(it[-offset] == vec[index]);
    }

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_access_tests(bool is_dummy) {
    if (is_dummy == true)
        dummy_iterator_return_pointer_operator_test();

    iterator_return_reference_operator_test<ItemType>();
    iterator_offset_dereference_operator_test<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void iterator_equal_test() {
    std::cout << "Vector::Iterator::operator==(const Iterator &iterator) const -> ";

    typename Vector<ItemType>::Iterator it;
    typename Vector<ItemType>::Iterator it_;

    assert(it == it_);

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    it = vec.begin();
    it_ = vec.begin();

    assert(it == it_);

    for ( ; it != vec.end() && it_ != vec.end(); it++, it_++)
        assert(it == it_);

    assert(it == it_);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_not_equal_test() {
    std::cout << "Vector::Iterator::operator!=(const Iterator &iterator) const -> ";

    typename Vector<ItemType>::Iterator it;
    typename Vector<ItemType>::Iterator it_;

    assert(!(it != it_));

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    it = vec.begin();
    it_ = vec.begin();

    assert(!(it != it_));

    for ( ; it != vec.end() && it_ != vec.end(); it++, it_++)
        assert(!(it != it_));

    assert(!(it != it_));

    assert(vec.begin() != vec.end());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_less_test() {
    std::cout << "Vector::Iterator::operator<(const Iterator &iterator) const -> ";

    typename Vector<ItemType>::Iterator it;
    typename Vector<ItemType>::Iterator it_;

    assert(!(it < it_) && !(it_ < it));

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    it = vec.begin();

    for (size_t index = 0; it < vec.end(); it++, index++)
        assert(*it == vec[index]);

    assert(it_ < it);
    assert(vec.begin() < it);
    assert(!(it < vec.end()));

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_greater_test() {
    std::cout << "Vector::Iterator::operator>(const Iterator &iterator) const -> ";

    typename Vector<ItemType>::Iterator it;
    typename Vector<ItemType>::Iterator it_;

    assert(!(it > it_) && !(it_ > it));

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    it = vec.end() - 1;

    for (size_t index = vec.size() - 1; it > vec.begin(); it--, index--)
        assert(*it == vec[index]);

    assert(it > it_);
    assert(!(vec.begin() > it));
    assert(it < vec.end());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_less_or_equal_test() {
    std::cout << "Vector::Iterator::operator<=(const Iterator &iterator) const -> ";

    typename Vector<ItemType>::Iterator it;
    typename Vector<ItemType>::Iterator it_;

    assert(it <= it_ && it_ <= it);

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    it = vec.begin();

    for (size_t index = 0; it < vec.end(); it++, index++)
        assert(*it == vec[index]);

    assert(it_ <= it);
    assert(vec.begin() <= it);
    assert(it <= vec.end());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_greater_or_equal_test() {
    std::cout << "Vector::Iterator::operator>=(const Iterator &iterator) const -> ";

    typename Vector<ItemType>::Iterator it;
    typename Vector<ItemType>::Iterator it_;

    assert(it >= it_ && it_ >= it);

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    it = vec.end() - 1;

    for (size_t index = vec.size() - 1; it >= vec.begin(); it--, index--)
        assert(*it == vec[index]);

    assert(it >= it_);
    assert(vec.begin() >= it);
    assert(vec.end() >= it);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void iterator_compare_tests() {
    iterator_equal_test<ItemType>();
    iterator_not_equal_test<ItemType>();

    std::cout << std::endl;

    iterator_less_test<ItemType>();
    iterator_greater_test<ItemType>();

    std::cout << std::endl;

    iterator_less_or_equal_test<ItemType>();
    iterator_greater_or_equal_test<ItemType>();
}

template <typename ItemType>
void iterator_test(bool is_dummy) {
    iterator_constructor_tests<ItemType>();
    iterator_copy_assignment_operator_test<ItemType>();
    iterator_increment_tests<ItemType>();
    iterator_decrement_tests<ItemType>();
    iterator_substraction_test<ItemType>();
    iterator_access_tests<ItemType>(is_dummy);
    iterator_compare_tests<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void const_iterator_default_constructor_test() {
    std::cout << "Vector::ConstIterator() -> ";

    typename Vector<ItemType>::ConstIterator it;

    typename Vector<ItemType>::ConstIterator it_;

    assert(it == it_);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_copy_constructor_test() {
    std::cout << "Vector::ConstIterator(const ConstIterator& other) -> ";

    typename Vector<ItemType>::ConstIterator it;
    typename Vector<ItemType>::ConstIterator copy(it);

    assert(it == copy);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_constructor_tests() {
    const_iterator_default_constructor_test<ItemType>();
    const_iterator_copy_constructor_test<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void const_iterator_copy_assignment_operator_test() {
    std::cout << "Vector::ConstIterator::operator=(const ConstIterator& other) -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::ConstIterator it = vec.cbegin();

    assert(it == vec.cbegin());
    assert(*it == vec[0]);

    std::cout << "SUCCESS" << std::endl;

    std::cout << std::endl;
}

template <typename ItemType>
void const_iterator_prefix_increment_test() {
    std::cout << "Vector::ConstIterator::operator++(int) -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::ConstIterator it = vec.cbegin();

    for (size_t index = 0; it != vec.cend(); ++it, index++)
        assert(*it == vec[index]);

    it = vec.cbegin();

    assert(++it > vec.cbegin());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_postfix_increment_test() {
    std::cout << "Vector::ConstIterator::operator++() -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::ConstIterator it = vec.cbegin();

    for (size_t index = 0; it != vec.cend(); it++, index++)
        assert(*it == vec[index]);

    it = vec.cbegin();

    assert(it++ == vec.cbegin() && it > vec.cbegin());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_increment_and_assign_test() {
    std::cout << "Vector::Iterator::operator+=(ptrdiff_t x) -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::ConstIterator it = vec.cbegin();

    for (size_t index = 0; it != vec.cend(); it += 1, index++)
        assert(*it == vec[index]);

    it = vec.cbegin();
    it += vec.size();

    assert(it == vec.cend());

    it = vec.cbegin();
    it += 0;

    assert(it == vec.cbegin());

    it += -1;

    assert(it < vec.cbegin());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_increment_tests() {
    const_iterator_prefix_increment_test<ItemType>();
    const_iterator_postfix_increment_test<ItemType>();
    const_iterator_increment_and_assign_test<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void const_iterator_prefix_decrement_test() {
    std::cout << "Vector::ConstIterator::operator--(int) -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::ConstIterator it = vec.cend();

    --it;
    for (size_t index = vec.size() - 1; it != vec.cbegin(); --it, --index)
        assert(*it == vec[index]);

    it = vec.cend();

    assert(--it < vec.cend());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_postfix_decrement_test() {
    std::cout << "Vector::ConstIterator::operator--() -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::ConstIterator it = vec.cend();

    it--;
    for (size_t index = vec.size() - 1; it != vec.cbegin(); it--, index--)
        assert(*it == vec[index]);

    it = vec.cend();

    assert(it-- == vec.cend() && it < vec.cend());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_decrement_and_assign_test() {
    std::cout << "Vector::ConstIterator::operator-=(ptrdiff_t x) -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    typename Vector<ItemType>::ConstIterator it = vec.cend();

    it -= 1;
    for (size_t index = vec.size() - 1; it != vec.cbegin(); it -= 1, index -= 1)
        assert(*it == vec[index]);

    it = vec.cend();
    it -= vec.size();

    assert(it == vec.cbegin());

    it -= 0;

    assert(it == vec.cbegin());

    it -= -1;

    assert(it > vec.cbegin());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_decrement_tests() {
    const_iterator_prefix_decrement_test<ItemType>();
    const_iterator_postfix_decrement_test<ItemType>();
    const_iterator_decrement_and_assign_test<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void const_iterator_substraction_test() {
    std::cout << "Vector::ConstIterator::operator-(const ConstIterator &iterator) const ->";

    Vector<ItemType> vec;

    assert(vec.cend() - vec.cbegin() == vec.size());
    assert(vec.cbegin() - vec.cend() == -vec.size());
    assert(vec.cend() - vec.size() == vec.cbegin());

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);
    vec.push_back(50);
    vec.push_back(60);
    vec.push_back(70);
    vec.push_back(80);

    assert(vec.cend() - vec.cbegin() == vec.size());
    assert(vec.cbegin() - vec.cend() == -vec.size());
    assert(vec.cend() - vec.size() == vec.cbegin());

    std::cout << "SUCCESS" << std::endl;

    std::cout << std::endl;
}

// Run only for Dummy class.
void const_dummy_iterator_return_pointer_operator_test() {
    std::cout << "Vector::ConstIterator::operator->() -> ";

    Vector<Dummy> vec;

    vec.push_back(Dummy(10));
    vec.push_back(Dummy(20));
    vec.push_back(Dummy(30));
    vec.push_back(Dummy(40));
    vec.push_back(Dummy(50));

    auto it = vec.cbegin();
    for (size_t index = 0; it != vec.cend(); it++, index++) 
        assert(it->get_id() == (vec[index]).get_id());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_return_reference_operator_test() {
    std::cout << "Vector::ConstIterator::operator*() -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));
    vec.push_back(ItemType(50));

    auto it = vec.cbegin();
    for (size_t index = 0; it != vec.cend(); it++, index++)
        assert(*it == vec[index]);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_offset_dereference_operator_test() {
    std::cout << "Vector::Iterator::operator[](ptrdiff_t index) const -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    auto it = vec.begin();

    assert(it[0] == vec[0]);

    for (size_t index = 0; index < vec.size(); index++)
        assert(it[index] == vec[index]);

    it = vec.end();
    for (size_t index = vec.size() - 1; index > 0; index--) {
        ptrdiff_t offset = vec.size() - index;

        // Tests negative argument
        assert(it[-offset] == vec[index]);
    }

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_access_tests(bool is_dummy) {
    if (is_dummy == true)
        const_dummy_iterator_return_pointer_operator_test();

    const_iterator_return_reference_operator_test<ItemType>();
    const_iterator_offset_dereference_operator_test<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void const_iterator_equal_test() {
    std::cout << "Vector::ConstIterator::operator==(const ConstIterator &iterator) const -> ";

    typename Vector<ItemType>::ConstIterator it;
    typename Vector<ItemType>::ConstIterator it_;

    assert(it == it_);

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    it = vec.cbegin();
    it_ = vec.cbegin();

    assert(it == it_);

    for ( ; it != vec.cend() && it_ != vec.cend(); it++, it_++)
        assert(it == it_);

    assert(it == it_);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_not_equal_test() {
    std::cout << "Vector::ConstIterator::operator!=(const ConstIterator &iterator) const -> ";

    typename Vector<ItemType>::ConstIterator it;
    typename Vector<ItemType>::ConstIterator it_;

    assert(!(it != it_));

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    it = vec.cbegin();
    it_ = vec.cbegin();

    assert(!(it != it_));

    for ( ; it != vec.cend() && it_ != vec.cend(); it++, it_++)
        assert(!(it != it_));

    assert(!(it != it_));

    assert(vec.cbegin() != vec.cend());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_less_test() {
    std::cout << "Vector::ConstIterator::operator<(const ConstIterator &iterator) const -> ";

    typename Vector<ItemType>::ConstIterator it;
    typename Vector<ItemType>::ConstIterator it_;

    assert(!(it < it_) && !(it_ < it));

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    it = vec.cbegin();

    for (size_t index = 0; it < vec.cend(); it++, index++)
        assert(*it == vec[index]);

    assert(it_ < it);
    assert(vec.cbegin() < it);
    assert(!(it < vec.cend()));

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_greater_test() {
    std::cout << "Vector::ConstIterator::operator>(const ConstIterator &iterator) const -> ";

    typename Vector<ItemType>::ConstIterator it;
    typename Vector<ItemType>::ConstIterator it_;

    assert(!(it > it_) && !(it_ > it));

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    it = vec.cend() - 1;

    for (size_t index = vec.size() - 1; it > vec.cbegin(); it--, index--)
        assert(*it == vec[index]);

    assert(it > it_);
    assert(!(vec.cbegin() > it));
    assert(it < vec.cend());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_less_or_equal_test() {
    std::cout << "Vector::ConstIterator::operator<=(const ConstIterator &iterator) const -> ";

    typename Vector<ItemType>::ConstIterator it;
    typename Vector<ItemType>::ConstIterator it_;

    assert(it <= it_ && it_ <= it);

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    it = vec.cbegin();

    for (size_t index = 0; it < vec.cend(); it++, index++)
        assert(*it == vec[index]);

    assert(it_ <= it);
    assert(vec.cbegin() <= it);
    assert(it <= vec.cend());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_greater_or_equal_test() {
    std::cout << "Vector::ConstIterator::operator>=(const Iterator &iterator) const -> ";

    typename Vector<ItemType>::ConstIterator it;
    typename Vector<ItemType>::ConstIterator it_;

    assert(it >= it_ && it_ >= it);

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    it = vec.cend() - 1;

    for (size_t index = vec.size() - 1; it >= vec.cbegin(); it--, index--)
        assert(*it == vec[index]);

    assert(it >= it_);
    assert(vec.cbegin() >= it);
    assert(vec.cend() >= it);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void const_iterator_compare_tests() {
    const_iterator_equal_test<ItemType>();
    const_iterator_not_equal_test<ItemType>();

    std::cout << std::endl;

    const_iterator_less_test<ItemType>();
    const_iterator_greater_test<ItemType>();

    std::cout << std::endl;

    const_iterator_less_or_equal_test<ItemType>();
    const_iterator_greater_or_equal_test<ItemType>();
}

template <typename ItemType>
void const_iterator_test(bool is_dummy) {
    const_iterator_constructor_tests<ItemType>();
    const_iterator_copy_assignment_operator_test<ItemType>();
    const_iterator_increment_tests<ItemType>();
    const_iterator_decrement_tests<ItemType>();
    const_iterator_substraction_test<ItemType>();
    const_iterator_access_tests<ItemType>(is_dummy);
    const_iterator_compare_tests<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void iterator_tests(bool is_dummy) {
    iterator_test<ItemType>(is_dummy);
    const_iterator_test<ItemType>(is_dummy);
}

template <typename ItemType>
void begin_test() {
    std::cout << "Vector::begin() -> ";

    Vector<ItemType> vec;
    auto it = vec.begin();

    assert(vec.begin() == vec.end());
    assert(vec.begin() == typename Vector<ItemType>::Iterator());

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);

    assert(vec.begin() != it);

    it = vec.begin();

    assert(vec.begin() != vec.end());
    assert(vec.begin() != typename Vector<ItemType>::Iterator());

    assert(*vec.begin() == vec[0]);
    assert(vec.begin() == vec.end() - vec.size());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void cbegin_test() {
    std::cout << "Vector::cbegin() -> ";

    Vector<ItemType> vec;
    auto it = vec.cbegin();

    assert(vec.cbegin() == vec.cend());
    assert(vec.cbegin() == typename Vector<ItemType>::ConstIterator());

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);

    assert(vec.cbegin() != it);

    it = vec.cbegin();

    assert(vec.cbegin() != vec.cend());
    assert(vec.cbegin() != typename Vector<ItemType>::ConstIterator());

    assert(*vec.cbegin() == vec[0]);
    assert(vec.cbegin() == vec.cend() - vec.size());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void begin_tests() {
    begin_test<ItemType>();
    cbegin_test<ItemType>();
}

template <typename ItemType>
void end_test() {
    std::cout << "Vector::end() -> ";

    Vector<ItemType> vec;
    auto it = vec.end();

    assert(vec.end() == vec.begin());
    assert(vec.end() == typename Vector<ItemType>::Iterator());

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);

    assert(vec.end() != it);

    it = vec.end();

    assert(vec.end() != vec.begin());
    assert(vec.end() != typename Vector<ItemType>::Iterator());

    assert(*(--vec.end()) == vec[vec.size() - 1]);
    assert(vec.end() == vec.begin() + vec.size());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void cend_test() {
    std::cout << "Vector::cend() -> ";

    Vector<ItemType> vec;
    auto it = vec.cend();

    assert(vec.cend() == vec.cbegin());
    assert(vec.cend() == typename Vector<ItemType>::ConstIterator());

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);

    assert(vec.cend() != it);

    it = vec.cend();

    assert(vec.cend() != vec.cbegin());
    assert(vec.cend() != typename Vector<ItemType>::ConstIterator());

    assert(*(--vec.cend()) == vec[vec.size() - 1]);
    assert(vec.cend() == vec.cbegin() + vec.size());

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void end_tests() {
    end_test<ItemType>();
    cend_test<ItemType>();
}

template <typename ItemType>
void iterator_limits_tests() {
    begin_tests<ItemType>();

    std::cout << std::endl;

    end_tests<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void empty_test() {
    std::cout << "Vector::empty() const -> ";

    Vector<ItemType> vec;

    assert(vec.empty() == true);

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);

    assert(vec.empty() == false);

    vec.clear();

    assert(vec.empty() == true);

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    assert(vec.empty() == false);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void size_test() {
    std::cout << "Vector::size() const -> ";

    Vector<ItemType> vec;

    assert(vec.size() == 0);

    for (size_t index = 0; index < 100; index++) {
        assert(vec.size() == index);

        vec.push_back(ItemType(index));
    }

    for (size_t index = 100; index > 0; index--) {
        assert(vec.size() == index);

        vec.pop_back();
    }

    assert(vec.size() == 0);

    vec.push_back(10);

    assert(vec.size() == 1);

    vec.clear();

    assert(vec.size() == 0);

    std::cout << "SUCESS" << std::endl;
}

template <typename ItemType>
void capacity_test() {
    std::cout << "Vector::capacity() const -> ";

    Vector<ItemType> vec;

    assert(vec.capacity() == 0);

    size_t capacity_tracker = 0;

    for (size_t index = 0; index < 100; index++) {
        if (vec.size() == capacity_tracker)
            if (vec.size() == 0)
                capacity_tracker = 1;
            else
                capacity_tracker *= 2;

        vec.push_back(ItemType(index));

        assert(vec.capacity() == capacity_tracker);
    }

    assert(vec.capacity() == 128);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void reserve_test() {
    std::cout << "Vector::reserve(size_t capacity) -> ";

    Vector<ItemType> vec;

    vec.reserve(100);

    assert(vec.capacity() == 100);

    vec.reserve(99);

    assert(vec.capacity() == 100);

    vec.reserve(101);

    assert(vec.capacity() == 101);

#ifndef MEMORY_CHECK
    try {
        vec.reserve(-1);
    } catch (std::exception &e) {
        const char *expected = "std::bad_alloc";

        assert(*e.what() == *expected);
    }
#endif

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void capacity_tests() {
    empty_test<ItemType>();
    size_test<ItemType>();
    capacity_test<ItemType>();
    reserve_test<ItemType>();
}

template <typename ItemType>
void clear_test() {
    std::cout << "Vector::clear() -> ";

    Vector<ItemType> vec;

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);
    vec.push_back(50);

    size_t temp_cap = vec.capacity();

    vec.clear();

    assert(vec.size() == 0);
    assert(vec.capacity() == temp_cap);

    assert(vec.empty() == true);

    assert(vec.begin() == vec.end());
    assert(vec.cbegin() == vec.cend());

    std::cout << "SUCCES" << std::endl;
}

template <typename ItemType>
void insert_value_test() {
    std::cout << "Vector::insert(Iterator position, const ItemType& value) -> ";

    Vector<ItemType> vec;

    auto pos = vec.insert(vec.begin(), ItemType(10));

    assert(*pos == ItemType(10));

    assert(vec.empty() == false);
    assert(vec.size() == 1);

    pos = vec.insert(vec.end(), ItemType(20));

    assert(*pos == ItemType(20));

    assert(vec.empty() == false);
    assert(vec.size() == 2);

    pos = vec.insert(vec.begin(), ItemType(30));

    assert(*pos == ItemType(30));

    assert(vec.empty() == false);
    assert(vec.size() == 3);

    assert(vec[0] == ItemType(30) && vec[1] == ItemType(10) && vec[2] == ItemType(20));

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void insert_count_values_test() {
    std::cout << "Vector::insert(Iterator position, size_t count, const ItemType& value) -> ";

    Vector<ItemType> vec;

    vec.insert(vec.begin(), size_t(10), ItemType(10));

    assert(vec.empty() == false);
    assert(vec.size() == 10);

    for (auto it = vec.cbegin(); it != vec.cend(); it++)
        assert(*it == ItemType(10));

    vec.insert(vec.end(), size_t(10), ItemType(20));

    assert(vec.size() == 20);

    int count = 10;
    for (auto it = vec.cend() - 1; count > 0; it--, count--)
        assert(*it == ItemType(20));

    vec.insert(vec.begin(), size_t(10), ItemType(30));

    assert(vec.size() == 30);

    count = 10;
    for (auto it = vec.cbegin(); count > 0; it++, count--)
        assert(*it == ItemType(30));

    for (auto it = vec.cbegin(); it != vec.cend(); it++)
        assert(*it == ItemType(10) || *it == ItemType(20) || *it == ItemType(30));

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void insert_range() {
    std::cout << "Vector::insert(Iterator position, IteratorType first, IteratorType last) -> ";

    std::vector<ItemType> vector;

    vector.push_back(ItemType(10));
    vector.push_back(ItemType(20));
    vector.push_back(ItemType(30));

    Vector<ItemType> vec;

    vec.insert(vec.begin(), vector.begin(), vector.end());

    assert(vec.size() == vector.size());
    assert(vec.empty() == false);

    for (auto it = vector.cbegin(), it_ = vec.cbegin(); it != vector.cend(), it_ != vec.cend(); it++, it_++)
        assert(*it == *it_);

    vec.insert(vec.end(), vector.begin(), vector.end());

    assert(vec.size() == vector.size() * 2);

    for (size_t index = 0; index < 6; index += 3) 
        assert(vec[index] == ItemType(10) && vec[index + 1] == ItemType(20) && vec[index + 2] == ItemType(30));

    vec.insert(vec.begin(), vector.begin(), vector.end());

    for (size_t index = 0; index < 9; index += 3) 
        assert(vec[index] == ItemType(10) && vec[index + 1] == ItemType(20) && vec[index + 2] == ItemType(30));

    assert(vec.size() == vector.size() * 3);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void insert_tests() {
    insert_value_test<ItemType>();
    insert_count_values_test<ItemType>();
    insert_range<ItemType>();
    
    std::cout << std::endl;
}

template <typename ItemType>
void erase_test() {
    std::cout << "Vector::erase(Iterator position) -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));

    auto pos = vec.erase(vec.begin());

    assert(vec.size() == 0);
    assert(pos == vec.end());

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));

    pos = vec.erase(vec.begin() + 1);

    assert(*pos == ItemType(30));
    assert(vec.size() == 2);

    assert(vec[0] == ItemType(10) && vec[1] == ItemType(30));

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void erase_range() {
    std::cout << "Vector::erase(Iterator first, Iterator last) -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));

    vec.erase(vec.begin(), vec.end());

    assert(vec.empty() == true);
    assert(vec.size() == 0);

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));

    vec.erase(vec.begin(), vec.begin() + 1);

    assert(vec.size() == 2);
    assert(vec[0] == ItemType(20) && vec[1] == ItemType(30));

    vec.erase(vec.begin(), vec.end());

    assert(vec.empty() == true);
    assert(vec.size() == 0);

    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));
    vec.push_back(ItemType(40));

    vec.erase(vec.begin() + 1, vec.end() - 1);

    assert(vec.size() == 2);
    assert(vec[0] == ItemType(10) && vec[1] == ItemType(40));

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void erase_tests() {
    erase_test<ItemType>();
    erase_range<ItemType>();

    std::cout << std::endl;
}

template <typename ItemType>
void push_back_test() {
    std::cout << "Vector::push_back(ItemType &value) -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));

    assert(vec.size() == 1);
    assert(vec.empty() == false);

    assert(*vec.begin() == ItemType(10));

    vec.clear();

    for (size_t index = 0; index < 100; index++) {
        vec.push_back(ItemType(index));

        assert(vec[index] == index);
        assert(vec.size() == index + 1);
    }

    assert(vec.size() == 100);
    assert(vec.capacity() > 100);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void pop_back_test() {
    std::cout << "Vector::pop_back() -> ";

    Vector<ItemType> vec;

    vec.push_back(ItemType(10));

    vec.pop_back();

    assert(vec.empty() == true);
    assert(vec.size() == 0);

    for (size_t index = 0; index < 100; index++)
        vec.push_back(ItemType(index));

    size_t count = 100;
    while (count--) {
        vec.pop_back();

        assert(vec.size() == count);
    }

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void resize_test() {
    std::cout << "Vector::resize(size_t count, const ItemType& value) -> ";

    Vector<ItemType> vec;

    vec.resize(100, ItemType(10));

    assert(vec.size() == 100);
    assert(vec.capacity() > 100);

    for (auto it = vec.cbegin(); it != vec.cend(); it++)
        assert(*it == ItemType(10));

    vec.resize(10, ItemType(10));

    assert(vec.size() == 10);

    for (auto it = vec.cbegin(); it != vec.cend(); it++)
        assert(*it == ItemType(10));

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void swap_test() {
    std::cout << "Vector::swap(const Vector& other) -> ";

    Vector<ItemType> vec;
    
    vec.push_back(ItemType(10));
    vec.push_back(ItemType(20));
    vec.push_back(ItemType(30));

    Vector<ItemType> vec_;

    vec_.push_back(ItemType(100));
    vec_.push_back(ItemType(200));

    vec.swap(vec_);

    assert(vec.size() == 2 && vec_.size() == 3);

    assert(vec[0] == ItemType(100) && vec[1] == ItemType(200));
    assert(vec_[0] == ItemType(10) && vec_[1] == ItemType(20) && vec_[2] == ItemType(30));

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void modifiers_tests() {
    clear_test<ItemType>();

    insert_tests<ItemType>();
    erase_tests<ItemType>();

    push_back_test<ItemType>();
    pop_back_test<ItemType>();

    std::cout << std::endl;
    
    resize_test<ItemType>();
    swap_test<ItemType>();
    
    std::cout << std::endl;
}

template <typename ItemType>
void equal_test() {
    std::cout << "Vector::operator==(const Vector& other) -> ";

    Vector<ItemType> vec;
    Vector<ItemType> vec_;

    assert(vec == vec_);

    vec.push_back(10);
    vec_.push_back(10);

    assert(vec == vec_);

    vec.erase(vec.begin());
    vec_.erase(vec_.begin());

    assert(vec == vec_);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void not_equal_test() {
    std::cout << "Vector::operator!=(const Vector& other) -> ";

    Vector<ItemType> vec;
    Vector<ItemType> vec_;

    assert(!(vec != vec_));

    vec.push_back(10);
    vec_.push_back(10);

    assert(!(vec != vec_));

    vec.erase(vec.begin());
    vec_.erase(vec_.begin());

    assert(!(vec != vec_));

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void less_test() {
    std::cout << "Vector::operator<(const Vector& other) -> ";

    Vector<ItemType> vec;
    Vector<ItemType> vec_;

    assert(!(vec < vec_) && !(vec_ < vec));

    vec.push_back(10);
    vec_.push_back(20);

    assert(vec < vec_);

    vec.erase(vec.begin());
    vec.push_back(100);

    assert(vec_ < vec);

    vec.clear();

    assert(vec < vec_);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void greater_test() {
    std::cout << "Vector::operator>(const Vector& other) -> ";

    Vector<ItemType> vec;
    Vector<ItemType> vec_;

    assert(!(vec > vec_) && !(vec_ > vec));

    vec.push_back(10);
    vec_.push_back(20);

    assert(vec_ > vec);

    vec.erase(vec.begin());
    vec.push_back(100);

    assert(vec > vec_);

    vec.clear();

    assert(vec_ > vec);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void less_or_equal_test() {
    std::cout << "Vector::operator<=(const Vector& other) -> ";

    Vector<ItemType> vec;
    Vector<ItemType> vec_;

    assert(vec <= vec_ && vec_ <= vec);

    vec.push_back(10);
    vec_.push_back(20);

    assert(vec <= vec_);

    vec.erase(vec.begin());
    vec.push_back(100);

    assert(vec_ <= vec);

    vec.clear();

    assert(vec <= vec_);

    vec_.clear();

    assert(vec <= vec_);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void greater_or_equal_test() {
    std::cout << "Vector::operator>=(const Vector& other) -> ";

    Vector<ItemType> vec;
    Vector<ItemType> vec_;

    assert(vec >= vec_ && vec_ >= vec);

    vec.push_back(10);
    vec_.push_back(20);

    assert(vec_ >= vec);

    vec.erase(vec.begin());
    vec.push_back(100);

    assert(vec >= vec_);

    vec.clear();

    assert(vec_ >= vec);

    vec_.clear();

    assert(vec >= vec_);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void compare_tests() {
    equal_test<ItemType>();
    not_equal_test<ItemType>();

    std::cout << std::endl;

    less_test<ItemType>();
    greater_test<ItemType>();

    std::cout << std::endl;

    less_or_equal_test<ItemType>();
    greater_or_equal_test<ItemType>();
}

template <typename ItemType>
void run_test(bool is_dummy) {
    constructor_tests<ItemType>();

    copy_assignment_operator_test<ItemType>();

    assign_test<ItemType>();
    
    element_access_tests<ItemType>();

    iterator_tests<ItemType>(is_dummy);

    iterator_limits_tests<ItemType>();

    capacity_tests<ItemType>();

    modifiers_tests<ItemType>();

    compare_tests<ItemType>();

    std::cout << std::endl;
}

void run_tests() {
    run_test<Dummy>(true);

    std::cout << "DUMMY TESTS PASSED" << std::endl;

    run_test<int>(false);

    std::cout << "INT TESTS PASSED" << std::endl;

    run_test<char>(false);

    std::cout << "CHAR TESTS PASSED" << std::endl;

    run_test<double>(false);

    std::cout << "DOUBLE TESTS PASSED" << std::endl;

    run_test<float>(false);

    std::cout << "FLOAT TESTS PASSED" << std::endl;

    std::cout << std::endl;
}

int main() {
    run_tests();

    std::cout << "ALL TESTS (375) PASSED SUCCESSFULLY" << std::endl;

    return 0;
}
