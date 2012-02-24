#include <iostream>
#include <algorithm>

#include "g.h"

using namespace std;

const string prompt = ">> ";

void read_input( string & command ){
  cout << prompt;
  //cin >> command;
  getline(cin,command);
  //cin.ignore();
  command.erase(remove_if(command.begin(), command.end(), 
			  ::isspace), command.end() );
}

int main( int argc, char *argv[] ){
  string command;
  read_input( command );
  while( command != "exit" && command != "quit" ){
    read_input( command );
  }  
}
