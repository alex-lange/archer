#include <iostream>
#include <stdlib.h>

using namespace std;

int main( int argc, char *argv[] ){
  string name = "g";
  cout << "create " << name << " " << argv[1] << endl;
  cout << "add_allcer " << name << " 4 " << argv[2] << " " << argv[3] << endl;
  cout << "printf_sh " << name << " " << argv[3] << ".eig" << endl;
  cout << "printf_sdp " << name << " " << argv[3] << ".dat-s" << endl;
  cout << "add_all " << name << " 4 " << endl;
  cout << "printf_sh " << name << " " << argv[3] << "+.eig" << endl;
  cout << "printf_sdp " << name << " " << argv[3] << "+.dat-s" << endl;
  cout << "exit" << endl;
}
