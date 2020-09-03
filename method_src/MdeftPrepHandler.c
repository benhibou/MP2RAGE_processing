
/* ***************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/MDEFT/MdeftPrepHandler.c,v $
 *
 * Copyright (c) 2013
 * Bruker Biospin MRI GMBH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: MdeftPrepHandler.c,v 1.14 2013/11/28 15:46:38 sako Exp $
 *
 * ***************************************************************/

static const char resid[] = "$Id: MdeftPrepHandler.c,v 1.14 2013/11/28 15:46:38 sako Exp $ (C) 2001 BRUKER MEDICAL GMBH";

#define DEBUG           0
#define DB_MODULE       1
#define DB_LINE_NR      1


#include "method.h"


void Mdeft_PrepSpoilGradRel( void )
{
  ParxRelsParRelations("Mdeft_PrepHandler",No);
}

/* relations of slice spoiler */
void Mdeft_SliceSpoilerRel(void)
{
  DB_MSG(("-->Mdeft_SliceSpoilerRel"));

  UT_SetRequest("Mdeft_SliceSpoiler");

  /* call the method handler */
  ParxRelsParRelations("Mdeft_PrepHandler",No);

  DB_MSG(("<--Mdeft_SliceSpoilerRel"));
}


/* relations of Mdeft module RF pulses */

void Mdeft_ExcPulse1EnumRelation(void)
{

  DB_MSG(("-->Mdeft_ExcPulse1EnumRelation\n"));

  UT_SetRequest("Mdeft_ExcPulse1Enum");

  /* call the method handler */
  ParxRelsParRelations("Mdeft_PrepHandler",No);

  DB_MSG(("<--Mdeft_ExcPulse1EnumRelation\n"));
}

void Mdeft_ExcPulse1AmplRel(void)
{
  DB_MSG(("-->Mdeft_ExcPulse1AmplRel"));
  UT_SetRequest("Mdeft_ExcPulse1Ampl");
  HandleMdeftExcPulseAmplitudes();
  DB_MSG(("<--Mdeft_ExcPulse1AmplRel"));
}

void HandleMdeftExcPulseAmplitudes(void)
{
  DB_MSG(("-->HandleMdeftExcPulseAmplitudes"));

  STB_UpdateRFShapeAmplitude("Mdeft_ExcPulse1Ampl",Yes);
  ATB_SetRFPulse("Mdeft_ExcPulse1","ACQ_RfShapes[1]");

  DB_MSG(("<--HandleMdeftExcPulseAmplitudes"));
}

void Mdeft_ExcPulse1Range(void)
{
  DB_MSG(("-->Mdeft_ExcPulse1Range\n"));

  // fields to be controlled may be
  // .Length  
  // .Bandwidth
  // .Flipangle
  // .Calculated
  // .Sharpness
  // .Flatness   
  double dval=Mdeft_ExcPulse1.Flipangle;
  
  Mdeft_ExcPulse1.Flipangle = MIN_OF(180.0,MAX_OF(dval,1.0));


  DB_MSG(("<--Mdeft_ExcPulse1Range\n"));
}


void Mdeft_ExcPulse1Relation(void)
{
  DB_MSG(("-->Mdeft_ExcPulseRelation\n"));

  UT_SetRequest("Mdeft_ExcPulse1");

  Mdeft_ExcPulse1Range();

  /* call the method handler */
  ParxRelsParRelations("Mdeft_PrepHandler",No);

  DB_MSG(("-->Mdeft_ExcPulse1Relation\n"));
}

void Mdeft_InvPulse1EnumRelation(void)
{

  DB_MSG(("-->Mdeft_InvPulseEnumRelation\n"));


  UT_SetRequest("Mdeft_InvPulse1Enum");

  /* call the method handler */
  ParxRelsParRelations("Mdeft_PrepHandler",No);


  DB_MSG(("<--Mdeft_InvPulseEnumRelation\n"));
}

void Mdeft_InvPulse1AmplRel(void)
{
  DB_MSG(("-->Mdeft_InvPulse1AmplRel"));
  UT_SetRequest("Mdeft_InvPulse1Ampl");
  HandleMdeftInvPulseAmplitudes();
  DB_MSG(("<--Mdeft_InvPulse1AmplRel"));
}

void HandleMdeftInvPulseAmplitudes(void)
{
  DB_MSG(("-->HandleMdeftInvPulseAmplitudes"));

  STB_UpdateRFShapeAmplitude("Mdeft_InvPulse1Ampl",Yes);
  ATB_SetRFPulse("Mdeft_InvPulse1","ACQ_RfShapes[2]");

  DB_MSG(("<--HandleMdeftInvPulseAmplitudes"));
}

void Mdeft_InvPulse1Range(void)
{
  DB_MSG(("-->Mdeft_InvPulse1Range\n"));

  double dval=Mdeft_InvPulse1.Flipangle;
  
  Mdeft_InvPulse1.Flipangle = MIN_OF(270,MAX_OF(dval,1));


  DB_MSG(("<--Mdeft_InvPulse1Range\n"));

}

void Mdeft_InvPulse1Relation(void)
{
  DB_MSG(("-->Mdeft_InvPulse1Relation\n"));


  UT_SetRequest("Mdeft_InvPulse1");


  Mdeft_InvPulse1Range();

  /* call the method handler */
  ParxRelsParRelations("Mdeft_PrepHandler",No);

  DB_MSG(("-->Mdeft_InvPulse1Relation\n"));
}




/*-------------------------------------------------------------*/
/*   Initialize the medeft preparation Module                  */
/*-------------------------------------------------------------*/

void InitMdeftPrepModule( void )
{
  DB_MSG(("Entering InitMdeftPrepModule()"));

  /* the standard imaging callback must be disabled during this */
  /* secondary relation to prevent endless recursion */

  ParxRelsResetPar("Mdeft_PreparationMode");
  
  ParxRelsHideInFile("Mdeft_PreparationMode");
  ParxRelsHideInEditor("Mdeft_PreparationMode");

  if(ParxRelsParHasValue("PreparationMode")==No)
     PreparationMode = Inversion_prep;
  if(ParxRelsParHasValue("PVM_InversionTime")==0) 
     PVM_InversionTime = 700.0;

  
  /* Initialisation of rf pulse parameters */
  
  STB_InitRFPulse("Mdeft_ExcPulse1",      // name of pulse structure
                  "Mdeft_ExcPulse1Enum",  // name of pulse list parameter
                  "Mdeft_ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "Mdeft_ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                      // used for excitation
                  "sech.exc",             // default shape
                  5000.0,                 // default bandwidth
                  90.0);                  // default pulse angle

  // check valid range for this specific pulse see parsRelations.c
  Mdeft_ExcPulse1Range();

  STB_InitRFPulse("Mdeft_InvPulse1",      // name of pulse structure
                  "Mdeft_InvPulse1Enum",  // name of pulse list parameter
                  "Mdeft_InvPulse1Ampl",  // name of pulse amplitude parameter
                  "Mdeft_InvPulse1Shape", // name of pulse shape (for calc. pulses)
                  2,                      // used for inversion
                  "Calculated",           // default shape
                  5000.0,                 // default bandwidth
                  180.0);                 // default pulse angle

  Mdeft_InvPulse1Range();

  /* Initialisation of spoiler */
  MRT_InitSpoiler("Mdeft_SliceSpoiler");


  DB_MSG(("Exiting InitMdeftPrepModule"));

}

/*------------------------------------------------------------------*/
/*   Mdeft Preparation Pulses  Handler                              */
/*------------------------------------------------------------------*/


void UpdateMdeftPrepModule(double fixedTime, YesNo deriveGains, double spoilResol)

{
  
  DB_MSG(("Entering UpdateMdeftPrepModule"));
   

  double amplifierenable = CFG_AmplifierEnable();
  double riseTime = CFG_GradientRiseTime();
 
  /* update slice spoiler */
  double mindur =  3.0*CFG_GradientRiseTime();
  double gradlim = 80.0;
  MRT_UpdateSpoiler("Mdeft_SliceSpoiler",5.0,gradlim,mindur,PVM_GradCalConst,spoilResol);
  Mdeft_PrepSpoilGrad =  Mdeft_SliceSpoiler.ampl;
  
  /* update RF pulses */
  STB_UpdateRFPulse("Mdeft_ExcPulse1",1,deriveGains,Conventional);
  STB_UpdateRFPulse("Mdeft_InvPulse1",1,deriveGains,AfpUp);   

  if(deriveGains == Yes)
  {
    ParxRelsHideInEditor("Mdeft_ExcPulse1Ampl,Mdeft_InvPulse1Ampl");
  }
  else
  {
    ParxRelsShowInEditor("Mdeft_ExcPulse1Ampl,Mdeft_InvPulse1Ampl");
  }

  /* update timing */
  if(PreparationMode == Inversion_prep)
  {
    ParxRelsHideInEditor("Mdeft_ExcPulse1,Mdeft_ExcPulse1Enum");

    MinInversionTime = fixedTime + Mdeft_InvPulse1.Length/2 + Mdeft_SliceSpoiler.dur;

    PVM_InversionTime = MAX_OF(MinInversionTime,PVM_InversionTime); 

    Mdeft_PrepModuleTime = PVM_InversionTime         +
                          (Mdeft_InvPulse1.Length/2) +
                           amplifierenable           +
                           0.01                      -
                           fixedTime;
  }
  else
  {

    ParxRelsShowInEditor("Mdeft_ExcPulse1,Mdeft_ExcPulse1Enum");

    Mintir1 = Mdeft_InvPulse1.Length/2 + Mdeft_ExcPulse1.Length/2 +  amplifierenable ;
    Mintir2 = Mdeft_InvPulse1.Length/2 + Mdeft_SliceSpoiler.dur + fixedTime;
    MinInversionTime = MAX_OF(Mintir1,Mintir2);
    PVM_InversionTime = MAX_OF(MinInversionTime,PVM_InversionTime);
    Mdeft_PrepModuleTime = amplifierenable            +
                           0.01                       +
                           (Mdeft_ExcPulse1.Length/2) +
                           (2.0 * PVM_InversionTime)  -
                           fixedTime; 

  }
  
                     
  ParxRelsMakeNonEditable( "T_A" );
  T_A = PVM_InversionTime - MinInversionTime + riseTime +
        Mdeft_InvPulse1.Length/2 + Mdeft_SliceSpoiler.dur;
  ParxRelsMakeNonEditable( "T_B" );
  T_B = SecondImageDelay;
  ParxRelsMakeNonEditable( "T_C" );
  T_C = SegmRepTime - ( T_A + T_B + 2 * SegmDuration );
  ParxRelsMakeNonEditable( "IT_1" );
  IT_1 = PVM_InversionTime;
  ParxRelsMakeNonEditable( "IT_2" );
  /*IT_2 = T_A + T_B + SegmDuration + fixedTime / 2;*/
  IT_2 = IT_1 + T_B + SegmDuration;
  PARX_change_dims("Mdeft_PrepFL",1);
  Mdeft_PrepFL[0] = 0.0;

  DB_MSG(("Exiting UpdateMdeftPrepModule"));

}







