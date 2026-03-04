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


int main(){
    std::cout<<"Program start\n";
    set s = set();
    s.insert(7);
    s.insert(1451);
    s.insert(785);
    s.insert(0);
    s.insert(90);
    s.insert(1221);
    s.print();
    std::cout<<"\n";
    set s2 = set();
    s2.insert(99);
    s2.insert(99);
    s2.insert(43);
    s2.insert(7435);
    s2.insert(99);
    s2.insert(738465);
    s2.insert(99);
    s2.print();
    std::cout<<"\n";
    s2 = s;
    s2.print();
    std::vector vec = intersection(s,s2);
    std::cout << "\nhjgjh\n";
    std::cout << vec.size()<<'\n';
    for (const auto& i : vec) {

        std::cout << i << " ";

    }
    return 0;
}
