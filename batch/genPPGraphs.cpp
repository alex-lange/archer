// g++ -o genPPGraphs /home/alex/research/archer/g.o /home/alex/research/archer/set_operations.o /home/alex/research/archer/number_algorithms.o genPPGraphs.cpp -L/usr/lib -lntl -lm


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <math.h>
#include "/home/alex/research/archer/g.h"
#include "/home/alex/research/archer/number_algorithms.h"

using namespace std;

struct two_ints{
  int a;
  int b;
};

int main( int argc, char *argv[] ){
  string path = "/home/alex/research/data/pp/";
  
  vector<two_ints> tests;

  two_ints pair;
  pair.a = 3;
  pair.b = 1;
  tests.push_back( pair );
  pair.a = 2;
  pair.b = 2;
  tests.push_back( pair );
  pair.a = 5;
  pair.b = 1;
  tests.push_back( pair );
  pair.a = 7;
  pair.b = 1;
  tests.push_back( pair );
  pair.a = 2;
  pair.b = 3;
  tests.push_back( pair );
  pair.a = 3;
  pair.b = 2;
  tests.push_back( pair );
  pair.a = 11;
  pair.b = 1;
  tests.push_back( pair );

  for( vector<two_ints>::iterator it = tests.begin(); it != tests.end(); it++ ){
    for( int i = 0; i < 2; i++ ){
      int a = it->a;
      int b = it->b;
      int q = pow( a, b );
      int n = q*q + q + 1;
      g test(n);
      if( a == 3 && b == 2 && i ){
	test.make_projective_plane( a, b, i, "[ 2 1 ]" );
      }
      else{
	test.make_projective_plane( a, b, i );
      }
				 
      ostringstream fn;
      fn << path << q << "_" << n;
      if( i ){
	fn << "_nha";
      }
      string g6File = fn.str() + ".g6";
      string adjFile = fn.str() + ".adj";
      ofstream g6 (g6File.c_str());
      ofstream adj (adjFile.c_str());
      test.print( &adj );
      test.print_g6( &g6 );

      fn.clear();
      
    }
  }
}
