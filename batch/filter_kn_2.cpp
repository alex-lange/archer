
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>

#include "../g.h"
#include "/home/alex/software/nauty24r2/gtools.h"

using namespace std;

int recurseCount = 0;
int totalNew;

int vertex_extend(g nautg, int n, int k, int v, int num ){
  recurseCount++;
  if( v == n - 1 ){
    return 0;
  }
  else{
    int addedWorked = 0;
    nautg.add_edge( v, n - 1 );
    bool okay = true;

    // min degree requirement
    int d = nautg.min_degree();
    if( !((d*d - d + 1) <= n) ){
      okay = false;
    }

    // check to see if it created a cycle
    for( int i = 0; i < n-1 && okay; i++ ){
      // find neighbors of n-1
      if( nautg.is_edge( i, n-1 ) && i != v ){
	for( int j = 0; j < n-1 && okay; j++ ){
	  // create cyle
	  if( nautg.is_edge( i, j ) && nautg.is_edge( v, j ) ){
	    okay = false;
	  }
	}
      }
    }

    // see if it created an IS of size k
    bool isK = true;
    if( okay ){
      isK = nautg.has_clique( k, true );
    }
    
    // if no C4 of IS of K
    if( okay && !isK ){
      totalNew++;
      addedWorked = 1;
      nautg.print_g6();
    }

    // if it didn't create a C4, we can still add more edges
    if( okay ){
      vertex_extend( nautg, n, k, v+1, num );
    }
    
    // move onto next vertex with out the edge there
    nautg.remove_edge(v,n-1);
    vertex_extend( nautg, n, k, v+1, num );
  }
}

int main( int argc, char *argv[] ){
  
  graph * inG;
  FILE *inFile;
  FILE *outFile;
  int codetype;

  if( argc == 1 || argc == 2 || argc == 3){
    inFile = opengraphfile( NULL, &codetype, 0, 1 );
    outFile = stdout;
  }

  if( inFile == NULL ){
    cerr << "Error: Cannot open input file" << endl;
    return 1;
  }

  int n, m;
  unsigned long long int bit, row;
  string filename;
  int count = 0;
  int graph_num = 0;
  int newCount = 0;
  bool alreadyFiltered = false;
  bool hasK;
  
  int k;
  char opt = 'r';
  // if only one arg, the arg is the independent set
  if( argc == 2 ){
    k = atoi( argv[1] );
  }
  else{
    // if the first arg is an option, use it
    if( argv[1][0] == '-' ){
      opt = argv[1][1];
      k = atoi( argv[2] );
      alreadyFiltered = true;
    }
    else{
      k = atoi( argv[1] );
    }
  }


  // read in graphs
  while( ( inG = readg(inFile, NULL, 0, &m, &n ) ) != NULL ){

    stringstream out;

    g * nautg;
    if( opt == 'v' ){
      nautg = new g(n + 1);
    }
    else{
      nautg = new g(n);
    }
    
    // convert g6 to graph
    for( int i = 0; i < n; i++ ){
      row = inG[i] >> (WORDSIZE - n );
      bit=((long long int)1<<(n-1));
      for( int j = 0; j < n; j++){
	if( bit&row ){
	  nautg->add_edge( i, j );
	}
	bit >>= 1;
      }
    }

    // if the graphs are already filtered, no need to look for IS
    if( alreadyFiltered ){
      hasK = false;
    }
    else{
      hasK = nautg->has_clique(k, true);
    }
    
    if( !hasK ){
      count++;
      if( opt == 'r' ){
	writeg6(outFile, inG, m, n );
      }
      else if( opt == 'v' ){
	graph_num++;
	// Add vertex to the graph	
	/*g one(1);
	g * nautg2 = new g( n + 1 );
	vector<g*> graphs;
	graphs.push_back(nautg);
	graphs.push_back(&one);
	nautg2->join_graphs( 2, graphs );*/
	
	// run vertex_extend on the new graph
	recurseCount = 0;
	totalNew = 0;
	vertex_extend(*nautg, n + 1, k, 0, count );
	newCount = newCount + totalNew;
	if( (graph_num % 1000) == 0 ){
	  cerr << graph_num << endl;
	}
	
        
	//	delete nautg;
      }
      else{
	cerr << "Error: Option not supported" << endl;
	return 1;
      }
    }
    free(inG);
    delete nautg;
 
  }

  cerr << "Total count = " << count << endl;

  delete inG;
}
