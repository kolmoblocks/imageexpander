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
    encoded.push_back(bits.at(0));
    while (i <len) {
        k = 1;
        while ((i + k < len) && bits.at(i + k) == bits.at(i)){
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
    cout<<"decoded"<<decoded.size()<<endl;
    for (int i = 33; i < size + 32;){
        int l = 0;
        while (encoded.at(i) == 0){
            l++;
            i++;
        }
//        i += l;
        if (i + l + 1  > size + 32) {
            cout<<"invalid incoding"<<endl;
        }
        int upper = i + l + 1;
        for(int j = i; j<upper; j++){
            bin.push_back(encoded.at(j));
        }
        int k = binToInt(bin);
        for (int j = 1; j <=k; j++){
            decoded.push_back(b);
        }
        i +=  l + 1;
//        cout<<i<<endl;
        b = 1 - b;
        bin.clear();
    }

}


//74659533