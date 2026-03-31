#include "index.h"
#include <iostream>

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

    unordered_map<T>& operator=(const unordered_map<T>& other){
        if (this == &other) {
        return *this;
        }
        index<T>* newIndices = nullptr;
            if (other._indices != nullptr) {
        newIndices = new index<T>(*other._indices);
        }

        _size = other._size;
        delete _indices;
        
        _indices = newIndices;

        return *this;
    }


    void print() const {
        if (_indices == nullptr) {
            std::cout << "Empty unordered_map" << std::endl;
            return;
        }

        index<T>* current = _indices;
        size_t count = 0;

        while (current != nullptr) {
            if (current->_pair != nullptr && current->_pair->_value != nullptr) {
                std::cout << '[' << count << "] (" 
                          << current->_pair->_key << ':' 
                          << *(current->_pair->_value) << ") ";
            } else if (current->_pair != nullptr) {
                std::cout << '[' << count << "] (" 
                          << current->_pair->_key << ":nullptr) ";
            } else {
                std::cout << '[' << count << "] (nullptr) ";
            }

            current = current->_next;
            count++;
        }
        std::cout << std::endl;
    }
    
};