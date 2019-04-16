#include "generateDiff.h"
using namespace std;

const int TYPE_MAP = 0, TYPE_RANGE = 1;


void populateBlocks(std::vector<blockParams> &blocks, std::vector<deltaUnit> &units, int width, int height, int highFactor) {
    // logic here to statisticallly determine "good" configuration of blocks
    int xincr = width/16, yincr=height/9;
    for (int j=0; j<height; j+=yincr) {
        for (int i=0; i<width; i+=xincr) {
            blocks.push_back(blockParams{posn{i/highFactor,j/highFactor}, posn{(i+xincr)/highFactor-1, (j+yincr)/highFactor-1}, 'R'});
        }
    }
}


void populateDeltas(std::vector<unsigned char> &image, int width, int height, int highFactor, int lowFactor, std::vector<deltaUnit> &units) {
    // calculate the delta unit length from default block configuration
    int deltaUnitLength = highFactor*highFactor - lowFactor*lowFactor;
    int upperInnerY, upperInnerX, index;
    
    if (deltaUnitLength < 0) {
        throw std::logic_error("delta unit length less than 0");
    }
    // loop across each block
    for (int y=0; y<height; y += highFactor) {
        upperInnerY = y+highFactor;
        for (int x=0; x<width; x+= highFactor) {
            deltaUnit curDeltaUnit(deltaUnitLength);
            curDeltaUnit.setMax(Color{-255,-255,-255});
            curDeltaUnit.setMin(Color{255,255,255});

            // loop into each block
            upperInnerX = x + highFactor;
            for (int innerY = y; innerY < upperInnerY ; ++innerY) {
                for (int innerX = x; innerX < upperInnerX; ++innerX) {
                    // only get and set pixel delta if the block is not included in the old block (for now it is the top left smaller square with sides of length "lowFactor")
                    if (innerX >= x+lowFactor || innerY >= y+lowFactor) {
                        // deltaColor is the delta to be pushed to the delta unit, deltaDonor is the Color that the delta is set relative to
                        Color deltaColor, donor;

                        // current loop-specified Color
                        index = (innerX + innerY * width) * 3;

                        Color curColor{image.at(index),
                                       image.at(index + 1),
                                       image.at(index + 2)};

                        // setting donor pixel logically as top, left, or top-left pixel relative to current pixel.
                        if (innerX-x == innerY-y){
                            index = ((innerX-1)+(innerY-1)*width)*3;
                            donor = {image.at(index),
                                        image.at(index+1),
                                        image.at(index+2)};
                        } else if (innerX-x > innerY-y){

                            index = ((innerX-1)+innerY*width)*3;
                            donor = {image.at(index),
                                        image.at(index+1),
                                        image.at(index+2)};
                        } else {
                            index = (innerX + (innerY - 1) * width) * 3;
                            donor = {image.at(index),
                                     image.at(index + 1),
                                     image.at(index + 2)};

                        }
                        deltaColor = donor - curColor;
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
    std::vector<unsigned char> lowResImage, highResImage, diff;
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


    diff.reserve(highResHeight * lowResWidth * 3 * 2);


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

            offset = (maxLim + minLim) / 2 ;

            float power = log(maxLim - minLim)/log(2); // get the number of bits needed then + 1 for sign
            rangeSize = (int)floor(power) + 2; //+1 for ceil and 1 for signed binary

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
        v = intToBin(false,'D', 8);

        diff.insert(diff.end(),v.begin(), v.end());
        v = intToBin(false,'I', 8);

        diff.insert(diff.end(),v.begin(), v.end());
        v = intToBin(false,'F', 8);

        diff.insert(diff.end(),v.begin(), v.end());

        diff.insert(diff.end(),v.begin(), v.end());
        v = intToBin(false,targetWidth, 32);

        diff.insert(diff.end(),v.begin(), v.end());
        v = intToBin(false,targetHeight, 32);

        diff.insert(diff.end(),v.begin(), v.end());
//        v = intToBin(false,'R', 8);
//        diff.insert(diff.end(),v.begin(), v.end());
//        v = intToBin(false,'G', 8);
//        diff.insert(diff.end(),v.begin(), v.end());
//        v = intToBin(false,'B', 8);
//        diff.insert(diff.end(),v.begin(), v.end());
//        v = intToBin(false,' ', 8);
//        diff.insert(diff.end(),v.begin(), v.end());

}

void insertBlockHeader(vector<unsigned char> &diff, int type, int rangeSize, int offset, int numPixels){
    if (type == TYPE_MAP) {
    } else if (type == TYPE_RANGE) {
//        vector<unsigned char> typeV = {0,0,0,0,0,0,0,1};
//        diff.insert(diff.end(),typeV.begin(), typeV.end());
        vector<unsigned char> rangeSizeV = intToBin(true,rangeSize,8);
        vector<unsigned char> offsetV = intToBin(true,offset,8);
        diff.insert(diff.end(),rangeSizeV.begin(), rangeSizeV.end());
        diff.insert(diff.end(), offsetV.begin(), offsetV.end());
        //RGB header
    }


}
