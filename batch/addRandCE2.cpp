// g++ -o genLGraphs /home/alex/research/archer/g.o /home/alex/research/archer/set_operations.o /home/alex/research/archer/number_algorithms.o genLGraphs.cpp -L/usr/lib -lntl -lm


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include "/home/alex/research/archer/g.h"
#include "/home/alex/research/archer/number_algorithms.h"

using namespace std;

int main( int argc, char *argv[] ){
  int num = atoi( argv[1] );
  string path = "/home/alex/research/data/all_cer/97/";
  int n = 97;

  for( int i = 1; i <= num; i++ ){
    cerr << i << endl;
    g gr(n);
    cout << i << ". ";
    vector<int> dists = gr.add_all_ce_rand();
    for( vector<int>::iterator it = dists.begin(); it != dists.end(); it++){
      cout << *it << " ";
    }
    cout << endl;

    int numEdges = gr.num_edges();

    ostringstream fn;
    fn << path << i << "_" << n;
    string rdyName = fn.str() + ".rdy";
    string eigName = fn.str() + ".eig";
    string sdpName = fn.str() + ".dat-s";
    string satName = fn.str() + ".sat";

    ofstream rdy( rdyName.c_str() );
    ofstream eig( eigName.c_str() );
    ofstream sdp( sdpName.c_str() );
    ofstream sat( satName.c_str() );
    
    gr.print_sparse_h( &rdy, true );
    gr.print_sparse_h( &eig );
    gr.print_sdpa( &sdp );
    gr.print_sat( &sat );
    
    int added = gr.add_all_noncrit(); 
    if( added > 0 ){
      cout << "   added " << added << endl;
      string rdyName2 = fn.str() + "+.rdy";
      string eigName2 = fn.str() + "+.eig";
      string sdpName2 = fn.str() + "+.dat-s";
      string satName2 = fn.str() + "+.sat";
      
      ofstream rdy2( rdyName2.c_str() );
      ofstream eig2( eigName2.c_str() );
      ofstream sdp2( sdpName2.c_str() );
      ofstream sat2( satName2.c_str() );

      gr.print_sparse_h( &rdy2, true );
      gr.print_sparse_h( &eig2 );
      gr.print_sdpa( &sdp2 );
      gr.print_sat( &sat2 );
    }
  }

  return 0;
}
