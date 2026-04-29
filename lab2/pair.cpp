#pragma once
#include <string>

template<typename K, typename V>
struct pair{
    K _key;
    V* _value;
    pair<K,V>* _next;
    
    pair() : _key(K()), _value(nullptr), _next(nullptr) {}

    pair(const K& key, const V& value) : _key(key), _value(new V(value)), _next(nullptr) {}
    
    pair(const pair<K,V>& other) : _key(other._key), _value(new V(*other._value)), _next(nullptr) {
        if (other._next != nullptr) {
            _next = new pair<K,V>(*other._next);
        }
    }    

    ~pair() {
        delete _value;
        delete _next;
    }
};