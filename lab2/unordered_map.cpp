#include "pair.cpp"
#include "TABLE.cpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

template<typename K, typename V>
class unordered_map{
    size_t _capacity;
    pair<K,V>** _table;
    size_t _elements;  

    unsigned char pearson_hash(const std::string& key) const {
        unsigned char h = 0;
        for (char c : key) {
            h = PEARSON_TABLE[h ^ static_cast<unsigned char>(c)];
        }
        return h;
    }
    
    size_t get_index(const std::string& key) const {
        if (_capacity == 0) return 0;
        return pearson_hash(key) % _capacity; 
    }

    size_t hash(int key) const {  
        if (_capacity == 0) return 0;  
        return key % _capacity;
    }

    void clear_chain(size_t index) {
        pair<K,V>* current = _table[index]; 
        while (current != nullptr) {
            pair<K,V>* to_delete = current;
            current = current->_next;
            delete to_delete;
        }
        _table[index] = nullptr;
    }

    pair<K,V>* find_pair(size_t index, const int& key) const {  
        pair<K,V>* current = _table[index];  
        while (current != nullptr) {
            if (current->_key == key) {  
                return current;
            }
            current = current->_next;
        }
        return nullptr;
    }
    
    pair<K,V>* find_pair(const std::string& key) const {
        size_t index = get_index(key);
        pair<K,V>* current = _table[index];
        while (current != nullptr) {
            if (current->_key == key) {
                return current;
            }
            current = current->_next;
        }
        return nullptr;
    }
    
    std::string random_string(int length) {
        const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::string result = "";
        for (int i = 0; i < length; i++) {
            result += chars[rand() % chars.length()];
        }
        return result;
    }

public:
    unordered_map() : _capacity(0), _table(nullptr), _elements(0) {}  

    unordered_map(size_t capacity) : _capacity(capacity), _elements(0) { 
        if (capacity == 0) {  
            _table = nullptr;
            return;
        }
        _table = new pair<K,V>*[capacity]();
        for (size_t i = 0; i < capacity; ++i) {
            _table[i] = nullptr;
        }
    }

    unordered_map(size_t capacity, size_t num_elements) : _capacity(capacity), _elements(0) {
        srand(static_cast<unsigned int>(time(nullptr)));
        
        if (capacity == 0) {
            _table = nullptr;
            return;
        }
        
        _table = new pair<K,V>*[capacity]();
        for (size_t i = 0; i < capacity; ++i) {
            _table[i] = nullptr;
        }
        
        for (size_t i = 0; i < num_elements; i++) {
            int str_length = 3 + rand() % 8;
            std::string random_key = random_string(str_length);
            
            V random_value = static_cast<V>(rand() % 1000 + 1);
            
            insert(random_key, random_value);
        }
    }

    unordered_map(const unordered_map<K,V>& other) : _capacity(other._capacity), _elements(0) {  
        _table = new pair<K,V>*[_capacity]();
        for (size_t i = 0; i < _capacity; ++i) {
            _table[i] = nullptr;
        }
        
        for (size_t i = 0; i < _capacity; ++i) {
            pair<K,V>* current = other._table[i];
            while (current != nullptr) {
                insert(current->_key, *current->_value);
                current = current->_next;
            }
        }
    }
    
    ~unordered_map() {
        clear();
        delete[] _table;
    }

    void clear() {
        if (_table == nullptr) return;
        for (size_t i = 0; i < _capacity; ++i) {
            clear_chain(i);
        }
        _elements = 0;
    }

    unordered_map<K,V>& operator=(const unordered_map<K,V>& other){  
        if (this != &other) {
            clear();
            delete[] _table;
            
            _capacity = other._capacity;
            _elements = 0;
            _table = new pair<K,V>*[_capacity]();
            for (size_t i = 0; i < _capacity; ++i) {
                _table[i] = nullptr;
            }
            
            for (size_t i = 0; i < _capacity; ++i) {
                pair<K,V>* current = other._table[i];
                while (current != nullptr) {
                    insert(current->_key, *current->_value);
                    current = current->_next;
                }
            }
        }
        return *this;
    }

    bool insert(int key, const V& value) {  
        size_t index = hash(key);  
        pair<K,V>* node = find_pair(index, key);
        
        if (node != nullptr) {
            *node->_value = value;  
        } else {
            pair<K,V>* new_pair = new pair<K,V>(key, value);
            new_pair->_next = _table[index];
            _table[index] = new_pair;
            _elements++;  
        }
        return true;  
    }

    bool insert(const std::string& key, const V& value) {
        if (_capacity == 0) return false;
        
        pair<K,V>* node = find_pair(key);
        
        if (node != nullptr) {
            *node->_value = value;
            return true;
        }
        
        size_t index = get_index(key);
        pair<K,V>* new_pair = new pair<K,V>(key, value);
        new_pair->_next = _table[index];
        _table[index] = new_pair;
        _elements++;
        return true;
    }

    void print() const {
        for (size_t i = 0; i < _capacity; ++i) {
            if (_table[i] != nullptr) {
                std::cout << "Bucket[" << i << "]: ";
                pair<K,V>* current = _table[i];
                while (current != nullptr) {
                    std::cout << "(" << current->_key << " -> " << *current->_value << ") ";
                    current = current->_next;
                }
                std::cout << std::endl;
            }
        }
    }

void insert_or_assign(int key, const V& value) {
    pair<K,V>* node = find_pair(hash(key), key);
    
    if (node != nullptr) {
        *node->_value = value;  
    } else {
        size_t index = hash(key);
        pair<K,V>* new_pair = new pair<K,V>(key, value);
        new_pair->_next = _table[index];
        _table[index] = new_pair;
        _elements++;
    }
}

    void insert_or_assign(const std::string& key, const V& value) {
        pair<K,V>* node = find_pair(key);
        
        if (node != nullptr) {
            *node->_value = value;
            return;
        }
        
        size_t index = get_index(key);
        pair<K,V>* new_pair = new pair<K,V>(key, value);
        new_pair->_next = _table[index];
        _table[index] = new_pair;
        _elements++;
    }

    bool contains(const V& value) const {  
        for (size_t i = 0; i < _capacity; ++i) {
            pair<K,V>* current = _table[i];
            while (current != nullptr) {
                if (*current->_value == value) {  
                    return true;
                }
                current = current->_next;
            }
        }
        return false;
    }

    V* search(int key) {  
        pair<K,V>* node = find_pair(hash(key), key);
        return node ? node->_value : nullptr;
    }
    V* search(const std::string& key) {
        pair<K,V>* node = find_pair(key);
        return node ? node->_value : nullptr;
    }

    bool erase(int key) {
        size_t index = hash(key);
        pair<K,V>* current = _table[index];
        pair<K,V>* prev = nullptr;

        while (current != nullptr) {
            if (current->_key == key) {
                if (prev != nullptr) {
                    prev->_next = current->_next;
                } else {
                    _table[index] = current->_next;
                }
                delete current;
                _elements--;  
                return true; 
            }
            prev = current;
            current = current->_next;
        }

        return false;
    }

    bool erase(const std::string& key) {
        size_t index = get_index(key);
        pair<K,V>* current = _table[index];
        pair<K,V>* prev = nullptr;

        while (current != nullptr) {
            if (current->_key == key) {
                if (prev != nullptr) {
                    prev->_next = current->_next;
                } else {
                    _table[index] = current->_next;
                }
                delete current;
                _elements--;
                return true;
            }
            prev = current;
            current = current->_next;
        }
        return false;
    }


    int count(int key) const {
        size_t index = hash(key);  
        int counter = 0;

        pair<K,V>* current = _table[index];
        while (current != nullptr) {
            counter++;
            current = current->_next;
        }

        return counter;
    }

    int count(const std::string& key) const {
        size_t index = get_index(key);
        int counter = 0;
        pair<K,V>* current = _table[index];
        while (current != nullptr) {
            counter++;
            current = current->_next;
        }
        return counter;
    }
    
    size_t size() const { return _elements; }  


};