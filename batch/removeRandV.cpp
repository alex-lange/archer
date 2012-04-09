#include <iostream>
#include <stdlib.h>

using namespace std;

int main( int argc, char *argv[] ){
  string name = "g";
  string filename = "/home/alex/research/data/127-v/g";
  filename.append(argv[1]);
  filename.append("r");
  filename.append(argv[2]);
  filename.append("v");
  filename.append(argv[3]);
  filename.append("_");
  filename.append(argv[4]);

  cout << filename << endl;
  cout << "create " << name << " " << argv[1] << endl;
  cout << "mk_rc " << name << " " << argv[2] << endl;
  cout << "remove_vs_rand " << name << " " << argv[3] << endl;
  cout << "add_all " << name << endl;
  cout << "printf_sh " << name << " " << filename << ".eig" << endl;
  cout << "printf_rh " << name << " " << filename << ".rdy" << endl;
  cout << "printf_sdp " << name << " " << filename << ".dat-s" << endl;
  cout << "printf_sat " << name << " " << filename << ".sat" << endl;
  cout << "exit" << endl;

}
