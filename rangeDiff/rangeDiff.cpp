#include "../lodepng.h"
#include "../generateDiff.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set> 
#include <numeric> 
#include <string>

using namespace std;


void insertRangeHeader(vector<unsigned char> &diff, int rangeSize, int offset){
    vector <unsigned char> offsetByte= lTrimZeroes(offset,8);
    vector <unsigned char> rangeSizeByte = lTrimZeroes(rangeSize);
    diff.insert(diff.end(), offsetByte.begin(), offsetByte.end());
    diff.insert(diff.end(), rangeSizeByte.begin(), rangeSizeByte.end());

}

void insertRangeBlock(vector<unsigned char> &diff, vector<short int> &deltas, int rangeSize, int offset){
    // generate headers
    // - range size, range start
    insertRangeHeader;
    for (auto it: deltas){
         vector <unsigned char> b  =  lTrimZeroes(it, rangeSize);
         //LTRIM then add 1 bit for sign
         if (it < 0) {
            writeBit(1);
         }
         padZeroes(b);
         
         for (auto bit: b){
             diff.push_back(bit);
         }
    }
    //insert delta into diff
}



