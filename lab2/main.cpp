// main.cpp
#include "unordered_map.cpp"
#include <iostream>
#include <string>

unsigned char pearson_hash_string(const std::string& str) {
    unsigned char h = 0;
    for (char c : str) {
        h = PEARSON_TABLE[h ^ static_cast<unsigned char>(c)];
    }
    return h;
}


bool compare_string_hashes(const std::string& str1, const std::string& str2) {
    return pearson_hash_string(str1) == pearson_hash_string(str2);
}

int main() {   
    unordered_map<std::string, int> map(20, 10);
    
    std::cout << "=== Hash Table ===" << std::endl;
    map.print();
    
    std::cout << "\nSearch: ";
    std::string key = "abc";
    int* val = map.search(key);
    if (val) std::cout << key << " -> " << *val << std::endl;
    else std::cout << key << " not found" << std::endl;
    
    map.insert("test", 999);
    map.insert_or_assign("test", 888);
    std::cout << "Inserted: test -> " << *map.search("test") << std::endl;
    
    map.erase("test");
    std::cout << "Erased: test" << std::endl;
    
    std::cout << "Contains value 999? " << (map.contains(999) ? "yes" : "no") << std::endl;
    
    std::cout << "Size: " << map.size() << std::endl;

    
    std::string input1, input2;
    
    std::cout << "Enter first string: ";
    std::getline(std::cin, input1);
    
    std::cout << "Enter second string: ";
    std::getline(std::cin, input2);
    
    bool result = compare_string_hashes(input1, input2);
    
    std::cout << "\nHash of \"" << input1 << "\": " << (int)pearson_hash_string(input1) << std::endl;
    std::cout << "Hash of \"" << input2 << "\": " << (int)pearson_hash_string(input2) << std::endl;
    std::cout << "Hashes are equal: " << (result ? "true" : "false") << std::endl;
    
    return 0;
}