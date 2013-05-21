#include "g.h"

using namespace std;

g::g( int vsize ) :
  n( vsize )
{
  set_up();
}

// copy constructor
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

// Destructor
g::~g(){
  for( int i = 0; i < oldN; i++ ){
    delete[] edges[i];
  }

  delete[] edges;
  k4s.clear();
  
  delete[] inTri;
  delete[] inKs;
  delete[] vdegree;
  
  for( int i = 0; i < n; i++ ){
    for( int j = 0; j < n; j++ ){
      delete[] isTri[i][j];
    }
    delete[] isTri[i];
  }
  delete[] isTri;

  for( vector<int*>::iterator it = ks.begin(); it != ks.end(); it++ ){
    delete *it;
  }
  ks.clear();
}

void g::set_up(){
  if( n < 1000 ){
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
    firstGo = true;
  }
  else{
    cerr << "WARNING: Graph is large, using less memory." << endl;
    oldN = n;
    arraySize = n / intSize;
    if( ( n % intSize ) != 0 ) arraySize++;
    gA.resize( n );
    gB.resize( n );
    vdegree = new int[n];
    for( int i = 0; i < n; i++ ){
      gA[i].resize( arraySize, 0 );
      gB[i].resize( arraySize, 0 );
      vdegree[i] = 0;
      for( int b = i + 1; b < n; b++ ){
	set_insert( b, gB[i] );
      }
    }
    numEdges = 0;
    beenModified = true;
    firstGo = true;
  }

}


int g::order() const{
  return n;
}

int g::size(){
  return num_edges();
}


int g::num_edges(){
  recount_data();
  return numEdges;
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

int g::max_degree(){
  int max = 0;
  cout << "degrees..." << endl;
  for( int i = 0; i < n; i++ ){
    cout << vdegree[i] << " ";
    if( vdegree[i] > max ){
      max = vdegree[i];
    }
  }
  cout << endl;
  return max;
}

void g::max_clique_backtrack( int l, int k ){
    // get new optimum value
  if( l > optSize ){
    optSize = l;
    optClique = X;
  }

  if( optSize == k ){
    return;
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
    max_clique_backtrack( l + 1, k );
    set_delete( x, cl );
    x = first_bit( cl, arraySize );
  }
}


vector<int> g::max_clique( bool print, int k ){
  // set all global variables needed
  // (only use them for max-clique, so no use putting them in set_up)
  gC.resize( n + 1 );
  gV.resize( arraySize );
  for( int i = 0; i < arraySize; i++ ){
    gV[i] = empty;
  }
  X.resize( n );
  
  if( k < 1 ){
    k = n;
  }
 
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
  
  max_clique_backtrack( 0, k );
  if(print){
    cout << "Order: " << optSize << endl;
  }
  vector<int> realClique;
  for( int i = 0; i < optSize; i++ ){
    realClique.push_back( optClique[i] );
  }

  return realClique;
}


vector<int> g::max_independent_set( bool print, int k ){
  make_complement();
  vector<int> realClique = max_clique( print, k );
  make_complement();
  return realClique;
}


bool g::has_clique( int k, bool is ){
  bool hasClique = false;
  if( is ){
    make_complement();
  }
  max_clique( false, k );
  if( optSize == k ){
    hasClique = true;
  }
  if( is ){
    make_complement();
  }
  return hasClique;
}

void g::read_g6( string g6 ){
  int numEntries = n*(n-1)/2;
  int c = 0;
  int c_count;
  if( n < 63 ) c_count = 1;
  else c_count = 4;
  int i = 0;
  int j = 0;
  int b = 0;
  for( int i = 0; i < n; i++ ){
    for( int j = 0; j < i; j++ ){
      if( b == 0 ){
	c = g6[c_count];
	c_count++;
	c = c - 63;
      }
      if( c & ( 1 << ( 5 - b ) ) ){
	add_edge( i, j );
      }
      b = ( b + 1 ) % 6;
    }
  }
}
