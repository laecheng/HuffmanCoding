#ifndef BitInputStream_HPP
#define BitInputStream_HPP
#include <iostream>

using namespace std;


class BitInputStream {

private:
  unsigned char buf;  // one byte buffer of bits 
  int nbits;          // how many bits have been read from buf
  istream & in;       // the input stream to use

public:

  /** constructor for BitInputStream object */
  BitInputStream(istream & is): in(is) {
    
    buf = 0;    // initilize all the fields
    nbits = 8;
  }

  /** fill the buffer from the input */
  void fill();

  /** read the next bit from the bit buffer 
   *  fill the buffer from the input stream first if needed
   *  return 1 if read 1
   *  return 0 if read 0
   * */
  int readBit();
};

#endif
