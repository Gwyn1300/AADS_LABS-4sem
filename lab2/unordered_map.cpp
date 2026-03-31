#include "index.h"

template<typename T>
class unordered_map{
    size_t _size;
    index<T>* _indices;

public:
    unordered_map() : _size(0), _indices(nullptr) {}

    unordered_map(size_t count) : _size(count), _indices(nullptr) {
        if (count == 0) return;
        
        _indices = new index<T>(0, pair<T>());
        index<T>* current = _indices;
        for (size_t i = 1; i < count; i++) {
            current->_next = new index<T>(static_cast<int>(i), pair<T>());
            current = current->_next;
        }
    }

    unordered_map(const unordered_map<T>& other) : _size(other._size), _indices(nullptr) {
        if (other._indices != nullptr) {
            _indices = new index<T>(*other._indices);
        }
    }
    
    ~unordered_map() {
        index<T>* current = _indices;
        while (current != nullptr) {
            index<T>* next = current->_next;
            delete current;
            current = next;
        }
    }
};