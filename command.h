//
// File: command.h
//

#ifndef _COMMAND_H
#define _COMMAND_H

#include <map>
#include <vector>

#include "g.h"

using namespace std;

const string prompt = ">> ";

class GraphCommand{
 public:
  virtual ~GraphCommand(){}
  virtual void execute( g * graph, vector<string> args) = 0;
 protected:
  string name;
};

class GraphCommandBase{
 public:
  typedef map< string, g* > g_map;
  typedef map< string, GraphCommand* > c_map;
  static GraphCommandBase & base();
  void run();
  void register_g( const string name, g * graph );
  void register_c( const string name, GraphCommand * cmd );
  void register_creator( const string name, bool creator );
 
 private:
  void split( string &line, char delim, vector<string> &parts );
  GraphCommand * lookup_command( const string & c );
  g * lookup_graph( const string & c, const string & gs );

 private:
  g_map graphMap;
  c_map cmdMap;
};

#endif
