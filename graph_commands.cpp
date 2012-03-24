#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

#include "command.h"
#include "g.h"
#include "graph_commands.h"

using namespace std;

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



add_all_noncrit_command::add_all_noncrit_command(){
  name = "add_all";
  GraphCommandBase::base().register_c( name, this );
}

void add_all_noncrit_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  int k = 4;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }
  int added = graph->add_all_noncrit();
  cout << "Added " << added << " edges avoiding K" << k << endl;
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
  cout << "Removed " << removed << " K" << k << "'s" << endl;
}



print_command::print_command(){
  name = "print";
  GraphCommandBase::base().register_c( name, this );
}

void print_command::execute( g * graph, vector<string> args){
  vector<string>::iterator pos = args.begin() + 2;
  if( pos != args.end() ){
    throw "Invalid argument number for " + name;
  }
  graph->print();
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

remove_edge_command removeEdgeCommand;
add_edge_command addEdgeCommand;
make_res_circ_command makeResCircCommand;
make_circ_command makeCircCommand;
add_all_noncrit_command addAllNonCritCommand;
remove_k_command removeKCommand;
print_sparse_command printSparseCommand;
print_command printCommand;
