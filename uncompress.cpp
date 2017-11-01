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

  // check if the user enter less than two input
  if(argc != 3) {
  
    cout << "Invalid number of arguments.\n"
         << "Usage: ./uncompress inputFile outputFile.\n";
    return -1;
  }

  // Open the input file
  ifstream input;
  BitInputStream bitIn (input);
  input.open(argv[1], ios::binary);

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
    
  // create a boolean to record whether there is a only one unique char
  bool isSingleChar = false;
  int singleChar = bitIn.readBit();
 
  // update the singleChar flag
  if(singleChar == 1){
    isSingleChar = true;
  }

  // read in the file size
  int fileSize = 0;
  for( int i = 31; i>=0; i--){
  	fileSize += (bitIn.readBit() << i);
  }

  // construct a HCTree and rebuild the leaves
  HCTree* tree = new HCTree();
  bitIn.readBit();                  // read in the bit for the root
  tree->setRoot(new HCNode(0, 0));
  vector<HCNode*> leaves (256, 0);
  
  // reconstruct the left part of the tree
  tree->constructTree(tree->getRoot(), 0, bitIn, leaves);
  
  // if there is more than one unique char, reconstruct the right part of tree
  if(!isSingleChar){
    tree->constructTree(tree->getRoot(), 1, bitIn, leaves);
  }
  tree->setLeaves(leaves);   // update leaves

  // open the output stream
  ofstream output;
  output.open(argv[2], ios::binary);
  
  // Check if output file was actually opened
  if(!output.is_open()){
    
    cout << "Invalid output file. No file was opened. Please try again.\n";
    return -1;
  }
    
  // use a while to call decode method by passing in the input stream
  int countOfEncodedChar = 0;
  unsigned char decodedChar;
  while(1) {
     
	if(countOfEncodedChar == fileSize){   // use the size of file to 
		break;                            // keep track of when to stop
	}
	countOfEncodedChar++;
    decodedChar = tree->decode(bitIn);
    output << decodedChar;
  }

  // close all stream and free the memory
  input.close();
  output.close();
  delete tree;  

  return 0;
}  
