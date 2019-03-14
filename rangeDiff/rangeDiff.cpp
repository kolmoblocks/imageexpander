#include "../lodepng.h"
#include "../generateDiff.h"
#include "../blockIterator.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set> 
#include <numeric> 
#include <string>

using namespace std;


void insertRangeHeader(vector<unsigned char> &diff, int rangeSize, int offset){
    vector <unsigned char> offsetByte= lTrimZeroes(offset,8);
    vector <unsigned char> rangeSizeByte = lTrimZeroes(rangeSize, 8);
    diff.insert(diff.end(), offsetByte.begin(), offsetByte.end());
    diff.insert(diff.end(), rangeSizeByte.begin(), rangeSizeByte.end());

}

void insertRangeBlock(vector<unsigned char> &diff, blockIterator &it, int rangeSize, int offset){
    // generate headers
    // - range size, range start
    insertRangeHeader(diff, rangeSize, offset);
     vector <unsigned char> b;
     
    while (!it.end()){
        if ((*it).r < 0) {
            b.push_back(1);
         } else {
            b.push_back(0);
         }
         b =  lTrimZeroes((*it).r, rangeSize - 1);
         //LTRIM then add 1 bit for sign
        
        diff.insert(diff.end(), b.begin(), b.end());
        b.clear();
    }
    while (diff.size() % 8 != 0){
        writeBit(0);
    }         
}
