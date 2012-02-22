#include <iostream>
#include <vector>

#include "g.h"

using namespace std;

g::g( int vsize ) :
  n( vsize )
{
  arraySize = n / intSize;
  if( ( n % intSize ) != 0 ) arraySize++;
  gA.resize( n );
  for( int i = 0; i < n; i++ ){
    gA[i].resize( arraySize, 0 );
  }
}

void g::add_edge( int u, int v ){
  if( u < n && u >= 0 && v < n && v >= 0 ){
    set_insert( u, gA[v] );
    set_insert( v, gA[u] );
  }
  else{
    cerr << "Error: Invalid vertices" << endl;
  }
}

void g::print(){
  uint64_t z;
  int count;
  for( int i = 0; i < n; i++ ){
    count = 0;
    for( int j = 0; j < arraySize; j++ ){
      z = shifter << (intSize - 1);
      for( ; z>0; z >>= 1 ){
	cout << (((gA[i][j] & z ) == z) ? " 1":" 0");
	count ++;
	if( count == n ) break;
      }		 
    }
    cout << endl;
  }
}
