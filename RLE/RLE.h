#ifndef RLE_H
#define RLE_H
#include <cmath>
#include <vector>
#include "../bitUtil/bitUtil.h"
using namespace std;

void encodeRLE (vector<unsigned char> &bits, vector<unsigned char> &encoded);
void decodeRLE (vector<unsigned char> &bits, vector<unsigned char>&decoded, int size);

#endif