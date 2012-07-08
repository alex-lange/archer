// g++ -o genLGraphs /home/alex/research/archer/g.o /home/alex/research/archer/set_operations.o /home/alex/research/archer/number_algorithms.o genLGraphs.cpp -L/usr/lib -lntl -lm


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "/home/alex/research/archer/g.h"
#include "/home/alex/research/archer/number_algorithms.h"

using namespace std;

int main( int argc, char *argv[] ){
  int num = atoi( argv[1] );
  string path = "/home/alex/research/data/embed/127_batch/";
  int copies = 2;
  int n = 127;
  int r = 3;

  for( int i = 0; i < num; i++ ){
    cerr << i << endl;
    g gr(n*copies);
    gr.make_embedded_rc( r, copies );
    cout << i << ". ";
    gr.add_all_ce_rand();

    int numEdges = gr.num_edges();

    ostringstream fn;
    fn << path << i << "_" << n <<  r << copies;
    string rdyName = fn.str() + ".rdy";
    string eigName = fn.str() + ".eig";
    string sdpName = fn.str() + ".dat-s";

    ofstream rdy( rdyName.c_str() );
    ofstream eig( eigName.c_str() );
    ofstream sdp( sdpName.c_str() );
    
    gr.print_sparse_h( &rdy, true );
    gr.print_sparse_h( &eig );
    gr.print_sdpa( &sdp );
    
    gr.add_all_noncrit(); 
    if( gr.num_edges() != numEdges ){
      string rdyName2 = fn.str() + "+.rdy";
      string eigName2 = fn.str() + "+.eig";
      string sdpName2 = fn.str() + "+.dat-s";
      
      ofstream rdy2( rdyName2.c_str() );
      ofstream eig2( eigName2.c_str() );
      ofstream sdp2( sdpName2.c_str() );

      gr.print_sparse_h( &rdy2, true );
      gr.print_sparse_h( &eig2 );
      gr.print_sdpa( &sdp2 );
    }
  }

  return 0;
}
