#ifndef BITUTIL_H
#define BITUTIL_H


#include "bitUtil.h"
using namespace std;

void writeBit (int bit, int &current_bit, unsigned char &bit_buffer, FILE *f) {
  if (bit)
    bit_buffer |= (1<<current_bit);

  current_bit++;
  if (current_bit == 8)
  {
    fwrite (&bit_buffer, 1, 1, f);
    current_bit = 0;
    bit_buffer = 0;
  }
}

void writeBitsToFile (std::vector<unsigned char> &bitBuff, FILE *f) {
    unsigned char bit_buffer=0;
    int current_bit=0, bitBuffLen = bitBuff.size();
    int nextWholeByte = bitBuffLen + ((bitBuffLen%8 == 0)? 0 : 8-bitBuffLen%8);

    for (int i = bitBuffLen; i < nextWholeByte; ++i) {
        bitBuff.push_back(0);
    }

    if (bitBuff.size() % 8 != 0) std::cout << "bitBuffLen not 8 after padding!" << std::endl;

    for (auto it : bitBuff) {
        writeBit(it, current_bit, bit_buffer, f);
    }
}

std::vector<unsigned char> intToBin(int c, int range) {
	std::vector<unsigned char> v;

	if (c < 0) {
        v.push_back(1);
        c *= -1;
    } else {
        v.push_back(0);
    }
    for (int i = range-1; i >= 0; i--){
            v.push_back((c & (1 << i)) != 0);
    }
	return v;
}

int binToInt(vector<unsigned char> &bin){
    int exponent = bin.size() - 1;
    int x = 0;
    for (auto it: bin){
        x += (int)it * pow(2, exponent);
        exponent-=1;
    }
    return x;
}

#endif