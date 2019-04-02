#include "mapDiff.h"
using namespace std;


void insertMapHeader(vector<unsigned char> &diff, vector<int> &mapVals){

    vector<unsigned char> mapSize = intToBin(mapVals.size(), 8);
    diff.insert(diff.end(), mapSize.begin(), mapSize.end());
    vector<unsigned char> curNumVec;

    for (auto it : mapVals) {
        curNumVec = intToBin(it, 9);
        diff.insert(diff.end(), curNumVec.begin(), curNumVec.end());
    }
}

void insertMapBlock(vector<unsigned char> &diff, blockIterator &it, vector<int> &mapVals){
    // generate headers
    // - range size, range start/
    ofstream f;


    vector <unsigned char> r,g,b;
    int ct = 0;

    while (!it.end()){

    }

    it.reset();
}
