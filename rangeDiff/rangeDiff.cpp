#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set> 
#include <numeric> 
#include <string>
#include "rangeDiff.h"

using namespace std;


void insertRangeHeader(vector<unsigned char> &diff, int rangeSize, int offset){
    vector <unsigned char> offsetByte= intToBin(offset,8);
    vector <unsigned char> rangeSizeByte = intToBin(rangeSize, 8);
    diff.insert(diff.end(), offsetByte.begin(), offsetByte.end());
    diff.insert(diff.end(), rangeSizeByte.begin(), rangeSizeByte.end());

}

void insertRangeBlock(vector<unsigned char> &diff, blockIterator &it, int rangeSize, int offset){
    // generate headers
    // - range size, range start
    insertRangeHeader(diff, rangeSize, offset);
     vector <unsigned char> r,g,b;
     
    while (!it.end()){
        if ((*it).r < 0) {
            b.push_back(1);
        } else {
            b.push_back(0);
        }
        r =  intToBin((*it).r, rangeSize - 1);
        g =  intToBin((*it).g, rangeSize - 1);
        b =  intToBin((*it).b, rangeSize - 1);
         //LTRIM then add 1 bit for sign
        diff.insert(diff.end(), r.begin(), r.end());
        diff.insert(diff.end(), g.begin(), g.end());
        diff.insert(diff.end(), b.begin(), b.end());
        r.clear();
        g.clear();
        b.clear();
    }      
}
