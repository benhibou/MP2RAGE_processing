/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/MDEFT/parsLayout.h,v $
 *
 * Copyright (c) 1999-2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: parsLayout.h,v 1.19 2013/05/22 11:05:07 sako Exp $
 *
 ****************************************************************/

/****************************************************************/
/*	PARAMETER CLASSES				       	*/
/****************************************************************/


/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

parclass
{
  PVM_EffSWh;
  SegmNumber;
  PVM_EchoPosition;
  PVM_MinEchoTime;
  EncGradDur;
  PVM_AcquisitionTime;
  ReadSpoiler;
  SliceSpoiler;
  DigitizerPars;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;

parclass
{
  ExcPulse1Enum;
  ExcPulse1;
  ExcPulse1Ampl;
  ExcPulse2Enum;
  ExcPulse2;
  ExcPulse2Ampl;
} 
attributes
{
  display_name "RF Pulses";
} RF_Pulses;

parclass
{
  Mdeft_ExcPulse1Enum;
  Mdeft_ExcPulse1;
  Mdeft_ExcPulse1Ampl;
  Mdeft_InvPulse1Enum;
  Mdeft_InvPulse1;
  Mdeft_InvPulse1Ampl;
  PVM_InversionTime;
  Mdeft_SliceSpoiler;
  Mdeft_PrepSpoilGrad;
  Mdeft_PrepModuleTime;
} 
attributes
{
  display_name "MDEFT Preparation Parameters";
} Mdeft_Preparation_Parameters;

parclass
{
  NDummyEchoes;
  DummyScans_Parameters;
  PVM_FatSupOnOff;
  Fat_Sup_Parameters;

  PVM_FovSatOnOff;
  Fov_Sat_Parameters;

  PVM_TriggerModule;
  Trigger_Parameters;

  RFSpoiling;

} Preparation;

parclass
{
  Method;
  EchoTime;
  EchoRepTime;
  SegmNumber;
  SegmRepTime;
  SegmDuration;
  SecondImageDelay;
  T_A;
  T_B;
  T_C;
  IT_1;
  IT_2;
  PVM_RepetitionTime;
  PVM_NEchoImages;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RF_Pulses;
  Nuclei;
  Encoding;
  PreparationMode;
  Mdeft_PreparationMode;
  Mdeft_Preparation_Parameters;
  Sequence_Details;
  ImageGeometry;
  Preparation;
  MapShim;
} MethodClass;

// parameters that should be tested after any editing
conflicts
{
  EchoTime;
  EchoRepTime;
  SegmRepTime;
  PVM_Fov;
  PVM_SliceThick;
};

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



