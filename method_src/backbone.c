/* ***************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/MDEFT/backbone.c,v $
 *
 * Copyright (c) 2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: backbone.c,v 1.19 2013/06/06 12:51:58 sako Exp $
 *
 * ***************************************************************/


static const char resid[] = "$Id $ (c) 2007Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


#include "method.h"

/* global variables */
double EchoDelay, EffPulseDur;

void backbone(void)
{
  double minFov[3];
  double minSliceThick;
  int dim;

  DB_MSG(("-->backbone"));


  /* update nuclei parameter group                            */

  STB_UpdateNuclei(No);

  dim=PTB_GetSpatDim();

  /* update encoding parameter group                          */

  PVM_RareFactor = 1;
  
  STB_UpdateEncoding(
    &PVM_RareFactor,
    SEG_INTERLEAVED,
    Yes,
    Yes,
    Yes,
    &PVM_EchoPosition);
     
  PVM_RareFactor = PVM_EncMatrix[1]/SegmNumber;

  STB_UpdateEncoding(
    &PVM_RareFactor,
    SEG_INTERLEAVED,
    Yes,
    Yes,
    Yes,
    &PVM_EchoPosition);
    
  SegmNumber = PVM_EncMatrix[1]/PVM_RareFactor;

  /* update parameters controlling the data sampling period   */

  STB_UpdateDigPars(&PVM_EffSWh,
                    PVM_EncMatrix[0],
                    &PVM_AntiAlias[0],
                    &PVM_AcquisitionTime);

  DB_MSG(("STB_UpdateDigPars completed"));

  /* update excitation pulse                                  */  
  
  UpdateRFPulses();
    

  /* general features of the method */

  PVM_NEchoImages = 2;

  PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);
  

  /* set limits for read, phase and slice gradients            */

  ControlGradientLimits(PVM_MajSliceOri);
  

  /* calculate minima for FOV and slice thickness             */

  UpdateGeometryMinima(minFov,
                       &minSliceThick);

 
  /* update geometry parameters                               */

  STB_UpdateImageGeometry(dim, 
                          PVM_Matrix,
                          minFov,
                          0, // total slices (no restr)
                          1, // man n pack
                          1, // max sl per pack
                          minSliceThick,
                          1.0); // sliceFovRatio in 3D

 
  /* update slice spoiler */
  double mindurSlice = 1.5*CFG_GradientRiseTime();
  MRT_UpdateSpoiler("SliceSpoiler",2.0,ExcSliceGradLim,mindurSlice,PVM_GradCalConst,PVM_SpatResol[2]*PVM_EncZf[2]);

  /* handling of modules */
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, PVM_SpatResol[2]*PVM_EncZf[2]);
  STB_UpdateTriggerModule();
  if(PVM_TriggerModule == On)
    PVM_TriggerMode = per_PhaseStep;
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
   
  UpdateRFSpoiling();

  /* Calculate read and slice gradients */
  UpdateReadSliceGradients();

  /* Sequence elements, TE, TR, total time: */
  UpdateSequenceTiming();
 
  /* calculate dephasing and phase-encoding gradients         */
  UpdatePhaseGradients();

  /* calculate frequency offsets                              */
  UpdateFrequencyOffsets();

  /* Dummy scan updater */
  STB_UpdateDummyScans(SegmRepTime);

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  /* Set ACQP parameters */
  SetBaseLevelParam();

  /* Set RECO parameters */
  SetRecoParam();
  
  DB_MSG(("<--backbone"));
  return;
}

/*-------------------------------------------------------
 * local utility routines to simplify the backbone 
 *------------------------------------------------------*/


void UpdateSequenceTiming()
/* -------------------------------------------------------
   Adapt sequence elements to the current geometry 
   (in this method, only EncGradDur is concerned),
   update TE and TR.
   ReadGrad and ExSliceGrad must be already set.
   ------------------------------------------------------*/
{
  double minEnc2d, minEnc3d, minRephSlice, minDephRead, 
         extensionPossible, extensionAllowed, extension;

  /* Part of the exctiation pulse to be refocused: */
  EffPulseDur = ExcPulse1.Length * (ExcPulse1.Rpfac/100);

  /* Part of the echo to be refocused */
  EchoDelay = PVM_AcquisitionTime * PVM_EchoPosition / 100;

  /* Minimum durations of all phase-gradient pulses */

  minEnc2d     = MRT_EncodingTime(PVM_SpatResol[1],
                                  PVM_GradCalConst*Phase2DGradLim/100);

  minEnc3d     = PTB_GetSpatDim()>2? 
                 MRT_EncodingTime(PVM_SpatResol[2],
                                     PVM_GradCalConst*Phase3DGradLim/100) 
                 : 0;
  
  minRephSlice = MRT_DephaseTime(EffPulseDur, 
                                 CFG_GradientRiseTime(),
                                 ExcSliceGrad,
                                 ExcSliceRephGradLim);
  
  minDephRead  = MRT_DephaseTime(EchoDelay, 
                                 CFG_GradientRiseTime(),
                                 ReadGrad,
                                 ReadDephGradLim);

 
  /* In this sequence all phase-gradient pulses are
     simultaneous with duration EncGradDur. We set it first
     to the common (longest) minimum: */

  EncGradDur = MAX_OF( MAX_OF(minEnc2d ,  minEnc3d),
                       MAX_OF(minRephSlice ,minDephRead ) ); 

  /* EncGradDur should also contain one ramp, thus: */
 
  EncGradDur = MAX_OF(EncGradDur, CFG_GradientRiseTime());

  DB_MSG(("minimum EncGradDur = %f", EncGradDur));

  /* Update TE with the mimimum EncGradDur */

  UpdateEchoTime();


  /* If there is some freedom, make EncGradDur longer
     (to avoid unnecessarily strong phase gradients), 
     but not longer than allowed by gradient resolution */

  extensionPossible = EchoTime - PVM_MinEchoTime; 
  extensionAllowed  = MRT_MaxEncodingTime(PVM_Fov[1], PVM_GradCalConst)-EncGradDur;

  DB_MSG(("ext possible = %f, allowed = %f", extensionPossible, extensionAllowed));

  extension = MIN_OF(extensionPossible, extensionAllowed);
  extension = MAX_OF(extension, 0);

  DB_MSG(("extension = %f",extension));

  EncGradDur += extension;
  TeFillDelay = EchoTime - PVM_MinEchoTime - extension;

  

  /* Other sequence elements, not involved in TE, e.g. spoilers  */
  double mindurRead =  MAX_OF(EncGradDur+CFG_GradientRiseTime(),PVM_DigEndDelOpt);
  MRT_UpdateSpoiler("ReadSpoiler",2.0,Phase2DGradLim,mindurRead,PVM_GradCalConst,PVM_SpatResol[0]*PVM_EncZf[0]);


  UpdateEchoRepTime();
 
}



void UpdateEchoRepTime(void)
{
  int nSlices;
  int segmsteps;
  int dim = PTB_GetSpatDim();
  double TotalTime=0;
  double riseTime = CFG_GradientRiseTime();
  double fixedTime;

  DB_MSG(("-->UpdateEchoRepTime"));


  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

    /* min TR in a movie: */
  MinEchoRepTime = nSlices * minLoopRepetitionTime();
 
  EchoRepTime = ( EchoRepTime < MinEchoRepTime ?
			 MinEchoRepTime : EchoRepTime );
  ParxRelsCopyPar("PVM_EchoTime","EchoRepTime");


  segmsteps = PVM_EncMatrix[1]/SegmNumber; 

  PreEchoTime =   PVM_FatSupModuleTime         +
	          PVM_FovSatModuleTime         +
	          SliceSpoiler.dur             +
                  riseTime                     +
                  CFG_AmplifierEnable()        +
                  ExcPulse1.Length - EffPulseDur;

  fixedTime = (EchoRepTime * (NDummyEchoes + PVM_EncCentralStep1 - 1))+ PreEchoTime; /* +NDummyEchoes */

  SegmDuration = EchoRepTime * (segmsteps + NDummyEchoes);  /* + NDummyEchoes */

  UpdateMdeftPrepModule(fixedTime, PVM_DeriveGains, PVM_SpatResol[2]); 

  if(PreparationMode == Inversion_prep)
  {
    ParxRelsMakeEditable("SegmRepTime");
    MinSegmRepTime = 2 * SegmDuration + Mdeft_PrepModuleTime + T_B; 
    SegmRepTime = ( SegmRepTime < MinSegmRepTime ?
		    MinSegmRepTime : SegmRepTime );
    SegmPad = SegmRepTime - MinSegmRepTime ; 
  }
  else
  {
    ParxRelsMakeNonEditable("SegmRepTime");
    SegmRepTime = 2 * SegmDuration + Mdeft_PrepModuleTime;
  }
      

  ParxRelsCopyPar("PVM_RepetitionTime","SegmRepTime");
  T_C = SegmRepTime - ( T_A + T_B + 2 * SegmDuration );

  if( dim >1 )
  {
    TotalTime = SegmRepTime * SegmNumber + 
              SegmRepTime * PVM_DummyScans;
  }

  if( dim >2 )
    TotalTime = TotalTime * PVM_EncMatrix[2];

  /* time for one repetition */
  OneRepTime = TotalTime* PVM_NAverages/1000.0;

  TotalTime = TotalTime * PVM_NAverages * PVM_NRepetitions;

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  
  
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  EchoRepTime = MAX_OF(MinEchoRepTime,EchoRepTime);

  DB_MSG(("<--UpdateEchoRepTime"));
  return;
}


void UpdateGeometryMinima( double *minFov,
                           double *minSliceThick)
{
  int dim;
  

  DB_MSG(("-->UpdateGeometryMinima"));

  
  dim=PTB_GetSpatDim();
  
  minFov[0]     = PVM_EffSWh / 
                  (1e-2*ReadDephGradLim * PVM_GradCalConst);
  *minSliceThick = ExcPulse1.Bandwidth /
                  (1e-2*ExcSliceGradLim * PVM_GradCalConst);

  if(dim >= 2)
  {
    minFov[1] = minFov[0]/8;
  }

  if(dim >= 3)
  {
    
    minFov[2] = *minSliceThick;
  }

  DB_MSG(("<--UpdateGeometryMinima"));
}


void UpdateReadSliceGradients(void)
{
  DB_MSG(("-->UpdateReadSliceGradients"));

  ReadGrad = MRT_ReadGrad(PVM_EffSWh,
                          PVM_Fov[0],
                          PVM_GradCalConst);

  ExcSliceGrad = MRT_SliceGrad(ExcPulse1.Bandwidth,
                               PVM_SliceThick,
                               PVM_GradCalConst);

  DB_MSG(("<--UpdateReadSliceGradients"));
}

void UpdatePhaseGradients()
{
  
  DB_MSG(("-->UpdatePhaseGradients"));

  /* Calculation of phase-encoding, 
     dephasing and rephasing gradients.
     
     (ReadGrad, ExcSliceGrad, EchoDelay, EffPulseDur,
      and EncGradDur must be calculated before)       */



  ReadDephGrad = MRT_DephaseGrad(EncGradDur,
                                 EchoDelay, 
                                 CFG_GradientRiseTime(),
                                 ReadGrad);
  
  ExcSliceRephGrad = MRT_DephaseGrad(EncGradDur,
                                 EffPulseDur, 
                                 CFG_GradientRiseTime(),
                                 ExcSliceGrad);

  Phase2DGrad = MRT_PhaseGrad(EncGradDur,
                              PVM_Matrix[1],
                              PVM_Fov[1],
                              PVM_GradCalConst);

  if(PTB_GetSpatDim() == 3)
  {
    Phase3DGrad = MRT_PhaseGrad(EncGradDur,
                                PVM_Matrix[2],
                                PVM_Fov[2],
                                PVM_GradCalConst);
  }
  else
  {
    Phase3DGrad = 0.0;
  }

  DB_MSG(("<--UpdatePhaseGradients"));
  return;
}

void UpdateFrequencyOffsets( void )
{
  int spatDim;
  int i,nslices;

  spatDim = PTB_GetSpatDim();
  nslices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);

  MRT_FrequencyOffsetList(nslices,
                          PVM_EffReadOffset,
                          ReadGrad,
                          PVM_GradCalConst,
                          PVM_ReadOffsetHz );

  MRT_FrequencyOffsetList(nslices,
                          PVM_EffSliceOffset,
                          ExcSliceGrad,
                          PVM_GradCalConst,
                          PVM_SliceOffsetHz );

  if(spatDim == 3)
  {
    for(i=0;i<nslices;i++)
      PVM_EffPhase2Offset[i] = -PVM_EffSliceOffset[i];
  }


}


/*--------------------------------------------------------
 * Routine to update RF pulse parameters
 *-------------------------------------------------------*/

void UpdateRFPulses(void)
{

  /* Updates all parameters that belong to ExcPulse1 pulse structure
     (as initialized by STB_InitRFPulse see initMeth.c)
  */

  STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);

  STB_UpdateRFPulse("ExcPulse2",1,PVM_DeriveGains,Conventional);


  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("ExcPulse1Ampl");
    ParxRelsHideInEditor("ExcPulse2Ampl");
  }
  else
  {
    ParxRelsShowInEditor("ExcPulse1Ampl");
    ParxRelsShowInEditor("ExcPulse2Ampl");
  }

  ParxRelsShowInFile("ExcPulse1Ampl");
  ParxRelsShowInFile("ExcPulse2Ampl");

  DB_MSG(("<--UpdateRFPulses"));

  return;
}

/*--------------------------------------------------------
 * Routine to control the visibility of parameters
 *-------------------------------------------------------*/


void ControlGradientLimits(YesNo NotOblique)
{
  DB_MSG(("-->ControlGradientLimits: Obliqueness forbidden: %s",NotOblique==Yes ? "Yes":"No"));



  if(NotOblique==Yes)
  {
    ReadDephGradLim     = 
    Phase2DGradLim      = 
    Phase3DGradLim      =
    ExcSliceRephGradLim = 100.0;
  }
  else
  {
    /* Gradient limits in % of max. Value 57 (1/sqrt(3))
       is needed for arbitrary oblique slices. */
    ReadDephGradLim     = 
    Phase2DGradLim      = 
    Phase3DGradLim      =
    ExcSliceRephGradLim = 57.0;
  }

  ReadGradLim        = 100.0;
  ExcSliceGradLim    = 100.0;

  DB_MSG(("-->ControlGradientLimits"));
}


/* Calculates PVM_MinEchoTime and restrict PVM_EchoTIme.
   EffPulseDur, EncGradDur must be set before */
void UpdateEchoTime( void )
{

  double riseTime = CFG_GradientRiseTime(),
    rampTime = CFG_GradientRampTime()+CFG_InterGradientWaitTime();

  DB_MSG(("-->UpdateEchoTime\n"));
  
  PVM_MinEchoTime = 
    EffPulseDur    +
    rampTime       + //falling ramp of slice grad
    EncGradDur     + //enc. time (ramp+plateau
    rampTime       + //min te-filling (end ramp of encoding)             
    riseTime       + //read-on ramp          +
    PVM_AcquisitionTime * PVM_EchoPosition / 100.0;


  EchoTime = MAX_OF(EchoTime, PVM_MinEchoTime);  
  
  DB_MSG(("<--echoTimeRels\n"));
}



double minLoopRepetitionTime(void)
/* ---------------------------------------------------------
this function rturns the minimum duration of the innermost
pulse program loop
----------------------------------------------------------*/
{
  double minTr, minD0,
    riseTime = CFG_GradientRiseTime(),
    igwt = CFG_InterGradientWaitTime();
  
  minD0 = 0.01  /* ADC_END */ + igwt;
  minTr = 
	SliceSpoiler.dur               +
        riseTime                       +
	CFG_AmplifierEnable()          +
	ExcPulse1.Length - EffPulseDur +
        EchoTime                       + /* FLASH: PVM_EchoTime */
	PVM_AcquisitionTime *(100.0 - PVM_EchoPosition) / 100.0 +
	ReadSpoiler.dur                +
	riseTime                       +
        0.02                           +
	minD0;

  return minTr;
}

void UpdateRFSpoiling(void)
{
  DB_MSG(("-->UpdateRFSpoiling"));

  if(RFSpoiling==No)
  {
    PARX_change_dims("RFPhaseList",1);
    RFPhaseList[0] = 0;
  }
  else
  { 
    int max = (PVM_EncMatrix[1]/SegmNumber)*(SegmNumber+PVM_DummyScans);
    int size = MAX_OF(256,max);
    PARX_change_dims("RFPhaseList",size);
    MRT_RFSpoilPhaseList(117,size,RFPhaseList,Yes);
  }

  DB_MSG(("<--UpdateRFSpoiling"));
}
