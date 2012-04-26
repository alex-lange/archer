#include <iostream>
#include <stdlib.h>

using namespace std;

int main( int argc, char *argv[] ){
  string name = "g";
  cout << "create " << name << " " << argv[1] << endl;
  cout << "add_allcer " << name << " 5 " << argv[2] << " " << argv[3] << endl;
  cout << "printf_satv44 " << name << " " << argv[3] << ".sat" << endl;
  cout << "add_all " << name << " 5" << endl;
  cout << "printf_satv44 " << name << " " << argv[3] << "+.sat" << endl;
  cout << "exit" << endl;
}
