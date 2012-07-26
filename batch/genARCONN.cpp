// g++ -o genARCONN /home/alex/research/archer/g.o /home/alex/research/archer/set_operations.o /home/alex/research/archer/number_algorithms.o genARCONN.cpp -L/usr/lib -lntl -lm


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "/home/alex/research/archer/g.h"
#include "/home/alex/research/archer/number_algorithms.h"

using namespace std;

int main( int argc, char *argv[] ){
  int n = atoi( argv[1] );
  int s = atoi( argv[2] );
  int num = atoi( argv[3] );
  string path = "/home/alex/research/data/457/457_arc/";

  g test(n);
  g tri(3);
  test.make_l_circ( s );
  tri.make_complete();
  for( int i = 0; i < num; i++ ){
    g both( test.order() + tri.order() );
    both.connect_graphs( &test, &tri, true, true, 4 );
    cout << "Test " << i << endl;
    ostringstream fn;
    fn << path << n << "_" << i;
    string rdyFile = fn.str() + ".rdy";
    string adjFile = fn.str() + ".adj";
    string g6File = fn.str() + ".g6";
    ofstream rdy (rdyFile.c_str());
    ofstream adj (adjFile.c_str());
    ofstream g6 (g6File.c_str() );
    both.print_sparse_h( &rdy, true );
    both.print( &adj );
    both.print_g6( &g6 );
  }
}
