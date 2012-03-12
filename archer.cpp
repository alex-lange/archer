#include <iostream>
#include <algorithm>
#include <sstream>

#include "g.h"
#include "command.h"
#include "graph_commands.h"

using namespace std;

/*vector<string> & split( const string &line, char delim, vector<string> &parts ){
  stringstream ss( line );
  string item;
  while( getline( ss, item, delim) ){
    if( !item.empty() ){
      parts.push_back( item );
    }
  }
  return parts;
  }*/

void read_input( string & command ){
  cout << prompt;
  getline(cin,command);

  vector<string> parts;
  //split( command, ' ', parts );
  
}

int main( int argc, char *argv[] ){
  /*string command;
  read_input( command );
  while( command != "exit" && command != "quit" ){
    read_input( command );
    } */
  GraphCommandBase::base().run();
}
