#include <cmath>
#include <vector>
#include <iostream>
#include "RLE.h"
using namespace std;




void encodeRLE (vector<unsigned char> &bits, vector<unsigned char> &encoded){
    vector <unsigned char> binary;
    int len = bits.size();
    encoded.reserve(len);
    int i = 0, k;
    encoded.push_back(bits[0]);
    while (i <len) {
        k = 1;
        while ((i + k < len) && bits[i + k] == bits[i]){
            k++;
        }
        int upper = (int)floor(log(k)/log(2));
        for (int l = 1; l <= upper; l++){
            encoded.push_back(0);
        }
        binary = intToUnsignedBin(k, upper+1);
        encoded.insert(encoded.end(), binary.begin(), binary.end());
        i+=k;
        binary.clear();
            
    }
}


void decodeRLE (vector<unsigned char> &encoded, vector<unsigned char>&decoded, int size){
    unsigned char b = encoded[32];
    vector<unsigned char> bin;
    cout<<encoded.size()<<endl;
    encoded.resize(encoded.size());
    for (int i = 33; i < size + 33;){
        int l = 0;
        while (encoded.at(i + l) == 0 || size + 33 < i + l + 34 ){
            l++;
            cout<<i + l << endl;
        }
        if (size + 33 < i + l + 34) {
            cout<<"invalid incoding"<<endl;
        }
        int upper = i + 2*l + 1;
        for(int j = i + l; j<upper; j++){
            bin.push_back(encoded.at(j));
        }
        int k = binToInt(bin);
        for (int j = 1; j <=k; j++){
            decoded.push_back(b);
        }
        i += 2*l + 1;
        b = 1 - b;
        bin.clear();
    }
}


