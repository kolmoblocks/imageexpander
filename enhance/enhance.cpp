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
        cout<<(int)buffer[i];
    }
    cout<<endl;
    return res;
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


void populateDiffPixelVec(FILE *pFile, std::vector<unsigned char> &diffPixelVec, std::vector<unsigned char> &diffEncodedVec, unsigned deltaUnitSize, unsigned highResWidth, unsigned highResHeight) {
    // assuming is RANGE
    // assuming default config for a 16:9 image
    unsigned blockdataSize = highResHeight*highResWidth / (16*9);
    unsigned numBlocks = 16*9;
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




void enhance(char *lowResFileName, char *diffFileName) {
    FILE *pDiff;
    pDiff = fopen(diffFileName, "rb");
    vector<unsigned char> deltas, diffPixelVec, lowResImage,diff;
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
    diff = decodeRLE(diff);

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
    // diff.reserve(3*totalDeltaUnits*deltaUnitSize);

    //iterate blocks
    // assuming aspect ratio is 16:9
    // assuming basic block configuration
    int numDeltaPixelsPerBlock = unitSize - highResWidth*highResHeight*lowFactor*lowFactor/(16*9*highFactor*highFactor);
    // THIS SHOULD BE 3 SINCE WE ARE RESIZING 2:1


    std::vector<unsigned char> finalImage;
    unsigned oldImgH, oldImgW;
    int diffX = 0, diffY = 0, counter = 128, deltaCount =0;
    int rangeSize, offset;


   
    unsigned newW = oldImgW * highFactor / lowFactor;
    unsigned newH = oldImgH * highFactor / lowFactor;
    std::vector<Color> newImgVec(newW * newH * 3);

    f2.open("range2.data", std::ios_base::app);

    for (std::size_t y=0; y<newH; y+= highFactor) {
        for (std::size_t x=0; x<newW; x+= highFactor) {
            // iterating through inner block pixels, innerX and innerY indicate the current position of the block we are at.
            // increment diffY, reset diffX to 0 each time we go to a new block
            ++diffY;
            diffX = 0;

            //rangesize
            cout<<numDeltaPixelsPerBlock<<endl;
            if (deltaCount == 0 || deltaCount == numDeltaPixelsPerBlock){
                deltaCount = 0;
                  bin = getBits(diff, counter, 8);
                counter += 8;
                rangeSize = binToInt(bin);
                //offset
                bin = getBits(diff, counter, 8);
                counter += 8;
                offset = binToSignedInt(bin);
                cout << rangeSize<<":"<<offset<<endl;

                f2 << rangeSize<<":"<<offset<<endl;
            }
          
    
            for (int innerX = x; innerX < x+highFactor; ++innerX) {
                for (int innerY = y; innerY < y+highFactor; ++innerY) {
                    // only get and set pixel if the block is not included in the old block (for now it is the top left smaller square with sides of length "loFactor")
                    if (!(innerX < x+lowFactor) || !(innerY < y+lowFactor)) {
                        // grab pixel from the diff
                            // newImgVec[innerX + innerY*newW + 4].r = diff[];
                            // newImgVec[innerX + innerY*newW + 4].g = diff[];
                            // newImgVec[innerX + innerY*newW + 4].b = diff[];
                        // increment diffX every time we increment through the inner block.
                        ++diffX;
                        counter+= rangeSize * 3;
                        deltaCount += 3;
                    }
                    else {
                        // grab pixel from the old image
                            finalImage.push_back(lowResImage[ 3*(x * lowFactor/highFactor + (innerX - x) + oldImgW * (y * lowFactor/highFactor + (innerY - y)))]);
                            finalImage.push_back(lowResImage[ 3*(x * lowFactor/highFactor + (innerX - x) + oldImgW * (y * lowFactor/highFactor + (innerY - y))) + 1]);
                            finalImage.push_back(lowResImage[ 3*(x * lowFactor/highFactor + (innerX - x) + oldImgW * (y * lowFactor/highFactor + (innerY - y))) + 2]);
                    }
                }
            }
        }
   }
    lodepng::encode("final.png",finalImage,newW,newH,LCT_RGB,8);
    f2.close();

}


