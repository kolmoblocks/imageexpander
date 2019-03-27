#include "enhance.h"

vector<unsigned char> getBits(vector<unsigned char>&buffer, int start, int len){
    vector<unsigned char> res;    
    res.reserve(len);
    // for (int i = start+len - 1; i >= start ; i--){
    //     res.push_back(buffer[i]); 
    //     cout<<(int)buffer[i];
    // }

    for (int i = start; i< start +len; i++){
        res.push_back(buffer[i]); 
        // cout<<(int)buffer[i];
    }
    // cout<<endl;
    return res;
}

void setBlockInfo(int &w, int &h, int highResImgW, int highResImgH){
    w = highResImgW / 16;
    h = highResImgH / 9;
}

void getPixels(vector<unsigned int> &pixels, vector<unsigned char> &diff, vector<unsigned char> &lowRes,
    int highResImgW, int highResImgH, int deltaUnitSize, int numDeltaPixelsPerBlock, int highFactor, int lowFactor, int lowResImgW){
    
    int diffPos = 96, rangeSize, offset, r, g, b, refR, refG, refB, blockW, blockH;
    setBlockInfo(blockW, blockH, highResImgW, highResImgH);

    //x,y = start of each block
    
    for (int blockY = 0; blockY < highResImgH - blockH; blockY+= blockH){
        for (int blockX = 0; blockX < highResImgW - blockW; blockX+= blockW){
            rangeSize = binToInt(getBits(diff, diffPos, 8));
            cout<<"rangeSize:"<<rangeSize<<endl;
            cout<<"diffpos:"<<diffPos<<endl;
            diffPos += 8;
            offset = binToInt(getBits(diff, diffPos, 8));
            diffPos += 8;    
            for (int deltaY = blockY; deltaY < blockY + blockH; deltaY += highFactor){
                int deltaYCpy = deltaY;
                for (int deltaX = blockX + highFactor - 1; deltaX < blockX + blockW; deltaX += highFactor) {
                    int deltaXCpy = deltaX;
                    for (int unit = 0; unit < deltaUnitSize; unit++){
                        // cout<<diffPos<<endl;
                        // cout<<lowRes.size()<<endl;;


                        // cout<<3*(highResImgW * (deltaYCpy) + (deltaXCpy-1)) + 2<<endl;
                        // cout<<highResImgW<<endl;
                        // cout<<deltaYCpy<<":"<<deltaXCpy<<endl;
                    // cout<<deltaYCpy<<":"<<deltaXCpy<<endl;
                    //     cout<<"unit"<<unit<<endl;
                        if (unit == deltaUnitSize - 1){
                                                        //cout<<"if 1"<<endl;
                        //                                 cout<<"size"<<lowRes.size()<<endl;
                            // cout <<3*(1920 * (deltaYCpy-1)-1 + (deltaXCpy-1))<<endl;
                            // refR = lowRes[3*((1920 * (deltaYCpy-1)-1) + (deltaXCpy-1))];




                            refR = lowRes[ 3*((deltaXCpy - 1) * lowFactor/highFactor 
                            + lowResImgW * (deltaYCpy-1) * lowFactor/highFactor)];

                            refG = lowRes[ 3*((deltaXCpy - 1) * lowFactor/highFactor 
                            + lowResImgW * (deltaYCpy-1) * lowFactor/highFactor ) + 1];

                            refB = lowRes[ 3*((deltaXCpy) * lowFactor/highFactor 
                            + lowResImgW * (deltaYCpy-1) * lowFactor/highFactor) + 2];

                            // refG = lowRes[3*((1920 * (deltaYCpy-1)-1) + (deltaXCpy-1)) + 1];
                            // refB = lowRes[3*((1920 * (deltaYCpy-1)-1) + (deltaXCpy-1)) + 2];
                            deltaYCpy -= (highFactor-1);


                        } else if (unit < (deltaUnitSize - 1)/2){
                                                                                    // cout<<"if 2"<<endl;
                            // cout<<3*((1920 * (deltaYCpy)-1) + (deltaXCpy-1))<<endl;
                            // refR = lowRes[3*((1920 * (deltaYCpy)-1) + (deltaXCpy-1))];
                            // refG = lowRes[3*((1920 * (deltaYCpy)-1) + (deltaXCpy-1)) + 1];
                            // refB = lowRes[3*((1920 * (deltaYCpy)-1) + (deltaXCpy-1)) + 2];



                            refR = lowRes[ 3*((deltaXCpy) * lowFactor/highFactor 
                            + lowResImgW * (deltaYCpy-1) * lowFactor/highFactor)];

                            refG = lowRes[ 3*((deltaXCpy) * lowFactor/highFactor 
                            + lowResImgW * (deltaYCpy-1) * lowFactor/highFactor ) + 1];

                            refB = lowRes[ 3*((deltaXCpy) * lowFactor/highFactor 
                            + lowResImgW * (deltaYCpy-1) * lowFactor/highFactor) + 2];


                            deltaYCpy +=1;

                        } else {
                            // cout<<"if 3"<<endl;

                            if (unit == (deltaUnitSize - 1)/2) deltaXCpy -= (highFactor - 1);
                            // refR = lowRes[3*((1920 * (deltaYCpy-1)-1) + (deltaXCpy))];
                            // refG = lowRes[3*(1920 * (deltaYCpy-1)-1 + (deltaXCpy)) + 1];
                            // refB = lowRes[3*(1920 * (deltaYCpy-1)-1 + (deltaXCpy)) + 2];

                            refR = lowRes[ 3*((deltaXCpy) * lowFactor/highFactor 
                            + lowResImgW * (deltaYCpy-1) * lowFactor/highFactor)];

                            refG = lowRes[ 3*((deltaXCpy) * lowFactor/highFactor 
                            + lowResImgW * (deltaYCpy-1) * lowFactor/highFactor ) + 1];

                            refB = lowRes[ 3*((deltaXCpy) * lowFactor/highFactor 
                            + lowResImgW * (deltaYCpy -1) * lowFactor/highFactor) + 2];

                            deltaXCpy +=1;
                        }


                        r = refR + offset + binToSignedInt(getBits(diff, diffPos, rangeSize));
                        pixels.push_back(r);
                        diffPos += rangeSize;
                        // cout<<"diffpos"<<diffPos<<endl;

                        g = refG + offset + binToSignedInt(getBits(diff, diffPos, rangeSize));
                        pixels.push_back(g);
                        diffPos += rangeSize;
                        // cout<<"diffpos"<<diffPos<<endl;

                        b = refB + offset + binToSignedInt(getBits(diff, diffPos, rangeSize));
                        pixels.push_back(b);
                        diffPos += rangeSize;
                                                // cout<<"diffpos"<<diffPos<<endl;

                    }


                }
            }    

        }
    }

}


char* getDiffFromFile(FILE *pFile){
    long lSize;
    char * buffer;
    size_t result = 0;

    pFile = fopen ( "diff.dat" , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    cout<<"lSize"<<lSize<<endl;
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
     fread (buffer,1,lSize,pFile);
    // for(int i = 0; i < lSize; i++) {
    //    fread(buffer+i, 1, 1, pFile); 
    // }
    //  for (int i = 0; i < 8 * lSize; i++){
    //      cout<<(int)buffer[i]<<endl;
    //  }
    fclose (pFile);
    free (buffer);
    cout<<"end getdiff"<<endl;
    return buffer;
}
void extractHeader(vector<unsigned char> diff, unsigned &highResWidth, unsigned &highResHeight) {
    vector<unsigned char> v;
    v.reserve(32);
    v = getBits(diff,32,32);
    highResWidth = (unsigned)binToInt(v);
    v = getBits(diff,64,32);
    highResHeight = (unsigned)binToInt(v);
    // for (int i = 86; i < 128; i++){
    //     v.push_back(diff[i]);
    // }
    //colormode = assume RGB for now

}

void populateBlocks(std::vector<blockParams> &blocks, int width, int height, int highFactor) {
    // logic here to statisticallly determine "good" configuration of blocks
    int xincr = width/16, yincr=height/9;

    for (int i=0; i<width; i+=xincr) {
        for (int j=0; j<height; j+=yincr) {
            blocks.push_back(blockParams{posn{i/highFactor,j/highFactor}, posn{(i+xincr)/highFactor-1, (j+yincr)/highFactor-1}, 'R'});
        }
    }
    // for (auto it : blocks) {
    //     std::cout << it.tl.x << " " << it.tl.y << " " << it.br.x << " " << it.br.y << std::endl;
    // }
}


void populateDiffPixelVec(std::vector<unsigned char> &diffPixelVec, std::vector<unsigned int> &blocksPixelVec, unsigned deltaUnitSize, unsigned highResWidth, unsigned highResHeight) {
    // assuming is RANGE 
    // assuming default config for a 16:9 image
    unsigned blockDataSize = highResHeight*highResWidth / (16*9);
    unsigned numBlocks = 16*9;

    std::vector<unsigned int> blockPosVec;
    for (int i=0; i<numBlocks; ++i) {
        blockPosVec.push_back(i * blockDataSize * 3);
    }

    std::vector<blockParams> bPs;
    populateBlocks(bPs, 1960, 1080, 2);

    auto bt = bPs.begin();

    for (auto it=blockPosVec.begin(); it<blockPosVec.end(); ++it) {

        unsigned blockBegin = *it;
        unsigned blockEnd = *(std::next(it,1));
        // tl, br are in UNITS not PIXELS
        int diffPixPos = (bt->tl.x + bt->tl.y * highResWidth/2)*deltaUnitSize*3;
        int blockW = bt->br.x - bt->tl.x;

        if (bt->br.x < bt->tl.x) {
            throw std::logic_error("error: br.x < tl.x");
        }

        int ct = diffPixPos;
        for (int i=blockBegin; i<blockEnd; ++i) {
            // in delta unit, but we don't care since it's iterating through each pixel anyways..
            // it is reading from blocksPixelVec, ct is where you are putting the pixels in diffPixelVec
            diffPixelVec[ct] = blocksPixelVec[i];

            ++ct;
            if ((ct-diffPixPos) == blockW ) {
                ct += (highResWidth/2 - blockW)*deltaUnitSize*3;
            }
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
    return res == "DIFF";
}


void expand_image( std::vector<unsigned char> oldImgVec, unsigned oldImgW, unsigned oldImgH, std::vector<unsigned char> diffVec, unsigned loFactor, unsigned hiFactor, int diffW) {


    unsigned newW = oldImgW * hiFactor / loFactor;
    unsigned newH = oldImgH * hiFactor / loFactor;

    std::vector<Color> newImgVec(newW * newH * 3);

    int diffX = 0;
    int diffY = 0;

    for (std::size_t y=0; y<newH; y+= hiFactor) {
        for (std::size_t x=0; x<newW; x+= hiFactor) {
            // iterating through inner block pixels, innerX and innerY indicate the current position of the block we are at.
            // increment diffY, reset diffX to 0 each time we go to a new block
            ++diffY;
            diffX = 0;
            for (int innerX = x; innerX < x+hiFactor; ++innerX) {
                for (int innerY = y; innerY < y+hiFactor; ++innerY) {
                    // only get and set pixel if the block is not included in the old block (for now it is the top left smaller square with sides of length "loFactor")
                    if (!(innerX < x+loFactor) || !(innerY < y+loFactor)) {
                        // grab pixel from the diff
                        newImgVec[innerX + innerY*newW + 4].r = diffVec[3*(diffX + diffY*diffW)];
                        newImgVec[innerX + innerY*newW + 4].g = diffVec[3*(diffX + diffY*diffW)+1];
                        newImgVec[innerX + innerY*newW + 4].b = diffVec[3*(diffX + diffY*diffW)+2];
                        // increment diffX every time we increment through the inner block.
                        ++diffX;
                    }
                    else {
                        // grab pixel from the old image
                        newImgVec[innerX + innerY*newW].r = oldImgVec[ 3*(x * loFactor/hiFactor + (innerX - x) + oldImgW * (y * loFactor/hiFactor + (innerY - y)))];
                        newImgVec[innerX + innerY*newW].g = oldImgVec[ 3*(x * loFactor/hiFactor + (innerX - x) + oldImgW * (y * loFactor/hiFactor + (innerY - y))) + 1];
                        newImgVec[innerX + innerY*newW].b = oldImgVec[ 3*(x * loFactor/hiFactor + (innerX - x) + oldImgW * (y * loFactor/hiFactor + (innerY - y))) + 2];
                    }

                }
            }
        }
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
    vector<unsigned char> deltas, diffPixelVec, lowResImage,diff;
    vector <unsigned int> blocksPixelVec;
    unsigned lowResWidth, lowResHeight, highResWidth, highResHeight,lowFactor, highFactor, 
    unitSize, deltaUnitSize, totalDeltaUnits, error;
    ofstream f2;
    // rle decode diff, then put into diffFileVector
    vector<unsigned char> bin;

    ifstream f("diff.dat", ios::binary | ios::in);
    char c;
    while (f.get(c)) {
        // for (int i = 7; i >= 0; 
        for (int i = 0; i < 8; i++)  //if you want reverse bit order in bytes
           diff.push_back((c >> i) & 1);
    }


    cout<<"after loop"<<endl;

    cout<<diff.size()<<endl;
    // diff = decodeRLE(diff);

    cout<<"successfully decoded"<<endl;
    if (!checkFileHeader(diff)){
        cerr<<"File is not in DIFF format"<<endl;
        return;
    }

    extractHeader(diff, highResWidth, highResHeight);

    cout<<highResWidth<<endl;
    cout<<highResHeight<<endl;

    cout<<lowResFileName<<endl;
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

    diffPixelVec.reserve(3*totalDeltaUnits*deltaUnitSize);
    blocksPixelVec.reserve(3*totalDeltaUnits*deltaUnitSize);

    //iterate blocks
    // assuming aspect ratio is 16:9
    // assuming basic block configuration
    int numDeltaPixelsPerBlock = unitSize - highResWidth*highResHeight*lowFactor*lowFactor/(16*9*highFactor*highFactor);
    // THIS SHOULD BE 3 SINCE WE ARE RESIZING 2:1

    getPixels(blocksPixelVec, diff, lowResImage, highResWidth, highResHeight, deltaUnitSize, numDeltaPixelsPerBlock, highFactor, lowFactor, lowResWidth);
cout<<"Gotpixels"<<endl;
    populateDiffPixelVec(diffPixelVec, blocksPixelVec, deltaUnitSize, highResWidth, highResHeight);
cout<<"populated diff pix vec"<<endl;

    expand_image(lowResImage, lowResWidth, lowResHeight, diffPixelVec, lowFactor, highFactor, deltaUnitSize);

}





