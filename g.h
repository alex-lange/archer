//
// File: g.h
//

#ifndef _G_H
#define _G_H

#include <vector>
#include "set_operations.h"

using namespace std;

class g {

 public:
  g( int vsize );

 public:
  void add_edge( int u, int v );
  int make_residue_circ( int r );
  void print();
  
 
 private:
  int n, arraySize, numEdges;
  vector<vset> gA;

};

#endif
