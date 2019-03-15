#include <fstream>
int current_bit = 0;
unsigned char bit_buffer;
using namespace std;
#include <iostream>
#include <vector>
#include <cmath>


FILE *f;

void WriteBit (int bit) {
 	f = fopen ("random.dat", "w");
	if (bit) bit_buffer |= (1<<current_bit);
	current_bit++;
	
	if (current_bit == 8) {
		fwrite (&bit_buffer, 1, 1, f);
		current_bit = 0;
		bit_buffer = 0;
	}
}



void Flush_Bits(){
		while (current_bit < 8) {
			bit_buffer = (bit_buffer<<1);
			current_bit++;
		}
		fwrite (&bit_buffer, 1, 1, f);
}


vector<unsigned char> intToBin(int c, int range) {
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

int main (){
	vector<unsigned char> v = intToBin(13, 4);
	for (auto it : v){
		cout<<(int)it;
	}
	cout<<v[0];
	cout<<endl;


		
}
	 
	 //xxd -b random.dat

