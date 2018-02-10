#include "vector.h"
#include "pair.h"

template <typename Key, typename Value>
class Map {
    protected:
        Vector<Key> *keys;
        Vector<Value> *vals;

    public:
        class Iterator {
            friend class Map;

            protected:
                typename Vector<Key>::Iterator key_it;
                typename Vector<Value>::Iterator val_it;

            public:
                Iterator();
                Iterator(typename Vector<Key>::Iterator key_it, typename Vector<Value>::Iterator val_it);
                Iterator(const Iterator &iterator);

                Iterator &operator=(const Iterator &iterator);

                Iterator operator++();
                Iterator operator++(int);

                Iterator operator--();
                Iterator operator--(int x);

                Pair<Key, Value> &operator*();
                Pair<Key, Value> *operator->();

                bool operator==(const Iterator &iterator) const;
                bool operator!=(const Iterator &iterator) const;
        };

        class ConstIterator {
            friend class Map;

            protected:
                typename Vector<Key>::ConstIterator key_it;
                typename Vector<Value>::ConstIterator val_it;

            public:
                ConstIterator();
                ConstIterator(typename Vector<Key>::ConstIterator key_it, typename Vector<Value>::ConstIterator val_it);
                ConstIterator(const ConstIterator &iterator);

                ConstIterator &operator=(const ConstIterator &iterator);

                ConstIterator operator++();
                ConstIterator operator++(int);

                ConstIterator operator--();
                ConstIterator operator--(int);

                const Pair<Key, Value> &operator*() const;
                const Pair<Key, Value> *operator->() const;

                bool operator==(const ConstIterator &iterator) const;
                bool operator!=(const ConstIterator &iterator) const;
        };

        Map();
        ~Map();

        Map& operator=(const Map &other);

        Value& at(const Key &key);
        const Value& at(const Key &key) const;

        Value& operator[](const Key &key);

        bool empty() const;
        unsigned int size() const;

        Iterator begin();
        Iterator end();

        ConstIterator cbegin() const;
        ConstIterator cend() const;

        void clear();

        Pair<Iterator, bool> insert(const Key &key, const Value &val);

        /*
        void erase(Iterator position);
        unsigned int erase(const Key &key);
        */

        ConstIterator find(const Key &key) const;
        Iterator find(const Key &key);

        unsigned int count(const Key &key) const;
        //Iterator erase(ConstIterator position);
//        void erase(Iterator position);

//        unsigned int count(const Key &key) const;
//        Iterator find(const Key &key);
};
