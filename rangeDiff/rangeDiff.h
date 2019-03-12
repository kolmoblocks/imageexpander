#ifndef RANGEDIFF_H
#define RANGEDIFF_H
#include "../lodepng.h"
#include "../generateDiff.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set> 
#include <numeric> 
#include <string>


void insertRangeHeader(vector<unsigned char> &diff, int rangeSize, int offset);

void insertRangeBlock(vector<unsigned char> &diff, vector<short int> &deltas, int rangeSize, int offset);

#endif