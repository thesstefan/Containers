#pragma once

template <typename Type_1, typename Type_2>
class Pair {
    public:
        Type_1 first;
        Type_2 second;

        Pair();
        Pair(const Type_1 &first, const Type_2 &second);
        Pair(const Pair<Type_1, Type_2> &pair);

        bool operator==(const Pair<Type_1, Type_2> &pair);
};

template <typename Type_1, typename Type_2>
Pair<Type_1, Type_2>::Pair() {
    this->first = Type_1();
    this->second = Type_2();
}

template <typename Type_1, typename Type_2>
Pair<Type_1, Type_2>::Pair(const Type_1 &first, const Type_2 &second) {
    this->first = first;
    this->second = second;
}

template <typename Type_1, typename Type_2>
Pair<Type_1, Type_2>::Pair(const Pair<Type_1, Type_2> &pair) {
    this->first = pair.first;
    this->second = pair.second;
}

template <typename Type_1, typename Type_2>
bool Pair<Type_1, Type_2>::operator==(const Pair<Type_1, Type_2> &pair) {
    return this->first == pair.first && this->second == pair.second;
}
