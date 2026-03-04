#include "set.cpp"
#include <vector>

void inSet(const set& a, const Node* node, std::vector<int>& vec) {
    if (node == nullptr) {
        return;
    }
    
    if (node != nullptr) {
        if (a.contains(node->_data)) {
            vec.push_back(node->_data);
        }
        
        if (node->_left != nullptr) {
            inSet(a, node->_left, vec);
        }
        
        if (node->_right != nullptr) {
            inSet(a, node->_right, vec);
        }
    }
}

std::vector<int>intersection(const set& a, const set& b){
    std::vector<int>vec;
    inSet(a, b.getRoot(), vec);
    return vec;
}

void notInSet(const set& a, const Node* node, std::vector<int>& vec) {
    if (node == nullptr) {
        return;
    }
    
    if (node != nullptr) {
        if (!a.contains(node->_data)) {
            vec.push_back(node->_data);
        }
        
        if (node->_left != nullptr) {  
            notInSet(a, node->_left, vec);
        }
        
        if (node->_right != nullptr) {
            notInSet(a, node->_right, vec);
        }
    }
}


std::vector<int>unification(const set& a, const set& b){
    std::vector<int>res;
    notInSet(a,b.getRoot(),res);
    notInSet(b,a.getRoot(),res);
    inSet(a,b.getRoot(), res);
    return res;
}


int main(){
    std::cout<<"Program start\n";
    set s = set();
    s.insert(1);
    s.insert(2);
    s.insert(3);
    s.insert(4);
    s.insert(5);
    s.insert(6);
    s.print();
    std::cout<<"\n";
    set s2 = set();
    s2.insert(4);
    s2.insert(5);
    s2.insert(6);
    s2.insert(7);
    s2.insert(8);
    s2.insert(9);
    s2.insert(10);
    s2.print();
    std::cout<<"\n";
    std::vector vec = unification(s,s2);
    std::cout << vec.size()<<'\n';
    for (const auto& i : vec) {

        std::cout << i << " ";

    }
    return 0;
}
