#ifndef BITUTIL_H
#define BITUTIL_H

#include <iostream>
#include <fstream>
#include <cmath>

void writeBit (int bit, int &current_bit, FILE *f) {
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
    int current_bit=0, bitBufflen = bitBuff.size();
    int nextWholeByte = bitBuffLen.size() + (bitBuffLen%8 == 0)? 0 : 8-bitBuffLen%8;

    for (int i = bitBuffLen; i < nextWholeByte; ++i) {
        bitBuff.push_back(0);
    }

    if (bitBuff.size() % 8 != 0) std::cout << "bitBuffLen not 8 after padding!" << std::endl;

    for (auto it : bitBuff) {
        writeBit(it, current_bit, bit_buffer, f)
    }
}

vector<unsigned char> intToBin(int c, int range) {
	vector<unsigned char> v;

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

#endif