#include <iostream>
#include <algorithm>
#include <sstream>

#include "g.h"

using namespace std;

int main(){
  g test(3);
  test.make_complete();
  
  int color[3] = {1,1,1};
  bool good = test.check_coloring( color );
  cout << ( good ? "good" : "bad" ) << endl;

  color[0] = 0;
  good = test.check_coloring( color );
  cout << ( good ? "good" : "bad" ) << endl;

  color[1] = 0;
  good = test.check_coloring( color );
  cout << ( good ? "good" : "bad" ) << endl;

  color[0] = 1;
  good = test.check_coloring( color );
  cout << ( good ? "good" : "bad" ) << endl;

  color[2] = 0;
  good = test.check_coloring( color );
  cout << ( good ? "good" : "bad" ) << endl;

  color[0] = 0;
  good = test.check_coloring( color );
  cout << ( good ? "good" : "bad" ) << endl;

}
