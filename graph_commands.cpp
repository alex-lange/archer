#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

#include "command.h"
#include "g.h"
#include "graph_commands.h"

using namespace std;

get_edges_command::get_edges_command(){
  name = "num_e";
  description = "Returns number of edges";
  GraphCommandBase::base().register_c( name, this );
}

void get_edges_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }

  cout << graph->num_edges( ) << endl;
}

get_edges2_command::get_edges2_command(){
  name = "size";
  description = "Returns number of edges";
  GraphCommandBase::base().register_c( name, this );
}

void get_edges2_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }

  cout << graph->num_edges( ) << endl;
}


order_command::order_command(){
  name = "order";
  description = "Returns number of vertices";
  GraphCommandBase::base().register_c( name, this );
}

void order_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }
  cout << graph->order() << endl;
}


num_tri_command::num_tri_command(){
  name = "num_tris";
  description = "Returns number of triangles of graph";
  GraphCommandBase::base().register_c( name, this );
}

void num_tri_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }
  cout << graph->num_tris() << endl;
}


max_clique_command::max_clique_command(){
  name = "max_cl";
  description = "Returns order of the maximum clique";
  GraphCommandBase::base().register_c( name, this );
}

void max_clique_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->max_clique();
}


max_is_command::max_is_command(){
  name = "max_is";
  description = "Returns order of the maximum independent set";
  GraphCommandBase::base().register_c( name, this );
}

void max_is_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->max_independent_set();
}



remove_edge_command::remove_edge_command(){
  name = "remove_e";
  description = "Removes edge {u,v}";
  GraphCommandBase::base().register_c( name, this );
}

void remove_edge_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  vector<int> iArgs;
  for( int i = 0; i < 2; i++ ){
    if( pos == args.end() ){
      throw "Invalid argument number for " + name;
    } 
    iArgs.push_back( atoi( pos->c_str() ));
    pos++;
  }

  graph->remove_edge( iArgs[0], iArgs[1] );
}


add_edge_command::add_edge_command(){
  name = "add_e";
  description = "Add edge {u,v}";
  GraphCommandBase::base().register_c( name, this );
}

void add_edge_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  vector<int> iArgs;
  for( int i = 0; i < 2; i++ ){
    if( pos == args.end() ){
      throw "Invalid argument number for " + name;
    } 
    iArgs.push_back( atoi( pos->c_str() ));
    pos++;
  }

  graph->add_edge( iArgs[0], iArgs[1] );
}


add_circ_edge_command::add_circ_edge_command(){
  name = "add_ce";
  description = "Add one circular edge of distance D";
  GraphCommandBase::base().register_c( name, this );
}

void add_circ_edge_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->add_circ_edge( atoi( pos->c_str() ) );
}


remove_circ_edge_command::remove_circ_edge_command(){
  name = "remove_ce";
  description = "Removes circular edge based on distance";
  GraphCommandBase::base().register_c( name, this );
}

void remove_circ_edge_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->remove_circ_edge( atoi( pos->c_str() ) );
}


remove_dist_vs_command::remove_dist_vs_command(){
  name = "remove_vsd";
  GraphCommandBase::base().register_c( name, this );
  description = "Removes N vertices that are all at distance D apart";
}

void remove_dist_vs_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 2 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->remove_distvs( atoi( pos->c_str() ), atoi( (pos+1)->c_str()) );
}


remove_rand_vs_command::remove_rand_vs_command(){
  name = "remove_vs_rand";
  description = "Removes N vertices at random";
  GraphCommandBase::base().register_c( name, this );
}

void remove_rand_vs_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->remove_randvs( atoi( pos->c_str() ) );
}


remove_max_is_command::remove_max_is_command(){
  name = "cut_is";
  description = "Removes a maximum independent set from the graph";
  GraphCommandBase::base().register_c( name, this );
}

void remove_max_is_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }
  vector<int> cuts = graph->max_independent_set();
  graph->remove_vs( cuts, cuts.size() );
}


k4_free_proc_command::k4_free_proc_command(){
  name = "k4_free";
  description = "Adds edges using the K4-free process";
    // (randomly permutes all edges, adds edge if no K4 is formed)";

  GraphCommandBase::base().register_c( name, this ); }

void k4_free_proc_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos  != args.end() ){
    throw "Invalid argument number for " + name;
  }
  int count = graph->k4_free_process();
  cout << "Added " << count << " edges." << endl;
}


make_complement_command::make_complement_command(){
  name = "mk_comp";
  description = "Makes the graph the complement of itself";
  GraphCommandBase::base().register_c( name, this );
}

void make_complement_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->make_complement();
}


make_cyc_command::make_cyc_command(){
  name = "mk_cyc";
  description = "Makes graph a cycle graph";
  GraphCommandBase::base().register_c( name, this );
}

void make_cyc_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->make_cycle();
}


make_comp_command::make_comp_command(){
  name = "mk_k";
  description = "Makes graph complete";
  GraphCommandBase::base().register_c( name, this );
}

void make_comp_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->make_complete();
}


make_res_circ_command::make_res_circ_command(){
  name = "mk_rc";
  description = "Makes Residue-Circulant graph (Dudek-Rodl)";
  GraphCommandBase::base().register_c( name, this );
}

void make_res_circ_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->make_residue_circ( atoi( pos->c_str() ) );
}


make_l_circ_command::make_l_circ_command(){
  name = "mk_lc";
  description = "Makes circulant graph as defined by Lu";
  GraphCommandBase::base().register_c( name, this );
}

void make_l_circ_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->make_l_circ( atoi( pos->c_str() ) );
}


make_gcd_circ_command::make_gcd_circ_command(){
  name = "mk_gcd";
  description = "Makes GCD circulant graph";
  GraphCommandBase::base().register_c( name, this );
}

void make_gcd_circ_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->make_gcd_circ( atoi( pos->c_str() ) );
}



make_joined_command::make_joined_command(){
  name = "mk_join";
  description = "Uses 'join' operator on two graphs";
  GraphCommandBase::base().register_c( name, this );
}

void make_joined_command::execute( g * graph, vector<string> args ){
}

void make_joined_command::execute( g * graph, vector<g*> args){
  graph->join_graphs( args.size(), args );
}


make_connected_command::make_connected_command(){
  name = "mk_conn";
  description = "Makes graph 'connected' with out adding any edges";
  GraphCommandBase::base().register_c( name, this );
}

void make_connected_command::execute( g * graph, vector<string> args ){
}

void make_connected_command::execute( g * graph, vector<g*> args){
  graph->connect_graphs( args[1], args[0] );
}



make_avoid_connected_command::make_avoid_connected_command(){
  name = "mk_aconn";
  description = "Connects two graphs, greedily adding edges with out causing a clique of order K";
  GraphCommandBase::base().register_c( name, this );
}

void make_avoid_connected_command::execute( g * graph, vector<string> args ){
}

void make_avoid_connected_command::execute( g * graph, vector<g*> args){
  cout << graph->connect_graphs( args[1], args[0], true, false, 4 ) << " edges added." << endl;
}



make_avoid_rand_connected_command::make_avoid_rand_connected_command(){
  name = "mk_arconn";
  description = "Connects two graphs, randomly adding edges with out causing a clique of order K";
  GraphCommandBase::base().register_c( name, this );
}

void make_avoid_rand_connected_command::execute( g * graph, vector<string> args ){
}

void make_avoid_rand_connected_command::execute( g * graph, vector<g*> args){
  cout << graph->connect_graphs( args[1], args[0], true, true, 4 ) << " edges added." << endl;
}



make_galois_circ_command::make_galois_circ_command(){
  name = "mk_gc";
  description = "Makes residue-circulant graph based off the Galois Field of the order";
  GraphCommandBase::base().register_c( name, this );
}

void make_galois_circ_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 3 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->make_galois_circ( atoi(pos->c_str()),atoi((pos+1)->c_str()),
			   atoi((pos+2)->c_str()));
}


make_projective_plane_command::make_projective_plane_command(){
  name = "mk_pp";
  description = "Makes graph based on projective plane";
  GraphCommandBase::base().register_c( name, this );
}

void make_projective_plane_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 2 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->make_projective_plane( atoi(pos->c_str()),atoi((pos+1)->c_str() ));
}


make_projective_plane_cut_command::make_projective_plane_cut_command(){
  name = "mk_ppc";
  description = "makes graph based on projective plane (only the 'absolute' vertices)";
  GraphCommandBase::base().register_c( name, this );
}

void make_projective_plane_cut_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 2 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->make_projective_plane( atoi(pos->c_str()),atoi((pos+1)->c_str() ), true );
}



make_circ_command::make_circ_command(){
  name = "mk_circ";
  description = "Makes graph circulant based on list of distances given";
  GraphCommandBase::base().register_c( name, this );
}

void make_circ_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }

  string filename = *pos;
  ifstream dists( filename.c_str() );
  int k;
  int d;
  vector< int > ds;
  if( dists.is_open() ){
    dists >> k;
    for( int i = 0; i < k; i++ ){
      dists >> d;
      ds.push_back( d );
    }
  }
  else{
    throw "Error opening file " + filename;
  }
  graph->make_circ( ds );
}


make_embedded_rc_command::make_embedded_rc_command(){
  name = "mk_erc";
  description = "Embeds multiple Residue-Circulant graphs inside graph";
  GraphCommandBase::base().register_c( name, this );
}

void make_embedded_rc_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 2 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  
  graph->make_embedded_rc( atoi( pos->c_str() ), atoi( (pos+1)->c_str()) );
}


make_turan_command::make_turan_command(){
  name = "mk_turan";
  description = "Makes Turan graph";
  GraphCommandBase::base().register_c( name, this );
}

void make_turan_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->make_turan( atoi( pos->c_str() ));
}



make_hamming_command::make_hamming_command(){
  name = "mk_hamming";
  description = "Makes Hamming graph H(q,d)";
  GraphCommandBase::base().register_c( name, this );
}

void make_hamming_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 == args.end() ){
    graph->make_hamming( atoi( pos->c_str() ) );
  }
  else if( pos + 2 == args.end() ){
    graph->make_hamming( atoi( pos->c_str() ), atoi( (pos+1)->c_str() ));
  }
  else{
    throw "Invalid argument number for " + name;
  }
}


load_adj_command::load_adj_command(){
  name = "ld_adj";
  description = "Loads file containing the adjacency list of a graph";
  GraphCommandBase::base().register_c( name, this );
}

void load_adj_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  
  graph->load_adj( *pos );
}

read_g6_command::read_g6_command(){
  name = "read_g6";
  description = "Reads in graph6 string and creates graph from it";
  GraphCommandBase::base().register_c( name, this );
}

void read_g6_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  
  graph->read_g6( *pos );
}



add_all_noncrit_command::add_all_noncrit_command(){
  name = "add_all";
  description = "Greedily add all edges that do not cause a k-clique";
  GraphCommandBase::base().register_c( name, this );
}

void add_all_noncrit_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos+1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  int k = atoi( pos->c_str() );
  int added = graph->add_all_noncrit(true,k);
  cout << "Added " << added << " edges avoiding K" << k << endl;
}



add_all_ce_command::add_all_ce_command(){
  name = "add_allce";
  description = "Greedily add all circular edges that do not cause a k-clique";
  GraphCommandBase::base().register_c( name, this );
}

void add_all_ce_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  int k = 4;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }
  vector<int> added = graph->add_all_ce();
  //cout << "Added distances " << added << endl;
}


add_all_cer_command::add_all_cer_command(){
  name = "add_allcer";
   description = "Randomly add all circular edges that do not cause a k-clique";
  GraphCommandBase::base().register_c( name, this );
}

void add_all_cer_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
 
  if( pos+1 != args.end() && pos+3 != args.end() ){
    throw "Invalid argument number for " + name;
  }

  int k = atoi( pos->c_str() );
  pos++;
  
  vector<int> added = graph->add_all_ce_rand(true,k);
  
  if( pos != args.end() ){
    string filename = *pos;
    string gname = *(pos+1);
    fstream distLog;
    distLog.open(filename.c_str(), fstream::in | fstream::out | fstream::app );
    distLog << gname;
    distLog << " - added distances: ";
    for( vector<int>::iterator it = added.begin(); it != added.end(); it++ ){
      distLog << *it << " ";
    }
    distLog << endl;
  }
  for( vector<int>::iterator it = added.begin(); it != added.end(); it++ ){
    cout << *it << " ";
  }
  cout << endl;
}


remove_k_command::remove_k_command(){
  name = "remove_k";
  description = "Removes each clique of order K by deleting edges greedily";
  GraphCommandBase::base().register_c( name, this );
}

void remove_k_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  int k = atoi( pos->c_str() );
  int removed = graph->remove_k( k );
  cout << "Removed " << removed << " edges" << endl;
}


count_k_command::count_k_command(){
  name = "count_k";
  description = "Count number of complete subgraphs of order K";
  GraphCommandBase::base().register_c( name, this );
}

void count_k_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  int k = atoi( pos->c_str() );
  int count = graph->remove_k( k, false );
  cout << "Graph has " << count << " K" << k << "'s" << endl;
}


count_kme_command::count_kme_command(){
  name = "count_kme";
  description = "Count number of complete subgraphs (minus one edge) of order K";
  GraphCommandBase::base().register_c( name, this );
}

void count_kme_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  int k = atoi( pos->c_str() );
  int count = graph->get_k4me( );
  cout << "Graph has " << count << " K4-e subgraphs" << endl;
}



has_c_command::has_c_command(){
  name = "has_c";
  description = "Checks if graphs contains cycle of order C";
  GraphCommandBase::base().register_c( name, this );
}

void has_c_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  int c = atoi( pos->c_str() );
  bool has = graph->has_c( c );
  cout << "Graph has ";
  if( !has ){
    cout << "NO ";
  }
  cout << "C" << c << endl;
}


print_all_command::print_all_command(){
  name = "print_all";
  description = "Prints adjacency matrix, graph6 format, and arrowing files: sh, rudy, SAT, and SDPA";
  GraphCommandBase::base().register_c( name, this );
}

void print_all_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  string filename = *pos;
  string adjFile = filename + ".adj";
  string g6File = filename + ".g6";
  string satFile = filename + ".sat";
  string shFile = filename + ".eig";
  string rdyFile = filename + ".rdy";
  string sdpaFile = filename + ".dat-s";

  ofstream adj (adjFile.c_str());
  ofstream g6 (g6File.c_str());
  ofstream sat (satFile.c_str());
  ofstream sh (shFile.c_str());
  ofstream rdy (rdyFile.c_str());
  ofstream sdpa (sdpaFile.c_str());
  if( sh.is_open() ){
    graph->print_sparse_h( &sh );
    graph->print( &adj );
    graph->print_g6( &g6 );
    graph->print_sparse_h( &rdy, true );
    graph->print_sat( &sat );
    graph->print_sdpa( &sdpa );
  }
  else{
    throw "Error opening file " + filename;
  }
}


print_command::print_command(){
  name = "print";
  description = "Prints adjacency matrix of graph";
  GraphCommandBase::base().register_c( name, this );
}

void print_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos != args.end() && pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  if( pos == args.end() ){
    graph->print();
  }
  else{
    string filename = *pos;
    ofstream adj (filename.c_str());
    if( adj.is_open() ){
      graph->print( &adj );
    }
    else{
      throw "Error opening file " + filename;
    }
  }
}


printg6_command::printg6_command(){
  name = "printg6";
  description = "Prints graph in graph6 format";
  GraphCommandBase::base().register_c( name, this );
}

void printg6_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
 
  if( pos != args.end() && pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  if( pos == args.end() ){
    graph->print_g6();
  }
  else{
    string filename = *pos;
    ofstream adj (filename.c_str());
    if( adj.is_open() ){
      graph->print_g6( &adj );
    }
    else{
      throw "Error opening file " + filename;
    }
  }
}


printgviz_command::printgviz_command(){
  name = "print_gviz";
  description = "Prints file for use in Graphviz";
  GraphCommandBase::base().register_c( name, this );
}

void printgviz_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
 
  if( pos != args.end() && pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  if( pos == args.end() ){
    graph->print_graphviz();
  }
  else{
    string filename = *pos;
    ofstream adj (filename.c_str());
    if( adj.is_open() ){
      graph->print_graphviz( &adj );
    }
    else{
      throw "Error opening file " + filename;
    }
  }
}



print_sparse_command::print_sparse_command(){
  name = "print_sh";
  description = "Prints H_G graph in 'Sparse-H' format (used in MinEigs tests)";
  GraphCommandBase::base().register_c( name, this );
}

void print_sparse_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  string filename = *pos;
  ofstream sparseH (filename.c_str());
  if( sparseH.is_open() ){
    graph->print_sparse_h( &sparseH );
  }
  else{
    throw "Error opening file " + filename;
  }
}


print_sdpa_command::print_sdpa_command(){
  name = "print_sdp";
  description = "Prints H_G file for SDP MAX-CUT test (SDPA format)";
  GraphCommandBase::base().register_c( name, this );
}

void print_sdpa_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  string filename = *pos;
  ofstream sparseH (filename.c_str());
  if( sparseH.is_open() ){
    graph->print_sdpa( &sparseH );
  }
  else{
    throw "Error opening file " + filename;
  }
}


print_rudy_command::print_rudy_command(){
  name = "print_rh";
  description = "Prints H_G graph in Rudy format";
  GraphCommandBase::base().register_c( name, this );
}

void print_rudy_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  string filename = *pos;
  ofstream sparseH (filename.c_str());
  if( sparseH.is_open() ){
    graph->print_sparse_h( &sparseH, true );
  }
  else{
    throw "Error opening file " + filename;
  }
}


print_sat_command::print_sat_command(){
  name = "print_sat";
  description = "Prints SAT reduction for arrowing triangles";
  GraphCommandBase::base().register_c( name, this );
}

void print_sat_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  string filename = *pos;
  ofstream sparseH (filename.c_str());
  if( sparseH.is_open() ){
    graph->print_sat( &sparseH );
  }
  else{
    throw "Error opening file " + filename;
  }
}


print_wsat_command::print_wsat_command(){
  name = "print_wsat";
  description = "Prints weighted SAT reduction for arrowing triangles";
  GraphCommandBase::base().register_c( name, this );
}

void print_wsat_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 2 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  int num = atoi(pos->c_str());
  pos++;
  string filename = *pos;
  ofstream sparseH (filename.c_str());
  if( sparseH.is_open() ){
    graph->print_sat( &sparseH, true, num );
  }
  else{
    throw "Error opening file " + filename;
  }
}


print_sat34_command::print_sat34_command(){
  name = "print_sat34";
  description = "Prints SAT reduction for arrowing (3,4)";
  GraphCommandBase::base().register_c( name, this );
}

void print_sat34_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  string filename = *pos;
  ofstream sparseH (filename.c_str());
  if( sparseH.is_open() ){
    graph->print_sat34( &sparseH );
  }
  else{
    throw "Error opening file " + filename;
  }
}


print_sat4me_command::print_sat4me_command(){
  name = "print_sat4me";
  description = "Prints SAT reduction for arrowing (K4-e,K4-e)";
  GraphCommandBase::base().register_c( name, this );
}

void print_sat4me_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  string filename = *pos;
  ofstream sparseH (filename.c_str());
  if( sparseH.is_open() ){
    graph->print_sat4me( &sparseH );
  }
  else{
    throw "Error opening file " + filename;
  }
}


print_satv44_command::print_satv44_command(){
  name = "print_satv44";
  GraphCommandBase::base().register_c( name, this );
  description = "Prints SAT reduction for vertex arrowing of (4,4)";
}

void print_satv44_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  string filename = *pos;
  ofstream sparseH (filename.c_str());
  if( sparseH.is_open() ){
    graph->print_satv44( &sparseH );
  }
  else{
    throw "Error opening file " + filename;
  }
}



get_edges_command getEdgesCommand;
get_edges2_command getEdges2Command;
order_command getOrderCommand;
num_tri_command numTriCommand;
max_clique_command maxCliqueCommand;
max_is_command maxISCommand;
remove_edge_command removeEdgeCommand;
add_edge_command addEdgeCommand;
add_circ_edge_command addCircEdgeCommand;
remove_circ_edge_command removeCircEdgeCommand;
remove_dist_vs_command removeDistVsCommand;
remove_rand_vs_command removeRandVsCommand;
remove_max_is_command removeMaxISCommand;
k4_free_proc_command k4FreeProcess;
make_complement_command makeComplementCommand;
make_cyc_command makeCycCommand;
make_comp_command makeCompCommand;
make_res_circ_command makeResCircCommand;
make_l_circ_command makeLCircCommand;
make_gcd_circ_command makeGCDCircCommand;
make_galois_circ_command makeGaloisCircCommand;
make_projective_plane_command makeProjectivePlaneCommand;
make_projective_plane_cut_command makeProjectivePlaneCutCommand;
make_circ_command makeCircCommand;
make_turan_command makeTuranCommand;
make_hamming_command makeHammingCommand;
make_embedded_rc_command makeEmbeddedRCCommand;
read_g6_command readG6Command;
load_adj_command loadAdjCommand;
make_joined_command makeJoinedCommand;
make_connected_command makeConnectedCommand;
make_avoid_connected_command makeAvoidConnectedCommand;
make_avoid_rand_connected_command makeAvoidRandConnectedCommand;
add_all_noncrit_command addAllNonCritCommand;
add_all_ce_command addAllCECommand;
add_all_cer_command addAllCERCommand;
remove_k_command removeKCommand;
count_k_command countKCommand;
count_kme_command countKmECommand;
has_c_command hasCCommand;
print_all_command printAllCommand;
print_command printCommand;
printg6_command printg6Command;
printgviz_command printgVizCommand;
print_sparse_command printSparseCommand;
print_sdpa_command printSDPACommand;
print_rudy_command printRudyCommand;
print_sat_command printSatCommand;
print_wsat_command printWSatCommand;
print_sat34_command printSat34Command;
print_sat4me_command printSat4meCommand;
print_satv44_command printSatV44Command;
