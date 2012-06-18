// g++ -o genERGraphs /home/alex/research/archer/g.o /home/alex/research/archer/set_operations.o /home/alex/research/archer/number_algorithms.o genERGraphs.cpp -L/usr/lib -lntl -lm


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "/home/alex/research/archer/g.h"
#include "/home/alex/research/archer/number_algorithms.h"

using namespace std;

int main( int argc, char *argv[] ){
  int n = atoi( argv[1] );
  int num = atoi( argv[2] );
  string path = "/home/alex/research/data/fv445/19_rand/";

  g test(n);
  test.make_rand_er(0.5);
  cout << "Test " << num << endl;
  cout << "Removed " << test.remove_k(5) << " K5s" << endl;
  cout << "Adding " << test.add_all_noncrit( true, 5 ) << " edges" << endl;
  ostringstream fn;
  fn << path << n << "_" << num;
  string satFile = fn.str() + ".sat";
  string adjFile = fn.str() + ".adj";
  ofstream sat (satFile.c_str());
  ofstream adj (adjFile.c_str());
  test.print_satv44( &sat );
  test.print( &adj );
  
}
