#pragma once

// ptrdiff_t and size_t definitions.
#include <cstddef>

class input_iterator_tag {};
class output_iterator_tag {};
class forward_iterator_tag : public input_iterator_tag {};
class bidirectional_iterator_tag : public forward_iterator_tag {};
class random_access_iterator_tag : public bidirectional_iterator_tag {};

template <typename Category, typename Type, typename Distance = ptrdiff_t, typename Pointer = Type*, typename Reference = Type&>
class Iterator {
    typedef Category    iterator_category;
    typedef Type        value_type;
    typedef Distance    difference_type;
    typedef Pointer     pointer;
    typedef Reference   reference;

    Iterator(const Iterator&);
    ~Iterator();

    Iterator& operator=(const Iterator&);
    Iterator& operator++();

    reference operator*() const;
};

template <typename Iterator>
struct IteratorTraits {
    typedef typename Iterator::iterator_category        iterator_category;
    typedef typename Iterator::value_type               value_type;
    typedef typename Iterator::difference_type          difference_type;
    typedef typename Iterator::pointer                  pointer;
    typedef typename Iterator::reference                reference;
};

template <typename Type>
struct IteratorTraits<Type *> {
    typedef Type                        value_type;
    typedef ptrdiff_t                   difference_type;
    typedef Type*                       pointer;
    typedef Type&                       reference;
    typedef random_access_iterator_tag  iterator_category;
};

template <typename Type>
struct IteratorTraits<const Type *> {
    typedef Type                        value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const Type*                 pointer;
    typedef const Type&                 reference;
    typedef random_access_iterator_tag  iterator_category;
};

template <typename Type, typename Distance> 
class InputIterator {
    typedef input_iterator_tag          iterator_category;
    typedef Type                        value_type;
    typedef Distance                    difference_type;
    typedef Type*                       pointer;
    typedef Type&                       reference;
};

template <typename Type, typename Distance> 
class OuputIterator {
    typedef output_iterator_tag         iterator_category;
    typedef void                        value_type;
    typedef void                        difference_type;
    typedef void                        pointer;
    typedef void                        reference;
};

template <typename Type, typename Distance> 
class ForwardIterator {
    typedef forward_iterator_tag        iterator_category;
    typedef Type                        value_type;
    typedef Distance                    difference_type;
    typedef Type*                       pointer;
    typedef Type&                       reference;
};

template <typename Type, typename Distance> 
class BidirectionalIterator {
    typedef bidirectional_iterator_tag iterator_category;
    typedef Type                        value_type;
    typedef Distance                    difference_type;
    typedef Type*                       pointer;
    typedef Type&                       reference;
};

template <typename Type, typename Distance> 
class RandomAccessIterator {
    typedef random_access_iterator_tag  iterator_category;
    typedef Type                        value_type;
    typedef Distance                    difference_type;
    typedef Type*                       pointer;
    typedef Type&                       reference;
};

template <typename Iterator, typename Container>
class NormalIterator {
    protected:
        typedef IteratorTraits<Iterator>                traits_type;

        Iterator current;

    public:
        typedef Iterator                                iterator_type;
        typedef typename traits_type::iterator_category iterator_category;
        typedef typename traits_type::value_type        value_type;
        typedef typename traits_type::difference_type   difference_type;
        typedef typename traits_type::reference         reference;
        typedef typename traits_type::pointer           pointer;

        typedef NormalIterator Self;

        constexpr NormalIterator() {
            this->current(Iterator());
        }

        NormalIterator(const Iterator& iterator) {
            this->current = iterator;
        }

        reference operator*() const {
            return *this->current;
        }

        pointer operator->() const {
            return this->current;
        }

        Self& operator++() {
            ++this->current;

            return *this;
        }

        Self operator++(int) {
            return Self(this->current++);
        }

        Self& operator--() {
            --this->current;

            return *this;
        }

        Self operator--(int) {
            return Self(this->current--);
        }

        reference operator[](difference_type offset) const {
            return this->current[offset];
        }

        Self& operator+=(difference_type x) {
            this->current += x;

            return *this;
        }

        Self operator+(difference_type x) const {
            return Self(this->current + x);
        }

        Self& operator-=(difference_type x) {
            this->current -= x;

            return *this;
        }

        Self operator-(difference_type x) const {
            return Self(this->current - x);
        }

        const Iterator& base() const {
            return this->current;
        }

        template <typename IteratorLeft, typename IteratorRight>
        bool operator==(const NormalIterator<IteratorLeft, Container> &left,
                        const NormalIterator<IteratorRight, Container> &right) {
            return left.base() == right.base();
        }

        bool operator==(const Self &left,
                        const Self &right) {
            return left.base() == right.base();
        }

        template <typename IteratorLeft, typename IteratorRight>
        bool operator!=(const NormalIterator<IteratorLeft, Container> &left,
                        const NormalIterator<IteratorRight, Container> &right) {
            return left.base() != right.base();
        }

        bool operator!=(const Self &left,
                        const Self &right) {
            return left.base() != right.base();
        } 

        template <typename IteratorLeft, typename IteratorRight>
        bool operator<(const NormalIterator<IteratorLeft, Container> &left,
                        const NormalIterator<IteratorRight, Container> &right) {
            return left.base() < right.base();
        }

        bool operator<(const Self &left,
                        const Self &right) {
            return left.base() < right.base();
        }

        template <typename IteratorLeft, typename IteratorRight>
        bool operator>(const NormalIterator<IteratorLeft, Container> &left,
                        const NormalIterator<IteratorRight, Container> &right) {
            return left.base() > right.base();
        }

        bool operator>(const Self &left,
                        const Self &right) {
            return left.base() > right.base();
        }

        template <typename IteratorLeft, typename IteratorRight>
        bool operator<=(const NormalIterator<IteratorLeft, Container> &left,
                        const NormalIterator<IteratorRight, Container> &right) {
            return left.base() <= right.base();
        }

        bool operator<=(const Self &left,
                        const Self &right) {
            return left.base() <= right.base();
        }

        template <typename IteratorLeft, typename IteratorRight>
        bool operator>=(const NormalIterator<IteratorLeft, Container> &left,
                        const NormalIterator<IteratorRight, Container> &right) {
            return left.base() >= right.base();
        }

        bool operator>=(const Self &left,
                        const Self &right) {
            return left.base() >= right.base();
        }
};
