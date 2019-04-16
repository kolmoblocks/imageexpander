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


std::vector<unsigned char> intToBin(bool signedBin ,int c, int range) {
	std::vector<unsigned char> v;
	if (signedBin && c < 0){
        v.push_back(1);
        c *= -1;
        range-=1;
    }

    for (int i = range-1; i >= 0; i--){
            v.push_back((c & (1 << i)) != 0);
    }
	return v;
}


int binToInt(bool signedBin, vector<unsigned char> bin){
    int exponent = signedBin?bin.size()-2:bin.size() - 1;
    int sign = signedBin && bin[0] == 1?-1:1;
    int x = 0;
    for (auto it = signedBin?bin.begin()+1:bin.begin();it!=bin.end();++it){
        x += (int)*it * pow(2, exponent);
        exponent-=1;
    }
    return sign*x;
}


void writeBitsToFile (std::vector<unsigned char> &bitBuff, FILE *f) {
    unsigned char bit_buffer=0;
    vector<unsigned char> encoded;
    int current_bit=0;
    encoded.reserve(bitBuff.size());
    encodeRLE(bitBuff, encoded);
    int RLElen = encoded.size();
    int nextWholeByte = RLElen + ((RLElen%8 == 0)? 0 : 8-RLElen%8);
    for (int i = RLElen; i < nextWholeByte; ++i) {
        encoded.push_back(0);
    }
    cout<<encoded.size()%8<<endl;
    if (encoded.size() % 8 != 0) std::cout << "bitBuffLen not 8 after padding!" << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    vector<unsigned char> RLEHeader = intToBin(false,RLElen, 32);
    cout<<RLElen<<endl;
    for (auto it : RLEHeader){
        writeBit(it, current_bit, bit_buffer, f);
    }

    for (auto it : encoded) {
        writeBit(it, current_bit, bit_buffer, f);
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    cout<<std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()<<endl;

}
