#pragma once
#include "Node.h"
#include <vector>

    
class set{
    Node* root;
   
    bool search(Node*node, int value)const;
    
    int getHeight(Node* node)const;
    
    void updateHeight(Node* node);

    int getBalance(Node* node)const;

    void swap(Node*a, Node*b);

    void rightRotate(Node* node);
    
    void leftRotate(Node* node);

    void balance(Node* node);
    
    Node* getMin(Node* node);

    Node* getMax(Node* node);

    void del(Node*& node, int value);

    void del(Node*& node);
   
    void insert(Node*& node, int value);

    void print(Node* node)const;
    
    void copy(Node*& node, const Node* other);
    
    void toVector(Node* node, std::vector<int>& vec) const;
    
    public:

    set();

    set(const set&other);

    ~set();
    
    bool contains(int value)const;

    bool insert(int value);

    bool strictly_balance()const;

    bool erase(int key);

    void print();

    set& operator=(const set &other);

    std::vector<int> toVector() const;
    
};