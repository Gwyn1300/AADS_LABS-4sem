#include "Node.h"
#include "set.h"
#include <iostream>
#include <vector>
    
bool set::search(Node*node, int value)const{
    if(node == nullptr) return false;
    if(node->_data == value){
        return true;
    }
    else if(value > node->_data&&node->_right!=nullptr){
        return search(node->_right, value);
    }
    else if(value <node->_data&&node->_left!=nullptr){
        return search(node->_left, value);
    }
    return false;
};
    
int set::getHeight(Node* node)const{
    return (node == nullptr)?-1:node->_hight;
};
    
void set::updateHeight(Node* node){
    if(node->_left==nullptr||node->_right==nullptr){
        if(node->_left!=nullptr) node->_hight = node->_left->_hight+1;
        else if(node->_right!=nullptr)node->_hight = node->_right->_hight+1;
        else node->_hight = 0;
    }
    else{
        if(node->_left->_hight > node->_right->_hight){
            node->_hight = node->_left->_hight + 1;
        }
        else{
            node->_hight = node->_right->_hight + 1;
        }
    }
};

int set::getBalance(Node* node)const{
    return (node == nullptr)?0:getHeight(node->_right) - getHeight(node->_left);
};

void set::swap(Node*a, Node*b){
    int a_data = a->_data;
    a->_data = b->_data;
    b->_data = a_data;
}

void set::rightRotate(Node* node){
    swap(node, node->_left);
    Node* buffer = node->_right;
    node->_right = node->_left;
    node->_left = node->_right->_left;
    node->_right->_left = node->_right->_right;
    node->_right->_right = buffer;
    updateHeight(node->_right);
    updateHeight(node);
};
    
void set::leftRotate(Node* node){
    swap(node, node->_right);
    Node* buffer = node->_left;
    node->_left = node->_right;
    node->_right = node->_left->_right;
    node->_left->_right = node->_left->_left;
    node->_left->_left = buffer;
    updateHeight(node->_left);
    updateHeight(node);
}

void set::balance(Node* node){
    int balance = getBalance(node);
    if (balance== -2){
        if(getBalance(node->_left) == 1){leftRotate(node->_left);}
        rightRotate(node);
    }
    else if(balance == 2){
        if(getBalance(node->_right) == -1){rightRotate(node->_right);}
        leftRotate(node);
    }
}
    
Node* set::getMin(Node* node){
    if(node==nullptr) return nullptr;
    if(node->_left == nullptr) return node;
    return getMin(node->_left);
}

Node* set::getMax(Node* node){
    if(node==nullptr) return nullptr;
    if(node->_right == nullptr) return node;
    return getMax(node->_right);
}

void set::del(Node*& node, int value){
    if(node == nullptr) return;
    else if(value <node->_data) del(node->_left,value);
    else if(value >node->_data) del(node->_right,value);
        
    else{
        if (node->_left == nullptr||node->_right == nullptr){
            if (node->_left == nullptr && node->_right == nullptr) {
                delete node;      
                node = nullptr;

            }
            else if (node->_left == nullptr) {                
                Node* temp = node;        
                node = node->_right;      
                delete temp;              
            }
        
            
            else if (node->_right == nullptr) {
                Node* temp = node;        
                node = node->_left;       
                delete temp;              
            }
            
        }
        else{
            Node*maxlnLeft = getMax(node->_left);
            node->_data = maxlnLeft->_data;
            del(node->_left,maxlnLeft->_data);
        }
    }

    if(node!=nullptr){
        updateHeight(node);
        balance(node);
    }
}

void set::del(Node*& node){
    if(node == nullptr) return;
    if(node->_left != nullptr && node->_right != nullptr){
        del(node->_left);
        node->_left=nullptr;
        del(node->_right);
        node->_right =nullptr;
    }
    delete node;
}
   
void set::insert(Node*& node, int value){
    if(node == nullptr) {
        node = new Node(value);
    }
    else if(value<node->_data){
        if(node->_left==nullptr){node->_left = new Node(value);}
        else insert(node->_left, value);
    }
    else if(value>=node->_data){
        if(node->_right==nullptr){node->_right = new Node(value);}
        else insert(node->_right, value);
    }
    updateHeight(node);
    balance(node);
}

void set::print(Node* node)const{
    if(node == nullptr){
        return;
    }
    print(node->_left);
    std::cout<<node->_data<<' ';
    print(node->_right);
}

void set::copy(Node*& node, const Node* other){
    if(other == nullptr) return;
    node = new Node(other->_data);
    copy(node->_left, other->_left);
    copy(node->_right, other->_right);
}
    
set::set():root(nullptr){}

set::set(const set&other):root(nullptr){
    copy(root, other.root);
}

set::~set(){del(root);}
    
bool set::contains(int value)const{
    return search(root, value);
}

bool set::insert(int value){
    if(!search(root, value)){
        insert(root, value);
        return true;
    }
    return false;
}

bool set::strictly_balance()const{
    int balance = getBalance(root);
    if(balance == -2||balance == 2) return false;
    return true;
}

bool set::erase(int key){
    if(search(root, key)){
        del(root, key);
        return true;
    }
    return false;
}

void set::print(){
    print(root);
    std::cout<<'\n';
}

set& set::operator=(const set &other){
    del(root);
    copy(root, other.root);
    return *this;
};

void set::toVector(Node* node, std::vector<int>& vec) const {
    if (node == nullptr) return;
    toVector(node->_left, vec);
    vec.push_back(node->_data);
    toVector(node->_right, vec);
}

std::vector<int> set::toVector() const {
    std::vector<int> result;
    toVector(root, result);
    return result;
}
