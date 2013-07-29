#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <math.h>
#include <map>
#include <time.h>

#include "g.h"

using namespace std;

uint64_t shift = 1;

int set_orderb( uint64_t s ){
  int c;
  for( c = 0; s; c++ ){
    s &= s -1;
  }
  return c;
}

bool in_setb( int i, uint64_t S ){
  if( S & (shift << i ) ){
    return true;
  }else{
    return false;
  }
}

void set_insertb( int u, uint64_t &S ){
  S = S | ( shift << u );
}

void set_deleteb( int u, uint64_t &S ){
  S = S & ~( shift << u );
}

int na, ea, ec;
//int old_na, old_ea, old_ec;
int n = 42;
uint64_t cut;
uint64_t * n_matrix;
uint32_t all;
int * all_counts;

uint64_t counter;

clock_t start, stop;
double cur_time;

void split_bt( int v ){
  if( v == n )
    return;

  int old_na = na; int old_ea = ea; int old_ec = ec;

  na += 1;
  ea += set_orderb( n_matrix[v] & cut );
  ec -= set_orderb( n_matrix[v] & ~cut );
  // eb = eb - set_order( n_matrix[v] & cut ) - set_order( n_matrix[v] & ~cut );
  //cout << v << " ";
  //cout << na << endl;
  all = (na << 16 ) | (ea << 8) | ec;
  //cout << all << endl;
  all_counts[all] = all_counts[all] + 1;
  counter++;

  if( counter % 1000000000 == 0 ){
    stop = clock();
    cur_time = ((float)(stop - start))/((float)CLOCKS_PER_SEC);
    cerr << cur_time << ": " << counter << endl;
    start = clock();
  }

  set_insertb( v, cut );

  split_bt( v + 1 );

  na = old_na; ea = old_ea; ec = old_ec;
  set_deleteb( v, cut );
  
  split_bt( v + 1);
}

int main(){
  int gn = 127;
  g gr(gn);
  gr.make_residue_circ(3);

  uint64_t big = 1;
  big = big << 41;
  cout << big << endl;

  int cur_v = 0;
  int neighbors[n];
  for( int i = 0; i < gn; i++ ){
    if( gr.is_edge(0,i) ){
      neighbors[cur_v] = i;
      cur_v++;
    }
  }

  uint64_t biga = 1;
  biga = biga << 22;

  cout << biga << endl;
  cout << endl;

  all_counts = new int[biga];
  for( uint32_t i = 0; i < biga; i++ )
    all_counts[i]  = 0;
  
  n_matrix = new uint64_t[n];
  for( int i = 0; i < n; i++ )
    n_matrix[i] = 0;
  
  int edges = 0;
  for( int x = 0; x < n-1; x++ ){
    for( int y = x+1; y < n; y++ ){
      int vx = neighbors[x];
      int vy = neighbors[y];
      if( gr.is_edge(vx,vy) ){
	set_insertb( y, n_matrix[x] );	
	set_insertb( x, n_matrix[y] );
	edges++;
      }
    }
  }

  cout << edges << endl;

  counter = 0;

  set_insertb( 0, cut );
  na = 1;
  ea = 0;
  ec = edges - set_orderb( n_matrix[0] & cut );

  start = clock();

  cout << "Starting backtracking..." << endl;
  
  split_bt( 1 );

  for( uint32_t i = 0; i < biga; i++ ){
    if( all_counts[i] > 0 ){
      cout << i << " " << all_counts[i] << endl;
    }
  }
  
  /*
  int n_a, n_b, e_a, e_b, e_c, all;
  string c;
  for( uint64_t s = 0; s < big; s++ ){
    n_a = set_orderb(s); 
    e_a = 0; e_b = 0;/*
    for( int x = 0; x < 41; x++ ){
      for( int y = x+1; y < 42; y++ ){
	int vx = neighbors[x];
	int vy = neighbors[y];
	if( gr.is_edge(vx,vy) ){
	  bool x_in = in_setb(s,vx);
	  bool y_in = in_setb(s,vy);
	  if( x_in && y_in ){
	    e_a++;
	  }
	  else if( !x_in && !y_in ){
	    e_b++;
	  }
	  else{
	    e_c++;
	  }
	}
      }
      } 
    //counts << n_a << " " << n_b << " " << e_a << " " << e_b << " " << e_c;
    //c = counts.str();
    // final_counts[c] = final_counts[c] + 1;
    //counts.str("");

    all = (n_a << 16 ) | (e_a << 8) | e_b;
    all_counts[all] = all_counts[all] + 1;

    count++;

    if( count % 10000000 == 0 ){
      stop = clock();
      cur_time = ((float)(stop - start))/((float)CLOCKS_PER_SEC);
      cout << cur_time << endl;
      start = clock();
    }
    }

  for( map<string,int>::iterator it = final_counts.begin(); it != final_counts.end(); it++ ){
    cout << it->first << "   " << it->second << endl;
    }*/

  cout << "DONE" << endl;

}
