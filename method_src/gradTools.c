/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/MDEFT/gradTools.c,v $
 *
 * Copyright (c) 2005
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: gradTools.c,v 1.1 2005/05/23 12:11:28 mawi Exp $
 *
 ****************************************************************/
static const char resid[] = "$Id: gradTools.c,v 1.1 2005/05/23 12:11:28 mawi Exp $(C) 2005 Bruker BioSpin MRI GmbH";

#include "method.h"

/* ----------------------------------------------------------------
   set_refoc: Setting a refocused pulse for readout or slice exc.
   
   Calculates the refocusing amplitude to get a zero pulse
   integral at the end of echo_del. The refocusing delay
   may be increased, if it is smaller than max_of(ramp_time,min_del)
   or if the corresponding refoc_amp surpasses max_amp.  
   ONLY for constant ramp times.
 
   refoc_del  - at input, wished duration of refocusing delay
                (including both ramps + plateau),
                at output, effective value allowed by max_amp,
   refoc_amp  - at output, calculated refocusing amplitude
   echo_del   - delay from the onset of readout plateau to echo centre
   echo_amp   - amplitude of readout plateau
   ramp_time  - rise time (d4)
   ramp_int   - ramp inegral, assuming ramp_time=1 and amplitude=100
                (ramp_int=50 for linear ramps)
   max_amp    - max amplitude allowed for refoc_amp (absolute value)
   min_del    - min duration allowed for refoc_del
   return value : 0 if refoc_del unchanged
                  1 if refoc_del changed
   
   error conditions : max_amp==0 || ramp_time <= 0 || echo_del<0
   --------------------------------------------------------------- */
 int set_refoc(double *refoc_del, double *refoc_amp,
               double echo_del,   double echo_amp,
	       double ramp_time,  double ramp_int, 
	       double max_amp,    double min_del)
{
  int ret=0;
  double d_min, ramp;
 
         ramp=ramp_time*ramp_int/100;
       
  
  if( max_amp==0 || ramp_time <= 0 || echo_del<0)
  {
    /* illegal value passed to set_refoc() */
    return (0);
  }
  
  
  d_min = fabs(echo_amp * (ramp + echo_del) / max_amp) + ramp_time;
                             
  if(d_min < MAX_OF(2*ramp_time, min_del))
    d_min = MAX_OF(2*ramp_time, min_del);
    
  if(*refoc_del < d_min)
  {
    *refoc_del = d_min;
    ret = 1;
  } 
  
  /*
    printf("echo_amp = %f\n",echo_amp);
    printf("ramp = %f\n",ramp);
    printf("echo_del = %f\n",echo_del);
    printf("refoc_del = %f\n",*refoc_del);
    printf("ramp_time = %f\n\n",ramp_time);
  */  
 
  *refoc_amp = -echo_amp * (ramp + echo_del) / (*refoc_del-ramp_time);

  /*
    printf("refoc_amp = %f\n\n\n",*refoc_amp);
  */

  return ret;
} /* ------- end of set_refoc() */
  
  
/* ---------------------------------------------------------------
   set_pulse: setting a phase encoding pulse
   
   Calculates the pulse amplitude for a given duration, and, 
   if necessary, increases the duration. The calculated amplitude
   corresponds to the highest phase-encoding step and can be used
   as the argument of r2d or r3d. Pulse duration will be increased 
   if its initial value is less than 2*ramp_time, or if the corresponding
   amplitude surpasses max_amp.
   
   del       - pulse duration (both ramps + plateau), at input - the wished
               value, at output - the effective value allowed
   amp       - at output, the calculated amplitude (trim). 
               A positive value is always given; the sign must be defined
	       separately.
   resol     - image resolution 
   ramp_time - gradient rise time
   max_amp   - max value allowed for amp
   grad_const- frequency gradient strength for amp=100. 
	       
   return value: 0 if del remained unchanged, 
                 1 if del increased
               
   
   About units:   
   grad_const must be supplied in units equal to 1/(resol_unit*del_unit).	       
   This means that if resol is in cm and del in sec, grad_const should be taken
   as PREEMP_grad_cal_const, provided the gradients are no_scale.
   If resol is in mm and del in ms, one should supply
   grad_const = 1e-4 * PREEMP_grad_cal_const 
              = 1e-3 * PVM_GradCalConst

   Error conditions: max_amp == 0 || resol==0 || grad_const == 0
   -------------------------------------------------------------- */
int set_pulse(double *del, double *amp, double resol,double ramp_time, 
          double max_amp,  double grad_const)
{
  int ret=0;
  double del_min;
  
  if(max_amp == 0 || resol==0 || grad_const == 0)
  {
    /* illegal value passed to set_refoc() */
    return (0);
  }
  del_min = 100.0/fabs(max_amp*2*resol*grad_const) + ramp_time;
  del_min = MAX_OF(del_min, 2*ramp_time);
  if(*del < del_min) 
  {
    ret = 1;
    *del = del_min;
  }
  else
    ret = 0;
    
  *amp = 100.0/((*del-ramp_time) * 2 * resol * grad_const);
  return ret;

}
