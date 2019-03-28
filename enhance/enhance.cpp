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
    
    int diffPos = 96, rangeSize, offset, refR, refG, refB, blockW, blockH;
    int r, g, b;
    setBlockInfo(blockW, blockH, highResImgW, highResImgH);

    for (int blockY = 0; blockY < highResImgH - blockH; blockY+= blockH){
        for (int blockX = 0; blockX < highResImgW - blockW; blockX+= blockW){

            rangeSize = binToInt(getBits(diff, diffPos, 8));
            cout<<"rangeSize:"<<rangeSize<<endl;
            cout<<"diffpos:"<<diffPos<<endl;
            diffPos += 8;
            offset = binToSignedInt(getBits(diff, diffPos, 8));
            cout<<"offset"<<offset<<endl;
            diffPos += 8;
//            unsigned int nPix = binToInt(getBits(diff, diffPos, 32));
//            int lim = nPix*rangeSize*3 + diffPos + 32;
//            diffPos += 32;

            for (int deltaY = blockY; deltaY < blockY + blockH; deltaY += highFactor){
                int deltaYCpy = deltaY;
                for (int deltaX = blockX + highFactor - 1; deltaX < blockX + blockW; deltaX += highFactor) {

                    int deltaXCpy = deltaX;
                    for (int unit = 0; unit < deltaUnitSize; unit++){
                        // cout<<diffPos<<endl;


                        if (unit == deltaUnitSize - 1){




                            refR = lowRes[ 3*((deltaXCpy - 1) * lowFactor/highFactor 
                            + lowResImgW * (deltaYCpy-1) * lowFactor/highFactor)];

                            refG = lowRes[ 3*((deltaXCpy - 1) * lowFactor/highFactor 
                            + lowResImgW * (deltaYCpy-1) * lowFactor/highFactor ) + 1];

                            refB = lowRes[ 3*((deltaXCpy-1) * lowFactor/highFactor
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

                            refR = lowRes[ 3*((deltaXCpy-1) * lowFactor/highFactor
                            + lowResImgW * (deltaYCpy) * lowFactor/highFactor)];

                            refG = lowRes[ 3*((deltaXCpy-1) * lowFactor/highFactor
                            + lowResImgW * (deltaYCpy) * lowFactor/highFactor ) + 1];

                            refB = lowRes[ 3*((deltaXCpy-1) * lowFactor/highFactor
                            + lowResImgW * (deltaYCpy ) * lowFactor/highFactor) + 2];

                            deltaXCpy +=1;
                        }

                        if (refR > 255 || refG > 255 || refB > 255 || refR < 0 || refG < 0 || refB < 0)
                        cout<<refR<<":"<<refG<<":"<<refB<<endl;

                        if (offset != 0) {
                            cout << "here" << std::endl;
                        }
                       


                        r = refR + offset + binToSignedInt(getBits(diff, diffPos, rangeSize));
                        if (r>255 || r<0){
                            //std::cout << binToSignedInt(getBits(diff, diffPos, rangeSize)) << std::endl;
                            //std::cout << "r: " << r << std::endl;
                            r=255;
                        }
                        diffPos += rangeSize;

                        g = refG + offset + binToSignedInt(getBits(diff, diffPos, rangeSize));
                        if (g>255 || g<0) {
                            //std::cout << binToSignedInt(getBits(diff, diffPos, rangeSize)) << std::endl;
                            //std::cout << "g: " << g << std::endl;
                            g = 255;
                        }
                        diffPos += rangeSize;

                        b = refB + offset + binToSignedInt(getBits(diff, diffPos, rangeSize));
                        if (b>255 || b<0) {
                            //std::cout << binToSignedInt(getBits(diff, diffPos, rangeSize)) << std::endl;
                            //std::cout << "b: " << b << std::endl;
                            g=255;
                        }
                        diffPos += rangeSize;


                        if (r>255 || r< 0){
                            std::cout << "r: " << r << std::endl;
                        }
                        if (g>255|| g< 0) {
                            std::cout << "g: " << g << std::endl;
                            g = 255;
                        }
                        if (b>255|| b< 0) {
                            std::cout << "b: " << b << std::endl;
                        }
                        pixels.push_back(r);

                        pixels.push_back(g);

                        pixels.push_back(b);
//                        if (diffPos > lim) {
//                            std::cout << "here" << std::endl;
//                        }


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
}


void populateDiffPixelVec(std::vector<unsigned int> &diffPixelVec, std::vector<unsigned int> &blocksPixelVec, unsigned deltaUnitSize, unsigned highResWidth, unsigned highResHeight) {
    // assuming is RANGE 
    // assuming default config for a 16:9 image
    unsigned blockDataSize = highResHeight*highResWidth / (16*9);
    unsigned numBlocks = 16*9;

    cout<<blocksPixelVec.size()<<endl;
    std::vector<unsigned int> blockPosVec;
    for (int i=0; i<numBlocks; ++i) {
        blockPosVec.push_back(i * blockDataSize/4 * 3); // convert to number of diff pixels per unit
    }

    std::vector<blockParams> bPs;
    populateBlocks(bPs, 1960, 1080, 2);

    auto bt = bPs.begin();

    for (auto it=blockPosVec.begin(); it<blockPosVec.end(); ++it) {

        unsigned blockBegin = *it;
        unsigned blockEnd = *(std::next(it,1));
        // tl, br are in UNITS not PIXELS
        int diffPixPos = (bt->tl.x + bt->tl.y * highResWidth/2)*deltaUnitSize*3;
        int blockW = blockEnd - blockBegin;
        //int blockH = bt->br.y - bt->br.y;

        if (bt->br.x < bt->tl.x) {
            throw std::logic_error("error: br.x < tl.x");
        }

        auto ct = diffPixelVec.begin() + diffPixPos;

        for (int i=blockBegin; i<blockEnd; i+=blockW) {
            // in delta unit, but we don't care since it's iterating through each pixel anyways..
            // it is reading from blocksPixelVec, ct is where you are putting the pixels in diffPixelVec

            //std::cout << blocksPixelVec[i] << " at " << i << " of " << blocksPixelVec.size() << " into " << ct - diffPixelVec.begin() << " in diffPixelVec" << std::endl;
            diffPixelVec.insert(ct, blocksPixelVec.begin() + i, blocksPixelVec.begin() + i + blockW);

            ct += (highResWidth/2 - blockW)*deltaUnitSize*3;
        }
        ++bt;
    }
    std::cout << diffPixelVec.size() << std::endl;
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

void expand_image( std::vector<unsigned char> oldImgVec, unsigned oldImgW, unsigned oldImgH, std::vector<unsigned int> &diffVec, unsigned loFactor, unsigned hiFactor, int diffW) {


    unsigned newW = oldImgW * hiFactor / loFactor;
    unsigned newH = oldImgH * hiFactor / loFactor;

    std::vector<Color> newImgVec(newW * newH * 3);
    std::cout << diffVec.size() / diffW << std::endl;
    std::cout << 2160 * 3840 / 4;

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
    vector<unsigned char> deltas, lowResImage,diff;
    vector <unsigned int> blocksPixelVec,diffPixelVec;
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

    diffPixelVec.resize(3*totalDeltaUnits*deltaUnitSize, 0);
    blocksPixelVec.reserve(3*totalDeltaUnits*deltaUnitSize);


    //iterate blocks
    // assuming aspect ratio is 16:9
    // assuming basic block configuration
    int numDeltaPixelsPerBlock =  3;
    // THIS SHOULD BE 3 SINCE WE ARE RESIZING 2:1

    getPixels(blocksPixelVec, diff, lowResImage, highResWidth, highResHeight, deltaUnitSize, numDeltaPixelsPerBlock, highFactor, lowFactor, lowResWidth);
cout<<"Gotpixels"<<endl;
    populateDiffPixelVec(diffPixelVec, blocksPixelVec, deltaUnitSize, highResWidth, highResHeight);
cout<<"populated diff pix vec, size "<< diffPixelVec.size()<<endl;
    std::cout << "reserved " << 3*totalDeltaUnits*deltaUnitSize << std:: endl;


    expand_image(lowResImage, lowResWidth, lowResHeight, diffPixelVec, lowFactor, highFactor, deltaUnitSize);

}





