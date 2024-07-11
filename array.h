#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h> //size_t,ptrdiff_t
#include <initializer_list>
#include <stdexcept> //std::out_of_range,std::range_error

///array: a class acted like std::vector but with fixed capacity.
template <class T, size_t N>
class array
{
public:
    //member type
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

private:
    value_type a[N]; //data storage
    size_type n; //size

public:
    ///construct/destruct
    array() : a(), n(0) {}
    array(size_type size) : array()
    {
        if(size > N) throw std::out_of_range("array(size_type)");
        else n = size;
    }
    array(size_type size, const value_type &value) : array(size)
    {
        for(size_type i = 0; i < n; ++i)
            a[i] = value;
    }
    array(const array &A) : array()
    {
        for(; n < A.n; ++n)
            a[n] = A.a[n];
    }
    template<typename InputIterator, typename = std::_RequireInputIter<InputIterator>>
    array(InputIterator first, InputIterator last) : array()
    {
        for(; first < last && n < N; ++first, ++n)
            a[n] = *first;
    }
    array(std::initializer_list<value_type> l): array()
    {
        for(auto it = l.begin(); it < l.end() && n < N; ++it, ++n)
            a[n] = *it;
    }
    array &operator=(const array &A)
    {
        if(&A == this) return *this;
        for(n = 0; n < A.n; ++n)
            a[n] = A.a[n];
        return *this;
    }
    array &operator=(std::initializer_list<value_type> l)
    {
        n = 0;
        for(auto it = l.begin(); it < l.end() && n < N; ++it, ++n)
            a[n] = *it;
        return *this;
    }
    ~array() { n = 0; }

    ///iterator
    iterator begin() { return a; }
    const_iterator begin() const { return a; }
    const_iterator cbegin() const { return a; }
    iterator end() { return a + n; }
    const_iterator end() const { return a + n; }
    const_iterator cend() const { return a + n; }

    ///capacity
    size_type size() const { return n; }
    size_type max_size() const { return N; }
    size_type capacity() const { return N; }
    bool empty() const { return !n; }
    bool full() const { return n == N; }
    void resize(size_type size)
    {
        if(size > N) throw std::out_of_range("array::resize(size_type)");
        if(size > n) n = size;
    }
    void resize(size_type size, const value_type &value)
    {
        if(size > N) throw std::out_of_range("void array::resize(size_type, const value_type &)");
        for(; n < size; ++n)
            a[n] = value;
    }

    ///element
    reference operator[](size_type index) { return a[index]; }
    const_reference operator[](size_type index) const { return a[index]; }
    reference at(size_type index)
    {
        if(index >= n) throw std::out_of_range("reference array::at(size_type)");
        return a[index];
    }
    const_reference at(size_type index) const
    {
        if(index >= n) throw std::out_of_range("const_reference array::at(size_type) const");
        return a[index];
    }
    reference front() { return a[0]; }
    const_reference front() const { return a[0]; }
    reference back() { return a[n - 1]; }
    const_reference back() const { return a[n - 1]; }
    pointer data() { return a; }
    const_pointer data() const { return a; }

    ///modifier
    void assign(size_type size, const value_type &value)
    {
        if(size > N) throw std::out_of_range("void array::assign(size_type, const value_type &)");
        for(n = 0; n < size; ++n)
            a[n] = value;
    }
    template<typename InputIterator, typename = std::_RequireInputIter<InputIterator>>
    void assign(InputIterator first, InputIterator last)
    {
        for(n = 0; first < last && n < N; ++first, ++n)
            if(n >= N) throw std::out_of_range("void array::assign(InputIterator, InputIterator)");
            else a[n] = *first;
    }
    void assign(std::initializer_list<value_type> l)
    {
        n = 0;
        for(auto it = l.begin(); it < l.end(); ++it, ++n)
            if(n >= N) throw std::out_of_range("void array::assign(initializer_list)");
            else a[n] = *it;
    }
    void push_back(const value_type &value)
    {
        if(full()) throw std::range_error("void push_back(const value_type &)");
        a[n++] = value;
    }
    void pop_back()
    {
        if(empty()) return;
        --n;
    }
    iterator insert(const_iterator position, const value_type &value)
    {
        if(position > end() || position < begin())
            throw std::out_of_range("iterator insert(const_iterator, const value_type &)");
        if(full())
            throw std::range_error("iterator insert(const_iterator, const value_type &)");
        iterator it;
        for(it = end(); it > position; --it)
            *it = *(it - 1);
        *it = value;
        ++n;
        return it;
    }
    iterator erase(const_iterator position)
    {
        if(position >= end() || position < begin())
            throw std::out_of_range("iterator erase(const_iterator)");
        --n;
        iterator it;
        for(it = (iterator)position; it < end(); ++it)
            *it = *(it + 1);
        return (iterator)position;
    }
    void clear() { n = 0; }
    template <class...Args>
    iterator emplace(const_iterator position, Args &&...args)
    {
        return insert(position, value_type(args...));
    }
    template <class...Args>
    void emplace_back(Args &&...args)
    {
        push_back(value_type(args...));
    }

    ///compare
    template <class _T, size_t _N>
    friend bool operator!=(const array<_T, _N> &a1, const array<_T, _N> &a2);
    template <class _T, size_t _N>
    friend bool operator==(const array<_T, _N> &a1, const array<_T, _N> &a2);
    template <class _T, size_t _N>
    friend bool operator<(const array<_T, _N> &a1, const array<_T, _N> &a2);
    template <class _T, size_t _N>
    friend bool operator>(const array<_T, _N> &a1, const array<_T, _N> &a2);
    template <class _T, size_t _N>
    friend bool operator<=(const array<_T, _N> &a1, const array<_T, _N> &a2);
    template <class _T, size_t _N>
    friend bool operator>=(const array<_T, _N> &a1, const array<_T, _N> &a2);
};
///to avoid redefinition, I have to define the friend outside the class
template <class _T, size_t _N>
bool operator!=(const array<_T, _N> &a1, const array<_T, _N> &a2)
{
    if(a1.n != a2.n) return true;
    for(size_t i = 0; i < a1.n; ++i)
        if(a1.a[i] != a2.a[i]) return true;
    return false;
}
template <class _T, size_t _N>
bool operator==(const array<_T, _N> &a1, const array<_T, _N> &a2)
{
    return !(a1 != a2);
}
template <class _T, size_t _N>
bool operator<(const array<_T, _N> &a1, const array<_T, _N> &a2)
{
    for(size_t i = 0; i < a1.n && i < a2.n; ++i)
    {
        if(a1.a[i] < a2.a[i]) return true;
        else if(a2.a[i] < a1.a[i]) return false;
    }
    return a1.n < a2.n;
}
template <class _T, size_t _N>
bool operator>(const array<_T, _N> &a1, const array<_T, _N> &a2)
{
    return a2 < a1;
}
template <class _T, size_t _N>
bool operator<=(const array<_T, _N> &a1, const array<_T, _N> &a2)
{
    return !(a2 < a1);
}
template <class _T, size_t _N>
bool operator>=(const array<_T, _N> &a1, const array<_T, _N> &a2)
{
    return !(a1 < a2);
}

//the only difference is that class array<char, N> can also be constructed by const char*.
template <size_t N>
class array<char, N>
{
public:
    //member type
    typedef char value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

private:
    value_type a[N]; //data storage
    size_type n; //size

public:
    ///construct/destruct
    array() : a(), n(0) {}
    array(size_type size) : array()
    {
        if(size > N) throw std::out_of_range("array(size_type)");
        else n = size;
    }
    array(size_type size, const value_type &value) : array(size)
    {
        for(size_type i = 0; i < n; ++i)
            a[i] = value;
    }
    array(const array &A) : array()
    {
        for(; n < A.n; ++n)
            a[n] = A.a[n];
    }
    template<typename InputIterator, typename = std::_RequireInputIter<InputIterator>>
    array(InputIterator first, InputIterator last) : array()
    {
        for(; first < last && n < N; ++first, ++n)
            a[n] = *first;
    }
    array(std::initializer_list<value_type> l): array()
    {
        for(auto it = l.begin(); it < l.end() && n < N; ++it, ++n)
            a[n] = *it;
    }
    array(const char *A): array()
    {
        for(; n < N && A[n]; ++n)
            a[n] = A[n];
        if(n < N) a[n++] = 0;
        else throw std::out_of_range("array(const char *)");
    }
    array &operator=(const char *A)
    {
        for(n = 0; n < N && A[n]; ++n)
            a[n] = A[n];
        if(n < N) a[n++] = 0;
        else throw std::out_of_range("array &operator=(const char *)");
        return *this;
    }
    array &operator=(const array &A)
    {
        if(&A == this) return *this;
        for(n = 0; n < A.n; ++n)
            a[n] = A.a[n];
        return *this;
    }
    array &operator=(std::initializer_list<value_type> l)
    {
        n = 0;
        for(auto it = l.begin(); it < l.end() && n < N; ++it, ++n)
            a[n] = *it;
        return *this;
    }
    ~array() { n = 0; }

    ///iterator
    iterator begin() { return a; }
    const_iterator begin() const { return a; }
    const_iterator cbegin() const { return a; }
    iterator end() { return a + n; }
    const_iterator end() const { return a + n; }
    const_iterator cend() const { return a + n; }

    ///capacity
    size_type size() const { return n; }
    size_type max_size() const { return N; }
    size_type capacity() const { return N; }
    bool empty() const { return !n; }
    bool full() const { return n == N; }
    void resize(size_type size)
    {
        if(size > N) throw std::out_of_range("array::resize(size_type)");
        if(size > n) n = size;
    }
    void resize(size_type size, const value_type &value)
    {
        if(size > N) throw std::out_of_range("void array::resize(size_type, const value_type &)");
        for(; n < size; ++n)
            a[n] = value;
    }

    ///element
    reference operator[](size_type index) { return a[index]; }
    const_reference operator[](size_type index) const { return a[index]; }
    reference at(size_type index)
    {
        if(index >= n) throw std::out_of_range("reference array::at(size_type)");
        return a[index];
    }
    const_reference at(size_type index) const
    {
        if(index >= n) throw std::out_of_range("const_reference array::at(size_type) const");
        return a[index];
    }
    reference front() { return a[0]; }
    const_reference front() const { return a[0]; }
    reference back() { return a[n - 1]; }
    const_reference back() const { return a[n - 1]; }
    pointer data() { return a; }
    const_pointer data() const { return a; }

    ///modifier
    void assign(size_type size, const value_type &value)
    {
        if(size > N) throw std::out_of_range("void array::assign(size_type, const value_type &)");
        for(n = 0; n < size; ++n)
            a[n] = value;
    }
    template<typename InputIterator, typename = std::_RequireInputIter<InputIterator>>
    void assign(InputIterator first, InputIterator last)
    {
        for(n = 0; first < last && n < N; ++first, ++n)
            if(n >= N) throw std::out_of_range("void array::assign(InputIterator, InputIterator)");
            else a[n] = *first;
    }
    void assign(std::initializer_list<value_type> l)
    {
        n = 0;
        for(auto it = l.begin(); it < l.end(); ++it, ++n)
            if(n >= N) throw std::out_of_range("void array::assign(initializer_list)");
            else a[n] = *it;
    }
    void push_back(const value_type &value)
    {
        if(full()) throw std::range_error("void push_back(const value_type &)");
        a[n++] = value;
    }
    void pop_back()
    {
        if(empty()) return;
        --n;
    }
    iterator insert(const_iterator position, const value_type &value)
    {
        if(position > end() || position < begin())
            throw std::out_of_range("iterator insert(const_iterator, const value_type &)");
        if(full())
            throw std::range_error("iterator insert(const_iterator, const value_type &)");
        iterator it;
        for(it = end(); it > position; --it)
            *it = *(it - 1);
        *it = value;
        ++n;
        return it;
    }
    iterator erase(const_iterator position)
    {
        if(position >= end() || position < begin())
            throw std::out_of_range("iterator erase(const_iterator)");
        --n;
        iterator it;
        for(it = (iterator)position; it < end(); ++it)
            *it = *(it + 1);
        return (iterator)position;
    }
    void clear() { n = 0; }
    template <class...Args>
    iterator emplace(const_iterator position, Args &&...args)
    {
        return insert(position, value_type(args...));
    }
    template <class...Args>
    void emplace_back(Args &&...args)
    {
        push_back(value_type(args...));
    }

    ///compare
    template <size_t _N>
    friend bool operator!=(const array<char, _N> &a1, const array<char, _N> &a2);
    template <size_t _N>
    friend bool operator==(const array<char, _N> &a1, const array<char, _N> &a2);
    template <size_t _N>
    friend bool operator<(const array<char, _N> &a1, const array<char, _N> &a2);
    template <size_t _N>
    friend bool operator>(const array<char, _N> &a1, const array<char, _N> &a2);
    template <size_t _N>
    friend bool operator<=(const array<char, _N> &a1, const array<char, _N> &a2);
    template <size_t _N>
    friend bool operator>=(const array<char, _N> &a1, const array<char, _N> &a2);
};

template <size_t _N>
bool operator!=(const array<char, _N> &a1, const array<char, _N> &a2)
{
    if(a1.n != a2.n) return true;
    for(size_t i = 0; i < a1.n; ++i)
        if(a1.a[i] != a2.a[i]) return true;
    return false;
}
template <size_t _N>
bool operator==(const array<char, _N> &a1, const array<char, _N> &a2)
{
    return !(a1 != a2);
}
template <size_t _N>
bool operator<(const array<char, _N> &a1, const array<char, _N> &a2)
{
    for(size_t i = 0; i < a1.n && i < a2.n; ++i)
    {
        if(a1.a[i] < a2.a[i]) return true;
        else if(a2.a[i] < a1.a[i]) return false;
    }
    return a1.n < a2.n;
}
template <size_t _N>
bool operator>(const array<char, _N> &a1, const array<char, _N> &a2)
{
    return a2 < a1;
}
template <size_t _N>
bool operator<=(const array<char, _N> &a1, const array<char, _N> &a2)
{
    return !(a2 < a1);
}
template <size_t _N>
bool operator>=(const array<char, _N> &a1, const array<char, _N> &a2)
{
    return !(a1 < a2);
}

#endif