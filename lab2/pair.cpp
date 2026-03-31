#pragma once
template<typename T>
struct pair{
    int _key;
    T* _value;
    pair<T>* _next;
    pair():_key(0), _value(nullptr), _next(nullptr){}

    pair(int key, T value): _key(key), _value(new T(value)), _next(nullptr){}
    
    pair(const pair<T>& other):_key(other._key), _value(new T(*other._value)), _next(nullptr){
        if (other._next != nullptr) {
            _next = new pair<T>(*other._next);
        }
    }

     ~pair() {
        delete _value;
        delete _next;
    }
};