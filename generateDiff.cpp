#include "lodepng.h"
#include "rangeDiff/rangeDiff.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set> 
#include <numeric> 
#include <string>
#include <memory>
#include <error>
const int TYPE_MAP = 0, TYPE_RANGE = 1;

using namespace std;

typedef struct {
    int x, y;
} posn;

typedef struct {
    posn tl, br;
    int type;
} blockParams;

typedef struct {
    short int r, g, b;
} color;

color &operator-(color col1, color col2) {
    return color{col1.r-col2.r, col1.g-col2.g, col1.b-col2.b};
}

color &max(color col1, color col2) {
    // sanity check for maximum delta
    return color{max(col1.r, col2.r), max(col1.g, col2.g), max(col1.b, col2.b)};
}

color &min(color col1, color col2) {
    // sanity check for minimum delta
    return color{min(col1.r, col2.r), min(col1.g, col2.g), min(col1.b, col2.b)};
}

class deltaUnit {
    int len;
    int ct=0;
    color *colorDeltas;
    color maxDeltas;
    color minDeltas;
public:
    deltaUnit(int len): len(len) {
        colors = (color*)malloc(len*sizeof(color));
    }

    deltaUnit(int len, color max, color min): len{len}, maxDeltas{max}, minDeltas{min} {
        colors = (color*)malloc(len*sizeof(color));
    }
    void push_back(color col) {
        if (ct >= len) throw std::logic_error("pushing past specified deltaUnit length");
        colorDeltas[ct] = col;
        ++ct;
    }
    ~deltaUnit() {
        free(colorDeltas);
    }
    void setMax(color col) {
        maxDeltas = col;
    }
    void setMin(color col) {
        minDeltas = col;
    }
    color getMax() {
        return maxDeltas;
    }
    color getMin() {
        return minDeltas;
    }
    int size() {
        return len;
    }
    color &at(int pos) {
        if (pos < len && pos >= 0) {
            return colors[pos];
        }
        throw std::logic_error("requested delta Unit color position is out of range.")
    }
};

class blockIterator {
    std::vector<deltaUnit> &units;
public:
    posn pos, tl, br;
    int unitLength, innerUnitPos=0;
    
    blockIterator(std::vector<deltaUnit> &units, posn tl, posn br, int width): 
        units{units}, pos{tl}, tl{tl}, br{br}, width{width}, unitLength{units[0].size()} {}

    blockIterator &operator++() {
        if (innerUnitPos < deltaUnitLength) {
            ++innerUnitPos;
        }
        else {
            innerUnitPos = 0;
            if (pos.x == br.x) {
                pos.x = tl.x;
                ++pos.y;
            }
            else if (pos.y == br.y) {
                return NULL;
            }
            else {
                ++pos.x;
            }
        }
        return this;
    }

    bool operator!=(blockIterator other) {
        return pos != other.pos;
    }

    color &operator*() {
        return units[pos.y*width + pos.x].at(innerUnitPos);
    }

    void reset() {
        pos = tl;
    }
}

FILE *f;
int current_bit = 0;
unsigned char bit_buffer;

vector<unsigned char> lTrimZeroes(int c, int range) {
	vector<unsigned char> v;
    bool leadingZero = true;

	if (c < 0) {
            v.push_back(1);
			c *= -1;
         } else {
            v.push_back(0);
         }
    for (int i = range-1; i >= 0; i--){
            if ((c & (1 << i)) != 0) {
                v.push_back(1);
                leadingZero = false; 
        } else{
            v.push_back((c & (1 << i)) != 0);
        }
    }
	return v;
}


void writeBit (int bit) {
  if (bit)
    bit_buffer |= (1<<current_bit);

  current_bit++;
  if (current_bit == 8)
  {
    fwrite (&bit_buffer, 1, 1, f);
    current_bit = 0;
    bit_buffer = 0;
  }
}

void padZeroes(vector <unsigned char> c){
    while (current_bit) {
        writeBit (0);
    }
}


void flushBits(){
    while (current_bit < 8) {
        bit_buffer = (bit_buffer<<1);
          current_bit++;
    }
    fwrite (&bit_buffer, 1, 1, f);

}

void populateBlocks(std::vector<blockParams> &blocks, std::vector<deltaUnit> &units) {
    // logic here to statisticallly determine "good" configuration of blocks

    blocks.push_back(blockParams{posn{0,0},posn{20,20},'R'});
    blocks.push_back(blockParams{posn{21,21},posn{25,25},'M'});
}

void populateDeltas(std::vector<unsigned char> &image, int width, int height, int highFactor, int lowFactor, std::vector<deltaUnit> &units) {
    // calculate the delta unit length from default block configuration
    int deltaUnitLength = highFactor*highfactor - lowFactor*lowfactor;
    
    if (deltaUnitLength < 0) {
        throw std::logic_error("delta unit length less than 0");
    }


    // loop across each block
    for (std::size_t y=0; y<height; y += highFactor) {
        for (std::size_t x=0; x<width; x+= highFactor) {
            int r, g, b;
            deltaUnit curDeltaUnit{deltaUnitLength};
            curDeltaUnit.setMax(color{-255,-255,-255});
            curDeltaUnit.setMin(color{255,255,255});

            // loop into each block
            for (int innerX = x; innerX < x+highFactor; ++innerX) {
                for (int innerY = y; innerY < y+highFactor; ++innerY) {
                
                    // only get and set pixel delta if the block is not included in the old block (for now it is the top left smaller square with sides of length "lowFactor")
                    if (!(innerX < x+lowFactor) || !(innerY < y+lowFactor)) {
                        // deltaColor is the delta to be pushed to the delta unit, deltaDonor is the color that the delta is set relative to
                        color deltaColor, deltaDonor;

                        // current loop-specified color
                        color curColor{image.at((innerX+innerY*width)*3),
                                        image.at((innerX+innerY*width)*3+1)
                                        image.at((innerX+innerY*width)*3+2)};

                        // setting donor pixel logically as top, left, or top-left pixel relative to current pixel.
                        if (innerX == innerY){
                            donor = {highResImage.at(((innerX-1)+(innerY-1)*highResWidth)*3),
                                        highResImage.at(((innerX-1)+(innerY-1)*highResWidth)*3+1),
                                        highResImage.at(((innerX-1)+(innerY-1)*highResWidth)*3+2)};
                        } else if (innerX > innerY){
                            donor = {highResImage.at(((innerX-1)+innerY*highResWidth)*3),
                                        highResImage.at(((innerX-1)+innerY*highResWidth)*3+1),
                                        highResImage.at(((innerX-1)+innerY*highResWidth)*3+2)};
                        } else {
                            donor = {highResImage.at((innerX+(innerY-1)*highResWidth)*3),
                                        highResImage.at((innerX+(innerY-1)*highResWidth)*3+1),
                                        highResImage.at((innerX+(innerY-1)*highResWidth)*3+2)};
                        }
                        
                        deltaColor = donor - curColor;
                        
                        // push delta Color to the unit
                        curDeltaUnit.push_back(deltaUnit);

                        curDeltaUnit.setMax(max(deltaColor, curDeltaUnit.getMax()));
                        curDeltaUnit.setMin(min(deltaColor, curDeltaUnit.getMin()));
                    }
                }
            }

            units.push_back(curDeltaUnit);
        }
    }
}


vector<unsigned char> generateDiff (const char *lowRes, const char *highRes,  int height, int width, int x, int y){
    f = fopen ("random.dat", "w");
    std::vector<unsigned char> lowResImage;
    std::vector<unsigned char> highResImage;
    std::vector<unsigned char> diff;

    diff.reserve( 100000 );

    unsigned int lowResWidth, lowResHeight, highResWidth, highResHeight;

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
    int lowFactor = lowResWidth / denom;
    int highFactor = highResWidth / denom;
    // finding diffWidth from the difference between smaller vs newer chunk sizes
    int diffWidth = highFactor * highFactor - lowFactor * lowFactor;
    int highResArea = highResWidth * highResHeight;

    // finding diffHeight from the number of chunks in a whole highRes image by finding the number of whole chunks (rounded up) for along the height and the width.
    int diffHeight = (highResWidth % highFactor == 0 ? highResWidth / highFactor : highResWidth / highFactor + 1 )
                    * (highResHeight % highFactor == 0 ? highResHeight / highFactor : highResHeight / highFactor + 1 );

    const unsigned int height = highResHeight;
    const unsigned int width  = highResWidth;

    vector<short int> deltas;
    unordered_set<int> deltaSet;
    int deltaR, deltaG,deltaB;
    // iterating through blocks, x and y indicate the top left positions of each block.
    // get units from pixels somehow
    std::vector<deltaUnit> units;
    std::vector<blockParams> blocksConfig;
    populateBlocks(blocksConfig, units);
    
    for (auto block : blocksConfig) {
            // iterating through inner block pixels, innerX and innerY indicate the current position of the block we are at.

            blockIterator begin{units, block.tl, block.br, highResWidth/highfactor};

            //move to helper function to abstract for all streams
            float power = log(maxDelta)/log(2); // get the number of bits needed then + 1 for sign
	        int rangeSize = (int)floor(power) + 2; //+1 for ceil and 1 for signed binary
            int offset;
            if (minDelta >= 0 || minDelta <= 0 && maxDelta <= 0) {
                offset = minDelta;
            } else {
                offset = (minDelta + maxDeltax) / 2;
            }
            
            //depending on config block - use either r or m

            //insertRangeBlock(diff, begin, rangeSize, offset);

            deltas.clear();
        }
    }

   std::cout << lodepng_error_text(error) << std::endl;
}




void writeDiffHeader(unsigned int targetWidth, unsigned int targetHeight, string colormode){
        f = fopen ("diff.dat", "w");
        char ID[] = "DIFF";
        fwrite (ID, 1, 4, f);


        // auto w = to_string(targetWidth);
        // auto h = to_string(targetHeight);
        // fwrite(&w, w.length(), 1, f);
        // fwrite(&h, h.length(), 1, f);
        fwrite(&targetWidth, sizeof(unsigned int), 1, f);
        fwrite(&targetHeight, sizeof(unsigned int), 1, f);
        fwrite(&colormode, 4,1,f);
}


void readBlock(){
    FILE * pFile;
    long lSize;
    char * buffer;
    int x,y;
    size_t result = 0;

    pFile = fopen ( "diff.dat" , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
    fread (buffer,4,1,pFile);
    cout<<buffer<<endl;

    fread (&x,1,4,pFile);
    cout<<x<<endl;

    fread (&y,1,4,pFile);
    cout<<y<<endl;

    fread (buffer,4,1,pFile);
    cout<<buffer<<endl;


 
    fclose (pFile);
    free (buffer);
}


int main(int argc, char *argv[]){
   // argv[1]: smaller file
//    generateDiff(argv[1], argv[2]);
    writeDiffHeader(1920,1080,"RGB");
    readBlock();
}
