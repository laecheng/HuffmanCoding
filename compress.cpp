#include "HCTree.hpp"
#include <iostream>                                                                
#include <algorithm>                                                               
#include <vector>                                                                  
#include <fstream>                                                                  
#include <string>                                                                   
#include "BitOutputStream.hpp"
#include "BitInputStream.hpp"


using namespace std;                                                            
int main(int argc, char* argv[]) {  

  if(argc != 3) {
  
    cout << "Invalid number of arguments.\n"
         << "Usage: ./compress inputFile outputFile.\n";
    return -1;
  }

  // Open the input file
  ifstream input;
  input.open(argv[1], ios::binary);
  unsigned char nextChar;

  // Check if input file was actually opened
  if(!input.is_open()){
  
    cout << "Invalid input file. No file was opened. Please try again.\n";
    return -1;
  }

  // Check for empty file
  input.seekg(0, ios_base::end);
  unsigned int len = input.tellg();
  if(len == 0){    
  
    ofstream output;
    output.open(argv[2], ios::binary);

    // Check if output file was actually opened
    if(!output.is_open()){
  
    cout << "Invalid output file. No file was opened. Please try again.\n";
    return -1;
    }
    output.close();
    return 0;
  }
  
  // Resets the stream to the begining of file
  input.seekg(0, ios_base::beg);
  
  std::vector<int> freq (256, 0);
  
  // use a while loop to count frequence 
  while(1){
    
    nextChar = (unsigned char)input.get();
    if(input.eof())  break;      // if end of file, then break
    freq[(int)nextChar] = freq[(int)nextChar]+1;
  }

  input.close();

  // create a output stream and open the output file
  ofstream output;
  BitOutputStream bitOut (output);
  output.open(argv[2], ios::binary);
  
  // Check if output file was actually opened
  if(!output.is_open()){
    
    cout << "Invalid output file. No file was opened. Please try again.\n";
    return -1;
  }	

  // construct a HCTree with input frequency 
  HCTree* tree = new HCTree();
  tree->build(freq);
   
  // open the input and count the size of the input file
  input.open(argv[1], ios::binary);
  int count = 0;
  while(1){
  
	input.get();
	if(!(input.good())){
	  break;
	}
	count++;
  }
  input.close();

  //  check if the input file only one unique character
  if(tree->getSingleChar(freq) == 1){
    bitOut.writeBit(1);
  }
  else {
    bitOut.writeBit(0);
  }

  // write the size of file into the output
  for(int i = 31; i >=0; i--){
	  bitOut.writeBit((count >> i) & 1);
  }

  // store the tree stucture in a postorder approach
  tree->storeTree(tree->getRoot(), bitOut);
  
  // Open the input file again
  input.open(argv[1], ios::binary);

  // use while loop to read in ASCII character in order and call decode to
  // get the decoded information
  while(1){
    
    nextChar = (unsigned char)input.get();
    if(input.eof())  break;      // if end of file, then break
    tree->encode(nextChar, bitOut);
  }
  bitOut.flush();

  // close all the stream and free memory allocated for HCTree
  input.close();
  output.close();
  delete tree;

  return 0;

}

