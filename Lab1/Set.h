#include "Node.h"
#include <vector>
#include <iostream>
    
class set{
    Node* root;
    
    bool serch(Node*node, int value)const{
        if(node == nullptr) return false;
        if(node->_data == value){
            return true;
        }
        else if(value > node->_data&&node->_right!=nullptr){
            return serch(node->_right, value);
        }
        else if(value <node->_data&&node->_left!=nullptr){
            return serch(node->_left, value);
        }
        return false;
    };
    
    int getHeight(Node* node)const{
        return (node == nullptr)?-1:node->_hight;
    };
    
    void updateHieght(Node* node){
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

    int getBalance(Node* node)const{
        return (node == nullptr)?0:getHeight(node->_right) - getHeight(node->_left);
    };

    void swap(Node*a, Node*b){
        int a_data = a->_data;
        a->_data = b->_data;
        b->_data = a_data;
    }

    void rightRotate(Node* node){
        swap(node, node->_left);
        Node* buffer = node->_right;
        node->_right = node->_left;
        node->_left = node->_right->_left;
        node->_right->_left = node->_right->_right;
        node->_right->_right = buffer;
        updateHieght(node->_right);
        updateHieght(node);
    };
    
    void leftRotate(Node* node){
        swap(node, node->_right);
        Node* buffer = node->_left;
        node->_left = node->_right;
        node->_right = node->_left->_right;
        node->_left->_right = node->_left->_left;
        node->_left->_left = buffer;
        updateHieght(node->_left);
        updateHieght(node);
    }

    void balance(Node* node){
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
    
    Node* getMin(Node* node){
        if(node==nullptr) return nullptr;
        if(node->_left == nullptr) return node;
        return getMin(node->_left);
    }

    Node* getMax(Node* node){
        if(node==nullptr) return nullptr;
        if(node->_right == nullptr) return node;
        return getMax(node->_right);
    }

    void del(Node* node, int value){
        if(node == nullptr) return;
        else if(value <node->_data) del(node->_left,value);
        else if(value >node->_data) del(node->_right,value);
        else{
            if (node->_left == nullptr||node->_right == nullptr){
                Node* nodeDel = (node->_left==nullptr)?node->_right:node->_left;
                node->_data = nodeDel->_data;
                if(node->_left == nodeDel)node->_left = nullptr;
                else node->_right = nullptr;
                delete nodeDel;

            }
            else{
                Node*maxlnLeft = getMax(node->_left);
                node->_data = maxlnLeft->_data;
                del(node->_left,maxlnLeft->_data);
            }
        }

        if(node!=nullptr){
            updateHieght(node);
            balance(node);
        }
    }

    void del(Node* node){
        if(node == nullptr) return;
        if(node->_left != nullptr && node->_right != nullptr){
            del(node->_left);
            node->_left=nullptr;
            del(node->_right);
            node->_right =nullptr;
        }
        delete node;

    }
   
    void insert(Node*& node, int value){
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
        updateHieght(node);
        balance(node);
    }

    void print(Node* node)const{
        if(node == nullptr){
            return;
        }
        print(node->_left);
        std::cout<<node->_data<<' ';
        print(node->_right);
    }

    void copy(Node*& node, const Node* other){
        if(other == nullptr) return;
        node = new Node(other->_data);
        copy(node->_left, other->_left);
        copy(node->_right, other->_right);

    }
    
    public:

    set():root(nullptr){}

    set(const set&other):root(nullptr){
        copy(root, other.root);
    }

    ~set(){del(root);}
    
    bool contains(int value)const{
        return serch(root, value);
    }

    bool insert(int value){
        if(!serch(root, value)){
            insert(root, value);
            return true;
        }
        return false;
    }

    bool strictly_balance()const{
        int balance = getBalance(root);
        if(balance == -2||balance == 2) return false;
        return true;
    }

    bool erase(int key){
        if(serch(root, key)){
            del(root, key);
            return true;
        }
        return false;
    }

    void print(){
        print(root);
    }

    set& operator=(const set &other){
        del(root);
        copy(root, other.root);
        return *this;
    };

    Node* getRoot()const{
        return root;
    }
};