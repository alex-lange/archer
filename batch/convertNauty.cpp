
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
  int codetype;

  if( argc == 1 ){
    inFile = opengraphfile( NULL, &codetype, 0, 1 );
  }
  else{
    inFile = opengraphfile( argv[1], &codetype, 0, 1 );
  }

  if( inFile == NULL ){
    cerr << "Error: Cannot open input file" << endl;
    return 1;
  }

  int n, m;
  unsigned long long int bit, row;
  int numGraphs = 0;
  string filename;
  
  while( ( inG = readg(inFile, NULL, 0, &m, &n ) ) != NULL ){
    stringstream out;
    out << numGraphs;

    g * nautg = new g (n);
    numGraphs++;
    
    for( int i = 0; i < n; i++ ){
      row = inG[i] >> (WORDSIZE - n );
      bit=((long long int)1<<(n-1));
      for( int j = 0; j < n; j++){
	if( bit&row ){
	  nautg->add_edge( i, j );
	}
	/*	if( !(bit&row) ){
	  if( i != j ){
	    nautg->add_edge( i, j );
	  }
	  }*/
	bit >>= 1;
      }
    }

    filename = "/home/alex/research/data/fe335/tests/" + out.str() + ".eig";
    cout << filename << endl;
    ofstream eigFile( filename.c_str() );
    if( eigFile.is_open() ){
      nautg->print_sparse_h( &eigFile );
      eigFile.close();
    }
    else{
      cout << "Error opening " << filename << endl;
    }

    filename = "/home/alex/research/data/fe335/tests/" + out.str() + ".sat";
    cout << filename << endl;
    ofstream satFile( filename.c_str() );
    if( satFile.is_open() ){
      nautg->print_sat( &satFile );
      satFile.close();
    }
    else{
      cout << "Error opening " << filename << endl;
    }

    filename = "/home/alex/research/data/fe335/tests/" + out.str() + ".dat-s";
    cout << filename << endl;
    ofstream sdpFile( filename.c_str() );
    if( sdpFile.is_open() ){
      nautg->print_sdpa( &sdpFile );
      sdpFile.close();
    }
    else{
      cout << "Error opening " << filename << endl;
    }
    
    free(inG);
    delete nautg;
  }
  delete inG;
}
