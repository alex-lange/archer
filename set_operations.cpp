#include <vector>
#include "set_operations.h"

/* A set of functions for set operations when a set is a 64 bit unsigned
   integer, or a vset, which is a c++ vector os unsigned ints.

   All algorithms based off those described in Chapter 1 of the textbook
   "Combinatorial Algorithms" by Donald Kreher and Douglas Stinson
*/

// inserts bit into set
// algorithm 1.3
void set_insert( int u, vset &S ){
  int j = intSize - 1 - ( u % intSize );
  int i = u / intSize;
  S[i] = S[i] | ( shifter << j );
}

// deletes bit from set
// Algorithm 1.4
void set_delete( int u, vset &S ){
  int j = intSize - 1 - ( u % intSize );
  int i = u / intSize;
  S[i] = S[i] & ~( shifter << j );
}

// determines if a bit is in S
// Algorithm 1.5
bool in_set( int u, vset S ){
  int j = intSize - 1 - ( u % intSize );
  int i = u / intSize;
  if( S[i] & (shifter << j ) ){
    return true;
  }else{
    return false;
  }
}

// removes bit from set
void set_cut( int u, vset &S, int arraySize ){
  int j = u % intSize;
  int i = u / intSize;
  uint64_t x = ( S[i] & ((~empty) >> (j+1))) << 1;
  if( j == 0 ){
    S[i] = x;
  }
  else if( j != (intSize - 1)){
    uint64_t y = ( S[i] & ((~empty) << (intSize-j)));
    S[i] = x | y;
  }
  //cout << "j " << j << endl;
  // cout << "i " << i << endl;
  for( ; i < arraySize - 1; i++ ){
    if( in_set( (i+1)*intSize, S ) ){
      S[i] = S[i] | shifter;
    }else{
      S[i] = (S[i] & ((~empty)<<1));
    }
    S[i+1] = S[i+1] << 1;
  } 
}

// determines union of two sets
// Algorithm 1.6
vset set_union( vset S, vset R, int arraySize){
  vset T;
  T.resize( arraySize, 0 );
  for( int i = 0; i < arraySize; i++ ){
    // OR sets together
    T[i] = S[i] | R[i];
  }
  return T;
}

// determines intersection of two sets
// Algorithm 1.7
vset set_intersection( vset S, vset R, int arraySize){
  vset T;
  T.resize( arraySize, 0 );
  for( int i = 0; i < arraySize; i++ ){
    // AND sets
    T[i] = S[i] & R[i];
  }
  return T;  
}
