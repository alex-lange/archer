
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
    int d = nautg.min_degree();
    if( !((d*d - d + 1) <= n) ){
      okay = false;
    }
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
    nautg.make_complement();
    bool isK = nautg.is_k( k );
    nautg.make_complement();

    if( okay && !isK ){
      totalNew++;
      addedWorked = 1;
      nautg.print_g6();
    }
    //if( okay ){
    //return addedWorked + vertex_extend( nautg, n, k, v+1, num );
      //}
    if( okay ){
      vertex_extend( nautg, n, k, v+1, num );
    }
    
    nautg.remove_edge(v,n-1);
    /*
    nautg.make_complement();
    isK = nautg.is_k();
    nautg.make_complement();
    if( !isK ){
      num++;
      }*/ 
    
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
  /*else{
    inFile = opengraphfile( argv[2], &codetype, 0, 1 );
    }*/

  if( inFile == NULL ){
    cerr << "Error: Cannot open input file" << endl;
    return 1;
  }

  int n, m;
  unsigned long long int bit, row;
  int numGraphs = 0;
  string filename;
  int count = 0;
  int newCount = 0;
  bool alreadyFiltered = true;
  bool hasK;
  
  int k;
  char opt = 'r';
  if( argc == 2 ){
    k = atoi( argv[1] );
  }
  else{
    if( argv[1][0] == '-' ){
      opt = argv[1][1];
      k = atoi( argv[2] );
    }
    else{
      k = atoi( argv[1] );
    }
  }

  while( ( inG = readg(inFile, NULL, 0, &m, &n ) ) != NULL ){

    stringstream out;
    out << numGraphs;

    g * nautg = new g (n);
    numGraphs++;
    
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

    if( alreadyFiltered ){
      hasK = false;
    }
    else{
      nautg->make_complement();
      hasK = nautg->is_k();
      nautg->make_complement();
    }
    
    if( !hasK ){
      count++;
      if( opt == 'r' ){
	writeg6(outFile, inG, m, n );
      }
      else if( opt == 'v' ){
	//vector<int*>* inds = nautg->get_ks(k-1);
	//  cout << inds->size() << endl;
	//
	//int indNum = inds->size();
	//  cout << "indNum = " << indNum << endl;
	//bool hit[indNum];
	//for( int i = 0; i < indNum; i++ ){
	//  hit[i] = false;
	//}
	
	g one(1);
	g * nautg2 = new g( n + 1 );
	vector<g*> graphs;
	graphs.push_back(nautg);
	graphs.push_back(&one);
	nautg2->join_graphs( 2, graphs );
	
	//int newNeighbor = 0;
	//bool addIt = true;
	//bool validGraph = true;
	// get neighers of new vertex
	//cout << "NEW GRAPH" << endl;
	//nautg2->print();
	
	recurseCount = 0;
      	cerr << count << endl;
	totalNew = 0;
	vertex_extend(*nautg2, n + 1, k, 0, count );
	newCount = newCount + totalNew;
	if( (newCount % 100) == 0 && newCount != 0 ){
	  cerr << "FOUND " << newCount << endl;
	}
	// cout << recurseCount << endl;
	
	//  newCount = vertex_extend(*nautg2, 0, k-1, 0 );
	
	/*for( int indCur = 0; indCur < indNum; indCur++ ){
	  if( !hit[indCur] ){
	  cout << indCur << endl;
	  for( int indMember = 0; indMember < k - 1; indMember++ ){
	  //   cout << indCur << " " << indMember << endl;
	  newNeighbor = (*inds)[indCur][indMember];
	  //    cout << "Got new neighbor " << newNeighbor << endl;
	  addIt = true;
	  if( !nautg2->is_edge(n,newNeighbor ) ){
	  for( int neighbor = 0; neighbor < n; neighbor++ ){
	  if( nautg2->is_edge( n, neighbor ) ){
	  for( int j = 0; j < n; j++ ){
	  if( nautg2->is_edge( neighbor, j ) && nautg->is_edge( newNeighbor, j ) ){
	  addIt = false;
	  break;
	  }
	  }
	  }
	  }
	  
	  if( addIt ){
	  cout << "Added edge " << newNeighbor << endl;
	  nautg2->add_edge( n, newNeighbor );
	  for( int ii = 0; ii < indNum; ii++ ){
	  for( int jj = 0; jj < k-1; jj++ ){
	  if( (*inds)[ii][jj] == newNeighbor ){
	  hit[ii] = true;
	  }
	  }
	  }
	  validGraph = true;
	  cout << indCur << endl;
	  for( int i = 0; i < indNum; i++ ){
	  cout << hit[i] << " ";
	  validGraph = validGraph && hit[i];
	  }
	  cout << endl;
	  if( validGraph ){
	  newCount++;
	  }
	  }
	  }
	  }
	  }
	  }// */
        
	delete nautg2;
      }
      else{
	cerr << "Error: Option not supported" << endl;
	return 1;
      }
    }
    free(inG);
    delete nautg;
 
  }
  /*cout << endl;
  cout << count << endl;
  cout << newCount << endl;*/

  delete inG;
}
