#include <stdint.h>
#include "set_operations.h"
// computes x^c mod n
int64_t square_and_multiply( uint64_t x, int64_t c, int64_t n ){
  int64_t z = 1;
  for( int i = 20 -1; i >=0; i-- ){
    z = z * z % n;
    if( c & shifter << i ){
      z = z * x % n;
    }
  }
  return z;
}

int gcd(int a, int b)
{
  if( b == 0 ){
    return a;
  }else{
    return gcd( b, a % b );
  }
}
