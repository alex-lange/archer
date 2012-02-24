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
  int make_residue_circ( int r );
  void print( ostream * o = &cout );
  void gen_sparse_h( ostream * = &cout );
  
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
