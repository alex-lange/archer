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
  GraphCommandBase::base().register_c( name, this );
}

void get_edges_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }

  cout << graph->num_edges( ) << endl;
}


order_command::order_command(){
  name = "order";
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
  name = "numTris";
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
  GraphCommandBase::base().register_c( name, this );
}

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
  GraphCommandBase::base().register_c( name, this );
}

void make_l_circ_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->make_l_circ( atoi( pos->c_str() ) );
}



make_joined_command::make_joined_command(){
  name = "mk_join";
  GraphCommandBase::base().register_c( name, this );
}

void make_joined_command::execute( g * graph, vector<string> args ){
}

void make_joined_command::execute( g * graph, vector<g*> args){
  graph->join_graphs( args.size(), args );
}


make_connected_command::make_connected_command(){
  name = "mk_conn";
  GraphCommandBase::base().register_c( name, this );
}

void make_connected_command::execute( g * graph, vector<string> args ){
}

void make_connected_command::execute( g * graph, vector<g*> args){
  graph->connect_graphs( args[1], args[0] );
}



make_avoid_connected_command::make_avoid_connected_command(){
  name = "mk_aconn";
  GraphCommandBase::base().register_c( name, this );
}

void make_avoid_connected_command::execute( g * graph, vector<string> args ){
}

void make_avoid_connected_command::execute( g * graph, vector<g*> args){
  cout << graph->connect_graphs( args[1], args[0], true, false, 4 ) << " edges added." << endl;
}



make_avoid_rand_connected_command::make_avoid_rand_connected_command(){
  name = "mk_arconn";
  GraphCommandBase::base().register_c( name, this );
}

void make_avoid_rand_connected_command::execute( g * graph, vector<string> args ){
}

void make_avoid_rand_connected_command::execute( g * graph, vector<g*> args){
  cout << graph->connect_graphs( args[1], args[0], true, true, 4 ) << " edges added." << endl;
}



make_galois_circ_command::make_galois_circ_command(){
  name = "mk_gc";
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
  GraphCommandBase::base().register_c( name, this );
}

void make_embedded_rc_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 2 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  
  graph->make_embedded_rc( atoi( pos->c_str() ), atoi( (pos+1)->c_str()) );
}



load_adj_command::load_adj_command(){
  name = "ld_adj";
  GraphCommandBase::base().register_c( name, this );
}

void load_adj_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos + 1 != args.end() ){
    throw "Invalid argument number for " + name;
  }
  
  graph->load_adj( *pos );
}



add_all_noncrit_command::add_all_noncrit_command(){
  name = "add_all";
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



has_c_command::has_c_command(){
  name = "has_c";
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

  ofstream adj (adjFile.c_str());
  ofstream g6 (g6File.c_str());
  ofstream sat (satFile.c_str());
  ofstream sh (shFile.c_str());
  ofstream rdy (rdyFile.c_str());
  if( sh.is_open() ){
    graph->print_sparse_h( &sh );
    graph->print( &adj );
    graph->print_g6( &g6 );
    graph->print_sparse_h( &rdy, true );
    graph->print_sat( &sat );
  }
  else{
    throw "Error opening file " + filename;
  }
}


print_command::print_command(){
  name = "print";
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


print_sparse_command::print_sparse_command(){
  name = "printf_sh";
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
  name = "printf_sdp";
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
  name = "printf_rh";
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
  name = "printf_sat";
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
  name = "printf_wsat";
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
  name = "printf_sat34";
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


print_satv44_command::print_satv44_command(){
  name = "printf_satv44";
  GraphCommandBase::base().register_c( name, this );
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
make_galois_circ_command makeGaloisCircCommand;
make_projective_plane_command makeProjectivePlaneCommand;
make_projective_plane_cut_command makeProjectivePlaneCutCommand;
make_circ_command makeCircCommand;
make_embedded_rc_command makeEmbeddedRCCommand;
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
has_c_command hasCCommand;
print_all_command printAllCommand;
print_command printCommand;
printg6_command printg6Command;
print_sparse_command printSparseCommand;
print_sdpa_command printSDPACommand;
print_rudy_command printRudyCommand;
print_sat_command printSatCommand;
print_wsat_command printWSatCommand;
print_sat34_command printSat34Command;
print_satv44_command printSatV44Command;
