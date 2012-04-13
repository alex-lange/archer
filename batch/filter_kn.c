// filter_kn.c is an edited version of basic.c by spr
//
// compile with:
// gcc -O4 -o filter_kn gtools.o  filter_kn.c 
// authors: spr
//          arl (Alexander Lange)

#include <stdio.h>
#include <stdlib.h>

#include "/home/alex/software/nauty24r2/gtools.h"

int
main(int argc, char *argv[])
{

  graph *g;
  int m, n, i, j, bit, row;
  int gmat[20][20];

  int x, y, z, a, b, c;

  int codetype;
  FILE *infile;
  FILE *log_g;
  FILE *log_counts;

  int k_n, found, count, s;

  char s_n[ 16 ];

  if( argc != 3 ){
    printf( "Usage: filter_kn n file\n");
    return 2;
  }
   
  // get n for k_n from args
  k_n = atoi( argv[ 1 ] );

  // log for the counts
  s = sprintf( s_n, "%i_count.txt", k_n );

  // Opens up stdin for piping
  infile = opengraphfile(NULL,&codetype,0,1);

  if (infile == NULL)
  {
      fprintf(stderr,"Error: Can't open input file\n");
      exit(1);
  }

  // Opens the two files writing to
  log_g = fopen( argv[ 2 ], "w" );
  log_counts = fopen( s_n, "a" );

  count = 0;

  // While files are still being piped
  while ((g = readg(infile,NULL,0,&m,&n)) != NULL)
  {

    // Convirts g6 format to an adjacency matrix
   for (i=0;i<n;i++) {
     row = g[i]>>(WORDSIZE-n);
     for (bit=(1<<(n-1)),j=0;j<n;j++,bit>>=1) {
       gmat[i][j] = (bit&row) ? 1 : 0;
       if( bit&row ){
	 printf( "%i %i\n", i, j );
	 printf( "%i\n", gmat[i][j] );
       }
     }
   }
   
   found = 0;

   // Simple switch statement which determines how many for loops 
   // are needed to find independent sets
   // Obviously, a brute force method using a recursive function
   // is more practical, and I hope to replace these loops with 
   // something like that soon. Also, I realize that 'goto' is
   // considered bad practice now, but I think having 6 nested
   // for loops is probably worse anyways.
   switch( k_n ){
   case 3:
     for( x = 0; x < n - 2; x ++ ){
       for ( y = x + 1; y < n - 1; y++ ){
	 if( !gmat[ x ][ y ] ){
	   for( z = y + 1; z < n; z++ ){
	     if( !gmat[ x ][ z ] && !gmat[ y ][ z ] ){
	       found = 1;
	       goto next;
	     }
	   }
	 }
       }
     }
     break;

   case 4:
     for( x = 0; x < n - 3; x ++ ){
       for ( y = x + 1; y < n - 2; y++ ){
	 if( !gmat[ x ][ y ] ){
	   for( z = y + 1; z < n - 1; z++ ){
	     if( !gmat[ x ][ z ] && !gmat[ y ][ z ] ){
	       for( a = z + 1; a < n; a++ ){
		 if( !gmat[ x ][ a ] && !gmat[ y ][ a ] 
		     && !gmat[ z ][ a ] ){
		   found = 1;
		   goto next;
		 }
	       }
	     }
	   }
	 }
       }
     }
     break;

   case 5:
     for( x = 0; x < n - 4; x ++ ){
       for ( y = x + 1; y < n - 3; y++ ){
	 if( !gmat[ x ][ y ] ){
	   for( z = y + 1; z < n - 2; z++ ){
	     if( !gmat[ x ][ z ] && !gmat[ y ][ z ] ){
	       for( a = z + 1; a < n - 1; a++ ){
		 if( !gmat[ x ][ a ] && !gmat[ y ][ a ] 
		     && !gmat[ z ][ a ] ){
		   for( b = a + 1; b < n; b++ ){
		     if( !gmat[ x ][ b ] && !gmat[ y ][ b ] && 
			 !gmat[ z ][ b ] && !gmat[ a ][ b ]){
		       found = 1;
		       goto next;
		     }
		   }
		 }
	       }
	     }
	   }
	 }
       }
     }
     break;

   case 6:
     for( x = 0; x < n - 5; x ++ ){
       for ( y = x + 1; y < n - 4; y++ ){
	 if( !gmat[ x ][ y ] ){
	   for( z = y + 1; z < n - 3; z++ ){
	     if( !gmat[ x ][ z ] && !gmat[ y ][ z ] ){
	       for( a = z + 1; a < n - 2; a++ ){
		 if( !gmat[ x ][ a ] && !gmat[ y ][ a ] 
		     && !gmat[ z ][ a ] ){
		   for( b = a + 1; b < n - 1; b++ ){
		     if( !gmat[ x ][ b ] && !gmat[ y ][ b ] && 
			 !gmat[ z ][ b ] && !gmat[ a ][ b ]){
		       for( c = b + 1; c < n; c++ ){
			 if( !gmat[ x ][ c ] && !gmat[ y ][ c ] && 
			     !gmat[ z ][ c ] && !gmat[ a ][ c ] &&
			     !gmat[ b ][ c ]){
			   found = 1;
			   goto next;
			 }
		       }
		     }
		   }
		 }
	       }
	     }
	   }
	 }
       }
     }
     break;

   default:
     printf( "Error: K_n chosen is not supported yet\n" );
     return 1;
   }

   if( !found ){
     count++;
     writeg6(log_g, g, m, n);
   }

  next:
   
   free(g);   /* After you finished with the graph */
  }

  fprintf( log_g, ">%i graph(s) found\n", count );
  fprintf( log_counts, "%i vertices -> %i graphs\n", n, count );
  fclose( log_g );
  fclose( log_counts );

  if( count == 0 ){
    return 0;
  }else{
    return 1;
  }

}

