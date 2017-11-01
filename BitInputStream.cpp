#include <iostream>
#include "BitInputStream.hpp"

using namespace std;


/** fill the buffer from the input */
void BitInputStream::fill() {

  buf = in.get();
  nbits = 0;
}

/** read the next bit from the bit buffer 
  *  fill the buffer from the input stream first if needed
  *  return 1 if read 1
  *  return 0 if read 0
  */
int BitInputStream::readBit() {

  // if all the bits are read
  // fill the buffer again; or fill at the first time
  if (nbits == 8) {
	fill();
  }

  // get bit at appropriate location in the bit buffer, and return 
  // the appropriate index
  int result = (buf & (1 <<(7 - nbits))) >>(7 - nbits);
  nbits++;   // increment the nbits

  return result;
}
