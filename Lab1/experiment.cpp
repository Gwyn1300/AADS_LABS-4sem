#include"experiment.h"
#include <chrono>
#include <iomanip>
#include<iostream>
#include <fstream>
#include <string>

size_t lcg(){ 
    static size_t x = 0; 
    x = (1021*x+24631) % 116640; 
    return x; 
};
 
void setRandomNumberFilling(set& s, size_t count){
    for(size_t i = 0; i<count; i++ ){
        s.insert(lcg());
    }
}

bool contains(const std::vector<int>& vec, int value) {
    for (int element : vec) {
        if (element == value) {
            return true;
        }
    }
    return false;
}

void removeAll(std::vector<int>& vec, int el) {
    for (auto it = vec.begin(); it != vec.end(); ) {
        if (*it == el) {
            it = vec.erase(it); 
        } else {
            ++it;
        }
    }
}

double setMeasureTimeFilling(size_t count){
    auto start = std::chrono::high_resolution_clock::now();
    set s;
    setRandomNumberFilling(s,count);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1000000.0; 
}

double setMeasureTimeSearch(size_t numberElem){
    int el = lcg();
    set s;
    setRandomNumberFilling(s,numberElem);
    auto start = std::chrono::high_resolution_clock::now();
    s.contains(el);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1000000.0; 
}

double setMeasureTimeAddRem(size_t numberElem){
    int el = lcg();
    set s;
    setRandomNumberFilling(s,numberElem);
    auto start = std::chrono::high_resolution_clock::now();
    s.insert(el);
    s.erase(el);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1000000.0; 
}

void printTime(std::vector<std::vector<std::vector<double>>> time){
    std::cout<<"Set:\n";
    for(int i = 0; i <3; ++i){
        std::cout << std::fixed << std::setprecision(12)<< int(time[0][i][0])<<" elements:\n\t" <<"Average filling time: " << time[0][i][1] << " seconds\n\t" 
                                                    <<"Average search time: " << time[0][i][2] << " seconds\n\t"
                                                    <<"Average time to add/remove: " << time[0][i][3] << " seconds\n\n";

    }
    
    std::cout<<"Vector:\n";
    for(int i = 0; i <3; ++i){
        std::cout << std::fixed << std::setprecision(12)<< int(time[1][i][0])<<" elements:\n\t" <<"Average filling time: " << time[1][i][1] << " seconds\n\t" 
                                                    <<"Average search time: " << time[1][i][2] << " seconds\n\t"
                                                    <<"Average time to add/remove: " << time[1][i][3] << " seconds\n\n";
    }
}

std::vector<double> setMaverageTime( size_t numberElem){
    std::vector<double> time = {double(numberElem), 0,0,0};
    for (size_t i = 0; i<100; ++i){
        time[1]+=setMeasureTimeFilling(numberElem);
    }
    time[1]/=100;

    for (size_t i = 0; i<1000; ++i){
        time[2]+=setMeasureTimeSearch(numberElem);
    }
    time[2]/=1000;

    for (size_t i = 0; i<1000; ++i){
        time[3]+=setMeasureTimeAddRem(numberElem);
    }
    time[3]/=1000;

    return time;
}

void vecRandomNumberFilling(std::vector<int>& vec, size_t count){
    for(size_t i = 0; i<count; i++ ){
        vec.push_back(lcg());
    }
}

double vecMeasureTimeFilling(size_t count){
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> vec;
    vecRandomNumberFilling(vec,count);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1000000.0; 
}

double vecMeasureTimeSearch(size_t numberElem){
    int el = lcg();
    std::vector<int> vec;
    vecRandomNumberFilling(vec,numberElem);
    auto start = std::chrono::high_resolution_clock::now();
    contains(vec, el);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1000000.0; 
}

double vecMeasureTimeAddRem(size_t numberElem){
    int el = lcg();
    std::vector<int> vec;
    vecRandomNumberFilling(vec,numberElem);
    auto start = std::chrono::high_resolution_clock::now();
    vec.push_back(el);
    removeAll(vec, el);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1000000.0; 
}

std::vector<double> vecMaverageTime( size_t numberElem){
    std::vector<double> time = {double(numberElem), 0,0,0};
    for (size_t i = 0; i<100; ++i){
        time[1]+=vecMeasureTimeFilling(numberElem);
    }
    time[1]/=100;

    for (size_t i = 0; i<1000; ++i){
        time[2]+=vecMeasureTimeSearch(numberElem);
    }
    time[2]/=1000;

    for (size_t i = 0; i<1000; ++i){
        time[3]+=vecMeasureTimeAddRem(numberElem);
    }
    time[3]/=1000;

    return time;
}

void writeResultToFile(std::vector<std::vector<std::vector<double>>> time) {
    std::ofstream file("../../../Lab1/result.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to create file");
    }
    file<<"Set:\n";
    for(int i = 0; i <3; ++i){
       file << std::fixed << std::setprecision(12)<< int(time[0][i][0])<<" elements:\n\t" <<"Average filling time: " << time[0][i][1] << " seconds\n\t" 
                                                    <<"Average search time: " << time[0][i][2] << " seconds\n\t"
                                                    <<"Average time to add/remove: " << time[0][i][3] << " seconds\n\n";

    }
    
    file<<"Vector:\n";
    for(int i = 0; i <3; ++i){
        file << std::fixed << std::setprecision(12)<< int(time[1][i][0])<<" elements:\n\t" <<"Average filling time: " << time[1][i][1] << " seconds\n\t" 
                                                    <<"Average search time: " << time[1][i][2] << " seconds\n\t"
                                                    <<"Average time to add/remove: " << time[1][i][3] << " seconds\n\n";
    }

    file.close();
}