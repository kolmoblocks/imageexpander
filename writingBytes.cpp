#include <fstream>
int current_bit = 0;
unsigned char bit_buffer;

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

int main (){
    WriteBit(0);
    WriteBit(0);
    WriteBit(0);
    WriteBit(0);
    WriteBit(1);


    //need to fill the 0bits from the right and not left
    
    Flush_Bits();
}
   
   //xxd -b random.dat
