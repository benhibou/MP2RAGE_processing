/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/MDEFT/parsDefinition.h,v $
 *
 * Copyright (c) 1999-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: parsDefinition.h,v 1.13 2012/11/27 12:48:01 anba Exp $
 *
 ****************************************************************/

#include "Mdeft_parsDefinition.h"

/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/
double parameter OneRepTime;

PVM_SPOILER_TYPE parameter 
{
  display_name "Read Spoiler";
  relations ReadSpoilerRel;
}ReadSpoiler;

PVM_SPOILER_TYPE parameter 
{
  display_name "Slice Spoiler";
  relations SliceSpoilerRel;
}SliceSpoiler;

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse 1 Shape";
  relations    ExcPulse1EnumRelation;
}ExcPulse1Enum;


PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse 1";
  relations    ExcPulse1Relation;
}ExcPulse1;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse 1 Amplitude";
  relations ExcPulse1AmplRel;
}ExcPulse1Ampl;

double parameter
{
  relations backbone;
}ExcPulse1Shape[];

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse 1 Shape";
  relations    ExcPulse2EnumRelation;
}ExcPulse2Enum;

PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse 2";
  relations    ExcPulse2Relation;
}ExcPulse2;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse Amplitude";
  relations ExcPulse2AmplRel;
}ExcPulse2Ampl;

double parameter
{
  relations backbone;
}ExcPulse2Shape[];

int parameter 
{
  display_name "Dummy Echoes";
  short_description "Number of Dummy Echoes.";
  relations deRelations;
}NDummyEchoes;

double parameter Mydim[];
  
int parameter
{
  display_name "Segments";
  short_description "Number of Segments.";
  relations SegmNumberRelations;
}SegmNumber;

double parameter
{
  display_name "Segment Repetition Time";
  relations backbone; 
  format "%.3f";
  units "ms";
}SegmRepTime;

double parameter MinSegmRepTime;

double parameter
{
  display_name "Segment Duration";
  /* relations SegmDurationRelations; */
  format "%.3f";
  units "ms";
}SegmDuration;

double parameter
{
  display_name "Second Image Delay";
  relations backbone; 
  format "%.3f";
  units "ms";
}SecondImageDelay;

double parameter SegmPad;
double parameter PreEchoTime;
double parameter SliceSegDelay;
double parameter MinSliceSegDur;
double parameter SliceSegEndDelay;

YesNo parameter
{
  display_name "RF Spoiling";
  relations backbone;
}RFSpoiling;


double parameter RFPhaseList[];


/* ---------------------------------------------------------
 * remaining local method parameters
 * --------------------------------------------------------*/

double parameter
{
  display_name "Read Dephase Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ReadDephGrad;

double parameter
{
  display_name "Max Read Dephase Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ReadDephGradLim;

double parameter
{
  display_name "Read Gradient";
  format "%f";
  units "%";
  relations backbone;
}ReadGrad;

double parameter
{
  display_name "Max Read Gradient";
  format "%f";
  units "%";
  relations backbone;
}ReadGradLim;

double parameter
{
  display_name "2D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase2DGrad;

double parameter
{
  display_name "Max. 2D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase2DGradLim;


double parameter
{
  display_name "3D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase3DGrad;

double parameter
{
  display_name "Max. 3D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase3DGradLim;

double parameter
{
  display_name "Exc. Slice Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ExcSliceGrad;


double parameter
{
  display_name "Max. Exc. Slice Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ExcSliceGradLim;

double parameter
{
  display_name "Exc. Slice Reph. Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ExcSliceRephGrad;

double parameter
{
  display_name "Max. Exc. Slice Reph. Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ExcSliceRephGradLim;

double parameter
{
  display_name "Encoding Duration";
  short_description "Duration of encoding gradient.";
  relations backbone;
  units "ms";
  format "%.3f";
}EncGradDur;


double parameter TeFillDelay; /* placeholder, no relations */
double parameter TrFillDelay;

double parameter
{
  display_name "Echo Repetition Time";
  relations backbone; 
  format "%.2f";
  units "ms";
}EchoRepTime;

double parameter
{
  display_name "Echo Time";
  relations backbone;
  format "%.2f";
  units "ms";
}EchoTime;

double parameter MinEchoRepTime;

double parameter
{
  display_name "Inv_Time_1";
  relations backbone;
  format "%.2f";
  units "ms";
}IT_1;

double parameter
{
  display_name "Inv_Time_2";
  relations backbone;
  format "%.2f";
  units "ms";
}IT_2;

double parameter
{
  display_name "Delay InvToAcq1";
  relations backbone;
  format "%.2f";
  units "ms";
}T_A;

double parameter
{
  display_name "Delay Acq1ToAcq2";
  relations backbone;
  format "%.2f";
  units "ms";
}T_B;

double parameter
{
  display_name "Delay Acq2ToNextInv";
  relations backbone;
  format "%.2f";
  units "ms";
}T_C;

/****************************************************************/
/* TYPE NAMES                                                   */
/****************************************************************/

enum_attributes PREPARATION_TYPE
{
  display_name Inversion_prep  "Inversion";
  display_name MDEFT_prep      "MDEFT";
};

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

