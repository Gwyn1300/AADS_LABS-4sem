#include "index.h"

template<typename T>
index<T>::index() : _index(0), _pair(nullptr), _next(nullptr) {}

template<typename T>
index<T>::index(int i, const pair<T>& p) : _index(i), _pair(new pair<T>(p)), _next(nullptr) {}

template<typename T>
index<T>::index(const index<T>& other) : _index(other._index), _pair(nullptr), _next(nullptr) {
    if (other._pair != nullptr) {
        _pair = new pair<T>(*other._pair);
    }
    if (other._next != nullptr) {
        _next = new index<T>(*other._next);
    }
}

template<typename T>
index<T>::~index() {
    delete _pair;
    delete _next;
}