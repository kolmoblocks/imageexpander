#include "rangeDiff.h"

using namespace std;

void insertRangeBlock(vector<unsigned char> &diff, blockIterator &it, int rangeSize, int offset){
    // generate headers
    // - range size, range start/
    ofstream f;
    vector <unsigned char> r,g,b;
    while (!it.end()){
        r =  intToBin(true,(*it).r - offset, rangeSize);
        g =  intToBin(true,(*it).g - offset, rangeSize);
        b =  intToBin(true,(*it).b - offset, rangeSize);

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
