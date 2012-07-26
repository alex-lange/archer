#include <sstream>
#include <map>
#include <algorithm>
#include <gf2x.h>
#include <NTL/vec_ZZ_pE.h>
#include <NTL/ZZ_pXFactoring.h>
#include <NTL/ZZ_pE.h>
#include <NTL/ZZ_pEX.h>

#include "g.h"

using namespace std;
using namespace NTL;

struct g::compZZ_pE{
  bool operator() (const ZZ_pE& left, const ZZ_pE& right) const
  {
    string sleft, sright;
    ostringstream loss, ross;
    loss << left;
    sleft = loss.str();
    sleft = sleft.substr(1,sleft.length()-2);
    ross << right;
    sright = ross.str();
    sright = sright.substr(1,sright.length()-2);
    istringstream liss(sleft,istringstream::in);
    istringstream riss(sright,istringstream::in);

    //  cout << "Comparing " << sleft << " and " << sright << endl;
    
    if( sleft.length() < sright.length() ){
      //    cout << sleft << " is less degree, so it is less" << endl;
      return true;
    }
    else if( sleft.length() == sright.length() ){
      int degree = sleft.length() / 2 + 1;
      int coeffs[2][degree];
      for( int i = 0; i < degree; i++ ){
	liss >> coeffs[0][i];
	riss >> coeffs[1][i];
      }
      for( int i = degree-1; i >= 0; i-- ){
	//	cout << "Comparing coeffs " << coeffs[0][i] << " and " << coeffs[1][i] << endl;
	if( coeffs[0][i] < coeffs[1][i] ){
	  return true;
	}
	else if( coeffs[0][i] > coeffs[1][i] ){
	  return false;
	}
      }
    }
    return false;
  }
};



void g::make_galois_circ( int p, int n, int r ){
  // define GF(p)
  ZZ_p::init(to_ZZ(p));

  // build irreducible polynomial P of degree n over GF(P)
  ZZ_pX poly;
  BuildIrred(poly, n);

  // define GF(p^n), which is built around the irreducible poly
  ZZ_pE::init(poly);

  cout << "Created GF(" << p << "^" << n << ") with irreducible polynomial ";
  cout << poly << endl;  
  cout << "Cardinality = " << ZZ_pE::cardinality() << endl;
  cout << "Degree = " << ZZ_pE::degree() << endl;
  cout << "Modulus = " << ZZ_pE::modulus() << endl;

  vector< ZZ_pE > elements;
    
  ZZ_pE x;
  string prim;
  if( p == 7 && n == 3 ){
    prim = "[ 1 1 1 ]";
  }
  else if( p == 23 && n == 2 ){
    prim = "[ 10 1 ]";
  }
  else{
    prim = "[ 1 1 ]";
  }
  istringstream iss( prim, istringstream::in);
  iss >> x;
  
  elements.push_back( x );
  ZZ_pE y = power(x,2);

  int count = 1;
  while( x !=y ){
    count++;
    elements.push_back(y);
    y = y * x;
  }

  cout << "Count = " << count << endl;

  compZZ_pE comparePolys;
  sort( elements.begin(), elements.end(), comparePolys );

  map< ZZ_pE, bool, compZZ_pE > residues;
  map< ZZ_pE, bool, compZZ_pE >::iterator res_it;

  for( vector< ZZ_pE >::iterator it = elements.begin(); 
       it != elements.end(); it++ ){
    // cout << power(*it,4) << endl;
    residues[power(*it,r)] = true;
    //cout << *it << endl;
  }

  string neg = "[ -1 ]";
  string one = "[ 1 ]";

  istringstream nss( neg, istringstream::in );
  ZZ_pE negp;
  nss >> negp;
  cout << "-1 = " << negp << " and is ";
  if( residues[negp] != true ){
    cout << "NOT ";
  }
  cout << "a residue" << endl;

  int res_count = 0;
  // cout << "*** RESIDUES ****" << endl;
  for( res_it = residues.begin(); res_it != residues.end(); res_it++ ){
    //cout << "  " << (*res_it).first << endl;
    res_count++;
  }
  cout << "Num Residues = " << res_count << endl;

  for( int i = 0; i < count; i++ ){
    if( residues[elements[i]] == true ){
      //  cout << elements[i] << " is a residue so add edge " << 0 << " " << i+1 << endl;
      add_edge( 0, i+1 );
    }
    for( int j = i+1; j < count; j++ ){
      if( (residues[elements[i] - elements[j]] == true) ){
	//	cout << elements[i] << " - " << elements[j] << " = " << elements[i] - elements[j] << " so add edge " << i+1 << " " << j+1 << endl;
	add_edge( i+1, j+1 );
      }
      else if( (residues[elements[j] - elements[i]] == true) ){
	//	cout << elements[j] << " - " << elements[i] << " = " << elements[j] - elements[i] << " so add edge " << i+1 << " " << j+1 << endl;
	add_edge( i+1, j+1 );
      }
    }
  }
}


struct g::gf_point{
  ZZ_pE x;
  ZZ_pE y;
  ZZ_pE z;
};


void g::make_projective_plane( int p, int k, bool cut, string pr ){

  int q = pow( p, k );
  if( (q*q +q + 1 ) != n ){
    cerr << "Error: Invalid q for n" << endl;
    return;
  }
   // define GF(p)
  ZZ_p::init(to_ZZ(p));

  // build irreducible polynomial P of degree n over GF(P)
  ZZ_pX poly;
  BuildIrred(poly, k);

  // define GF(p^n), which is built around the irreducible poly
  ZZ_pE::init(poly);

  cout << "Created GF(" << p << "^" << k << ") with irreducible polynomial ";
  cout << poly << endl;  
  cout << "Cardinality = " << ZZ_pE::cardinality() << endl;
  cout << "Degree = " << ZZ_pE::degree() << endl;
  cout << "Modulus = " << ZZ_pE::modulus() << endl;

  vector< ZZ_pE > elements;
  vector< gf_point > points;
  
  ZZ_pE x, z, o;
  string zero = "[ 0 ]";
  string one = "[ 1 ]";
  string prim;
  if( p == 7 && k == 3 ){
    prim = "[ 1 1 1 ]";
  }
  else if( p == 23 && k == 2 ){
    prim = "[ 10 1 ]";
  }
  else if( p == 7 && k == 1 ){
    prim = "[ 3 ]";
  }
  else if( k == 1 ){
    prim = "[ 2 ]";
  }
  else{
    prim = pr;
  }
  
  istringstream iss1( zero, istringstream::in);
  iss1 >> z;
  elements.push_back(x);
 
  istringstream iss2( prim, istringstream::in);
  iss2 >> x;

  istringstream iss3( one, istringstream::in);
  iss3 >> o;
  
  elements.push_back( x );
  ZZ_pE y = power(x,2);

  int count = 2;
  while( x !=y ){
    count++;
    elements.push_back(y);
    y = y * x;
  }

  cout << "Count = " << count << endl;

  for( vector< ZZ_pE >::iterator it = elements.begin(); it != elements.end(); it++ ){
    //cout << *it << endl;
  }

  gf_point start;
  start.x = o;
  start.y = o;
  start.z = o;
  
  //points.push_back( start );
  for( int i = 0; i < count; i++ ){
    gf_point copy = start;
    copy.x = elements[i];
    points.push_back( copy );
    for( int j = 0; j < count; j++ ){
      gf_point copy2 = copy;
      if( !IsOne(elements[ j ]) ){
	copy2.y = elements[j];
	points.push_back( copy2 );
      }
    }
  }

  start.z = z;
  for( int i = 0; i < count; i++ ){
    gf_point copy = start;
    copy.x = elements[i];
    points.push_back( copy );
  }

  start.y = z;
  points.push_back(start);
  cout << points.size() << endl;

  for( vector<gf_point>::iterator it = points.begin(); it != points.end(); it++ ){
    for( vector<gf_point>::iterator it2 = it + 1; it2 != points.end(); it2++ ){
      for( int i = 0; i < count ; i++ ){
	//	if( !IsOne(elements[i] ) ){
	  if( it->x == it2->x*elements[i] && it->y == it2->y*elements[i] &&
	      it->z == it2->z*elements[i] ){
	    cout << it->x << " " << it->y << " " << it->z << " and ";
	    cout << it2->x << " " << it2->y << " " << it2->z;
	    cout << " ARE EQUIVALENT!" << endl;
	    //	  }
	}
      }
    }
  }

  cout << "SIZE = " << points.size() << endl;

  for( int i = 0; i < points.size(); i++ ){
    cout << i << ": " << points[i].x << " " << points[i].y << " " << points[i].z << endl;
    for( int j = 0; j < points.size(); j++ ){
      if( i != j ){
	if( IsZero( points[i].x * points[j].x + points[i].y * points[j].y + points[i].z * points[j].z ) ){
	  //	  cout << points[i].x << " " << points[i].y << " " << points[i].z << " and ";
	  //cout << points[j].x << " " << points[j].y << " " << points[j].z << " are edges" << endl;
	  add_edge( i, j );
	}
	if( IsOne( points[i].x * points[j].x + points[i].y * points[j].y + points[i].z * points[j].z ) ){
	  //	  cout << points[i].x << " " << points[i].y << " " << points[i].z << " and ";
	  //cout << points[j].x << " " << points[j].y << " " << points[j].z << " are edges" << endl;
	  // add_edge( i, j );
	}
      }
    }
    //    cout << endl;
  }

  
  /*for( int i = 0; i < points.size(); i++ ){
    cout << i << ": " << vdegree[i] << endl;
    }*/

  // if we want to cut out the absolute and hyperbolic vertices
  if( cut ){
    // Find absolute vertices
    int absoluteNumber = 0;
    vector<int> cuts;
    bool cutThese[n];
    for( int i = 0; i < n; i++ ){
      cutThese[i] = false;
    }

    // find all absolute verticfes
    for( int i = 0; i < n; i++ ){
      if( vdegree[i] == q){
	absoluteNumber++;
	cuts.push_back( i );
      }
    }


    if( absoluteNumber != q + 1 && p != 2){
	cerr << "Error: graph did not generate correctly, "
	     << "incorrect number of absolute vertices." << endl;
    }

    // find all vertices adjacent to absolute vertices (hyperbolic)
    for( int i = 0; i < absoluteNumber; i++ ){
      for( int j = 0; j < n; j++ ){
	if( is_edge( cuts[i], j ) ){
	  cutThese[j] = true;
	}
      }
    }

    // we do not want to cut the same vertex twice
    for( int i = 0; i < absoluteNumber; i++ ){
      cutThese[cuts[i]] = false;
    }
    
    for( int i = 0; i < n; i++ ){
      if( cutThese[i] ){
	cuts.push_back(i );
      }
    }

    if( p != 2 && (n - cuts.size()) != (q * (q-1) / 2) ){
      cerr << "Error: graph did not generate correctly, "
	   << "number of neither hyperbolic nor absolute is not correct"
	   << endl;
    }
    
    remove_vs( cuts, cuts.size() );

  }
 
}
