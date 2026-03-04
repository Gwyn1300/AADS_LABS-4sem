#pragma once
#include"set.h"
#include <cstddef>
#include<vector>


size_t lcg();

void setRandomNumberFilling(set& s, size_t count);

bool contains(const std::vector<int>& vec, int value);

void removeAll(std::vector<int>& vec, int el);

double setMeasureTimeFilling(size_t count);

double setMeasureTimeSearch(size_t numberElem);

double setMeasureTimeAddRem(size_t numberElem);

void printTime(std::vector<std::vector<std::vector<double>>> time);


void printTime(std::vector<double> time);

std::vector<double> setMaverageTime( size_t numberElem);

void vecRandomNumberFilling(std::vector<int>& vec, size_t count);

double vecMeasureTimeFilling(size_t count);

double vecMeasureTimeSearch(size_t numberElem);

double vecMeasureTimeAddRem(size_t numberElem);

std::vector<double> vecMaverageTime( size_t numberElem);

void writeResultToFile(std::vector<std::vector<std::vector<double>>> time);