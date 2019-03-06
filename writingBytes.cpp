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



void Flush_Bits(){
		while (current_bit < 8) {
				bit_buffer = (bit_buffer<<1);
					current_bit++;
		}
		fwrite (&bit_buffer, 1, 1, f);
		fclose (f); 

}

int bits[8];
		vector<int> v;


void Trim_Leading(int bits) {
		unsigned char c = 2;		

		int i =8;
		while(i >= 8 - bits){
				if ((c & (1 << i)) != 0) {
					v.push_back(1);
					i--;
					break;
				}
				i--;
		}
		for ( ; i >= 0 ; i--) {
						cout<<i;

				v.push_back((c & (1 << i)) != 0);
		}



}

int main (){
		Trim_Leading(6);
		cout<<endl;
		for (auto it : v){
				cout<<it;
		}
		cout<<endl;
		//need to fisll the 0bits from the right and not left
		
		//Flush_Bits();
}
	 
	 //xxd -b random.dat
