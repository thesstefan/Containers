#include <iostream>
#include <cassert>

#include "vector.h"

template <typename ItemType>
void default_constructor_test() {
    std::cout << "Default constructor -> ";

    Vector<ItemType> vec;

    assert(vec.size() == 0);

    std::cout << "SUCCESS" << std::endl;
}

template <typename ItemType>
void run_constructor_tests() {
    default_constructor_test<ItemType>();
}

int main() {
    run_constructor_tests<int>();

    return 0;
}
