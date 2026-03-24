#include "experiment.h"
#include <vector>
#include <iostream>

void inSet(const set& a, const set& b, std::vector<int>& vec);

void notInSet(const set& a, const set& b, std::vector<int>& vec);

std::vector<int>intersection(const set& a, const set& b);

std::vector<int>unification(const set& a, const set& b);

void experement();

void chalange(set a, set b);