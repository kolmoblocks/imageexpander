#include "lodepng.h"
#include "rangeDiff/rangeDiff.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <unordered_set> 
#include <numeric> 
#include <string>
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
    unsigned char r, g, b;
} color;

class delUnit {
    int len;
    color *colors;
public:
    delUnit(int len): len{len} {
        colors = malloc(len*sizeof(color));
    }
    ~delUnit() {
        free(colors);
    }

};

FILE *f;
int current_bit = 0;
unsigned char bit_buffer;

vector<unsigned char> lTrimZeroes(int c, int range) {
	vector<unsigned char> v;
    bool leadingZero = true;

    for (int i = range; i >= 0; i--){
        if (leadingZero){
            if ((c & (1 << i)) != 0) {
                v.push_back(1);
                leadingZero = false;
		    }  
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
    fclose (f); 

}


void flushBits(){
    while (current_bit < 8) {
        bit_buffer = (bit_buffer<<1);
          current_bit++;
    }
    fwrite (&bit_buffer, 1, 1, f);
    fclose (f); 

}

void populateBlocks(std::vector<blockParams> &blocks, std::vector<delUnit> &units) {
    // logic here to statisticallly determine "good" configuration of blocks

    blocks.push_back(blockParams{posn{0,0},posn{20,20},'R'};
    blocks.push_back(blockParams{posn{21,21},posn{25,25},'M');
}


vector<short int> generateDiff (const char *lowRes, const char *highRes,  int height, int width, int x, int y){
    f = fopen ("random.dat", "w");
    std::vector<unsigned char> lowResImage;
    std::vector<unsigned char> highResImage;
    std::vector<short int> diff;

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
    vector<int> block;
    unordered_set<int> deltaSet;
    int deltaR, deltaG,deltaB;
    // iterating through blocks, x and y indicate the top left positions of each block.
    // get units from pixels somehow
    std::vector<delUnit> units;
    std::vector<blockParams> blocks;
    populateBlocks(blocks, units);

    for (std::size_t y=0; y<height; y+= highFactor) {
        for (std::size_t x=0; x<width; x+= highFactor) {
            // iterating through inner block pixels, innerX and innerY indicate the current position of the block we are at.
           
            block.reserve(10);
            int maxDelta=-255, minDelta=255; 
            int r, g, b;

            for (int innerX = x; innerX < x+highFactor; ++innerX) {
                for (int innerY = y; innerY < y+highFactor; ++innerY) {
                // only get and set pixel if the block is not included in the old block (for now it is the top left smaller square with sides of length "lowFactor")
                    if (!(innerX < x+lowFactor) || !(innerY < y+lowFactor)) {
                        // set pixel of the diff at diffX , diffY with the color at the highResImage at innerX , innerY


                        r = (int)highResImage.at((innerX+innerY*highResWidth)*3);
                        g = (int)highResImage.at((innerX+innerY*highResWidth)*3+1);
                        b = (int)highResImage.at((innerX+innerY*highResWidth)*3+2);

                        //get deltas
                        //get reference pixel with formula


                        if (innerX == innerY){
                            deltaR = (int)highResImage.at(((innerX-1)+(innerY-1)*highResWidth)*3)   - r;
                            deltaG = (int)highResImage.at(((innerX-1)+(innerY-1)*highResWidth)*3+1) - b;
                            deltaB = (int)highResImage.at(((innerX-1)+(innerY-1)*highResWidth)*3+2) - g;
                        } else if (innerX > innerY){
                            deltaR = (int)highResImage.at(((innerX-1)+innerY*highResWidth)*3)   - r;
                            deltaG = (int)highResImage.at(((innerX-1)+innerY*highResWidth)*3+1) - g;
                            deltaB = (int)highResImage.at(((innerX-1)+innerY*highResWidth)*3+2) - b;
                        } else {
                            deltaR = (int)highResImage.at((innerX+(innerY-1)*highResWidth)*3)   - r;
                            deltaG = (int)highResImage.at((innerX+(innerY-1)*highResWidth)*3+1) - b;
                            deltaB = (int)highResImage.at((innerX+(innerY-1)*highResWidth)*3+2) - g;
                        }
                        
                        
                        deltaSet.insert(deltaR);
                        deltaSet.insert(deltaG);
                        deltaSet.insert(deltaB);

                        deltas.push_back(deltaR);
                        deltas.push_back(deltaG);
                        deltas.push_back(deltaB);
                        
                        maxDelta = max(maxDelta,max(deltaR,deltaG,deltaB));
                        minDelta = min(minDelta,min(deltaR,deltaG,deltaB));
                       

                    }
                }
            }

            
            //move to helper function to abstract for all streams
            float power = log(maxDelta)/log(2); // get the number of bits needed then + 1 for sign
	        int rangeSize = (int)floor(power) + 2; //+1 for ceil and 1 for signed binary
            int offset;
            if (minDelta >= 0 || minDelta <= 0 && maxDelta <= 0) {
                offset = minDelta;
            } else {
                offset = (minDelta + maxDelta) / 2;
            }
            
            //depending on config block - use either r orm

            insertRangeBlock(diff, deltas, rangeSize, offset);


            // diff.insert( diff.end(), block.begin(), block.end() );
            deltas.clear();
            deltaSet.clear();
        }
    }

   std::cout << lodepng_error_text(error) << std::endl;
    fclose (f); 
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
        fclose(f);
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
