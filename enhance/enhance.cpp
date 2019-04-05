#include "enhance.h"

const int RGB_CHANNELS = 3;

vector<unsigned char> getBits(vector<unsigned char>&buffer, int start, int len){
    vector<unsigned char> res;
    res.reserve(len);
    for (int i = start; i< start +len; i++){
        res.push_back(buffer[i]);
    }
    return res;
}


int getRLELength(vector<unsigned char>&diff){
    vector<unsigned char> v;
    v = getBits(diff, 0, 32);
    return binToInt(v);
}

void setBlockInfo(int &w, int &h, int highResImgW, int highResImgH){
    w = highResImgW / 16;
    h = highResImgH / 9;
}

void getPixels(vector<unsigned int> &pixels, vector<unsigned char> &diff, vector<unsigned char> &lowRes,
    int highResImgW, int highResImgH, int deltaUnitSize, int highFactor, int lowFactor, unsigned lowResImgW){
    
    int diffPos = 96, rangeSize, offset, refR, refG, refB, blockW, blockH, r, g, b,
    index, upperBlockY, upperBlockX, deltaYCpy, deltaXCpy, upperDeltaY, upperDeltaX;
    setBlockInfo(blockW, blockH, highResImgW, highResImgH);

    upperBlockY = highResImgH - blockH;
    upperBlockX = highResImgW - blockW;
    for (int blockY = 0; blockY <= upperBlockY; blockY+= blockH){
        upperDeltaY = blockY + blockH;
        for (int blockX = 0; blockX <= upperBlockX; blockX+= blockW){
            rangeSize = binToInt(getBits(diff, diffPos, 8));
            diffPos += 8;
            offset = binToSignedInt(getBits(diff, diffPos, 8));
            diffPos += 8;

            upperDeltaX = blockX + blockW;

            for (int deltaY = blockY; deltaY < upperDeltaY; deltaY += highFactor){

                deltaYCpy = deltaY;
                for (int deltaX = blockX + highFactor - 1; deltaX < upperDeltaX; deltaX += highFactor) {
                    deltaXCpy = deltaX;
                    for (int unit = 0; unit < deltaUnitSize; unit++){
                        if (unit == deltaUnitSize - 1){

                            index =  3*(((deltaXCpy - 1) + lowResImgW * (deltaYCpy-1))* lowFactor/highFactor);
                            refR = lowRes[index];
                            refG = lowRes[index + 1];
                            refB = lowRes[index + 2];
                            deltaYCpy -= (highFactor-1);

                        } else if (unit < (deltaUnitSize - 1)/2){
                            index =  3*(((deltaXCpy - 1) + lowResImgW * deltaYCpy)* lowFactor/highFactor);
                            refR = lowRes[index];
                            refG = lowRes[index + 1];
                            refB = lowRes[index + 2];
                            deltaYCpy +=1;

                        } else {
                            if (unit == (deltaUnitSize - 1)/2) deltaXCpy -= (highFactor - 1);
                            index =  3*((deltaXCpy + lowResImgW * (deltaYCpy-1))* lowFactor/highFactor);
                            refR = lowRes[index];
                            refG = lowRes[index + 1];
                            refB = lowRes[index + 2];
                            deltaXCpy +=1;
                       }

                        r = refR - offset - binToSignedInt(getBits(diff, diffPos, rangeSize));
                        diffPos += rangeSize;

                        g = refG - offset - binToSignedInt(getBits(diff, diffPos, rangeSize));
                        diffPos += rangeSize;

                        b = refB - offset - binToSignedInt(getBits(diff, diffPos, rangeSize));
                        diffPos += rangeSize;
                        pixels.push_back(r);
                        pixels.push_back(g);
                        pixels.push_back(b);

                    }
                }
            }    
        }
    }
}

void extractHeader(vector<unsigned char> diff, unsigned &highResWidth, unsigned &highResHeight) {
    vector<unsigned char> v;
    v.reserve(32);


    v = getBits(diff,32,32);
    highResWidth = (unsigned)binToInt(v);
    v = getBits(diff,64,32);
    highResHeight = (unsigned)binToInt(v);
    // for (int i = 86; i < 124; i++){
    //     v.push_back(diff[i]);
    // }
    //colormode = assume RGB for now

}

void populateBlocks(std::vector<blockParams> &blocks, int width, int height, int highFactor) {
    // logic here to statisticallly determine "good" configuration of blocks
    int xincr = width/16, yincr=height/9;

        for (int j=0; j<height; j+=yincr) {
            for (int i=0; i<width; i+=xincr) {

                blocks.push_back(blockParams{posn{i/highFactor,j/highFactor}, posn{(i+xincr)/highFactor, (j+yincr)/highFactor}, 'R'});
        }
    }
}


void populateDiffPixelVec(std::vector<unsigned int> &diffPixelVec, std::vector<unsigned int> &blocksPixelVec, unsigned deltaUnitSize, unsigned highResWidth, unsigned highResHeight, int highFactor) {
    // assuming is RANGE 
    // assuming default config for a 16:9 image
    unsigned blockDataSize = highResHeight*highResWidth / (16*9);
    unsigned numBlocks = 16*9;

    cout<<blocksPixelVec.size()<<endl;
    std::vector<unsigned int> blockPosVec;
    for (int i=0; i<=numBlocks; ++i) {
        blockPosVec.push_back(i * blockDataSize/(highFactor*highFactor) * deltaUnitSize * RGB_CHANNELS); // convert to number of diff pixels per unit
    }

    std::vector<blockParams> bPs;
    populateBlocks(bPs, highResWidth, highResHeight, highFactor);

    auto bt = bPs.begin();
    for (auto it=blockPosVec.begin(); it<blockPosVec.end()-1; ++it) {
        unsigned blockBegin = *it;
        unsigned blockEnd = *(std::next(it,1));
        // tl, br are in UNITS not PIXELS
        int diffPixPos = (bt->tl.x + bt->tl.y * highResWidth/highFactor)*deltaUnitSize*3;
        int blockW = highResWidth / (16 * highFactor) *deltaUnitSize*RGB_CHANNELS;//(bt->br.x - bt->tl.x) * deltaUnitSize * 3;
        //int blockH = bt->br.y - bt->br.y;

        if (bt->br.x < bt->tl.x) {
            throw std::logic_error("error: br.x < tl.x");
        }

        int ct = diffPixPos;
        for (int i=blockBegin; i<blockEnd; i+=blockW) {
            // in delta unit, but we don't care since it's iterating through each pixel anyways..
            // it is reading from blocksPixelVec, ct is where you are putting the pixels in diffPixelVec

            for (int j=i; j<i+blockW; ++j) {
                diffPixelVec[ct] = blocksPixelVec[j];
                ++ct;
            }

            ct += highResWidth/highFactor * deltaUnitSize * RGB_CHANNELS - blockW;
        }
        ++bt;
    }
}


bool checkFileHeader(vector<unsigned char>&diff){
    vector<unsigned char> v;
    string res = "";
    v = getBits(diff,0,8);
    res += (char)binToInt(v);
    cout<<"size"<<v.size()<<endl;
    v = getBits(diff,8,8);
    res += (char)binToInt(v);
    v = getBits(diff,16,8);
    res += (char)binToInt(v);
    v.clear();
    v = getBits(diff,24,8);
    res += (char)binToInt(v);
    cout<<res<<endl;
    return res == "DIFF";
}

void expand_image( std::vector<unsigned char> oldImgVec, unsigned oldImgW, unsigned oldImgH, std::vector<unsigned int> &diffVec, unsigned loFactor, unsigned hiFactor, int diffW) {
    unsigned newW = oldImgW * hiFactor / loFactor;
    unsigned newH = oldImgH * hiFactor / loFactor;
    std::vector<Color> newImgVec(newW * newH * 3);
    int diffX = 0, diffY = 0, newImgIndex, targetIndex, upperInnerY, upperInnerX;
    int deltaUnitSize = 5;
    int ct=0;
    for (std::size_t y=0; y<newH; y+= hiFactor) {
        upperInnerY = y+hiFactor;
        for (std::size_t x=0; x<newW; x+= hiFactor) {
            // iterating through inner block pixels, innerX and innerY indicate the current position of the block we are at.
            // increment diffY, reset diffX to 0 each time we go to a new block
            ++diffY;
            diffX = 0;
            upperInnerX = x+hiFactor;
            for (int innerY = y; innerY < upperInnerY; ++innerY) {
                    for (int innerX = x; innerX < upperInnerX; ++innerX) {
                        // only get and set pixel if the block is not included in the old block (for now it is the top left smaller square with sides of length "loFactor")
                        newImgIndex = innerX + (innerY)*newW;
                        if (innerX >= x+loFactor || innerY >= y+loFactor) {
                            targetIndex = 3*(diffX-deltaUnitSize + diffY*diffW);
                            // grab pixel from the diff
                            newImgVec[newImgIndex].r = diffVec[targetIndex];
                            newImgVec[newImgIndex].g = diffVec[targetIndex + 1];
                            newImgVec[newImgIndex].b = diffVec[targetIndex + 2];
                            // increment diffX every time we increment through the inner block.
                            ++diffX;
                        }
                        else {
                            // grab pixel from the old image
                            targetIndex = 3*(((innerX) + oldImgW * (innerY))* loFactor/hiFactor + (innerX - x));
                            if (targetIndex == oldImgVec.size() - 5 * 3){
                                std::cout << "last" << std::endl;
                            }

                            newImgVec[newImgIndex].r = oldImgVec[targetIndex];
                            newImgVec[newImgIndex].g = oldImgVec[targetIndex + 1];
                            newImgVec[newImgIndex].b = oldImgVec[targetIndex + 2];
                        }

                }
            }

        }
        ++ct;
    }
    std::vector<unsigned char> finalImage;
    for (auto it : newImgVec) {
        finalImage.push_back(it.r);
        finalImage.push_back(it.g);
        finalImage.push_back(it.b);
    }
    lodepng::encode("final.png",finalImage,newW,newH,LCT_RGB,8);
}


void enhance(char *lowResFileName, char *diffFileName) {
    FILE *pDiff;
    pDiff = fopen(diffFileName, "rb");
    vector<unsigned char> deltas, lowResImage,diff, encoded;
    vector <unsigned int> blocksPixelVec,diffPixelVec;
    unsigned lowResWidth, lowResHeight, highResWidth, highResHeight,lowFactor, highFactor, 
    unitSize, deltaUnitSize, totalDeltaUnits, error;
    ofstream f2;
    // rle decode diff, then put into diffFileVector
    vector<unsigned char> bin;

    ifstream f("diff.dat", ios::binary | ios::in);
    char c;



    while (f.get(c)) {
        for (int i = 0; i < 8; i++) encoded.push_back((c >> i) & 1);
    }

    int RLELen = getRLELength(encoded);
    cout<<RLELen<<endl;

    // diff.resize(encoded.size());

    decodeRLE(encoded, diff, RLELen);
cout<<encoded.size()<<endl;
//    diff.insert(diff.end(),encoded.begin() + 32, encoded.end());
    cout<<"successfully decoded"<<endl;
    cout<<"size"<<diff.size()<<endl;
    if (!checkFileHeader(diff)){
        cerr<<"File is not in DIFF format"<<endl;
        return;
    }
    extractHeader(diff, highResWidth, highResHeight);

    error = lodepng::decode(lowResImage, lowResWidth, lowResHeight, lowResFileName, LCT_RGB, 8);
    if (error) {
        std::cout << lodepng_error_text(error) << std::endl;
        return;
    }
    lowFactor = lowResWidth / gcd(highResWidth, lowResWidth);
    highFactor = highResWidth / gcd(highResWidth, lowResWidth);
    
    unitSize = highFactor*highFactor;
    deltaUnitSize = unitSize - lowFactor*lowFactor;
    totalDeltaUnits = highResHeight*highResWidth / unitSize;

    diffPixelVec.resize(3*totalDeltaUnits*deltaUnitSize, 255);
    blocksPixelVec.reserve(3*totalDeltaUnits*deltaUnitSize);


    //iterate blocks
    // assuming aspect ratio is 16:9
    // assuming basic block configuration
    int numDeltaPixelsPerBlock =  3;
    // THIS SHOULD BE 3 SINCE WE ARE RESIZING 2:1

    getPixels(blocksPixelVec, diff, lowResImage, highResWidth, highResHeight, deltaUnitSize, highFactor, lowFactor, lowResWidth);
    cout<<"Gotpixels"<<endl;
    populateDiffPixelVec(diffPixelVec, blocksPixelVec, deltaUnitSize, highResWidth, highResHeight, highFactor);
    cout<<"populated diff pix vec, size "<< diffPixelVec.size()<<endl;
    expand_image(lowResImage, lowResWidth, lowResHeight, diffPixelVec, lowFactor, highFactor, deltaUnitSize);

}





