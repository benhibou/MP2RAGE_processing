#!/usr/bin/env python

import sys
import os
import re
import numpy as np
import matplotlib.pyplot as plt
import time
from matplotlib.widgets import Slider, Button, RadioButtons
from cube_show_slider import *


if ( len ( sys.argv ) == 2 ) :
        cmdarg =  sys.argv[1];
        if ( re.search ( "help$", cmdarg ) or re.match( '-h$', cmdarg ) ):
                print ( "Usage: B1Slide.py -file <file> -npoints <npoints> -nslices <nslices>" );
                exit ( 1 );

if ( len ( sys.argv ) != 9 ) :
        print ( "Usage: B1Slide.py -file <file> -npoints <npoints> -npoints1 <npoints1> -nslices <nslices>" );
        exit ( 1 )

cur_dir = os.getcwd ( )

while ( sys.argv ):
        cmdarg = sys.argv.pop ( 0 );
        if ( cmdarg == '-file' ):
                file = sys.argv.pop ( 0 )
        elif ( cmdarg == "-npoints" ):
                npoints = sys.argv.pop ( 0 )
        elif ( cmdarg == "-npoints1" ):
                npoints1 = sys.argv.pop ( 0 )
        elif ( cmdarg == "-nslices" ):
                nslices = sys.argv.pop ( 0 )


axis_color = 'lightgoldenrodyellow'
amp_0 = 15

def main():
    #data = read_data('/home/spectro/B1map/workspace/B1map', 32, 64)
    data = read_data( file, int ( nslices ), int ( npoints ), int ( npoints1 ) )
    cube_show_slider( data, 0, cmap='gray' )
    #cube_show_slider( data, 0, cmap='gray', vmin = 0.4, vmax = 4 )
    #cube_show_slider( data, 0, cmap='gray', vmin = -0.4, vmax = 0.4 )
    #cube_show_slider( data, 0, vmin = 0.01, vmax = 0.8 )
    #cube_show_slider( data, 0, cmap='terrain',vmin = 0.75, vmax=1.2 )

def read_data(filename, width, height, depth):
    with open(filename, 'r') as infile:
        # Skip the header
        #infile.seek(64)
        data = np.fromfile(infile, dtype=np.float64)
    # Reshape the data into a 3D array. (-1 is a placeholder for however many
    # images are in the file... E.g. 2000)
    return data.reshape( ( width, height, depth ) )

main()
