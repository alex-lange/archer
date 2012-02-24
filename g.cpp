#include <iostream>
#include <vector>

#include "g.h"
#include "number_algorithms.h"

using namespace std;

g::g( int vsize ) :
  n( vsize )
{
  oldN = n;
  arraySize = n / intSize;
  if( ( n % intSize ) != 0 ) arraySize++;
  gA.resize( n );
  for( int i = 0; i < n; i++ ){
    gA[i].resize( arraySize, 0 );
  }
  numEdges = 0;
  edges = new int*[n];
  for( int i = 0; i < n; i++ ){
    edges[i] = new int[n];
  }
  
  calcedTris = false;
}

g::~g(){
  for( int i = 0; i < oldN; i++ ){
    delete[] edges[i];
  }

  delete[] edges;
}

void g::add_edge( int u, int v ){
  if( u < n && u >= 0 && v < n && v >= 0
      && !in_set(u, gA[v]) && !in_set(v, gA[u]) ){
    numEdges++;
    set_insert( u, gA[v] );
    set_insert( v, gA[u] );
    edges[u][v] = numEdges;
    edges[v][u] = numEdges;
  }
  else{
    cerr << "Error: Invalid vertices" << endl;
  }
}

void g::remove_edge( int u, int v ){
  if( u < n && u >= 0 && v < n && v >= 0
      && in_set(u, gA[v]) && in_set(v, gA[u]) ){
    numEdges--;
    set_delete( u, gA[v] );
    set_delete( v, gA[u] );
    edges[u][v] = 0;
    edges[v][u] = 0;
  }
}

void g::remove_vs( int * v, int k ){
  int x;
  int removed = 0;
  for( int j = 0; j < k; j++ ){
    x = v[j] - removed;
    //cout << "Removing " << v << "..." << endl;
    for( int i = 0; i < n; i++ ){
      if( i != x ){
	set_cut( x, gA[i], arraySize );
	if( i > x ){
	  gA[i-1] = gA[i];
	}
      }
    }  
    n--;
    removed++;
  }
  recalc_edges();
}

int g::make_residue_circ( int r ){
  vset R(arraySize,0);
  int64_t rth;
  for( int i = 1; i < n; i++ ){
    rth = square_and_multiply( i, r, n );
    set_insert( rth, R );
  }

  for( int v = 0; v < n; v++ ){
    for( int x = v + 1; x < n; x++ ){
      if( in_set( x - v, R ) ){
	add_edge( v, x );
      }
    }
  }

  return 0;
}

void g::count_tris(){
  numTris = 0;
  for( int i = 0; i < n-2; i++ ){
    for( int ii = i; ii < n-1; ii++ ){
      if( in_set( ii, gA[i] ) ){
	for( int iii = ii; iii < n; iii++ ){
	  if( in_set( iii, gA[i] ) && in_set( iii, gA[ii] ) ){
	    numTris++;
	  }
	} 
      }
    }
  }
}

void g::get_tris(){
  if( calcedTris ){
    for( int i = 0; i < numTris; i++ ){
      delete[] tris[i];
    }
    delete[] tris;
  }
  count_tris();
  tris = new int*[numTris];
  for( int i = 0; i < numTris; i++ ){
    tris[i] = new int[3];
  }

  int a, b, c;
  int numTri = 0;
  for( int i = 0; i < n-2; i++ ){
    for( int ii = i; ii < n-1; ii++ ){
      if( in_set( ii, gA[i] ) ){
	for( int iii = ii; iii < n; iii++ ){
	  if( in_set( iii, gA[i] ) && in_set( iii, gA[ii] ) ){
	    a = edges[i][ii];
	    b = edges[i][iii];
	    c = edges[ii][iii];
	    tris[numTri][0] = a;
	    tris[numTri][1] = b;
	    tris[numTri][2] = c; 
	    numTri++;
	  }
	} 
      }
    }
  }

  calcedTris = true;
}

void g::recalc_edges(){
  numEdges = 0;
  for( int i = 0; i < n - 1; i++ ){
    for( int j = i; j < n; j++ ){
      if( in_set( j, gA[i] )){
	numEdges++;
	edges[i][j] = numEdges;
	edges[j][i] = numEdges;
      }
      else{
	edges[i][j] = 0;
	edges[j][i] = 0;
      }
    }
  }
}

void g::print( ostream * o ){
  uint64_t z;
  int count;
  for( int i = 0; i < n; i++ ){
    count = 0;
    for( int j = 0; j < arraySize; j++ ){
      z = shifter << (intSize - 1);
      for( ; z>0; z >>= 1 ){
	*o << (((gA[i][j] & z ) == z) ? " 1":" 0");
	count ++;
	if( count == n ) break;
      }		 
    }
    *o << endl;
  }
}

void g::gen_sparse_h( ostream * o ){
  count_tris();
  get_tris();

  int a, b, c;
  int j = 0;

  *o << numEdges << " " << numTris << "\n";
  for( int t = 0; t < numTris; t++ ){
    a = tris[t][0];
    b = tris[t][1];
    c = tris[t][2];
 
    *o << a << " " << b << endl;
    j++;
    
    *o << a << " " << c << endl;
    j++;

    *o << b << " " << c << endl;
    j++;
  }
}
