#include <vector>

template <typename T>

class set{
    struct Node{
    T _data;
    Node* _left;
    Node* _right;
    Node(): _data(T(0)), _left(nullptr), _right(nullptr){};
    Node(T data, Node* left, Node* right): _data(data), _left(left), _right(right){};
    ~Node() = default;
    };

    void clear(Node*root){
        if (root._left == nullptr&&root._right == nullptr){
            delete root;
        }
        clear(root._left);
        clear(root._right);
    };

    Node* _root;

    public:
    set():root(nullptr){};
    
    set(std::vector<T>){

    }

    ~set(){clear(_root);}

    set* operator(const set* other){

    }
}
