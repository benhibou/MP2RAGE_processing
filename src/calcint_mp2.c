/********************************************
** calcint_mp2.c
** Calculate the mp2rage intensity as
** given in equation 3 of Marques et Al., 
** Neuroimage 49, (2010) 1271 - 1281; 
** Values of the two complex images are
** contained in previously extracted files
** and named fid_1r, fid_1i, fid_2r, fid_2i. 
*********************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int 	main ( int argc, char ** argv )
{
	FILE	* fp, *gp, ** hp;
	void	** ptr;
	int	i, j, k, m, n, ni, readpt, phase1pt, phase2pt;
	char 	** filnam, * filwr, * filwr_2;
	short	** l;
        long	* denom, * numer;
        double	* mp2Rage, * mp2Rage_2;

	/* fp points to the file that will receive the
           mp2rage intensities. */
        
        
        filwr = ( char * ) malloc ( strlen ( * ++argv ) * sizeof ( char ) );
        if ( ( ( strcpy ( filwr, * argv ) ) == NULL ) ){ 
	   perror ( "Error in strcpy" );
           exit ( 1 );
        }
        printf ( "%s\n", filwr ); 
        filwr_2 = ( char * ) malloc ( (strlen ( filwr ) + 2 ) * sizeof ( char ) );
        if ( ( ( strcat ( strcat ( filwr_2, filwr ), "_2" ) ) == NULL ) ){ 
	   perror ( "Error in strcat" );
           exit ( 1 );
        }
        printf ( "%s\n", filwr_2 ); 
	if ( ( ( fp = fopen ( filwr, "w" ) ) == 0 ) ) {
	   perror ( "Error creating fp for write" );
	   exit ( 1 );
 	}
	if ( ( ( gp = fopen ( filwr_2, "w" ) ) == 0 ) ) {
	   perror ( "Error creating fp for write" );
	   exit ( 1 );
 	}
	readpt = atoi ( *++argv );
	ni = 4;
	
	hp = ( FILE ** ) malloc ( ni * sizeof ( FILE * ) );
	filnam = ( char ** ) malloc ( ni * sizeof ( char * ) );
	for ( i = 0 ; i < ni ; i++ ) {
	    hp[i] = ( FILE * ) malloc ( sizeof ( FILE ) );
	    filnam[i] = ( char * ) malloc ( 12 * sizeof ( char ) );
            if ( i == 0 ) {
	       if ( sprintf ( filnam[i], "fid_1r" ) == EOF ) {
	          perror ( "Copying filename" );
	          exit ( 1 );
	       }
            }
            else if ( i == 1 ){
	       if ( sprintf ( filnam[i], "fid_2r" ) == EOF ) {
	          perror ( "Copying filename" );
	          exit ( 1 );
	       }
            }
            if ( i == 2 ) {
	       if ( sprintf ( filnam[i], "fid_1i" ) == EOF ) {
	          perror ( "Copying filename" );
	          exit ( 1 );
	       }
            }
            else if ( i == 3 ){
	       if ( sprintf ( filnam[i], "fid_2i" ) == EOF ) {
	          perror ( "Copying filename" );
	          exit ( 1 );
	       }
            }
	    if ( ( ( hp[i] = fopen ( filnam[i], "r" ) ) == 0 ) ) {
	       perror ( "Error opening File 2dseq_i for read" );
	       exit ( 1 );
 	    }
	}

	ptr = ( void ** ) malloc ( ni * sizeof ( void * ) );
	l = ( short ** ) malloc ( ni * sizeof ( short * ) );
	numer = ( long * ) malloc ( readpt * sizeof ( long ) );
	denom = ( long * ) malloc ( readpt * sizeof ( long ) );
        mp2Rage = ( double * ) malloc ( readpt * sizeof ( double ) );
        mp2Rage_2 = ( double * ) malloc ( readpt * sizeof ( double ) );
        for ( i = 0 ; i < 4 ; i++ ) {
            ptr[i] = (  short * ) malloc ( readpt * sizeof (  short ) );     
        }
	i = 1;
	j = 0;
	k = 0;
	m = 0;
	while ( 1 ) {
            for ( i = 0 ; i < 4 ; i++ ) {
	        j = fread ( ptr[i], sizeof (  short ), readpt, hp[i] );
            }
	    if ( j < 1 ) {
	       break;
	    }
            for ( i = 0 ; i < 4 ; i++ ) {
	        l[i] = ptr[i];
            }
            
            for ( j = 0 ; j < readpt ; j++ ) {
                denom[j] = 0;
                numer[j] = 0;
                mp2Rage[j] = 0;
                mp2Rage_2[j] = 0;
                for ( n = 0 ; n < ni ; n++ ) {
                    denom[j] += ( l[n][j] * l[n][j] );
                }
                numer[j] = l[0][j] * l[1][j] + l[2][j] * l[3][j]; 
                mp2Rage[j] = ( numer[j] / (double)denom[j] );
		/* To help render the background darker one tries the following */
                denom[j] += 2000000; 
                numer[j] -= 1000000;
                mp2Rage_2[j] = ( numer[j] / (double)denom[j] );
            }
            k += readpt ;
            printf ( "%f\n", k/(double)128 );
	    fwrite ( mp2Rage, sizeof ( double ), readpt, fp );
	    fwrite ( mp2Rage_2, sizeof ( double ), readpt, gp );
	}
	fclose ( fp );
	fclose ( gp );
	return 1;
}
