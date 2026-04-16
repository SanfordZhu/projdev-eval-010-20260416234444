#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        T *data;
        node *prev;
        node *next;

        node() : data(nullptr), prev(nullptr), next(nullptr) {}

        explicit node(const T &val) : data(new T(val)), prev(nullptr), next(nullptr) {}

        ~node() {
            if (data != nullptr) {
                delete data;
                data = nullptr;
            }
        }
    };

    node *head;
    node *tail;
    size_t len;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        cur->prev = pos->prev;
        cur->next = pos;
        pos->prev->next = cur;
        pos->prev = cur;
        return cur;
    }

    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        list *lst;
        node *ptr;

    public:
        iterator() : lst(nullptr), ptr(nullptr) {}

        iterator(list *l, node *p) : lst(l), ptr(p) {}

        /**
         * iter++
         */
        iterator operator++(int) {
            if (ptr == nullptr || ptr == lst->tail) {
                throw invalid_iterator();
            }
            iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }

        /**
         * ++iter
         */
        iterator & operator++() {
            if (ptr == nullptr || ptr == lst->tail) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }

        /**
         * iter--
         */
        iterator operator--(int) {
            if (ptr == nullptr || ptr == lst->head->next) {
                throw invalid_iterator();
            }
            iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }

        /**
         * --iter
         */
        iterator & operator--() {
            if (ptr == nullptr || ptr == lst->head->next) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }

        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (ptr == nullptr || ptr == lst->head || ptr == lst->tail || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }

        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (ptr == nullptr || ptr == lst->head || ptr == lst->tail || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return ptr->data;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class list;
        friend class const_iterator;
    };

    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        const list *lst;
        node *ptr;

    public:
        const_iterator() : lst(nullptr), ptr(nullptr) {}

        const_iterator(const list *l, node *p) : lst(l), ptr(p) {}

        const_iterator(const iterator &other) : lst(other.lst), ptr(other.ptr) {}

        /**
         * iter++
         */
        const_iterator operator++(int) {
            if (ptr == nullptr || ptr == lst->tail) {
                throw invalid_iterator();
            }
            const_iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }

        /**
         * ++iter
         */
        const_iterator & operator++() {
            if (ptr == nullptr || ptr == lst->tail) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }

        /**
         * iter--
         */
        const_iterator operator--(int) {
            if (ptr == nullptr || ptr == lst->head->next) {
                throw invalid_iterator();
            }
            const_iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }

        /**
         * --iter
         */
        const_iterator & operator--() {
            if (ptr == nullptr || ptr == lst->head->next) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }

        const T & operator *() const {
            if (ptr == nullptr || ptr == lst->head || ptr == lst->tail || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }

        const T * operator ->() const {
            if (ptr == nullptr || ptr == lst->head || ptr == lst->tail || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return ptr->data;
        }

        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class list;
    };

    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        len = 0;
    }

    list(const list &other) {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        len = 0;

        node *p = other.head->next;
        while (p != other.tail) {
            push_back(*(p->data));
            p = p->next;
        }
    }

    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
        delete head;
        delete tail;
    }

    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) {
            return *this;
        }

        clear();
        node *p = other.head->next;
        while (p != other.tail) {
            push_back(*(p->data));
            p = p->next;
        }

        return *this;
    }

    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (empty()) {
            throw container_is_empty();
        }
        return *(head->next->data);
    }

    const T & back() const {
        if (empty()) {
            throw container_is_empty();
        }
        return *(tail->prev->data);
    }

    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(this, head->next);
    }

    const_iterator cbegin() const {
        return const_iterator(this, head->next);
    }

    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(this, tail);
    }

    const_iterator cend() const {
        return const_iterator(this, tail);
    }

    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return len == 0;
    }

    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return len;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        node *p = head->next;
        while (p != tail) {
            node *tmp = p;
            p = p->next;
            delete tmp;
        }
        head->next = tail;
        tail->prev = head;
        len = 0;
    }

    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.lst != this || pos.ptr == nullptr) {
            throw invalid_iterator();
        }

        node *new_node = new node(value);
        insert(pos.ptr, new_node);
        len++;

        return iterator(this, new_node);
    }

    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (empty()) {
            throw container_is_empty();
        }
        if (pos.lst != this || pos.ptr == nullptr || pos.ptr == head || pos.ptr == tail) {
            throw invalid_iterator();
        }

        node *next_node = pos.ptr->next;
        erase(pos.ptr);
        delete pos.ptr;
        len--;

        return iterator(this, next_node);
    }

    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        node *new_node = new node(value);
        insert(tail, new_node);
        len++;
    }

    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) {
            throw container_is_empty();
        }
        node *to_remove = tail->prev;
        erase(to_remove);
        delete to_remove;
        len--;
    }

    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        node *new_node = new node(value);
        insert(head->next, new_node);
        len++;
    }

    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) {
            throw container_is_empty();
        }
        node *to_remove = head->next;
        erase(to_remove);
        delete to_remove;
        len--;
    }

    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (len <= 1) {
            return;
        }

        T **arr = new T*[len];
        node *p = head->next;
        for (size_t i = 0; i < len; i++) {
            arr[i] = p->data;
            p = p->next;
        }

        sjtu::sort<T*>(arr, arr + len, [](const T *a, const T *b) {
            return *a < *b;
        });

        p = head->next;
        for (size_t i = 0; i < len; i++) {
            p->data = arr[i];
            p = p->next;
        }

        delete[] arr;
    }

    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other) {
            return;
        }

        node *p1 = head->next;
        node *p2 = other.head->next;

        while (p1 != tail && p2 != other.tail) {
            if (*(p1->data) < *(p2->data)) {
                p1 = p1->next;
            } else {
                node *tmp = p2;
                p2 = p2->next;
                other.erase(tmp);
                insert(p1, tmp);
            }
        }

        while (p2 != other.tail) {
            node *tmp = p2;
            p2 = p2->next;
            other.erase(tmp);
            insert(tail, tmp);
        }

        len += other.len;
        other.len = 0;
    }

    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        node *p = head;
        while (p != nullptr) {
            node *tmp = p->prev;
            p->prev = p->next;
            p->next = tmp;
            p = p->prev;
        }

        node *tmp = head;
        head = tail;
        tail = tmp;
    }

    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (len <= 1) {
            return;
        }

        node *p = head->next;
        while (p->next != tail) {
            if (*(p->data) == *(p->next->data)) {
                node *to_remove = p->next;
                erase(to_remove);
                delete to_remove;
                len--;
            } else {
                p = p->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
