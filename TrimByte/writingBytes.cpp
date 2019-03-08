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
		fclose (f); 

}


vector<char> LTrim_Zeroes(int c) {
	vector<char> v;
	float lenF = log(c)/log(2);
	int i = (int)floor(lenF) + 1;
	cout<<i;
	while(i >  0){
		if ((c & (1 << i)) != 0) {
			v.push_back(1);
			i--;
			break;
		}
		i--;
	}
	for ( ; i >= 0 ; i--) {
		v.push_back((c & (1 << i)) != 0);
	}
	return v;
}

int main (){
	vector<char> v = LTrim_Zeroes(250);
	for (auto it : v){
		cout<<it;
	}
	cout<<endl;
		
}
	 
	 //xxd -b random.dat

