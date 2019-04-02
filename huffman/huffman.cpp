//
// Created by sam on 3/20/19.
//

#include "huffman.h"
#define TOKENSIZE 256


struct compare{
    bool operator()(dNode *left, dNode *right) {
        return left->weight > right->weight;
    }
};


void inOrder(dNode *curNode, std::vector<unsigned char> bitstring, std::vector<unsigned char> (&encTable)[TOKENSIZE]) {
    if (curNode == nullptr) {
        return;
    } else if (!(curNode->right) && !(curNode->left)) {
        // if node is a leaf, add the bitstring to the encTable
        //std::cout << "0.9" << std::endl;
        //std::cout << curNode->weight << " " << (int)(curNode->ch) << std::endl;
        int c = curNode->ch;
        //std::cout << "1";
        encTable[c].insert(encTable[c].end(), bitstring.begin(), bitstring.end());
        //if (bitstring.size() < 7) std ::cout << bitstring.size() << std::endl;

        //std::cout << "2" << std::endl;
        return;

    } else {
        //std::cout << "3" << std::endl;
        // if node is a char, add the bitstring
        bitstring.push_back(0);
        inOrder(curNode->left, bitstring, encTable);
        bitstring.pop_back();
        //std::cout << "4" << std::endl;
        bitstring.push_back(1);
        inOrder(curNode->right, bitstring, encTable);
        bitstring.pop_back();
        //std::cout << "5" << std::endl;
        return;
    }
}

dNode *encodeHuffman(std::vector<unsigned char> &rawData) {
    std::cout << "huff" << std::endl;
    int charFreq[TOKENSIZE] = {0};
    int rawDataSize = rawData.size();

    int incr = floor(log(TOKENSIZE)/log(2));

    std::vector<unsigned char> binVector;

    for (int i=0; i<rawDataSize; i+=incr) {
        for (int j=i; j<i+incr; ++j) {
            binVector.push_back(rawData[j]);
        }
        int index = binToInt(binVector);
        if (index <= TOKENSIZE-1) {
            ++(charFreq[index]);
        }
        binVector.clear();
    }

    std::priority_queue<dNode*, std::vector<dNode*>, compare> mpq;

    for (int i=0; i<TOKENSIZE; ++i) {
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
    std::vector<unsigned char> encTable[TOKENSIZE];

    std::vector<unsigned char> bitstring;
    inOrder(encodeDictPtr, bitstring, encTable);

    std::vector<unsigned char> curCharVec;
    auto leadingIt = rawData.begin();
    auto trailingIt = rawData.begin();

    int i=0;
    while ( i<rawDataSize ) {
        int lim = i+incr;
        for (; i<lim; ++i) {
            curCharVec.push_back(*leadingIt);
            ++leadingIt;
        }
        int pos = binToInt(curCharVec);

        //if (encTable[pos].size() > 8){std::cout << encTable[pos].size()<<std::endl ;}
        for (auto it=encTable[pos].begin(); it<encTable[pos].end(); ++it) {
            *trailingIt = *it;
            ++trailingIt;
        }
        curCharVec.clear();
    }
    std::cout << "before: " << rawData.size() << std::endl;
    rawData.erase(trailingIt, rawData.end());
    std::cout << "after: " << rawData.size() << std::endl;
    return encodeDictPtr;
}




void decodeHuffman(std::vector<unsigned char> &encoded, std::vector<unsigned char> &decoded, dNode *encodeDict) {
    auto encodedIt = encoded.begin();
    std::vector<unsigned char> curBinaryVec;
    dNode *dictPtr = encodeDict;


    while (encodedIt != encoded.end()) {
        if (!(dictPtr->left) && !(dictPtr->right)) {
            decoded.insert(decoded.end(), curBinaryVec.begin(), curBinaryVec.end());

            dictPtr = encodeDict;
            curBinaryVec.clear();
        }
        else{
            if (*encodedIt == 0) {
                dictPtr = dictPtr->left;
            }
            else {
                dictPtr = dictPtr->right;
            }
            curBinaryVec.push_back(*encodedIt);
        }
        ++encodedIt;
    }
}
