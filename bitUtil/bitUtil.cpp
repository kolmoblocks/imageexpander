#include "bitUtil.h"
using namespace std;

unsigned int gcd(unsigned int u, unsigned int v)
{
    // simple cases (termination)
    if (u == v)
        return u;

    if (u == 0)
        return v;

    if (v == 0)
        return u;

    // look for factors of 2
    if (~u & 1) // u is even
    {
        if (v & 1) // v is odd
            return gcd(u >> 1, v);
        else // both u and v are even
            return gcd(u >> 1, v >> 1) << 1;
    }

    if (~v & 1) // u is odd, v is even
        return gcd(u, v >> 1);

    // reduce larger argument
    if (u > v)
        return gcd((u - v) >> 1, v);

    return gcd((v - u) >> 1, u);
}

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


std::vector<unsigned char> intToBin(int c, int range) {
	std::vector<unsigned char> v;

	if (c < 0) {
        v.push_back(1);
        c *= -1;
    } else {
        v.push_back(0);
    }
    for (int i = range-2; i >= 0; i--){
            v.push_back((c & (1 << i)) != 0);
    }
	return v;
}


std::vector<unsigned char> intToUnsignedBin(int c, int range) {
	std::vector<unsigned char> v;


    for (int i = range-1; i >= 0; i--){
            v.push_back((c & (1 << i)) != 0);
    }
	return v;
}


int binToInt(vector<unsigned char> bin){
    int exponent = bin.size() - 1;
    int x = 0;
    for (auto it: bin){
        x += (int)it * pow(2, exponent);
        exponent-=1;
    }
    return x;
}

int binToSignedInt(vector<unsigned char> bin){
    int exponent = bin.size() - 2;
    int x = 0;
    int sign = 1;
    if (bin[0] == 1) {
        sign = -1;
    } 
    for (auto it = bin.begin() + 1; it != bin.end(); ++it ){
        x += (int)*it * pow(2, exponent);
        exponent-=1;
    }
    return sign * x;
}


void writeBitsToFile (std::vector<unsigned char> &bitBuff, FILE *f) {
    unsigned char bit_buffer=0;
    vector<unsigned char> encoded;
    int current_bit=0;


    encoded.reserve(bitBuff.size());

    encodeRLE(bitBuff, encoded);
//    encoded.insert(encoded.end(), bitBuff.begin(), bitBuff.end());
    int RLElen = encoded.size();

    int nextWholeByte = RLElen + ((RLElen%8 == 0)? 0 : 8-RLElen%8);


    cout<<encoded.size()%8<<endl;

    for (int i = RLElen; i < nextWholeByte; ++i) {
        encoded.push_back(0);
    }
    cout<<encoded.size()%8<<endl;
    if (encoded.size() % 8 != 0) std::cout << "bitBuffLen not 8 after padding!" << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();

    vector<unsigned char> RLEHeader = intToUnsignedBin(RLElen, 32);
   cout<<RLElen<<endl;
    for (auto it : RLEHeader){
        writeBit(it, current_bit, bit_buffer, f);
    }

    for (auto it : encoded) {
        writeBit(it, current_bit, bit_buffer, f);
    }

//    fwrite(&bitBuff[0], 1, bitBuff.size()/8, f);

    auto endTime = std::chrono::high_resolution_clock::now();

    cout<<std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<<endl;

}
