#pragma once
#include "Node.h"

    
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

    void del(Node* node, int value);

    void del(Node* node);
   
    void insert(Node*& node, int value);

    void print(Node* node)const;
    
    void copy(Node*& node, const Node* other);
    
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

    Node* getRoot()const;
};