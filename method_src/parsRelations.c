/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/MDEFT/parsRelations.c,v $
 *
 * Copyright (c) 2002 - 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: parsRelations.c,v 1.25 2012/01/13 10:43:50 sako Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: parsRelations.c,v 1.25 2012/01/13 10:43:50 sako Exp $ (C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


#include "method.h"




/*==============================================================
 * relation of NDummyEchoes
 *==============================================================*/

void deRelations(void)
{
 
  DB_MSG(("-->deRelations\n"));
 
  deRange();
  backbone();
 
  DB_MSG(("<--deRelations\n"));
 
}
 
void deRange(void)
{
  if(ParxRelsParHasValue("NDummyEchoes") == No)
    NDummyEchoes = 0;
  else
    NDummyEchoes = MAX_OF(0, NDummyEchoes);
}

/*===============================================================
 * ExcPulse1EnumRelation
 * Relation of ExcPulse1Enum (a dynamic enmueration parameter which
 * allows to select one of the existing  pulses)
 *===============================================================*/

void ExcPulse1EnumRelation(void)
{
  DB_MSG(("-->ExcPulse1EnumRelation"));

  UT_SetRequest("ExcPulse1Enum");
  backbone();
  
  DB_MSG(("<--ExcPulse1EnumRelation"));                                       
}

/*===============================================================
 * ExcPulse2EnumRelation
 * Relation of ExcPulse1Enum (a dynamic enmueration parameter which
 * allows to select one of the existing  pulses)
 *===============================================================*/

void ExcPulse2EnumRelation(void)
{
  DB_MSG(("-->ExcPulse2EnumRelation"));

  UT_SetRequest("ExcPulse2Enum");
  backbone();
  
  DB_MSG(("<--ExcPulse2EnumRelation"));                                       
}

/*===============================================================
 * ExcPulse1AmplRel
 * Relation of ExcPulseAmpl 
 * This parameter is used in the setup parameter card to adjust
 * the RF pulse amplitude manually
 *===============================================================*/

void ExcPulse1AmplRel(void)
{
  DB_MSG(("-->ExcPulse1AmplRel"));
  UT_SetRequest("ExcPulse1Ampl");
  HandleRFPulseAmplitude();
  DB_MSG(("-->ExcPulse1AmplRel"));
}



/*===============================================================
 * ExcPulse2AmplRel
 * Relation of ExcPulseAmpl 
 * This parameter is used in the setup parameter card to adjust
 * the RF pulse amplitude manually
 *===============================================================*/

void ExcPulse2AmplRel(void)
{
  DB_MSG(("-->ExcPulse2AmplRel"));
  UT_SetRequest("ExcPulse2Ampl");
  HandleRFPulseAmplitude();
  DB_MSG(("-->ExcPulse2AmplRel"));
}

void HandleRFPulseAmplitude(void)
{
  DB_MSG(("-->HandleRFPulseAmplitude"));

  STB_UpdateRFShapeAmplitude("ExcPulse1Ampl",No);
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]");
  STB_UpdateRFShapeAmplitude("ExcPulse2Ampl",No);
  ATB_SetRFPulse("ExcPulse2","ACQ_RfShapes[10]");

  DB_MSG(("<--HandleRFPulseAmplitude"));
}



/* ===================================================================
 * Relation of ExcPulse
 * 
 * All pulses of type PVM_RF_PULSE must have relations like this.
 * However, if you clone this funtion for a different pulse parameter
 * remember to replace the param name in the call to UT_SetRequest!
 *
 * IMPORTANT: this function should not be invoked in the backbone!
 ====================================================================*/

void ExcPulse1Relation(void)
{
  DB_MSG(("-->ExcPulse1Relation"));
  
  /*
   * Tell the request handling system that the parameter
   * ExcPulse has been edited 
   */
  
  UT_SetRequest("ExcPulse1");
  
  /* Check the values of ExcPulse1 */
  
  ExcPulse1Range();
  
  /* 
   * call the backbone; further handling will take place there
   * (by means of STB_UpdateRFPulse)  
   */
  
  backbone();
  
  DB_MSG(("<--ExcPulse1Relation"));
}



void ExcPulse1Range(void)
{
  DB_MSG(("-->ExcPulse1Range"));
  
  // range checker fields to be controlled may be
  // .Length  
  // .Bandwidth
  // .Flipangle
  // .Calculated
  // .Sharpness
  // .Flatness   
 double dval=ExcPulse1.Flipangle;
  
 ExcPulse1.Flipangle = MIN_OF(90.0,MAX_OF(dval,1.0));

 DB_MSG(("<--ExcPulse1Range")); 
}


void ExcPulse2Relation(void)
{
  DB_MSG(("-->ExcPulse2Relation"));
  
  /*
   * Tell the request handling system that the parameter
   * ExcPulse has been edited 
   */
  
  UT_SetRequest("ExcPulse2");
  
  /* Check the values of ExcPulse2 */
  
  ExcPulse2Range();
  
  /* 
   * call the backbone; further handling will take place there
   * (by means of STB_UpdateRFPulse)  
   */
  
  backbone();
  
  DB_MSG(("<--ExcPulse2Relation"));
}



void ExcPulse2Range(void)
{
  DB_MSG(("-->ExcPulse2Range"));
  
  // range checker fields to be controlled may be
  // .Length  
  // .Bandwidth
  // .Flipangle
  // .Calculated
  // .Sharpness
  // .Flatness   
 double dval=ExcPulse2.Flipangle;
  
 ExcPulse2.Flipangle = MIN_OF(90.0,MAX_OF(dval,1.0));

 DB_MSG(("<--ExcPulse2Range")); 
}

/*===============================================================
 *
 * Range checking routine for parameter PVM_NAverages
 *
 *==============================================================*/


void NAveragesRange(void)
{
  int ival;
  DB_MSG(("-->NAveragesRange\n"));
  
  /* 
   *  Range check of PVM_NAverages: prevent it to be negative or 0
   */
  
  if(ParxRelsParHasValue("PVM_NAverages") == No)
  {
    PVM_NAverages = 1;
  }

  ival = PVM_NAverages;
  PVM_NAverages = MAX_OF(ival,1);
  
  
  DB_MSG(("<--AveragesRange\n"));

}



void NAveragesRels(void)
{

  DB_MSG(("-->NAveragesRels\n"));


  NAveragesRange();

  /*
   *   Averages range check is finished, handle the request by
   *   the method:
   */
  
  
  backbone();
  
  
  DB_MSG(("<--NAveragesRels\n"));
  return;
}




/*==================================================
 *     relation to update Echotime from scan editor
 *==================================================*/

void LocalEchoTimeRelation(void)
{
  DB_MSG(("-->LocalEchoTimeRelation\n"));
  EchoRepTime = PVM_EchoTime;
  backbone();

  DB_MSG(("<--LocalEchoTimeRelation\n"));
}

void SegmNumberRelations(void)
{
  SegmNumberRange();
  backbone();
}

void SegmNumberRange(void)
{
  if( ParxRelsParHasValue("SegmNumber") == No)
    SegmNumber = 1;
  if(SegmNumber < 1) SegmNumber =1;
}


/* rangechecking and redirected relations of PVM_EffSWh */

void EffSWhRange(void)
{
  DB_MSG(("-->EffSWhRange"));

  if(!ParxRelsParHasValue("PVM_EffSWh"))
  {
    PVM_EffSWh = 50000;
  }
  else
  {
    PVM_EffSWh = MIN_OF(MAX_OF(PVM_EffSWh,2000.0),1000000);
  }

  DB_MSG(("-->EffSWhRange"));
  return;
}

void EffSWhRel(void)
{
  DB_MSG(("-->EffSWhRel"));

  EffSWhRange();
  backbone();

  DB_MSG(("-->EffSWhRel"));
  return;
}


/* relations of PVM_EchoPosition */
void EchoPositionRels(void)
{
  EchoPositionRange();
  backbone();
}

void EchoPositionRange(void)
{
  if(!ParxRelsParHasValue("PVM_EchoPosition"))
    PVM_EchoPosition = 50;
  else
  {
    PVM_EchoPosition = MAX_OF(PVM_EchoPosition, 0);
    PVM_EchoPosition = MIN_OF(PVM_EchoPosition, 100);
  }
}

/* relations of read/slice spoiler */
void ReadSpoilerRel(void)
{
  DB_MSG(("-->ReadSpoilerRel"));
  UT_SetRequest("ReadSpoiler");
  backbone();
  DB_MSG(("<--ReadSpoilerRel"));
}

void SliceSpoilerRel(void)
{
  DB_MSG(("-->SliceSpoilerRel"));
  UT_SetRequest("SliceSpoiler");
  backbone();
  DB_MSG(("<--SliceSpoilerRel"));
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/








