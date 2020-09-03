/****************************************
** Marques_T1Map.c
** Read complex process data recorded
** per to Marques method to create
** a T1 map.
******************************************/


#include <stdio.h>
#include <errno.h>

int 	main ( int argc, char ** argv )
{
	FILE	* fp, ** hp;
	void	* ptr;
	int	i, j, k, m, ni, readpt, phase1pt, phase2pt;
	char 	** filnam;
	int	* l;

	if ( ( ( fp = fopen ( * ++argv, "r" ) ) == 0 ) ) {
	   perror ( "Error reading fp" );
	   exit ( 1 );
 	}
	readpt = atoi ( *++argv );
	ni = atoi ( *++argv );
	
	hp = ( FILE ** ) malloc ( ni * sizeof ( FILE * ) );
	filnam = ( char ** ) malloc ( ni * sizeof ( char * ) );
	for ( i = 0 ; i < ni ; i++ ) {
	    hp[i] = ( FILE * ) malloc ( sizeof ( FILE ) );
	    filnam[i] = ( char * ) malloc ( 12 * sizeof ( char ) );
	    if ( sprintf ( filnam[i], "fid_%d", i  ) == EOF ) {
	       perror ( "Copying filename" );
	       exit ( 1 );
	    }
	    if ( ( ( hp[i] = fopen ( filnam[i], "w" ) ) == 0 ) ) {
	       perror ( "Error opening File 2dseq_i for write" );
	       exit ( 1 );
 	    }
	}

	ptr = ( int * ) malloc ( readpt * sizeof ( int ) );
	i = 1;
	j = 0;
	k = 0;
	m = 0;
	while ( 1 ) {
	    i = fread ( ptr, sizeof ( int ), readpt, fp );
	    if ( i < 1 ) {
	       break;
	    }
	    l = ptr;
	    fwrite ( l, sizeof ( int ), readpt, hp[k] );
	    j++;
	    k++;
	    if ( k == ni ) {
	       k = 0;
            }
	}
	fclose ( fp );
	for ( i = 0 ; i < ni ; i++ ) {
	    fclose ( hp[i] );
	    free ( hp[i] );
	    free ( filnam[i] );
        }
	free ( ptr );
	free ( hp );
	free ( filnam );
	return 1;
}
