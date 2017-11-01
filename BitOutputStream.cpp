#include <iostream>
#include "BitOutputStream.hpp"

using namespace std;


/** Send the buffer to the output, and clear it
 */
void BitOutputStream::flush() {

  out.put(buf);
  out.flush();
  buf = nbits = 0;
}

/** Write the least significant bit of the argument to the bit buffer, 
 * and increment the bit buffer index. But flush the buffer first, 
 * if it is full
 */
void BitOutputStream::writeBit(int i) {

  // first check whether the buffer is full; if so, flush it
  if (nbits == 8) {
	flush();
  }

  // write the least significant bit of i into the current bit of the buffer
  int temp = 1 & i; 
  if (temp == 1) {
    buf = (1 << (7 - nbits++)) | buf;
  }
  else {
    nbits++;
  }
}
