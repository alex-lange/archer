#include <iostream>


#include "g.h"

using namespace std;

int main( int argc, char *argv[] ){
  g a(20);
  a.add_edge( 0 ,1 );
  a.add_edge( 1, 3 );
  a.print();
}
