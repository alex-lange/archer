#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <math.h>
#include <map>
#include <time.h>

#include "g.h"

using namespace std;

int main(){

  int test_num = 20;

  string path = "/home/alex/research/folkman/data/tr_k4_free/11-25-2013_400-600/";

  ofstream log((path+"log.txt").c_str());

  for( int n = 400; n <= 600; n+=25 ){
    for( int i = 0; i < test_num; i++ ){
      g k4_free(n);
      g tr_k4_free(n);

      int e1 = k4_free.k4_free_process2();
      cout << "k4-free done" << endl;
      int e2 = tr_k4_free.tr_k4_free_process();
      cout << "tr-k4-free done" << endl;      
      
      stringstream ss;
      ss << path <<  n << '_' << i;
      string file_name = ss.str();
 
      log << n << " " << i << " " << e1 << " " << k4_free.remove_k(4,false) 
	  << " " << e2 << " " <<  tr_k4_free.remove_k(4,false) << endl;
      
      ofstream k4f_rdy((file_name+"_krf.rdy").c_str());
      ofstream k4f_g6((file_name+"_krf.g6").c_str());
      ofstream trk4f_rdy((file_name+"_trkrf.rdy").c_str());
      ofstream trk4f_g6((file_name+"_trkrf.g6").c_str());

      k4_free.print_g6( &k4f_g6 );
      k4_free.print_sparse_h( &k4f_rdy );
      tr_k4_free.print_g6( &trk4f_g6 );
      tr_k4_free.print_sparse_h( &trk4f_rdy );


      cout << file_name << endl;

      cout << n << endl;

    }
  }

}
