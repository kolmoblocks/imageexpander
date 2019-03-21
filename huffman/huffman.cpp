//
// Created by sam on 3/20/19.
//

#include "huffman.h"


struct compare{
    bool operator()(dNode *left, dNode *right) {
        return left->weight < right->weight;
    }
};


void inOrder(dNode *curNode, std::vector<unsigned char> bitstring, std::vector<unsigned char> (&encTable)[256]) {
    if (curNode == nullptr) {
        std::cout << "0" << std::endl;
        return;
    } else if (!(curNode->right) && !(curNode->left)) {
        // if node is a leaf, add the bitstring to the encTable
        std::cout << "0.9" << std::endl;
        std::cout << curNode->weight << " " << (int)(curNode->ch) << std::endl;
        int c = curNode->ch;
        std::cout << "1";
        encTable[c].insert(encTable[c].end(), bitstring.begin(), bitstring.end());
        std::cout << "2" << std::endl;
        return;

    } else {
        std::cout << "3" << std::endl;
        // if node is a char, add the bitstring
        bitstring.push_back(0);
        inOrder(curNode->left, bitstring, encTable);
        bitstring.pop_back();
        std::cout << "4" << std::endl;
        bitstring.push_back(1);
        inOrder(curNode->right, bitstring, encTable);
        bitstring.pop_back();
        std::cout << "5" << std::endl;
        return;
    }
}

dNode *encodeHuffman(std::vector<unsigned char> &rawData) {
    int charFreq[256] = {0};
    int rawDataSize = rawData.size();

    if (rawDataSize % 8 != 0) std::cout << "sum ting wong" << std::endl;

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

    for (int i=0; i<256; ++i) {
        if (charFreq[i] > 0) {
            mpq.push(new dNode{i, charFreq[i], nullptr, nullptr});
        }
    }

    dNode *left, *right;

    while(mpq.size() != 1)
    {
        left = mpq.top();
        mpq.pop();

        right = mpq.top();
        mpq.pop();

        dNode *top = new dNode{'X', (left? left->weight: 0) + (right?right->weight: 0), nullptr, nullptr};

        top->left  = left;
        top->right = right;
        mpq.push(top);
    }


    dNode *encodeDictPtr = mpq.top();
    std::vector<unsigned char> encTable[256];

    std::vector<unsigned char> bitstring;
    inOrder(encodeDictPtr, bitstring, encTable);
    std::cout << "here" << std::endl;

    std::vector<unsigned char> curCharVec;
    auto leadingIt = rawData.begin();
    auto trailingIt = rawData.begin();
    int i=0;
    while ( i<rawDataSize ) {
        int lim = i+8;
        for (; i<lim; ++i) {
            curCharVec.push_back(*leadingIt);
            ++leadingIt;
        }
        std::cout << curCharVec.size() << std::endl;
        int pos = binToInt(curCharVec);

        for (auto it=encTable[pos].begin(); it<encTable[pos].end(); ++it) {
            *trailingIt = *it;
            ++trailingIt;
        }
        curCharVec.clear();
    }
    std::cout << "before: " << rawDataSize << std::endl;
    rawData.erase(trailingIt, rawData.end());
    std::cout << "after: " << rawData.size() << std::endl;

    return encodeDictPtr;
}




void decodeHuffman(std::vector<unsigned char> &encoded, std::vector<unsigned char> &decoded, dNode *encodeDict) {
    auto encodedIt = encoded.begin();
    while (encodedic)
}
