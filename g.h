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
  g( const g &otherG );
  ~g();
  struct edge;

 public:
  int order() const;
  int num_edges();
  int num_tris();
  void add_edge( int u, int v );
  void add_circ_edge( int d );
  void remove_edge( int u, int v);
  void remove_circ_edge( int e );
  bool is_edge( int u, int v ) const;
  int min_degree();
  vector<int> max_clique();
  vector<int> max_independent_set();
  void remove_vs( vector<int> vs, int k );
  void remove_distvs( int k, int d, int s = 0 );
  void remove_randvs( int num );
  int k4_free_process();
  int k4_free_process( vector<edge> * edges_in);
  void make_complement();
  void make_cycle();
  void make_complete();
  void make_residue_circ( int r );
  bool make_l_circ( int s );
  void make_galois_circ( int p, int n, int r );
  void make_projective_plane( int p, int k, bool cut = false,
			      string pr = "[ 1 1 ]" );
  void make_circ( vector<int> dists );
  void make_embedded_rc( int r, int num = 2 );
  int make_rand_er( float sigma );
  void load_adj( string filename);
  bool join_graphs( int num, vector<g*> graphs );
  int connect_graphs( g* g1, g* g2, bool avoid = false, bool rand = false, int k = 4 );
  bool add_noncrit_edge( int v, bool avoid = true, int k = 4 );
  int remove_k( int k = 4, bool remove = true );
  bool is_k( int k = 4 );
  bool has_c( int c = 4 );
  bool causes_k( int u, int v, int k = 4 );
  vector<int *>* get_ks( int k );
  int add_all_noncrit( bool avoid = true, int k = 4 );
  vector<int> add_all_ce( bool avoid = true, int k = 4 );
  vector<int> add_all_ce_rand( bool avoid = true, int k = 4 );
  void create_h( g * h );
  void print( ostream * o = &cout );
  void print_g6( ostream *o = &cout );
  void print_sparse_h( ostream * = &cout, bool isRudy = false);
  void print_sdpa( ostream * o = &cout );
  void print_sat( ostream * o = &cout, bool weighted = false, int numWeighted = 0 );
  void print_sat34( ostream * o = &cout );
  void print_satv44( ostream * o = &cout );
  
 public:
  struct compZZ_pE;
  struct gf_point;
  struct primTable;

 private:
  void count_tris();
  void get_tris( bool vertex = false );
  void get_k4s( bool vertex = false );
  void recalc_edges();
  void set_up();
  void max_clique_backtrack( int l );
  void fix_data();
  void recount_data();
 
 private:
  int n, arraySize, numEdges, oldN;
  int numTris;
  int numK4s;
  int ** edges;
  int ** tris;
  int * vdegree;
  vector< int * > k4s;
  vector< int * > k4s_v;
  vector< int * > ks;
  bool * inTri;
  bool * inKs;
  bool beenModified;
  bool *** isTri;
  vector<vset> gA;
  vector<vset> gB;
  vset gV;

  // used for max-clique algorithm
  vector<vset> gC;
  vector< int > X, optClique;
  int optSize;

};

#endif
