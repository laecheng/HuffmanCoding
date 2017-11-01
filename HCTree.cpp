#include "HCTree.hpp"
#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <algorithm>
#include "BitInputStream.hpp"
#include "BitInputStream.hpp"

using namespace std;


/** Destructor used to free the memoery allocated for HCTree
 *  and also set the root to 0
 */
HCTree::~HCTree() {
   
   deleteAll(root);
   root = 0; 
 }


/** Use the Huffman algorithm to build a Huffman coding trie.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
 *  the frequency of occurrence of byte i in the message.
 *  POSTCONDITION:  root points to the root of the trie,
 *  and leaves[i] points to the leaf node containing byte i.
 */
void HCTree::build(const vector<int>& freqs){

  std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp> queue;
 
  // create HCNode as leaves and then push them into priority queue
  for(int i = 0;i < freqs.size(); ++i){
   
    if(freqs[i] != 0){
      HCNode* node = new HCNode(freqs[i],i);
      this->leaves[i] = node;
      queue.push(node);
    }
  }

  // handle the case that there is only one distinct character by creating
  // a dummy root and set its c0 child to the only leave
  if(queue.size() == 1){
  
    HCNode* curr = queue.top();
    queue.pop();
    HCNode*  dummyRoot = new HCNode(curr->count, curr->symbol);
    dummyRoot->c0 = curr;
    curr->p = dummyRoot;
    queue.push(dummyRoot);
  }

  while(queue.size() > 1){
    
    HCNode* nodeL = queue.top();  // get the 1st smallest node, then delete it
    queue.pop(); 
    HCNode* nodeR = queue.top();  // get the 2nd smallest node, then delete it
    queue.pop();

    int tempCount =  nodeL->count + nodeR->count; // update count of tempRoot
    byte tempSymbol;
    
    // decide the count and symbol for the tempRoot and then create the 
    // HCNode tempRoot
    if(*nodeR < *nodeL){
      tempSymbol = nodeL->symbol;
    }
    else {
      tempSymbol = nodeR->symbol; 
    }
    HCNode* tempRoot = new HCNode(tempCount, tempSymbol);
     
    // update the parent relationship and children relationship
    nodeL->p = tempRoot;   
    nodeR->p = tempRoot;
    tempRoot->c0 = nodeL;
    tempRoot->c1 = nodeR;
      
    queue.push(tempRoot); // push the tempRoot into the queue
  }
  
  this->root = queue.top();  // update the root
}

/** Write to the given BitOutputStream
 *  the sequence of bits coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const{

  HCNode* curr = this->leaves[(int)symbol];
  vector<int> codeWord;
        
  while (curr != this->root) {           
    
    if (curr == curr->p->c0) {  // if the symbol is a c0 child
      codeWord.push_back(0);
    }
    else {                      // if the symbol is a c1 child
      codeWord.push_back(1);
    }
    curr = curr->p;  // going up by one layer
  }

  std::reverse(codeWord.begin(), codeWord.end()); // reverse the order or code
	
  // use for loop to write code word into the output file
  int index = 0;
  for(auto i = codeWord.begin(); i != codeWord.end(); i++, index++){
    out.writeBit(codeWord[index]);
  }
}


/** Write to the given ofstream
 *  the sequence of bits (as ASCII) coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT 
 *  BE USED IN THE FINAL SUBMISSION.
 */
void HCTree::encode(byte symbol, ofstream& out) const{

  HCNode* curr = this->leaves[(int)symbol];
  std::string str = "";
  
  // use a while loop to go from leave to root to find the correct code word
  // for the specific character
  while (curr != this->root){
    
    if(curr == curr->p->c0){  // if the symbol is zero child
                              // then append '0'   
      str = str + "0";
    }
    else {                    // if the symbol is one child
      str = str + "1";        // then append '1'
    }
    curr = curr->p;
  }
  
  std::reverse(str.begin(), str.end());  // reverse the encoded string
  out << str;                            // return the decode symbol
}


/** Return symbol coded in the next sequence of bits from the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
int HCTree::decode(BitInputStream& in) const{

  unsigned char currChar;
  HCNode* curr = this->root;

  // while the current node is not a leave
  while ((curr->c0 != 0) || (curr->c1 != 0)) {  
    
    int bit = in.readBit();
    	
    if (bit == 0) {         // if the symbol is zero child
      curr = curr->c0;      // then append '0'
    }                      
    else {                  // if the symbol is one child
      curr = curr->c1;      // then append '1'
    } 
  } 
    
  return (int) curr->symbol;  // return the decode symbol
}

/** Return the symbol coded in the next sequence of bits (represented as 
 *  ASCII text) from the ifstream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
 *  IN THE FINAL SUBMISSION.
 */
int HCTree::decode(ifstream& in) const{
  
  unsigned char currChar;
  HCNode* curr = this->root;

  // while the current node is not a leave
  while((curr->c0 != 0) || (curr->c1 != 0)){
    
    currChar = in.get();        // get the current char
    
    if( currChar == '0'){       // if the current char is 0, we go c0 child 
      
      curr = curr->c0;
    }
    else if( currChar == '1') { // if the current char is 1, go to c1 child
     
      curr = curr->c1;
    }
    else {        // if the current char is not 0 ot 1, break out the loop
      break;
    }
  }
  return (int)curr->symbol;
}

/** getter method for root */
HCNode* HCTree::getRoot() const{

  return this->root;
}

/** setter method for leaves */
void HCTree::setLeaves(vector<HCNode*> & leaves) {

	this->leaves = leaves;
}

/** setter method for root */
void HCTree::setRoot(HCNode* root) {

	this->root = root;
}
 
/** helper method to store the structure of the huffman tree into the header
 * of the output file by using pre order traversal
 */
void HCTree::storeTree(HCNode * curr, BitOutputStream & bitOut) {                          
   
  // when the current node is leave
  if ((curr->c0 == 0) && (curr->c1 == 0)) {                                        
    bitOut.writeBit(1);                                                                    
    unsigned char symbol = curr->symbol;                                           
    
    // use a for loop to write the symbol 1 byte into the header
    for (int i = 0; i < 8; i++) {                                                 
      bitOut.writeBit(((1 << (7 - i)) & symbol) >> (7 - i));                       
    }
	
    return;                                                                        
  }                                                                                
  else { 
	
    bitOut.writeBit(0);     // else write a '0' to indicate not node    
  } 

  // if left child exists
  // print 0 if interal node, print 1 if node followed by byte value
  // then recursive call left child
  if (curr->c0 != 0) {                                                               
    storeTree(curr->c0, bitOut);     
  }
  
  // if right child exits                                                          
  // print 0 ...                                                                   
  // recusive call right child   
  if (curr->c1 != 0) {                                                             
    storeTree(curr->c1, bitOut);                                                   
  }                                                                                  
}

/** helper method to rebuild the tree after writing the header */
void HCTree::constructTree(HCNode* curr, 
    int child, BitInputStream & bitIn, vector<HCNode*> & leaves){

  int bit = bitIn.readBit();
  
  // base case when it reach the leave node
  // then create a new node with symbol in it
  if(bit == 1) {
    
    // get the symbol 8 bits after the '1' bit
    unsigned int total = 0;
    for(int i = 7; i >= 0; i--) {
       total += bitIn.readBit() << i; 
    }

    unsigned char currByte = (unsigned char) total;
    // create a new node with decoded symbol
    HCNode* currNode = new HCNode(0,currByte);

    // update the parent relationship of current 
    currNode->p = curr;
    if(child == 0) {
      curr->c0 = currNode;
    } 
    else {
      curr->c1 = currNode;
    }

	// set the corresponding leaves
	leaves[total] = currNode;
  }
  else {
    
    // else if the current should be a intermediate node, then we 
    // write a '0' and recursively call this method to write the rest of 
    // the tree
    HCNode* zeroNode = new HCNode(0,0);
	
    // update parent relationship
    zeroNode->p = curr;
	if(child == 0){
		curr->c0 = zeroNode;
	}
	else {
		curr->c1 = zeroNode;
	}

	this->constructTree(zeroNode, 0, bitIn, leaves);  // recursively call
    this->constructTree(zeroNode, 1, bitIn, leaves);  // recursively call
  }
}

/** helper method to get leaves */
int HCTree::getSingleChar(const vector<int> & freqs){
   
  int count = 0;
  int isSingleChar = 0;
  
  // create HCNode as leaves and then push them into priority queue
  for(int i = 0;i < freqs.size(); ++i){
    if(freqs[i] != 0){
      count++;
    }
  }

  // check the count of the non zero symbol
  // if it is 1, then return 1, else return 0
  if(count == 1){
    isSingleChar = 1;
  }
  else {
    isSingleChar = 0;
  }
  return isSingleChar; 
}

