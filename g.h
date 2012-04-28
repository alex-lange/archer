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
  int size();
  int num_edges();
  void add_edge( int u, int v );
  void add_circ_edge( int d );
  void remove_edge( int u, int v);
  void remove_circ_edge( int e );
  bool is_edge( int u, int v );
  void remove_vs( vector<int> vs, int k );
  void remove_distvs( int k, int d, int s = 0 );
  void remove_randvs( int num );
  void make_cycle();
  void make_complete();
  void make_residue_circ( int r );
  void make_galois_circ( int p, int n, int r );
  //void make_galois_circ( long p, long n );
  void make_circ( vector<int> dists );
  void make_embedded_rc( int r, int num = 2 );
  void load_adj( string filename);
  bool join_graphs( int num, vector<g*> graphs );
  int connect_graphs( g* g1, g* g2 );
  bool add_noncrit_edge( int v, bool avoid = true, int k = 4 );
  int remove_k( int k = 4, bool remove = true );
  bool is_k( int k = 4 );
  int add_all_noncrit( bool avoid = true, int k = 4 );
  vector<int> add_all_ce( bool avoid = true, int k = 4 );
  vector<int> add_all_ce_rand( bool avoid = true, int k = 4 );
  void print( ostream * o = &cout );
  void print_sparse_h( ostream * = &cout, bool isRudy = false);
  void print_sdpa( ostream * o = &cout );
  void print_sat( ostream * o = &cout );
  void print_sat34( ostream * o = &cout );
  void print_satv44( ostream * o = &cout );
  
 public:
  struct compZZ_pE;
  struct primTable;

 private:
  void count_tris();
  void get_tris( bool vertex = false );
  void get_k4s( bool vertex = false );
  void recalc_edges();
 
 private:
  int n, arraySize, numEdges, oldN;
  int numTris;
  int numK4s;
  int ** edges;
  int ** tris;
  vector< int * > k4s;
  vector< int * > k4s_v;
  bool * inTri;
  bool * inKs;
  bool calcedTris;
  vector<vset> gA;

};

#endif
