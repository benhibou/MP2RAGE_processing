/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/MDEFT/initMeth.c,v $
 *
 * Copyright (c) 2002 - 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: initMeth.c,v 1.20 2013/05/22 11:05:07 sako Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: initMeth.c,v 1.20 2013/05/22 11:05:07 sako Exp $(C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


#include "method.h"

/*:=MPB=:=======================================================*
 *
 * Global Function: initMeth
 *
 * Description: This procedure is implicitly called when this
 *	method is selected.
 *
 * Error History:
 *
 * Interface:							*/

void initMeth()
/*:=MPE=:=======================================================*/
{
  int dimRange[2] = { 3,3 };
  int lowMat[3]   = { 32, 32, 8 };
  int upMat[3]    = { 2048, 2048, 512 };
  int dfMat[3]    = { 256, 128, 32};
  
  
  DB_MSG(( "-->MDEFT:initMeth\n" ));
  
  /* which version of toolboxes should be active */
  
  PTB_VersionRequirement( Yes,20100101,"");
    
  /*  initialize local and redirected parameters */
  
  NAveragesRange();

  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
    PVM_NRepetitions = 1;

  /*  Initialize dummy scans */
  STB_InitDummyScans(0.0);

  /*  Initialize dummy echoes */
  deRange();

  EchoPositionRange();
 
  /* encoding group */
  STB_InitEncoding( 3, dimRange, lowMat, upMat, dfMat);  

   
  /* Initialisation of rf pulse parameters */
  
  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  "ExcPulse1Enum",  // name of pulse list parameter
                  "ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "Calculated",     // default shape
                  3000.0,           // default bandwidth
                  2.0);            // default pulse angle
  
  // check valid range for this specific pulse see parsRelations.c
  ExcPulse1Range(); 
 
  
  STB_InitRFPulse("ExcPulse2",      // name of pulse structure
                  "ExcPulse2Enum",  // name of pulse list parameter
                  "ExcPulse2Ampl",  // name of pulse amplitude parameter
                  "ExcPulse2Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "Calculated",     // default shape
                  3000.0,           // default bandwidth
                  5.0);            // default pulse angle
  
  // check valid range for this specific pulse see parsRelations.c
  ExcPulse2Range(); 
 
  
  /* Initialize segment parameters */
  ParxRelsMakeNonEditable("SegmNumber");
  if(ParxRelsParHasValue("SegmNumber") == No)
    SegmNumber = 1;
  SegmNumberRange();
  ParxRelsMakeNonEditable("SegmDuration");
  if(ParxRelsParHasValue("SegmRepTime") == No)
    SegmRepTime = 3200.0;

  /* Initialisation of nucleus */  
  STB_InitNuclei(1);

  /* Initialisation of geometry parameters */
  STB_InitImageGeometry();
 
  /* 1: method specific initialisation */
  if(ParxRelsParHasValue("SecondImageDelay") == No)
    SecondImageDelay = 30.0;
  
  if(ParxRelsParHasValue("EchoRepTime") == No)
    EchoRepTime = 10.0;

  if(ParxRelsParHasValue("EchoTime") == No)
    EchoTime = 2.5;

  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;
   
  /* Initialisation of spoilers */
  MRT_InitSpoiler("ReadSpoiler");
  MRT_InitSpoiler("SliceSpoiler");

  /* initialize digitizer parameter */

  STB_InitDigPars();
  EffSWhRange();

  /* not a csi experiment */
  PTB_SetSpectrocopyDims( 0, 0 );
  
  /* Initialisation of modules */

  InitMdeftPrepModule();
  STB_InitTriggerModule();
  STB_InitFovSatModule();
  STB_InitFatSupModule();

  /* initialize mapshim parameter class */
  STB_InitMapShim();

  if (!ParxRelsParHasValue("RFSpoiling"))
    RFSpoiling=No;

  if (ParxRelsParHasValue("PVM_MotionSupOnOff") == 0)
     PVM_MotionSupOnOff = Off;

  ParxRelsMakeNonEditable("PVM_MinEchoTime,PVM_AcquisitionTime");
  ParxRelsMakeNonEditable("EncGradDur,PVM_EchoPosition");

  /* 
   * Once all parameters have initial values, the backbone is called
   * to assure they are consistent 
   */
  
  backbone();
  
  
  DB_MSG(("<--MDEFT:initMeth\n"));
  
}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/









