#include "generateDiff.h"

int main(int argc, char *argv[]){
   // argv[1]: smaller file
    FILE *diffFile = fopen ("diff.dat", "w");
    vector<unsigned char> diff = generateDiff(argv[1], argv[2]);
    writeBitsToFile(diff, diffFile);
    fclose(diffFile);
    //writeDiffHeader(1920,1080,"RGB");
    // readBlock();
}
