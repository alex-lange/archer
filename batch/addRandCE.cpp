#include <iostream>
#include <stdlib.h>

using namespace std;

int main( int argc, char *argv[] ){
  string name = "g";
  string filename = "/home/alex/research/data/all_cer/run01.txt";
  cout << "create " << name << " " << argv[1] << endl;
  cout << "add_allcer " << name << " " << argv[2] << " " << argv[3] << endl;
  cout << "printf_sh " << name << " " << argv[3] << ".eig" << endl;
  cout << "add_all " << name << endl;
  cout << "printf_sh " << name << " " << argv[3] << "+.eig" << endl;
  cout << "exit" << endl;
}
