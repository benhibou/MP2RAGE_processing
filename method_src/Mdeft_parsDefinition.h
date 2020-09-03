/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/MDEFT/Mdeft_parsDefinition.h,v $
 *
 * Copyright (c) 1999-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: Mdeft_parsDefinition.h,v 1.5 2012/11/27 12:48:01 anba Exp $
 *
 ****************************************************************/



/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/



PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse";
  relations    Mdeft_ExcPulse1EnumRelation;
}Mdeft_ExcPulse1Enum;
 
 
PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  relations    Mdeft_ExcPulse1Relation;
}Mdeft_ExcPulse1;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Exc Pulse Amplitude";
  relations Mdeft_ExcPulse1AmplRel;
}Mdeft_ExcPulse1Ampl;

double parameter
{
  relations backbone;
}Mdeft_ExcPulse1Shape[];

PV_PULSE_LIST parameter
{
  display_name "Inversion Pulse";
  relations    Mdeft_InvPulse1EnumRelation;
}Mdeft_InvPulse1Enum;
 
PVM_RF_PULSE parameter
{
  display_name "Inversion Pulse";
  relations    Mdeft_InvPulse1Relation;
}Mdeft_InvPulse1;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Inv Pulse Amplitude";
  relations Mdeft_InvPulse1AmplRel;
}Mdeft_InvPulse1Ampl;

double parameter
{
  relations backbone;
}Mdeft_InvPulse1Shape[];



/********************************************************************
*                      mdeft preparation
********************************************************************/
 
double parameter Mintir1;
double parameter Mintir2;
double parameter MinInversionTime;

MDEFT_PREPARATION_TYPE parameter
{
 display_name "MDEFT Preparation Mode";
 relations backbone;
}Mdeft_PreparationMode; 

PREPARATION_TYPE parameter
{
 display_name "MDEFT Preparation Mode";
 relations backbone;
}PreparationMode; 
 
YesNo parameter
{
relations backbone;
}Mdeft_PrepHandler;
 
  
double parameter
{
   display_name " Spoiler Strength";
   relations Mdeft_PrepSpoilGradRel;
   format "%.3f";
   editable false;
}Mdeft_PrepSpoilGrad;
 
double parameter Mdeft_PrepFL[];
 
PVM_SPOILER_TYPE parameter 
{
  display_name "Spoiler";
  short_description "Slice Spoiler of MDEFT module."; 
  relations Mdeft_SliceSpoilerRel;
}Mdeft_SliceSpoiler;
 
double parameter
{
   display_name "Module Duration";
   short_description "Duration of MDEFT module.";
   units "ms";
   format "%.3f";
   editable false;
}Mdeft_PrepModuleTime;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


















