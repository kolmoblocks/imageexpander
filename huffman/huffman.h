//
// Created by sam on 3/20/19.
//

#ifndef IMAGEEXPANDER_HUFFMAN_H
#define IMAGEEXPANDER_HUFFMAN_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set>
#include <numeric>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include "../bitUtil/bitUtil.h"


typedef struct dNode{
    int ch;
    int weight;

    struct dNode *left;
    struct dNode *right;
} dNode;


dNode * encodeHuffman(std::vector<unsigned char> &rawdata);

void decodeHuffman(std::vector<unsigned char> &encoded, std::vector<unsigned char> &rawData, dNode *enco);



#endif //IMAGEEXPANDER_HUFFMAN_H
