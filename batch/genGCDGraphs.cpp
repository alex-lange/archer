// g++ -o genGCDGraphs /home/alex/research/archer/g.o /home/alex/research/archer/set_operations.o /home/alex/research/archer/number_algorithms.o genGCDGraphs.cpp -L/usr/lib -lntl -lm


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <math.h>
#include "/home/alex/research/archer/g.h"
#include "/home/alex/research/archer/number_algorithms.h"

using namespace std;


int main( int argc, char *argv[] ){
  string path = "/home/alex/research/data/gcd/";
  int start = 2;
  
  for( int i = 50; i < 700; i++ ){
    cout << i << endl;
    cout << " ";
    g test(i);
    test.make_gcd_circ( start );
    if( test.remove_k( 4, false ) == 0 ){
      cout << "NO K4!" << endl;
    ostringstream fn;
    fn << path << i;
    
    string rdyFile = fn.str() + ".rdy";
    string adjFile = fn.str() + ".adj";
    string g6File = fn.str() + ".g6";
    ofstream rdy (rdyFile.c_str());
    ofstream adj (adjFile.c_str());
    ofstream g6 (g6File.c_str() );
    test.print_sparse_h( &rdy, true );
    test.print( &adj );
    test.print_g6( &g6 );

    vector<int> added = test.add_all_ce_rand(); 
    if( added.size() > 0 ){
      cout << "Added circ edges" << endl;
      for( vector<int>::iterator it = added.begin(); it != added.end(); it++ ){
	cout << *it << " ";
      }
      cout << endl;
      string rdyFile2 = fn.str() + "+cer.rdy";
      string adjFile2 = fn.str() + "+cer.adj";
      string g6File2 = fn.str() + "+cer.g6";
      
      ofstream rdy2( rdyFile2.c_str() );
      ofstream adj2( adjFile2.c_str() );
      ofstream g62( g6File2.c_str() );

      test.print_sparse_h( &rdy2, true );
      test.print( &adj2 );
      test.print_g6( &g62 );
    }
    else{
      cout << "NO CIRCS ADDED" << endl;
    }

    int added2 = test.add_all_noncrit(); 
    if( added2 > 0 ){
      cout << "   added " << added2 << endl;
      string rdyFile3 = fn.str() + "+cer+.rdy";
      string adjFile3 = fn.str() + "+cer+.adj";
      string g6File3 = fn.str() + "+cer+.g6";
      
      ofstream rdy3( rdyFile3.c_str() );
      ofstream adj3( adjFile3.c_str() );
      ofstream g63( g6File3.c_str() );

      test.print_sparse_h( &rdy3, true );
      test.print( &adj3 );
      test.print_g6( &g63 );
    }
    else{
      cout << "NO EXTRA ADDED" << endl;
    }

    cout << endl;

    fn.clear();
    }   
  }
}

