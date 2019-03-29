#include "rangeDiff.h"

using namespace std;


void insertRangeHeader(vector<unsigned char> &diff, int rangeSize, int offset){
    vector <unsigned char> offsetByte= intToBin(offset,8);
    vector <unsigned char> rangeSizeByte = intToBin(rangeSize, 8);
    diff.insert(diff.end(), rangeSizeByte.begin(), rangeSizeByte.end());
    diff.insert(diff.end(), offsetByte.begin(), offsetByte.end());

}

void insertRangeBlock(vector<unsigned char> &diff, blockIterator &it, int rangeSize, int offset){
    // generate headers
    // - range size, range start/
        ofstream f;


    vector <unsigned char> r,g,b;
    int ct = 0;

    while (!it.end()){
        ++ct;

        r =  intToBin((*it).r - offset, rangeSize);
        g =  intToBin((*it).g - offset, rangeSize);
        b =  intToBin((*it).b - offset, rangeSize);

        diff.insert(diff.end(), r.begin(), r.end());
        diff.insert(diff.end(), g.begin(), g.end());
        diff.insert(diff.end(), b.begin(), b.end());

        r.clear();
        g.clear();
        b.clear();
        ++it;
    }

    it.reset();
}
