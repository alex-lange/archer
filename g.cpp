#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <utility>
#include <math.h>


#include "g.h"
#include "number_algorithms.h"

using namespace std;


int g::num_tris(){
  recount_data();
  return numTris;
}

void g::add_circ_edge( int d ){
  int x;
  for( int v = 0; v < n; v++ ){
    x = ( v + d ) % n;
    add_edge( v, x );
  }
}

void g::remove_circ_edge( int d ){
  int x;
  for( int v = 0; v < n; v++ ){
    x = ( v + d ) % n;
    remove_edge( v, x );
  }
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


void g::make_gcd_circ( int start ){
  int numAdded = 0;
  vset D(arraySize,0);
  for( int64_t i = start; i < n; i++ ){
    if( n % i == 0 ){
      set_insert( i, D );
    }
  }
  for( int v = 0; v < n; v++ ){
    for( int x = v + 1; x < n; x++ ){
      if( in_set( gcd(x - v, n), D ) ){
	add_edge( v, x );
	numAdded++;
      }
    }
  }
  cout << "Added " << numAdded << " edges" << endl;

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


void g::make_turan( int r ){
  int num_large = n % r;
  int num_small = r - num_large;

  int num_per_large, num_per_small;

  if( num_large == 0 ){
    num_per_large = n / r;
    num_per_small = num_per_large;
  }
  else{
    num_per_large = n / r + 1;
    num_per_small = num_per_large - 1;
  }

  cout << "num_large " << num_large << endl;
  cout << "num_small " << num_small << endl;
  cout << "num_per_large " << num_per_large << endl;
  cout << "num_per_small " << num_per_small << endl;

  int cur_group = 0;
  bool large_group = false;

  if( num_large > 0 )
    large_group = true;
  
  for( int v = 0; v < n; v++ ){
    if( large_group && cur_group >= num_large ){
      large_group = false;
      cur_group = 0;
    }
    if( large_group ){
      if( v % num_per_large == 0 && v != 0 )
	cur_group++;
      for( int u = 0; u < n; u++ ){
	if( u < cur_group * num_per_large || 
	    u >= (cur_group+1) * num_per_large ){
	  add_edge( u, v );
	}
      }
    }
    else{
      if( (v - num_large*num_per_large) % num_per_small == 0 && v != num_large*num_per_large ){
	cur_group++;
      }
      int group_area = num_large*num_per_large + cur_group*num_per_small;
      cout << group_area << endl;
      for( int u = 0; u < n; u++ ){
	if( u <  group_area || u >= (group_area+num_per_small) ){
	  add_edge( u, v );
	}
      }
    }
  }
}

int ** hamming;
int cur_hami;

void g::make_hamming( int d, int x ){
  int length = pow( x, d );
  if( n != length ){
    cerr << "Error: order is " << n << ". Need order " << length << endl;
    return;
  }
  hamming = new int*[ length ];
  for( int i = 0; i < length; i++ ){
    hamming[ i ] = new int[d];
  }
  cur_hami=0;
  int * cur_ham = new int[d];
  make_hamming_help(d,0,cur_ham);

  cout << "Creating hamming graph with order=" << length << ", d="<< d
       << " and q=" << x << endl;

  // Print vectors just to make sure it's right
  /* for( int i = 0; i < length; i++ ){
    for( int j = 0; j < d; j++ ){
      cout << hamming[i][j];
    }
    cout << endl;
    }*/
  
  int num_diff = 0;
  for( int i = 0; i < length-1; i++ ){
    for( int j = i+1; j < length; j++ ){
      num_diff = 0;
      for( int k = 0; k < d; k++ ){
	if( hamming[i][k] != hamming[j][k] )
	  num_diff++;
      }
      if( num_diff == 1 )
	add_edge( i, j );
    }
  }
}

void g::make_hamming_help( int d, int s, int * cur_ham ){
  if( s == d ){
    for( int i = 0; i < d; i++ ){
      hamming[ cur_hami ][i] = cur_ham[i];
    }
    cur_hami++;
  }
  else{
    for( cur_ham[s] = 0; cur_ham[s] < 3; cur_ham[s]++ ){
      make_hamming_help(d,s+1,cur_ham);
    }
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

  return &ks;
}


int g::get_k4me(){
  for( vector<int*>::iterator it = ks.begin(); it != ks.end(); it++ ){
    delete *it;
  }
  ks.clear();
  if( beenModified ){
    recalc_edges();
  }
  int num_kme = 0;
 
  // i j and k find all triangles
  for( int i = 0; i < n-1; i++ ){
    for( int j = i+1; j < n; j++ ){
      if( is_edge(i, j) ){
	for( int k = 0; k < n-1; k++ ){
	  if( k != i && k!= j ){
	    if( is_edge(i,k) && is_edge(j,k) ){
	      for( int l = k+1; l < n; l++ ){
		if( l != i && l!= j ){
		  if( is_edge(i,l) && is_edge(j,l) ){
		    ks.push_back( new int[5] );
		    ks[num_kme][0] = edges[i][j]; 
		    ks[num_kme][1] = edges[i][k];
		    ks[num_kme][2] = edges[i][l];
		    ks[num_kme][3] = edges[j][k];
		    ks[num_kme][4] = edges[j][l];
		    num_kme++;
		  }
		  //	cout << i << " " << j << " " << k << " " << l << endl;
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
  return num_kme;
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
  char next = 0;

  if( n <= 62 ){
    start = n+63;
    g6_string.push_back( start );
  }
  else if(63 <= n && n <= 258047){
    start = 126;
    g6_string.push_back(start);
    //   cout << 126 << " ";
    int s = 6;
    int t = 0;
    for( int i = 17; i >= 0; i-- ){
      s--;
      if( n & ( 1 << i ) ){
	t = t | ( 1 << s );
      }
      if( s == 0 ){
	//	cout << t + 63 << " ";
	next = t + 63;
	g6_string.push_back(next);
	s = 6;
	t = 0;
      }
    }
    cout << endl;
  }
  else{
    cout << "Error: Order not supported" << endl;
  }

  

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


string g::to_g6(){
  const int powers[] = {32,16,8,4,2,1};
  string g6_string;
  char start = 0;
  char next = 0;

  if( n <= 62 ){
    start = n+63;
    g6_string.push_back( start );
  }
  else if(63 <= n && n <= 258047){
    start = 126;
    g6_string.push_back(start);
    //   cout << 126 << " ";
    int s = 6;
    int t = 0;
    for( int i = 17; i >= 0; i-- ){
      s--;
      if( n & ( 1 << i ) ){
	t = t | ( 1 << s );
      }
      if( s == 0 ){
	//	cout << t + 63 << " ";
	next = t + 63;
	g6_string.push_back(next);
	s = 6;
	t = 0;
      }
    }
    cout << endl;
  }

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
  
  return g6_string;
}


void g::print_graphviz( ostream * o ){
  *o << "graph g {" << endl;
  for( int i = 0; i < n-1; i++ ){
    for( int j = i + 1; j < n; j++ ){
      if( is_edge(i,j) )
	*o << i << " -- " << j << ";" << endl;
    }
  }
  *o << "}" << endl;
}


void g::print_sparse_h( ostream * o, bool isRudy ){
  recount_data();
  cout << "Recounted data" << endl;

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


void g::print_sat4me( ostream *o ){
  int a, b, c, d, e;
  int num = get_k4me();

  *o << "c SAT reduction for edge arrowing (K4-e,K4-e)" << endl;
  *o << "c" << endl;
  *o << "p cnf " << numEdges << " " << num*2 << endl;

  for( int i = 0; i < num; i++ ){
    for( int j = 0; j < 5; j++ ){
      *o << ks[i][j] << " ";
    }
    *o << "0" << endl;
    for( int j = 0; j < 5; j++ ){
      *o << "-" << ks[i][j] << " ";
    }
    *o << "0" << endl;
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
    
    if( !firstGo ){
      for( int i = 0; i < numTris; i++ ){
	delete[] tris[i];
      }
      delete[] tris;
    }
    else{
      firstGo = false;
    }
    get_tris();

    beenModified = false;
  }
}
