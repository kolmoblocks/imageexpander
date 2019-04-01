#include "generateDiff.h"
using namespace std;

FILE *f;
const int TYPE_MAP = 0, TYPE_RANGE = 1;


void populateBlocks(std::vector<blockParams> &blocks, std::vector<deltaUnit> &units, int width, int height, int highFactor) {
    // logic here to statisticallly determine "good" configuration of blocks
    int xincr = width/16, yincr=height/9;

    for (int j=0; j<height; j+=yincr) {
        for (int i=0; i<width; i+=xincr) {
            blocks.push_back(blockParams{posn{i/highFactor,j/highFactor}, posn{(i+xincr)/highFactor-1, (j+yincr)/highFactor-1}, 'R'});
        }
    }
    // for (auto it : blocks) {
    //     std::cout << it.tl.x << " " << it.tl.y << " " << it.br.x << " " << it.br.y << std::endl;
    // }
}

void populateDeltas(std::vector<unsigned char> &image, int width, int height, int highFactor, int lowFactor, std::vector<deltaUnit> &units) {
    // calculate the delta unit length from default block configuration
    int deltaUnitLength = highFactor*highFactor - lowFactor*lowFactor;
    
    if (deltaUnitLength < 0) {
        throw std::logic_error("delta unit length less than 0");
    }


    cout<<"ref : "<<(int)image.at((1147 + 175 * width) * 3)<<endl;
//    cout<<"delta : "<<(int)image.at((17 + 175 * width) * 3)<<endl;



    // loop across each block
    for (int y=0; y<height; y += highFactor) {
        for (int x=0; x<width; x+= highFactor) {
            int r, g, b;
            deltaUnit curDeltaUnit(deltaUnitLength);
            curDeltaUnit.setMax(Color{-255,-255,-255});
            curDeltaUnit.setMin(Color{255,255,255});

            // loop into each block
            for (int innerY = y; innerY < y+highFactor; ++innerY) {
                for (int innerX = x; innerX < x+highFactor; ++innerX) {
                    // only get and set pixel delta if the block is not included in the old block (for now it is the top left smaller square with sides of length "lowFactor")
                    if (innerX >= x+lowFactor || innerY >= y+lowFactor) {
                        // deltaColor is the delta to be pushed to the delta unit, deltaDonor is the Color that the delta is set relative to
                        Color deltaColor, donor;

                        // current loop-specified Color
                        Color curColor{image.at((innerX+innerY*width)*3),
                                        image.at((innerX+innerY*width)*3+1),
                                        image.at((innerX+innerY*width)*3+2)};

                        // setting donor pixel logically as top, left, or top-left pixel relative to current pixel.
                        if (innerX == innerY){
                            donor = {image.at(((innerX-1)+(innerY-1)*width)*3),
                                        image.at(((innerX-1)+(innerY-1)*width)*3+1),
                                        image.at(((innerX-1)+(innerY-1)*width)*3+2)};
                        } else if (innerX > innerY){
                            donor = {image.at(((innerX-1)+innerY*width)*3),
                                        image.at(((innerX-1)+innerY*width)*3+1),
                                        image.at(((innerX-1)+innerY*width)*3+2)};
                        } else {
                            donor = {image.at((innerX+(innerY-1)*width)*3),
                                        image.at((innerX+(innerY-1)*width)*3+1),
                                        image.at((innerX+(innerY-1)*width)*3+2)};
                        }
                        
                        deltaColor = donor - curColor;
                        if (deltaColor.r==63){
                            cout<<innerY<<":"<<innerX<<endl;
                        }

                        // push delta Color to the unit
                        curDeltaUnit.push_back(deltaColor);

                        curDeltaUnit.setMax(max(deltaColor, curDeltaUnit.getMax()));
                        curDeltaUnit.setMin(min(deltaColor, curDeltaUnit.getMin()));
                    }
                }
            }

            units.push_back(curDeltaUnit);
        }
    }
}


std::vector<unsigned char> generateDiff (const char *lowRes, const char *highRes){
    std::vector<unsigned char> lowResImage;
    std::vector<unsigned char> highResImage;
    std::vector<unsigned char> diff;

    // diff.reserve( 100000 );

    unsigned lowResWidth, lowResHeight, highResWidth, highResHeight;

    unsigned error = lodepng::decode(lowResImage, lowResWidth, lowResHeight, lowRes, LCT_RGB, 8);
    if (error) {
        std::cout << error;
        return diff;
    }

    error = lodepng::decode(highResImage, highResWidth, highResHeight, highRes, LCT_RGB, 8);
    if (error) {
        std::cout << error;
        return diff;
    }

    unsigned denom = gcd(lowResWidth, highResWidth);
    // finding numerator and denominator of ratio (lowFactor, highFactor respectively)
    unsigned int lowFactor = lowResWidth / denom;
    unsigned int highFactor = highResWidth / denom;
    // finding diffWidth from the difference between smaller vs newer chunk sizes
    int diffWidth = highFactor * highFactor - lowFactor * lowFactor;
    int highResArea = highResWidth * highResHeight;

    const unsigned int height = highResHeight;
    const unsigned int width  = highResWidth;

    // iterating through blocks, x and y indicate the top left positions of each block.
    // get units from pixels somehow
    std::vector<deltaUnit> units;
    std::vector<blockParams> blocksConfig;
    populateDeltas(highResImage, highResWidth, highResHeight, highFactor, lowFactor, units);
    populateBlocks(blocksConfig, units, highResWidth, highResHeight, highFactor);
    cerr<<"populated blocks"<<endl;
    insertDiffHeader(diff, highResWidth, highResHeight, "RGB");
    cerr<<"diff header inserted"<<endl;
    int offset, rangeSize;
    for (auto block : blocksConfig) {
        // iterating through inner block pixels, innerX and innerY indicate the current position of the block we are at.
        int numPixels = (block.br.x - block.tl.x + 1 )*(block.br.y - block.tl.y + 1)*units[0].size();

        blockIterator it{units, block.tl, block.br, highResWidth/highFactor};
        Color maxDelta{-255,-255,-255}, minDelta{255,255,255};

        while (!it.end()) {
            maxDelta = max(*it, maxDelta);
            minDelta = min(*it, minDelta);
            ++it;
        }

        it.reset();
        //move to helper function to abstract for all streams
        int minLim = min(min(minDelta.r, minDelta.g), minDelta.b);
        int maxLim = max(max(maxDelta.r, maxDelta.g), maxDelta.b);
        //depending on config block - use either r or m
        if (block.type == 'R') {
            if (maxLim == 0){
                maxLim = 1;
            }
            if (maxLim < 0 ) cout<<maxLim<<endl;

            maxLim = max(maxLim, abs(minLim));

            float power = log(maxLim)/log(2); // get the number of bits needed then + 1 for sign
            rangeSize = (int)floor(power) + 2; //+1 for ceil and 1 for signed binary


            offset = 0;

//            if (maxLim >= 0 || (minLim <= 0 && maxLim <= 0)) {
//                offset = minLim;
//            } else {
//                offset = minLim;
//            }


            insertBlockHeader(diff,TYPE_RANGE, rangeSize, offset, numPixels);
            insertRangeBlock(diff, it, rangeSize, offset);
        }
        else if (block.type == 'M') {
            // insertMapBlock(diff, it, minDelta.r, maxDelta.r);
        }

    }
    std::cout << lodepng_error_text(error) << std::endl;
    return diff;
}




void insertDiffHeader(std::vector<unsigned char> &diff, unsigned int targetWidth, unsigned int targetHeight, string colormode){
        vector<unsigned char> v;
        v = intToUnsignedBin('D', 8);

        diff.insert(diff.end(),v.begin(), v.end());
        v = intToUnsignedBin('I', 8);

        diff.insert(diff.end(),v.begin(), v.end());
        v = intToUnsignedBin('F', 8);

        diff.insert(diff.end(),v.begin(), v.end());

        diff.insert(diff.end(),v.begin(), v.end());
        v = intToUnsignedBin(targetWidth, 32);

        diff.insert(diff.end(),v.begin(), v.end());
        v = intToUnsignedBin(targetHeight, 32);

        diff.insert(diff.end(),v.begin(), v.end());
//        v = intToUnsignedBin('R', 8);
//        diff.insert(diff.end(),v.begin(), v.end());
//        v = intToUnsignedBin('G', 8);
//        diff.insert(diff.end(),v.begin(), v.end());
//        v = intToUnsignedBin('B', 8);
//        diff.insert(diff.end(),v.begin(), v.end());
//        v = intToUnsignedBin(' ', 8);
//        diff.insert(diff.end(),v.begin(), v.end());

        // fwrite(&w, w.length(), 1, f);
        // fwrite(&h, h.length(), 1, f);
        // diff.push_back();
        // fwrite(&targetWidth, sizeof(unsigned int), 1, f);
        // fwrite(&targetHeight, sizeof(unsigned int), 1, f);
        // fwrite(&colormode, 4,1,f);
}

void insertBlockHeader(vector<unsigned char> &diff, int type, int rangeSize, int offset, int numPixels){
    if (type == TYPE_MAP) {
//
    } else if (type == TYPE_RANGE) {

//        vector<unsigned char> typeV = {0,0,0,0,0,0,0,1};
//        diff.insert(diff.end(),typeV.begin(), typeV.end());
        vector<unsigned char> rangeSizeV = intToBin(rangeSize,8);
//        for(auto it: rangeSizeV){
//            cout<<(int)it;
//        }
//
//        cout<<endl;
        vector<unsigned char> offsetV = intToBin(offset,8);
//        vector<unsigned char> numPixVec = intToUnsignedBin(numPixels, 32);


        diff.insert(diff.end(),rangeSizeV.begin(), rangeSizeV.end());



        diff.insert(diff.end(), offsetV.begin(), offsetV.end());
//        diff.insert(diff.end(), numPixVec.begin(), numPixVec.end());
        //RGB header
    }


}
