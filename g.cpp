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
#include <math.h>


#include "g.h"
#include "number_algorithms.h"

using namespace std;
using namespace NTL;

g::g( int vsize ) :
  n( vsize )
{
  set_up();
}

g::g( const g &otherG ){
  n = otherG.order();
  set_up();
  for( int i = 0; i < n; i++ ){
    for( int j = 0; j < n; j++ ){
      if( otherG.is_edge(i,j) ){
	add_edge(i,j);
      }
    }
  }
}

g::~g(){
  for( int i = 0; i < oldN; i++ ){
    delete[] edges[i];
  }

  delete[] edges;
  k4s.clear();
  ks.clear();

  delete[] inTri;
  delete[] inKs;
  
  for( int i = 0; i < n; i++ ){
    for( int j = 0; j < n; j++ ){
      delete[] isTri[i][j];
    }
    delete[] isTri[i];
  }
  delete[] isTri;
}

void g::set_up(){
  oldN = n;
  arraySize = n / intSize;
  if( ( n % intSize ) != 0 ) arraySize++;
  gA.resize( n );
  gB.resize( n );
  for( int i = 0; i < n; i++ ){
    gA[i].resize( arraySize, 0 );
    gB[i].resize( arraySize, 0 );
  }
  numEdges = 0;
  edges = new int*[n];
  inKs = new bool[n];
  inTri = new bool[n];
  isTri = new bool**[n];
  vdegree = new int[n];

  for( int i = 0; i < n; i++ ){
    edges[i] = new int[n];
    inKs[i] = false;
    inTri[i] = false;
    vdegree[i] = 0;

    for( int b = i + 1; b < n; b++ ){
      set_insert( b, gB[i] );
    }
  }

  for( int i = 0; i < n; i++ ){
    isTri[i] = new bool*[n];
    for( int j = 0; j < n; j++ ){
      isTri[i][j] = new bool[n];
    }
  }

  beenModified = true;

}



int g::order() const{
  return n;
}

int g::num_edges(){
  recount_data();
  return numEdges;
}

int g::num_tris(){
  recount_data();
  return numTris;
}

void g::add_edge( int u, int v ){
  if( u < n && u >= 0 && v < n && v >= 0 ){
    if( !is_edge(u,v) ){
      numEdges++;
      set_insert( u, gA[v] );
      set_insert( v, gA[u] );
      edges[u][v] = numEdges;
      edges[v][u] = numEdges;
      vdegree[u]++;
      vdegree[v]++;
      beenModified = true;
    }
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
    if( is_edge(u,v) ){
      numEdges--;
      set_delete( u, gA[v] );
      set_delete( v, gA[u] );
      edges[u][v] = 0;
      edges[v][u] = 0;
      vdegree[u]--;
      vdegree[v]--;
      beenModified = true;
    }
  }
}

void g::remove_circ_edge( int d ){
  int x;
  for( int v = 0; v < n; v++ ){
    x = ( v + d ) % n;
    remove_edge( v, x );
  }
}

bool g::is_edge( int u, int v ) const{
  return in_set( u, gA[v] );
}

int g::min_degree(){
  int min = n+1;
  for( int i = 0; i < n; i++ ){
    if( vdegree[i] < min ){
      min = vdegree[i];
    }
  }
  return min;
}


void g::max_clique_backtrack( int l ){
    // get new optimum value
  if( l > optSize ){
    optSize = l;
    optClique = X;
  }
  
  // calculates new tree level
  if( l == 0 ){
    gC[l] = gV;
  }else{
    // gets only the vertices we want
    gC[l] = set_intersection( gC[l-1], set_intersection( gA[X[l-1]], gB[X[l-1]], arraySize ), arraySize);
  }
  vset cl = gC[l];

  // Bounding/Tree Pruning
  int m;
  // m = greedy_color( C[l] ) + l;` (not implemented yet)
  // m = sampling_bound( C[l] ) + l; (not implmented yet)
  m = l + set_order( gC[l], arraySize );
  
  int x = first_bit( cl, arraySize );
  while( x != -1 ){
    // bound
    if( m <= optSize ) return;
    X[l] = x;
    max_clique_backtrack( l + 1 );
    set_delete( x, cl );
    x = first_bit( cl, arraySize );
  }
}


vector<int> g::max_clique(){
  // set all global variables needed
  // (only use them for max-clique, so no use putting them in set_up)
  gC.resize( n + 1 );
  gV.resize( arraySize );
  for( int i = 0; i < arraySize; i++ ){
    gV[i] = empty;
  }
  X.resize( n );
 
  for( int i = 0; i < n+1; i++ ){
    gC[i].resize( arraySize );
    for( int j = 0; j < arraySize; j++ ){
      gC[i][j] = empty;
    }
  }
  for( int i = 0; i < n; i++ ){
    X[i] = 0;
    set_insert( i, gV );
  }
  optSize = 0;
  
  max_clique_backtrack( 0 );
  cout << "Size: " << optSize << endl;
  vector<int> realClique;
  for( int i = 0; i < optSize; i++ ){
    realClique.push_back( optClique[i] );
  }

  return realClique;
}


vector<int> g::max_independent_set(){
  make_complement();
  vector<int> realClique = max_clique();
  make_complement();
  return realClique;
}


void g::fix_data(){
  recount_data();
  arraySize = n / intSize;
  if( ( n % intSize ) != 0 ) arraySize++;
  gB.resize(n);
  for( int i = 0; i < n; i++ ){
    for( int j = 0; j < arraySize; j++ ){
      gB[i][j] = empty;
    }
  }
  for( int i = 0; i < n; i++ ){
    for( int b = i + 1; b < n; b++ ){
      set_insert( b, gB[i] );
    }
  }
}


void g::remove_vs( vector<int> cuts, int k ){
  int x;
  int removed = 0;
  
  // gets the array of cuts and sorts them
  // int elements = sizeof(cuts) / sizeof(cuts[0]);
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
    //cout << "Removing " << x << "..." << endl;
    for( int i = 0; i < n; i++ ){
      if( i != x ){
	set_cut( x, gA[i], arraySize );
	if( i > x ){
	  gA[i-1] = gA[i];
	  vdegree[i-1] = vdegree[i];
	}
      }
    }
    n--;
    for( int i = 0; i < arraySize; i++ ){
      gA[n][i] = empty;
    }
  }
  beenModified = true;
  fix_data();
}

void g::remove_distvs( int k, int d, int s ){
  vector<int> vs;
  for( int i = 0; i < k; i++ ){
    vs.push_back( (s + i*d) % n );
  }
  remove_vs( vs, k );
}

void g::remove_randvs( int num ){
  vector<int> vs;
  srand((unsigned)time(0));
  for( int i = 0; i < num; i++ ){
    vs.push_back( rand() % n );
  }
  remove_vs( vs, num );
}


struct g::edge{
  int u;
  int v;
};

int g::k4_free_process(){
  vector<edge> edges_in;
  edge e;
  
  for( int i = 0; i < n; i++ ){
    for( int j = i+1; j < n; j++ ){
      e.u = i;
      e.v = j;
      edges_in.push_back( e );
    }
  }
  return k4_free_process( &edges_in );
}

int g::k4_free_process( vector<edge>* edges_in ){
  srand((unsigned)time(0));
  random_shuffle( edges_in->begin(), edges_in->end() );
  int u, v;
  int count = 0;
  //  cout << "Added edges ";
  for( vector<edge>::iterator it = edges_in->begin();
       it != edges_in->end(); it++ ){
    u = it->u;
    v = it->v;
    if( !is_edge(u,v) ){
      if( !causes_k( u, v ) ){
	add_edge( u, v );
	count++;
	//	cout << "(" << u << ", " << v << "), ";
      }
    }
  }
  return count;
}

void g::make_complement(){
  for( int i = 0; i < n; i++ ){
    gA[i]=set_complement( gA[i], arraySize );
    remove_edge( i, i );
  }
}

void g::make_cycle(){
  add_circ_edge( 1 );
}

void g::make_complete(){
  for( int i = 0; i < n-1; i++ ){
    for( int j = i+1; j < n; j++ ){
      add_edge(i,j);
    }
  }
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
  int num = 0;
  cout << "Residues: ";
  for( int i = 0; i < n; i++ ){
    if( in_set( i, R ) ){
      cout << i << " ";
      num++;
    }
  }
  cout << endl;
  cout << "Count: " << num << endl;

  for( int v = 0; v < n; v++ ){
    for( int x = v + 1; x < n; x++ ){
      if( in_set( x - v, R ) ){
	add_edge( v, x );
      }
    }
  }
}

bool g::make_l_circ( int s ){
  vset R(arraySize,0);
  int64_t rth;
  for( int i = 0; i < n; i++ ){
    rth = square_and_multiply( s, i, n );
    set_insert( rth, R );
  }

  if( !in_set( n - 1, R ) ){
    return false;
  }

  for( int v = 0; v < n; v++ ){
    for( int x = v + 1; x < n; x++ ){
      if( in_set( x - v, R ) ){
	add_edge( v, x );
      }
    }
  }

  return true;
}


// Makes the graph a random graph from Erdos-Renyi algorithm
// @return the number of edges added
int g::make_rand_er( float sigma ){
  // set seed for random variable
  srand((unsigned)time(0));
  float r;

  for( int x = 0; x < n - 1; x++ ){
    for( int y = x + 1; y < n; y++ ){
     
      // Uses random value to determine if edge or not edge
      r = rand() / (float)RAND_MAX;
      if( r >= ( 1 - sigma )){
	add_edge( x, y );
      }
    }
  }
  return numEdges;
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


struct g::gf_point{
  ZZ_pE x;
  ZZ_pE y;
  ZZ_pE z;
};


void g::make_projective_plane( int p, int k, bool cut, string pr ){

  int q = pow( p, k );
  if( (q*q +q + 1 ) != n ){
    cerr << "Error: invalid q for n" << endl;
    return;
  }
   // define GF(p)
  ZZ_p::init(to_ZZ(p));

  // build irreducible polynomial P of degree n over GF(P)
  ZZ_pX poly;
  BuildIrred(poly, k);

  // define GF(p^n), which is built around the irreducible poly
  ZZ_pE::init(poly);

  cout << "Created GF(" << p << "^" << k << ") with irreducible polynomial ";
  cout << poly << endl;  
  cout << "Cardinality = " << ZZ_pE::cardinality() << endl;
  cout << "Degree = " << ZZ_pE::degree() << endl;
  cout << "Modulus = " << ZZ_pE::modulus() << endl;

  vector< ZZ_pE > elements;
  vector< gf_point > points;
  
  ZZ_pE x, z, o;
  string zero = "[ 0 ]";
  string one = "[ 1 ]";
  string prim;
  if( p == 7 && k == 3 ){
    prim = "[ 1 1 1 ]";
  }
  else if( p == 23 && k == 2 ){
    prim = "[ 10 1 ]";
  }
  else if( p == 7 && k == 1 ){
    prim = "[ 3 ]";
  }
  else if( k == 1 ){
    prim = "[ 2 ]";
  }
  else{
    prim = pr;
  }
  
  istringstream iss1( zero, istringstream::in);
  iss1 >> z;
  elements.push_back(x);
 
  istringstream iss2( prim, istringstream::in);
  iss2 >> x;

  istringstream iss3( one, istringstream::in);
  iss3 >> o;
  
  elements.push_back( x );
  ZZ_pE y = power(x,2);

  int count = 2;
  while( x !=y ){
    count++;
    elements.push_back(y);
    y = y * x;
  }

  cout << "Count = " << count << endl;

  for( vector< ZZ_pE >::iterator it = elements.begin(); it != elements.end(); it++ ){
    //cout << *it << endl;
  }

  gf_point start;
  start.x = o;
  start.y = o;
  start.z = o;
  
  //points.push_back( start );
  for( int i = 0; i < count; i++ ){
    gf_point copy = start;
    copy.x = elements[i];
    points.push_back( copy );
    for( int j = 0; j < count; j++ ){
      gf_point copy2 = copy;
      if( !IsOne(elements[ j ]) ){
	copy2.y = elements[j];
	points.push_back( copy2 );
      }
    }
  }

  start.z = z;
  for( int i = 0; i < count; i++ ){
    gf_point copy = start;
    copy.x = elements[i];
    points.push_back( copy );
  }

  start.y = z;
  points.push_back(start);
  cout << points.size() << endl;

  for( vector<gf_point>::iterator it = points.begin(); it != points.end(); it++ ){
    for( vector<gf_point>::iterator it2 = it + 1; it2 != points.end(); it2++ ){
      for( int i = 0; i < count ; i++ ){
	//	if( !IsOne(elements[i] ) ){
	  if( it->x == it2->x*elements[i] && it->y == it2->y*elements[i] &&
	      it->z == it2->z*elements[i] ){
	    cout << it->x << " " << it->y << " " << it->z << " and ";
	    cout << it2->x << " " << it2->y << " " << it2->z;
	    cout << " ARE EQUIVALENT!" << endl;
	    //	  }
	}
      }
    }
  }

  cout << "SIZE = " << points.size() << endl;

  for( int i = 0; i < points.size(); i++ ){
    cout << i << ": " << points[i].x << " " << points[i].y << " " << points[i].z << endl;
    for( int j = 0; j < points.size(); j++ ){
      if( i != j ){
	if( IsZero( points[i].x * points[j].x + points[i].y * points[j].y + points[i].z * points[j].z ) ){
	  //	  cout << points[i].x << " " << points[i].y << " " << points[i].z << " and ";
	  //cout << points[j].x << " " << points[j].y << " " << points[j].z << " are edges" << endl;
	  add_edge( i, j );
	}
	if( IsOne( points[i].x * points[j].x + points[i].y * points[j].y + points[i].z * points[j].z ) ){
	  //	  cout << points[i].x << " " << points[i].y << " " << points[i].z << " and ";
	  //cout << points[j].x << " " << points[j].y << " " << points[j].z << " are edges" << endl;
	  // add_edge( i, j );
	}
      }
    }
    //    cout << endl;
  }

  
  /*for( int i = 0; i < points.size(); i++ ){
    cout << i << ": " << vdegree[i] << endl;
    }*/

  // if we want to cut out the absolute and hyperbolic vertices
  if( cut ){
    // Find absolute vertices
    int absoluteNumber = 0;
    vector<int> cuts;
    bool cutThese[n];
    for( int i = 0; i < n; i++ ){
      cutThese[i] = false;
    }

    // find all absolute verticfes
    for( int i = 0; i < n; i++ ){
      if( vdegree[i] == q){
	absoluteNumber++;
	cuts.push_back( i );
      }
    }


    if( absoluteNumber != q + 1 && p != 2){
	cerr << "Error: graph did not generate correctly, "
	     << "incorrect number of absolute vertices." << endl;
    }

    // find all vertices adjacent to absolute vertices (hyperbolic)
    for( int i = 0; i < absoluteNumber; i++ ){
      for( int j = 0; j < n; j++ ){
	if( is_edge( cuts[i], j ) ){
	  cutThese[j] = true;
	}
      }
    }

    // we do not want to cut the same vertex twice
    for( int i = 0; i < absoluteNumber; i++ ){
      cutThese[cuts[i]] = false;
    }
    
    for( int i = 0; i < n; i++ ){
      if( cutThese[i] ){
	cuts.push_back(i );
      }
    }

    if( p != 2 && (n - cuts.size()) != (q * (q-1) / 2) ){
      cerr << "Error: graph did not generate correctly, "
	   << "number of neither hyperbolic nor absolute is not correct"
	   << endl;
    }
    
    remove_vs( cuts, cuts.size() );

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
    sum += graphs[i]->order();
  }
  if( sum != n ){
    throw "Error: Orders of graphs do not sum to " + order();
    return false;
  }
  int current = 0;
  int start = 0;
  int end;
  while( current < num ){
    end = graphs[current]->order();
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

int g::connect_graphs( g* g1, g* g2, bool avoid, bool rand, int k ){
  int edgesAdded = 0;
  vector<g*> graphs;
  graphs.push_back(g1);
  graphs.push_back(g2);
  bool joined = join_graphs( 2, graphs);
  if( joined ){
    cout << "Joined succesfully." << endl;
  }
  vector<edge> edges_in;
  edge e;
  cout << "Edges added: ";

  for( int i = 0; i < g1->order(); i++ ){
    for( int j = g1->order(); j < n; j++ ){
      if( !rand ){
	if( avoid ){
	  if( !causes_k( i, j, k ) ){
	    add_edge(i,j);
	    edgesAdded++;
	    cout << "(" << i << "," << j << ") ";
	  }
	}
	else{
	  add_edge(i,j);
	  edgesAdded++;
	}
      }
      else{
	e.u = i;
	e.v = j;
	edges_in.push_back( e );
      }
    }
  }
  if( rand ){
    edgesAdded = k4_free_process( &edges_in );
  }

  return edgesAdded;
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
    break; 
  }
  case 5:{
    for( int i = 0; i < n-4; i++ ){
      for( int j = i; j < n-3; j++ ){
	if( is_edge(i, j) ){
	  for( int k = j; k < n-2; k++ ){
	    if( is_edge(i,k) && is_edge(j,k) ){
	      for( int l = k; l < n-1; l++ ){
		if( is_edge(i,l) && is_edge(j,l) && is_edge(k,l) ){
		  for( int x = l; x < n; x++ ){
		    if( is_edge(i,x) && is_edge(j,x) && is_edge(k,x)
			&& is_edge(l,x) ){
		      numK++;
		      if(remove){
			remove_edge(l,x);
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }  
    }
    break;
  }
  case 6:{
    for( int i = 0; i < n-5; i++ ){
      for( int j = i; j < n-4; j++ ){
	if( is_edge(i, j) ){
	  for( int k = j; k < n-3; k++ ){
	    if( is_edge(i,k) && is_edge(j,k) ){
	      for( int l = k; l < n-2; l++ ){
		if( is_edge(i,l) && is_edge(j,l) && is_edge(k,l) ){
		  for( int x = l; x < n-1; x++ ){
		    if( is_edge(i,x) && is_edge(j,x) && is_edge(k,x)
			&& is_edge(l,x) ){
		      for( int y = x; y < n; y++ ){
			if( is_edge(i,y) && is_edge(j,y) && is_edge(k,y)
			    && is_edge(l,y) && is_edge(x,y) ){
			  numK++;
			  if(remove){
			    remove_edge(l,x);
			  }
			}
		      }
		    }
		  }
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
  case 5:{
    for( int i = 0; i < n-4; i++ ){
      for( int j = i; j < n-3; j++ ){
	if( is_edge(i, j) ){
	  for( int k = j; k < n-2; k++ ){
	    if( is_edge(i,k) && is_edge(j,k) ){
	      for( int l = k; l < n-1; l++ ){
		if( is_edge(i,l) && is_edge(j,l) && is_edge(k,l) ){
		  for( int x = l; x < n; x++ ){
		    if( is_edge(i,x) && is_edge(j,x) && is_edge(k,x)
			&& is_edge(l,x) ){
		      return true;   
		    }
		  }
		}
	      }
	    }
	  }
	}
      }  
    }
    break;
  }
  case 6:{
    for( int i = 0; i < n-5; i++ ){
      for( int j = i; j < n-4; j++ ){
	if( is_edge(i, j) ){
	  for( int k = j; k < n-3; k++ ){
	    if( is_edge(i,k) && is_edge(j,k) ){
	      for( int l = k; l < n-2; l++ ){
		if( is_edge(i,l) && is_edge(j,l) && is_edge(k,l) ){
		  for( int x = l; x < n-1; x++ ){
		    if( is_edge(i,x) && is_edge(j,x) && is_edge(k,x)
			&& is_edge(l,x) ){
		      for( int y = x; y < n; y++ ){
			if( is_edge(i,y) && is_edge(j,y) && is_edge(k,y)
			    && is_edge(l,y) && is_edge(x,y) ){
			  return true;   
			}
		      }
		    }
		  }
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



bool g::has_c( int c ){
  switch(c){
  case 4:{
    for( int i = 0; i < n - 3; i++ ){
      for( int j = i+1; j < n - 2; j++ ){
	if( is_edge( i, j ) ){
	  for( int k = j+1; k < n - 1; k++ ){
	    if( is_edge( i, k ) ){
	      for( int l = k+1; l < n; l++ ){
		if( is_edge( j, l ) && is_edge( k, l ) ){
		  cout << i << " " << j << " " << k << " " << l << endl;
		  return true;
		}
	      }
	    }
	    else if( is_edge( j, k ) ){
	      for( int l = k+1; l < n; l++ ){
		if( is_edge( i, l ) && is_edge( k, l ) ){
		  cout << i << " " << j << " " << k << " " << l << endl;
		  return true;
		}
	      }
	    }
	  }
	}
      }
    }
    return false;
  }
  default:
    cerr << "Error: Counting/Removing C" << c << " is not supported" << endl;
    return false;
  }
  

}


vector<int*>* g::get_ks( int k ){
  int count = 0;
  switch(k){
  case 5:{
    for( int i = 0; i < n-4; i++ ){
      for( int j = i; j < n-3; j++ ){
	if( is_edge(i, j) ){
	  for( int k = j; k < n-2; k++ ){
	    if( is_edge(i,k) && is_edge(j,k) ){
	      for( int l = k; l < n-1; l++ ){
		if( is_edge(i,l) && is_edge(j,l) && is_edge(k,l) ){
		  for( int x = l; x < n; x++ ){
		    if( is_edge(i,x) && is_edge(j,x) && is_edge(k,x)
			&& is_edge(l,x) ){
		      ks.push_back(new int[5]);
		      ks[count][0] = i;
		      ks[count][1] = j;
		      ks[count][2] = k;
		      ks[count][3] = l;
		      ks[count][4] = x;
		      count++;
		      // cout << i << " " << j << " " << k << " " << l << " " << x << endl;
		    }
		  }
		}
	      }
	    }
	  }
	}
      }  
    }
    
    break;
  }
  case 6:{

    break;
  }

  default:
    cout << "Error: Counting/Removing K" << k << " is not supported" << endl;
  }

  //cout << "count = " << count << endl;
  
  return &ks;
}

bool g::causes_k( int u, int v, int k ){
  switch(k){
  case 4:{
    for( int i = 0; i < n-1; i++ ){
      if( is_edge( u, i ) && is_edge( v, i )){
	for( int j = 0; j < n; j++ ){
	  if( is_edge( u, j ) && is_edge( v, j ) && is_edge(i, j )){
	    return true;
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
	case 5:{
	  for( int j = 0; (j < n - 2) && canAdd; j++ ){
	    if( is_edge(j,v) && is_edge(j,i) ){
	      for( int k = j; (k < n-1) && canAdd; k++ ){
		if( is_edge(k,v) && is_edge(k,i) && is_edge(k,j) ){
		  for( int l = k; (l < n) && canAdd; l++ ){
		    if( is_edge(l,v) && is_edge(l,i) && is_edge(l,j)
			&& is_edge(l,k) ){
		      canAdd = false;
		    }
		  }
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
    if( is_edge( 0, d ) ){
      cout << "...already exists" << endl;
    }
    else{
      add_circ_edge( d );
      if( is_k(k) ){
	remove_circ_edge( d );
	cout << "...NOT added" << endl;
      }
      else{
	dists.push_back( d );
	cout << "...added" << endl;
      }
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
    if( is_k( k ) ){
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

void g::get_tris( bool vertex ){
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
	    if( !vertex ){
	      a = edges[i][ii];
	      b = edges[i][iii];
	      c = edges[ii][iii];
	      tris[numTri][0] = a;
	      tris[numTri][1] = b;
	      tris[numTri][2] = c; 
	    }
	    else{
	      tris[numTri][0] = i;
	      tris[numTri][1] = ii;
	      tris[numTri][2] = iii;
	      inTri[i] = true;
	      inTri[ii] = true;
	      inTri[iii] = true;
	    }
	    numTri++;
	  }
	} 
      }
    }
  }
}

void g::get_k4s( bool vertex ){
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
		if( !vertex ){
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
		}
		else{
		  k4s.push_back( new int[4] );
		  k4s[numK4s][0] = i + 1;
		  k4s[numK4s][1] = j + 1;
		  k4s[numK4s][2] = k + 1;
		  k4s[numK4s][3] = l + 1;
		  inKs[i]=true;
		  inKs[j]=true;
		  inKs[k]=true;
		  inKs[l]=true;
		}
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

void g::create_h( g * h ){
  recount_data();

  int a, b, c;
  int j = 0;
  string weight;

  for( int t = 0; t < numTris; t++ ){
    a = tris[t][0] - 1;
    b = tris[t][1] - 1;
    c = tris[t][2] - 1;

    h->add_edge( a, b );
    h->add_edge( a, c );
    h->add_edge( b, c );

  }
}


// Prints the adjacency matrix of the graph
// @param o - the stream to print out to (default is standard out)
void g::print( ostream * o ){
  uint64_t z;
  int count;
  bool go = true;
  for( int i = 0; i < n; i++ ){
    count = 0;
    go = true;
    for( int j = 0; j < arraySize && go; j++ ){
      z = shifter << (intSize - 1);
      for( ; z>0 && go; z >>= 1 ){
	*o << (((gA[i][j] & z ) == z) ? " 1":" 0");
	count ++;
	if( count == n ){
	  go = false;
	}
      }		 
    }
    *o << endl;
  }
}


// Prints the graph in g6 format for nauty
// @param o - the stream to print out to (default is standard out)
void g::print_g6( ostream *o ){
  const int powers[] = {32,16,8,4,2,1};
  vector<char> g6_string;
  char start = 0;

  if( n <= 62 ){
    start = n+63;
  }

  g6_string.push_back( start );

  int bitSize = n*(n-1)/2;
  int count = 0;
  int cur = 63;
  int pow = 0;

  for( int i = 0; i < n; i++ ){
    for( int j = 0; j < i; j++ ){
      pow = count % 6;
      if( is_edge( j, i ) ){
	cur = cur + powers[pow];
      }
      if( pow == 5 || j == n - 2 ){
	g6_string.push_back( cur );
	cur = 63; 
      }
      count++;
    }
  }
  
  for( vector<char>::iterator it = g6_string.begin(); it != g6_string.end();
       it++ ){
    *o << *it;
  }
  *o << endl;
}


void g::print_sparse_h( ostream * o, bool isRudy ){
  recount_data();

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
  recount_data();

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

void g::print_sat( ostream *o, bool weighted, int numWeighted ){
  int top = 0;
  recount_data();

  int a, b, c;

  if( numWeighted > numTris ){
    weighted = false;
  }

  if( weighted ){
    top = 2 * ( numTris - numWeighted + 1 );
  }
  
  *o << "c SAT reduction for arrowing triangles " << endl;
  *o << "c" << endl;
  *o << "p ";
  if( weighted ){
    *o << "w";
  }
  *o << "cnf " << numEdges << " " << numTris*2;
  if( weighted ){
    *o << " " << top;
  }
  *o << endl;

  int numHard = 0;
  int numSoft = 0;

  for( int t = 0; t < numTris; t++ ){
    a = tris[t][0];
    b = tris[t][1];
    c = tris[t][2];
    if( weighted ){
      if( numHard < numWeighted ){
	*o << top;
      }
      else{
	*o << "1";
	numSoft++;
      }
      *o << " ";
    }
    *o << a << " " << b << " " << c << " 0" << endl;

    if( weighted ){
      if( numHard < numWeighted ){
	*o << top;
	numHard++;
      }
      else{
	*o << "1";
      }
      *o << " ";
    }
    *o << "-" << a << " -" << b << " -" << c << " 0" << endl;
  }
  cout << "Number of triangles hard = " << numHard << endl;
  cout << "Number of triangles soft = " << numSoft << endl;
}


void g::print_sat34( ostream *o ){
  recount_data();
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

void g::print_satv44( ostream * o ){
  int a, b, c, d;
  int numVar = 0;
  recount_data();
  get_k4s( true );
  for( int i = 0; i < n; i++ ){
    if( inKs[i] ){
      numVar++;
    }
  }

  *o << "c SAT reduction for vertex arrowing " << endl;
  *o << "c For arrowing (4,4)^v" << endl;
  *o << "c" << endl;
  *o << "p cnf " << numVar << " " << numK4s*2 << endl;

  for( int t = 0; t < numK4s; t++ ){
    a = k4s[t][0];
    b = k4s[t][1];
    c = k4s[t][2];
    d = k4s[t][3];
    *o << a << " " << b << " " << c << " " << d << " 0" << endl;
    *o << "-" << a << " -" << b << " -" << c << " -" << d << " 0" << endl;
  }
}

void g::recount_data(){
  if( beenModified ){
    recalc_edges();
    
    for( int i = 0; i < numTris; i++ ){
      delete[] tris[i];
    }
    delete[] tris;
    get_tris();

    beenModified = false;
  }
}
