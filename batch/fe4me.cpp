#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>

#include "../g.h"
#include "/home/alex/software/nauty24r2/gtools.h"

using namespace std;

int main( int argc, char *argv[] ){
  graph * inG;
  FILE *inFile;
  FILE *outFile;
  int codetype;

  int edge_cap = 36;

  inFile = opengraphfile( NULL, &codetype, 0, 1 );
  outFile = stdout;
  
  int n, m;
  unsigned long long int bit, row;
  int numGraphs = 0;
  string filename;
  int count = 0;

  int k = atoi( argv[1] );
    
  while( ( inG = readg(inFile, NULL, 0, &m, &n ) ) != NULL ){

    stringstream out;
    
    g * nautg = new g (n);
    numGraphs++;
    
    for( int i = 0; i < n; i++ ){
      row = inG[i] >> (WORDSIZE - n );
      bit=((long long int)1<<(n-1));
      for( int j = 0; j < n; j++){
	if( bit&row ){
	  nautg->add_edge( i, j );
	}
	bit >>= 1;
      }
    }
        
    if( nautg->max_clique(false).size() < k ){
      count++;
      stringstream out;
      out << count;
      string filename = "/home/alex/research/data/K4-e/11/" + out.str() + ".sat";
      ofstream sat( filename.c_str() );
      writeg6(outFile, inG, m, n );
      nautg->print_sat4me( &sat );
    }
  
    free(inG);
    delete nautg;
  }
}
