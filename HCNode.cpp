#include "HCNode.hpp"
#include <iostream>

using namespace std;

/** Less-than comparison, so HCNodes will work in std::priority_queue
 *  We want small counts to have high priority.
 *  And we want to break ties deterministically.
 */
bool HCNode::operator<(const HCNode& other){
  
  // if count are different, compare counts
  if(this->count != other.count){
    return this->count > other.count; 
  }
 
  // if count are equal. compare symbol
  return this->symbol < other.symbol;
}

/** To compare the HCNode counts similar to <. Return 1 if the count of one
 *  is less than the count of other, else return 0
 */
bool comp(HCNode* one, HCNode* other){
  if(one == 0 || other == 0) { return false; }  // Null check on one and other
  
  // if count are different, compare counts
  if(one->count != other->count){
    return one->count > other->count;
  }

  // if count are equal, compare symbol
  return one->symbol > other->symbol;
}
  
 
