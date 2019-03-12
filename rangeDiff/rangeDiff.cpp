#include "../lodepng.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set> 
#include <numeric> 
#include <string>

using namespace std;

void insertRangeBlock(vector<unsigned char> &diff, vector<short int> &deltas, int rangeSize, int){
    vector<int> block;
    // generate header
    // - range size, range start

    for (auto it: block){
         vector <char> r =  LTrim_Zeroes(it, rangeSize){
             LTRIM then add 1 bit for sign
         }
    }
    //insert delta into diff
}



i