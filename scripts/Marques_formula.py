#!/usr/bin/env python

# Marques_formula.py
# J.P. Marques et Al., Neuroimage 49 (2010) 1271-1281
# The BeBo Company, 2017

import sys
import re
import matplotlib.pyplot as plt
import numpy as np

def main():
	if ( len ( sys.argv ) == 2 ) :
		cmdarg =  sys.argv[1];
		if ( re.search ( "help$", cmdarg ) or re.match( '-h$', cmdarg ) ):
			print ( "Usage: Marques_formula.py -enne <n> -alpha1 <alpha1> -alpha2 <alpha2>" \
				 " -eff <eff> -t_arre <TR> -t_er <TER> -t_ah <TA> -t_beh <TB> -t_ceh <TC>"\
				 " -datadir <datadir> -T1low <T1low> -T1up <T1up>" );
			exit ( 1 );

	if ( len ( sys.argv ) != 25 ) :
		print ( "Usage: Marques_formula.py -enne <n> -alpha1 <alpha1> -alpha2 <alpha2>" \
			 " -eff <eff> -t_arre <TR> -t_er <TER> -t_ah <TA> -t_beh <TB> -t_ceh <TC>"\
			 " -datadir <datadir> -T1low <T1low> -T1up <T1up>" );
		exit ( 1 )

	while ( sys.argv ):
		cmdarg = sys.argv.pop ( 0 );
		if ( cmdarg == '-enne' ):
			enne = int ( sys.argv.pop ( 0 ) )
		elif ( cmdarg == "-alpha1" ):
			alpha_1 = float ( sys.argv.pop ( 0 ) ) * 2 * np.pi / 360
		elif ( cmdarg == "-alpha2" ):
			alpha_2 = float ( sys.argv.pop ( 0 ) ) * 2 * np.pi / 360
		elif ( cmdarg == "-eff" ):
			eff = float ( sys.argv.pop ( 0 ) ) 
		elif ( cmdarg == "-t_arre" ):
			TR = float ( sys.argv.pop ( 0 ) ) 
		elif ( cmdarg == "-t_er" ):
			TER = float ( sys.argv.pop ( 0 ) ) 
		elif ( cmdarg == "-t_ah" ):
			TA = float ( sys.argv.pop ( 0 ) ) 
		elif ( cmdarg == "-t_beh" ):
			TB = float ( sys.argv.pop ( 0 ) ) 
		elif ( cmdarg == "-t_ceh" ):
			TC = float ( sys.argv.pop ( 0 ) ) 
		elif ( cmdarg == "-datadir" ):
			data_dir = sys.argv.pop ( 0 )
		elif ( cmdarg == "-T1low" ):
			T1L = float ( sys.argv.pop ( 0 ) ) 
		elif ( cmdarg == "-T1up" ):
			T1U = float ( sys.argv.pop ( 0 ) ) 

	T1 = np.array([]);
	T1 = np.arange(T1L, T1U, 0.005)
	y = (int)(T1.shape[0])
	print ( y )
	E1 = np.array([])
	E1 = np.exp( -TER / T1 )
	EA = np.array([])
	EA = np.exp ( - TA / T1 )
	EB = np.array([])
	EB = np.exp ( - TB / T1 )
	EC = np.array([])
	EC = np.exp ( - TC / T1 )
	ED = np.array([])
	ED = np.exp (  TC / T1 )

	# the steady-state
	mon_z_ss = np.array([])
	first_expr = np.array([])
	second_expr = np.array([])
	first_expr = ( 1 - EA ) * ( np.power ( np.cos ( alpha_1 ) * E1 , enne ) )
	second_expr = np.array([])
	second_expr = ( 1 - E1 ) * ( ( 1 - np.power ( np.cos ( alpha_1 ) * E1, enne ) ) \
				   / ( 1 - np.cos ( alpha_1 ) * E1 ) )
	third_expr = np.array([])
	third_expr = np.power ( np.cos ( alpha_2 ) * E1, enne )
	fourth_expr = np.array([])
	fourth_expr = ( 1 - E1 ) * ( ( 1 - np.power ( np.cos ( alpha_2 ) * E1, enne ) ) \
				   / ( 1 - np.cos ( alpha_2 ) * E1 ) ) 
	denom = np.array([])
	denom = 1 + eff * np.power ( np.cos ( alpha_1 ) * np.cos ( alpha_2 ), enne ) \
		    * np.exp ( - TR / T1 )
	mon_z_ss = ( ( ( ( first_expr + second_expr ) * EB + ( 1 - EB ) ) * third_expr + \
		     fourth_expr ) * EC + ( 1 - EC ) ) / denom

	Factor_1n = np.array([])
	Factor_1n = np.power ( np.cos ( alpha_1 ) * E1, enne/2 - 1 )  
	Factor_2n = np.array([])
	Factor_2n = np.power ( np.cos ( alpha_2 ) * E1, enne/2 )  
	Factor_2np = np.power ( np.cos ( alpha_2 ) * E1, -enne/2 )  
	Factor_1d = np.array([])
	Factor_1d = 1 - E1 * np.cos ( alpha_1 )
	Factor_2d = np.array([])
	Factor_2d = 1 - np.cos ( alpha_2 ) * E1
	GRE_I = np.array([])
	GRE_I = np.sin ( alpha_1 ) * ( ( ( 1 - EA ) - ( eff * mon_z_ss * EA) ) * Factor_1n + \
				      ( 1 - E1 ) * ( ( 1 - Factor_1n ) / Factor_1d ) )
	GRE_II = np.array([])
	GRE_II = np.sin ( alpha_2 ) * ( ( mon_z_ss - ( 1 - EC ) ) / ( EC * Factor_2n ) - \
				      ( 1 - E1 ) * ( ( Factor_2np - 1 ) / Factor_2d ) )
	MP2RAGE = np.array([])
	MP2RAGE = np.divide ( np.multiply ( GRE_I, GRE_II ), ( np.multiply ( GRE_I, GRE_I ) + \
						    np.multiply ( GRE_II, GRE_II ) ) )
	plt.plot ( MP2RAGE, T1 )
	plt.show()

	#for i in range(0, len(T1)):
	#    for j in range(0, len(T1)):
        #        if ( MP2RAGE[i] > 0.4 ):
        #           if ( MP2RAGE[j] < MP2RAGE[i] ):
        #              T1[j] = T1[i]
	MP2RAGEFil = data_dir + "/MP2Rage-T1"
	MMF = open (MP2RAGEFil, 'w' )
	for i in range(0, len(T1)):
	     s = str ( MP2RAGE[i] ) + "\t" + str ( T1[i] ) + "\n"
	     MMF.write ( s )
	MMF.close ( )
	return y

main()
