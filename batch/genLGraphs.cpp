// g++ -o genLGraphs /home/alex/research/archer/g.o /home/alex/research/archer/set_operations.o /home/alex/research/archer/number_algorithms.o genLGraphs.cpp -L/usr/lib -lntl -lm


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "/home/alex/research/archer/g.h"
#include "/home/alex/research/archer/number_algorithms.h"

using namespace std;

int main( int argc, char *argv[] ){
  int start = atoi( argv[1] );
  int end = atoi( argv[2] );
  string path = "/home/alex/research/data/l_circ/tests/";

  if( start % 2 == 0 ){
    start++;
  }

  while( start < end ){
    for( int i = 2; i < start; i++ ){
      if( gcd( i, start ) == 1 ){
	g test(start);
	if( test.make_l_circ(i) ){
	  if( test.num_tris() > 0 ){
	    if( !test.is_k() ){
	      cout << "Graph L(" << test.order() << "," << i << ") has "
		   << test.remove_k( 4, false ) << " K4s and " << test.num_edges()
		   << " edges" << " and " <<  test.num_tris() << " tris" << endl;
	      ostringstream fn;
	      fn << path << start << "_" << i;
	      string filename = fn.str() + ".sat";
	      ofstream sat (filename.c_str());
	      test.print_sat( &sat );
	    }
	  }
	}
      }
    }
    start = start + 2;
  }
  return 0;
}
