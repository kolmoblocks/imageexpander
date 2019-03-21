//
// Created by sam on 3/20/19.
//

#include "huffman.h"


struct compare{
    bool operator()(dNode *left, dNode *right) {
        return left->weight < right->weight;
    }
};


void inOrder(dNode *curNode, std::vector<unsigned char> bitstring, std::vector<unsigned char> (&encTable)[255]) {
    if (curNode == nullptr) {
        return;
    } else if (!(curNode->right || curNode->left)) {
        // if node is a leaf, add the bitstring to the encTable
        int ch = (int)curNode->ch;
        encTable[ch].insert(encTable[ch].begin(), bitstring.begin(), bitstring.end());
        return;

    } else {
        // if node is a char, add the bitstring
        bitstring.push_back(0);
        inOrder(curNode->left, bitstring, encTable);
        bitstring.pop_back();

        bitstring.push_back(1);
        inOrder(curNode->right, bitstring, encTable);
        bitstring.pop_back();
        return;
    }
}

dNode *encodeHuffman(std::vector<unsigned char> &rawData) {
    int charFreq[255] = {0};
    int rawDataSize = rawData.size();

    if (rawDataSize % 8 == 0) std::cout << "sum ting wong" << std::endl;
    std::vector<unsigned char> binVector;

    for (int i=0; i<rawDataSize; i+=8) {
        for (int j=i; j<i+8; ++j) {
            binVector.push_back(rawData[j]);
        }
        int index = binToInt(binVector);
        if (index <= 255) {
            ++(charFreq[index]);
        }
        binVector.clear();
    }

    std::priority_queue<dNode*, std::vector<dNode*>, compare> mpq;

    for (size_t i=0; i<255; ++i) {
        mpq.push(new dNode({(char)i, charFreq[i], nullptr, nullptr}));
    }

    dNode *left, *right;

    while(mpq.size() != 1)
    {
        left = mpq.top();
        mpq.pop();

        right = mpq.top();
        mpq.pop();

        dNode *top = new dNode{'X', left->weight + right->weight, nullptr, nullptr};
        top->left  = left;
        top->right = right;
        mpq.push(top);
    }

    dNode *encodeDictPtr = mpq.top();

    std::vector<unsigned char> encTable[255];

    std::vector<unsigned char> bitstring;
    inOrder(encodeDictPtr, bitstring, encTable);

    std::vector<unsigned char> curCharVec;
    auto leadingIt = rawData.begin();
    auto trailingIt = rawData.begin();

    for (int i=0; i<rawDataSize; ++i) {
        for (int j=0; j<8; ++j) {
            curCharVec.push_back(*leadingIt);
            ++leadingIt;
        }
        int pos = binToInt(curCharVec);

        rawData.insert(trailingIt, encTable[pos].begin(), encTable[pos].end());
        trailingIt += encTable[pos].size();
        curCharVec.clear();
    }

    return encodeDictPtr;
}




void decodeHuffman(std::vector<unsigned char> &encoded, std::vector<unsigned char> &rawData, dNode *encodeDict) {

}
