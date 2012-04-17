#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
//#include <NTL>
#include <utility>
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
  k4s.clear();
}

int g::size(){
  return n;
}

int g::num_edges(){
  recalc_edges();
  return numEdges;
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

struct g::compZZ_pE{
  bool operator() (const ZZ_pE& left, const ZZ_pE& right) const
  {
    string sleft, sright;
    ostringstream loss, ross;
    loss << left;
    sleft = loss.str();
    sleft = sleft.substr(1,sleft.length()-2);
    ross << right;
    sright = ross.str();
    sright = sright.substr(1,sright.length()-2);
    istringstream liss(sleft,istringstream::in);
    istringstream riss(sright,istringstream::in);

    //  cout << "Comparing " << sleft << " and " << sright << endl;
    
    if( sleft.length() < sright.length() ){
      //    cout << sleft << " is less degree, so it is less" << endl;
      return true;
    }
    else if( sleft.length() == sright.length() ){
      int degree = sleft.length() / 2 + 1;
      int coeffs[2][degree];
      for( int i = 0; i < degree; i++ ){
	liss >> coeffs[0][i];
	riss >> coeffs[1][i];
      }
      for( int i = degree-1; i >= 0; i-- ){
	//	cout << "Comparing coeffs " << coeffs[0][i] << " and " << coeffs[1][i] << endl;
	if( coeffs[0][i] < coeffs[1][i] ){
	  return true;
	}
	else if( coeffs[0][i] > coeffs[1][i] ){
	  return false;
	}
      }
    }
    return false;
  }
};

//struct g::primTable{
//  static map<pair<int,int>,string> gen_table(){
//    map<pair<int,int>,string> t;
//    t[(3,6)] = "[ 1 1 ]";
//  }
  
//};

void g::make_galois_circ( int p, int n, int r ){
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

  vector< ZZ_pE > elements;
    
  ZZ_pE x;
  string prim;
  if( p == 7 && n == 3 ){
    prim = "[ 1 1 1 ]";
  }
  else if( p == 23 && n == 2 ){
    prim = "[ 10 1 ]";
  }
  else{
    prim = "[ 1 1 ]";
  }
  istringstream iss( prim, istringstream::in);
  iss >> x;
  
  elements.push_back( x );
  ZZ_pE y = power(x,2);

  int count = 1;
  while( x !=y ){
    count++;
    elements.push_back(y);
    y = y * x;
  }

  cout << "Count = " << count << endl;

  compZZ_pE comparePolys;
  sort( elements.begin(), elements.end(), comparePolys );

  map< ZZ_pE, bool, compZZ_pE > residues;
  map< ZZ_pE, bool, compZZ_pE >::iterator res_it;

  for( vector< ZZ_pE >::iterator it = elements.begin(); 
       it != elements.end(); it++ ){
    // cout << power(*it,4) << endl;
    residues[power(*it,r)] = true;
    //cout << *it << endl;
  }

  string neg = "[ -1 ]";
  string one = "[ 1 ]";

  istringstream nss( neg, istringstream::in );
  ZZ_pE negp;
  nss >> negp;
  cout << "-1 = " << negp << " and is ";
  if( residues[negp] != true ){
    cout << "NOT ";
  }
  cout << "a residue" << endl;

  int res_count = 0;
  // cout << "*** RESIDUES ****" << endl;
  for( res_it = residues.begin(); res_it != residues.end(); res_it++ ){
    //cout << "  " << (*res_it).first << endl;
    res_count++;
  }
  cout << "Num Residues = " << res_count << endl;

  for( int i = 0; i < count; i++ ){
    if( residues[elements[i]] == true ){
      //  cout << elements[i] << " is a residue so add edge " << 0 << " " << i+1 << endl;
      add_edge( 0, i+1 );
    }
    for( int j = i+1; j < count; j++ ){
      if( (residues[elements[i] - elements[j]] == true) ){
	//	cout << elements[i] << " - " << elements[j] << " = " << elements[i] - elements[j] << " so add edge " << i+1 << " " << j+1 << endl;
	add_edge( i+1, j+1 );
      }
      else if( (residues[elements[j] - elements[i]] == true) ){
	//	cout << elements[j] << " - " << elements[i] << " = " << elements[j] - elements[i] << " so add edge " << i+1 << " " << j+1 << endl;
	add_edge( i+1, j+1 );
      }
    }
  }
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

void g::load_adj( string filename ){
  string line;
  ifstream ifs( filename.c_str(), ifstream::in );
  int i = 0;
  while( ifs.good() && i < n ){
    getline( ifs, line );
    //    cout << line << endl;
    if( line.length() != n ){
      throw "Error: Conflicting sizes";
    }
    for( int j = i; j < n; j++ ){
      if( line[j] == '1' ){
	add_edge(i,j);
      }
    }
    i++;
  }
  ifs.close();
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

bool g::is_k( int k ){
  switch(k){
  case 4:{
    for( int i = 0; i < n-3; i++ ){
      for( int j = i; j < n-2; j++ ){
	if( is_edge(i, j) ){
	  for( int k = j; k < n - 1; k++ ){
	    if( is_edge(i,k) && is_edge(j,k) ){
	      for( int l = k; l < n; l++ ){
		if( is_edge(i,l) && is_edge(j,l) && is_edge(k,l) ){
		  return true;
		}
	      }
	    }
	  }
	}
      }
    }
    
    break;
  }
  default:
    cout << "Error: Counting/Removing K" << k << " is not supported" << endl;
  }
  return false;
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

vector<int> g::add_all_ce( bool avoid, int k ){
  vector<int> dists;
  for( int d = 1; d <= n/2; d++ ){
    cout << "d = " << d;
    add_circ_edge( d );
    if( is_k() ){
      remove_circ_edge( d );
      cout << "...NOT added" << endl;
    }
    else{
      dists.push_back( d );
      cout << "...added" << endl;
    }
  }
  return dists;
}

vector<int> g::add_all_ce_rand( bool avoid, int k ){
  vector<int> dists;
  srand((unsigned)time(0));
  int p[n/2];
  for( int i = 0; i < n/2; i++ ){
    int j = rand() % (i+1);
    //cout << j << endl;
    p[i] = p[j];
    p[j] = i;
  }
  for( int i = 0; i < n/2; i++ ){
    int d = p[i] + 1;
    //cout << "d = " << d;
    add_circ_edge( d );
    if( is_k() ){
      remove_circ_edge( d );
      //cout << "...NOT added" << endl;
    }
    else{
      dists.push_back( d );
      //cout << "...added" << endl;
    }
  }
  return dists;
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

void g::get_k4s(){
  numK4s = 0;
  k4s.clear();
  int a, b, c, d, e, f;
  for( int i = 0; i < n-3; i++ ){
    for( int j = i; j < n-2; j++ ){
      if( is_edge(i, j) ){
	for( int k = j; k < n - 1; k++ ){
	  if( is_edge(i,k) && is_edge(j,k) ){
	    for( int l = k; l < n; l++ ){
	      if( is_edge(i,l) && is_edge(j,l) && is_edge(k,l) ){
		a = edges[i][j];
		b = edges[i][k];
		c = edges[i][l];
		d = edges[j][k];
		e = edges[j][l];
		f = edges[k][l];
		k4s.push_back( new int[6] );
		k4s[numK4s][0] = a;
		k4s[numK4s][1] = b;
		k4s[numK4s][2] = c;
		k4s[numK4s][3] = d;
		k4s[numK4s][4] = e;
		k4s[numK4s][5] = f;
		numK4s++;
		//cout << a << " " << b << " " << c << " " << d << " " << e << " " << f << endl;
	      }
	    }
	  }
	}
      }
    }
  }
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

void g::print_sat34( ostream *o ){
  recalc_edges();
  get_tris();
  get_k4s();

  *o << "c SAT reduction for arrowing (3,4)^e" << endl;
  *o << "c" << endl;
  *o << "p cnf " << numEdges << " " << numTris + numK4s << endl;
  for( int t = 0; t < numTris; t++ ){
    *o << tris[t][0] << " " << tris[t][1] << " " << tris[t][2] << " 0" << endl;
  }

  for( int s = 0; s < numK4s; s++ ){
    *o << "-" << k4s[s][0] << " -" << k4s[s][1] << " -" << k4s[s][2]
       << " -" << k4s[s][3] << " -" << k4s[s][4] << " -" << k4s[s][5]
       << " 0" << endl;
  }
}
