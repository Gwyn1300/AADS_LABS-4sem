#pragma once
#include "pair.cpp"

template<typename T>
struct index{
    int _index;
    pair<T>* _pair;
    index<T>* _next;
    
    index();
    index(int i, const pair<T>& p);
    index(const index<T>& other);
    ~index();
};