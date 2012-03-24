//
// File: g.h
//

#ifndef _G_H
#define _G_H

#include <vector>
#include "set_operations.h"

using namespace std;

class g {

 public:
  g( int vsize );
  ~g();

 public:
  void add_edge( int u, int v );
  void remove_edge( int u, int v);
  void remove_vs( int *v, int k );
  void make_residue_circ( int r );
  void make_circ( vector<int> dists );
  void make_embedded_rc( int r, int num = 2 );
  bool add_noncrit_edge( int v, bool avoid = true, int k = 4 );
  int remove_k( int k = 4, bool remove = true );
  int add_all_noncrit( bool avoid = true, int k = 4 );
  void print( ostream * o = &cout );
  void print_sparse_h( ostream * = &cout, bool isRudy = false );
  void print_sat( ostream * o = &cout );
  
 private:
  void count_tris();
  void get_tris();
  void recalc_edges();
 
 private:
  int n, arraySize, numEdges, oldN;
  int numTris;
  int ** edges;
  int ** tris;
  bool calcedTris;
  vector<vset> gA;

};

#endif
