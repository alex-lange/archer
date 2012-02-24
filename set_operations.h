#ifndef _SET_OPERATIONS_H
#define _SET_OEPRATIONS_H

#include <vector>
#include <stdint.h>

using namespace std;

typedef vector< uint64_t > vset;
const int intSize = sizeof( uint64_t ) * 8;
const uint64_t shifter = 1;
const uint64_t empty = 0;

void set_insert( int u, vset &S );
void set_delete( int u, vset &S );
bool in_set( int u, vset S );
void set_cut( int u, vset &S, int arraySize );
vset set_union( vset S, vset R, int arraySize );
vset set_intersection( vset S, vset R, int arraySize );
bool not_empty( vset S );

#endif
