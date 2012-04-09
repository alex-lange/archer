#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
//#include <NTL>
#include <gf2x.h>
#include <NTL/vec_ZZ_pE.h>
#include <NTL/ZZ_pXFactoring.h>
#include <NTL/ZZ_pE.h>
#include <NTL/ZZ_pEX.h>


#include "g.h"
#include "number_algorithms.h"

using namespace std;
using namespace NTL;

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

int g::size(){
  return n;
}

void g::add_edge( int u, int v ){
  if( u < n && u >= 0 && v < n && v >= 0 ){
    numEdges++;
    set_insert( u, gA[v] );
    set_insert( v, gA[u] );
    edges[u][v] = numEdges;
    edges[v][u] = numEdges;
  }
  else{
    cerr << "Error: Invalid vertices " << u << " and " << v << endl;
    cerr << "Size of graph = " << n << endl;
  }
}


void g::add_circ_edge( int d ){
  int x;
  for( int v = 0; v < n; v++ ){
    x = ( v + d ) % n;
    add_edge( v, x );
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

void g::remove_circ_edge( int d ){
  int x;
  for( int v = 0; v < n; v++ ){
    x = ( v + d ) % n;
    remove_edge( v, x );
  }
}

bool g::is_edge( int u, int v ){
  return in_set( u, gA[v] );
}

void g::remove_vs( vector<int> cuts, int k ){
  int x;
  int removed = 0;
  
  // gets the array of cuts and sorts them
  //int elements = sizeof(cuts) / sizeof(cuts[0]);
  //cout << elements << endl;
  sort(cuts.begin(),cuts.end());
  cout << "Cuts: ";
  for( int i = 0; i < k; i++ ){
    cout << cuts[i] << " ";
    cuts[i] = cuts[i] - i;
  }
  cout << endl;
  
  // remove each vertex from the graph
  for( int j = 0; j < k; j++ ){
    x = cuts[j];
    cout << "Removing " << x << "..." << endl;
    for( int i = 0; i < n; i++ ){
      if( i != x ){
	set_cut( x, gA[i], arraySize );
	if( i > x ){
	  gA[i-1] = gA[i];
	}
      }
    }  
    n--;
  }
  recalc_edges();
}

void g::remove_randvs( int num ){
  vector<int> vs;
  srand((unsigned)time(0));
  for( int i = 0; i < num; i++ ){
    vs.push_back( rand() % n );
  }
  remove_vs( vs, num );
}

void g::make_circ( vector<int> dists ){
  int x;
  for( int v = 0; v < n; v++ ){
    for( vector<int>::iterator it = dists.begin(); it < dists.end(); it++ ){
      x = ( v + *it ) % n;
      add_edge( v, x );
    }
  }
}

void g::make_residue_circ( int r ){
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
}

void g::make_galois_circ( int p, int n ){
  // define GF(p)
  ZZ_p::init(to_ZZ(p));

  // build irreducible polynomial P of degree n over GF(P)
  ZZ_pX poly;
  BuildIrred(poly, n);

  // define GF(p^n), which is built around the irreducible poly
  ZZ_pE::init(poly);

  cout << "Created GF(" << p << "^" << n << ") with irreducible polynomial ";
  cout << poly << endl;  
  cout << "Cardinality = " << ZZ_pE::cardinality() << endl;
  cout << "Degree = " << ZZ_pE::degree() << endl;
  cout << "Modulus = " << ZZ_pE::modulus() << endl;

  /*ZZ_pE f;
  f = 5;
  cout << f << endl;
  random(f);
  cout << f << endl;
  cout << power( f, 5 ) << endl;
  ZZ_pX h(1,1);*/

  
  ZZ_pEX g;
  random(g,1);
  cout << g << endl;
  cout << power(g,2) << endl;
  g.rep[0]=1;
  cout << g << endl;
  
  ZZ_pEX f;
  SetX(f);
  cout << f << endl;
  cout << power(f,10) << endl;
  // random(f,1); 

}

void g::make_embedded_rc( int r, int num ){
  int subsize = n / num;
  vector< g* > subgraphs;
  //g sub( subsize );
  //sub.make_residue_circ( r );
  int current, x, y, stepx, stepy;
  for( int i = 0; i < num; i++ ){
    subgraphs.push_back( new g(subsize) );
    subgraphs[i]->make_residue_circ( r );
    //subgraphs[i]->print();
  }
 
  for( int i = 0; i < n; i++ ){
    for( int j = 0; j < n; j++ ){
      x = i % subsize;
      y = j % subsize;
      stepx = i / subsize;
      stepy = j /subsize;
      if( stepx == stepy ){
	if( subgraphs[stepx]->is_edge( x, y ) ){
	  add_edge( i, j );
	}
      }
    }
  }
  for( int i = 0; i < num; i++ ){
    delete subgraphs[i];
  }
}

bool g::join_graphs( int num, vector<g*> graphs ){
  int sum = 0;
  for( int i = 0; i < num; i++ ){
    sum += graphs[i]->size();
  }
  if( sum != n ){
    return false;
  }
  int current = 0;
  int start = 0;
  int end;
  while( current < num ){
    end = graphs[current]->size();
    for( int i = 0; i < end - 1; i++ ){
      for( int j = i; j < end; j++ ){
	if( graphs[current]->is_edge( i, j ) ){
	  add_edge( start + i, start + j );
	}
      }
    }
    start += end;
    current++;
  }
  return true;
}

int g::remove_k( int k, bool remove ){
  int numK = 0;
  switch(k){
  case 4:{
    for( int i = 0; i < n-3; i++ ){
      for( int ii = i; ii < n-2; ii++ ){
	if( in_set( ii, gA[i] ) ){
	  for( int iii = ii; iii < n-1; iii++ ){
	    if( in_set( iii, gA[i] ) && in_set( iii, gA[ii] ) ){
	      for( int iiii = iii; iiii < n; iiii++ ){
		if( in_set( iiii, gA[i] ) && in_set( iiii, gA[ii] ) 
		    && in_set( iiii, gA[iii] )){
		  numK++;
		  if( remove ){
		    remove_edge( iii, iiii );
		  }
		}
	      }
	    }
	  } 
	}
      }
    }
    break; }
  default:
    cout << "Error: Counting/Removing K" << k << " is not supported" << endl;
  }
  return numK;
}

bool g::add_noncrit_edge( int v, bool avoid, int k ){
  bool canAdd;
  for( int i = v+1; i < n - 2; i++ ){
    canAdd = true;
    
    // finds non-adjacent vertex for possible edge
    if( !in_set( i, gA[v] ) ){      
      if( avoid ){
	switch(k){
	case 4:{
	  // checks all edges for possible k_4
	  for( int j = 0; (j < n - 1) && canAdd; j++ ){
	    if( in_set( j, gA[v] ) && in_set( j, gA[i] ) ){
	      for( int k = j; (k < n) && canAdd; k++ ){
		if( in_set( k, gA[v] ) && in_set( k, gA[i] ) 
		    && in_set( k, gA[j] )){
		  canAdd = false;
		}
	      }
	    }
	  }
	  break;
	}
	default:
	  cerr << "Error: Avoiding K" << k << " is not supported" << endl;
	}
      }

      // if i can be added, return true and add it!
      if( canAdd ){
	add_edge( i, v );
	return true;
      }
    }
  }
  return false;
}

int g::add_all_noncrit( bool avoid, int k ){
  int edgesAdded = 0;
  bool canAdd = true;
  for( int i = 0; i < n; i++ ){
    while( canAdd = add_noncrit_edge( i, avoid, k ) ){
      edgesAdded++;
    }
  }
  return edgesAdded;
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
      if( is_edge(i, j)){
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

void g::print_sparse_h( ostream * o, bool isRudy ){
  recalc_edges();
  get_tris();

  int a, b, c;
  int j = 0;
  string weight;

  *o << numEdges << " ";
  if( !isRudy ){
    *o << numTris;
    weight = "";
  }
  else{
    *o << 3 * numTris;
    weight = " 1";
  }
  *o << endl;

  for( int t = 0; t < numTris; t++ ){
    a = tris[t][0];
    b = tris[t][1];
    c = tris[t][2];
 
    *o << a << " " << b << weight << endl;
    j++;
    
    *o << a << " " << c << weight << endl;
    j++;

    *o << b << " " << c << weight << endl;
    j++;
  }
}

void g::print_sdpa( ostream * o ){
  recalc_edges();
  get_tris();

  int a, b, c;
  int j = 0;

  int degrees[ numEdges ];
  for( int i = 0; i < numEdges; i++ ){
    degrees[i] = 0;
  }

  *o << "*This graph" << endl;
  *o << numEdges << endl;
  *o << 1 << endl;
  *o << numEdges << endl;
  *o << 1;
  for( int i = 0; i < numEdges-1; i++ ){
    *o << ", " << 1;
  }
  *o << endl;

  for( int t = 0; t < numTris; t++ ){
    a = tris[t][0];
    b = tris[t][1];
    c = tris[t][2];

    degrees[a-1] += 2;
    degrees[b-1] += 2;
    degrees[c-1] += 2;
 
    *o << "0 1 " << a << " " << b << " -1" << endl;
    *o << "0 1 " << a << " " << c << " -1" << endl;
    *o << "0 1 " << b << " " << c << " -1" << endl;
  }  

  for( int i = 0; i < numEdges; i++ ){
    *o << "0 1 " << i+1 << " " << i+1 << " " << degrees[i] << endl;
  }

  for( int i = 1; i <= numEdges; i++ ){
    *o << i << " 1 " << i << " " << i << " 1" << endl;
  }
}

void g::print_sat( ostream *o ){
  recalc_edges();
  get_tris();

  int a, b, c;
  
  *o << "c SAT reduction for arrowing triangles " << endl;
  *o << "c" << endl;
  *o << "p cnf " << numEdges << " " << numTris*2 << endl;
  for( int t = 0; t < numTris; t++ ){
    a = tris[t][0];
    b = tris[t][1];
    c = tris[t][2];
    *o << a << " " << b << " " << c << " 0" << endl;
    *o << "-" << a << " -" << b << " -" << c << " 0" << endl;
  }
}
