#!/usr/bin/env python

# Read_param_MP2RAGE.py             
# to read some parameters associated    
# with a MP2RAGE experiment.             

"""
"""
import sys
import os
import time
import re
import numpy as np
import matplotlib.pyplot as plt
from pylab import *
import pylab as P
import string

def find_element_in_list(element, list_element):
    try:
        index_element = list_element.index(element)
        return index_element
    except ValueError:
        return None

if ( len ( sys.argv ) == 2 ) :
        cmdarg =  sys.argv[1];
        if ( re.search ( "help$", cmdarg ) or re.match( '-h$', cmdarg ) ):
                print ( "Usage: Read_Param_MP2RAGE.py -basedir <base_dir> -datadir <data_dir>"\
                        " -datadir <datadir> -processdir <processdir> -bindir <bindir>");
                exit ( 1 );

if ( len ( sys.argv ) != 11 ) :
        print ( "Usage: Read_Param_MP2RAGE.py -basedir <base_dir> -datadir <data_dir>"\
                 " -bindir <bindir> -processdir <processdir> -filename <filename>" );
        exit ( 1 )

cur_dir = os.getcwd ( )

while ( sys.argv ):
        cmdarg = sys.argv.pop ( 0 );
        if ( cmdarg == '-basedir' ):
                base_dir = sys.argv.pop ( 0 )
        elif ( cmdarg == "-datadir" ):
                data_dir = sys.argv.pop ( 0 )
        elif ( cmdarg == "-bindir" ):
                bin_dir = sys.argv.pop ( 0 )
        elif ( cmdarg == "-processdir" ):
                process_dir = data_dir + sys.argv.pop ( 0 )
        elif ( cmdarg == "-filename" ):
                name_from = sys.argv.pop ( 0 )
print (process_dir)
m = 0
name_from = data_dir + "/" + name_from
f = open ( name_from, 'r' )
#for line in f:
while True:
    line = f.readline ( )
    if not line: break
    if ( re.search ( "EchoRepTime=", line ) ):
       TER = float ( line.replace ( "##$EchoRepTime=", "" ) ) * 1e-3
    if ( re.search ( "T_A=", line ) ):
       TA = float ( line.replace ( "##$T_A=", "" ) ) * 1e-3
    if ( re.search ( "T_B=", line ) ):
       TB = float ( line.replace ( "##$T_B=", "" ) ) * 1e-3
    if ( re.search ( "T_C=", line ) ):
       TC = float ( line.replace ( "##$T_C=", "" ) ) * 1e-3
    if ( re.search ( "PVM_RepetitionTime=", line ) ):
       TR = float ( line.replace ( "##$PVM_RepetitionTime=", "" ) ) * 1e-3
    if ( re.search ( "ExcPulse1=", line ) ):
       if m == 0:
          Exc = line.replace ( "##$ExcPulse1=(", "" )
          Exc.split(',')
          alpha_1 = float ( (Exc.split(',')[2]) )
       m = 1
    if ( re.search ( "ExcPulse2=", line ) ):
       if m == 1:
          Exc = line.replace ( "##$ExcPulse2=(", "" )
          Exc.split(',')
          alpha_2 = float ( (Exc.split(',')[2]) )
       m = 2
    if ( re.search ( "PVM_EncMatrix=", line ) ):
       line = f.readline ( )
       re_points = ( line.split ( ' ' )[0] )
       ph_points = ( line.split ( ' ' )[1] )
       nslice = ( line.split ( ' ' )[2] )
       nslices = nslice.replace ( '\n', '' )
       enne = int ( line.split ( ' ' )[1] )
       totnum = int ( line.split( ' ' )[0] ) * enne * int ( line.split( ' ' )[2] )
    #if ( re.search ( "PVM_EncValues1=", line ) ):
    #   enn = line.replace ( "##$PVM_EncValues1=(", "" )
    #   enne = int ( enn.replace ( ")", "" ) )
       

print (TER, TA, TB, TC, TR, alpha_1, alpha_2, enne, totnum)

T1L = 0.2
T1U = 2.55
T1 = np.arange ( T1L, T1U, 0.005 )
y = T1.shape[0]

cmd_arg = "-enne " + str ( enne ) + " -alpha1 " + str ( alpha_1 ) + " -alpha2 " + str ( alpha_2 ) \
          + " -eff 0.99" + " -t_arre " + str ( TR ) + " -t_er " + str ( TER ) + " -t_ah " + str ( TA ) \
          + " -t_beh " + str ( TB ) + " -t_ceh " + str ( TC ) + " -datadir " + data_dir + " -T1low " \
          + str ( T1L ) + " -T1up " + str ( T1U )
cmd_1 = base_dir + "/scripts/Marques_formula.py " + cmd_arg
print ( cmd_1 )
os.system ( cmd_1 )

cmd_2 = bin_dir + "/extr_mp2r " + process_dir + "/2dseq " +  re_points +  " " + ph_points + " " + nslice
print ( cmd_2 )
os.system ( cmd_2 )

tmpfil = data_dir + "/bazuk "
cmd_3 = bin_dir + "/calcint_mp2 " + tmpfil + str ( totnum )
print ( cmd_3 )
os.system ( cmd_3 )

tmpfil2 = data_dir + "/MP2Rage-T1 "
cmd_4 = bin_dir + "/unravel " + tmpfil + str ( totnum ) + " " + tmpfil2 + str ( y )
print ( cmd_4 )
os.system ( cmd_4 )

scr_dir = bin_dir.replace ( "bin", "scripts" )
myfil = "./T1map"
cmd_5 = scr_dir + "/B1Slide.py " + " -file " + myfil + " -npoints " + re_points + \
        " -npoints1 " + ph_points + " -nslices " + nslices
print ( cmd_5 )
os.system ( cmd_5 )
