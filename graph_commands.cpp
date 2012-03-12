#include <iostream>
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


create_res_circ_command::create_res_circ_command(){
  name = "res_circ";
  GraphCommandBase::base().register_c( name, this );
}

void create_res_circ_command::execute( g * graph, vector<string> args){
  if( graph != NULL ){
    delete graph;
  }

  vector<string>::iterator pos = args.begin() + 2;

  vector<int> iArgs;
  for( int i = 0; i < 2; i++ ){
    if( pos == args.end() ){
      throw "Invalid argument number for " + name;
    } 
    iArgs.push_back( atoi( pos->c_str() ));
    pos++;
  }
  g temp( iArgs[0] );

  graph = &temp;
  graph->make_residue_circ( iArgs[1] );
}

remove_edge_command removeEdgeCommand;
add_edge_command addEdgeCommand;
print_command printCommand;
create_res_circ_command createResCircCommand;
