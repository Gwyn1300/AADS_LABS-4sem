#include "function.h"

void inSet(const set& a, const set& b, std::vector<int>& vec) {
    std::vector<int> bToVec = b.toVector();
    if (bToVec.size()==0) return;
    for(int elem:bToVec){
        if(a.contains(elem)) vec.push_back(elem);
    }
}

void notInSet(const set& a, const set& b, std::vector<int>& vec) {
    std::vector<int> bToVec = b.toVector();
    if (bToVec.size()==0) return;
    
    for(int elem:bToVec){
        if(!a.contains(elem)) vec.push_back(elem);
    }
}

std::vector<int>intersection(const set& a, const set& b){
    std::vector<int> res;
    inSet(a,b,res);
    return res;
}

std::vector<int>unification(const set& a, const set& b){
    std::vector<int>res;
    notInSet(a,b,res);
    notInSet(b,a,res);
    inSet(a,b,res);
    return res;
}

void experement(){ 
    std::cout<<"Program start\n";
    std::vector<double> vecTime3 = vecMaverageTime(1000);
    std::vector<double> vecTime4 = vecMaverageTime(10000);
    std::vector<double> vecTime5 = vecMaverageTime(100000);
    std::vector<double> setTime3 = vecMaverageTime(1000);
    std::vector<double> setTime4 = vecMaverageTime(10000);
    std::vector<double> setTime5 = vecMaverageTime(100000);
    std::vector<std::vector<std::vector<double>>> time = {{vecTime3,vecTime4, vecTime5},{setTime3,setTime4,setTime5}};
    printTime(time);
    writeResultToFile(time);
}
