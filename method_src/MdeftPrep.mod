;****************************************************************
;
; Copyright (c) 2002-2003
; Bruker BioSpin MRI GmbH
; D-76275 Ettlingen, Germany
;
; All Rights Reserved
;
; MdeftPrep.mod : MDEFT Preparation Module 
;
;****************************************************************
;   


if(PreparationMode == Inversion_prep)
{
	10u     mdeftlist:f1
        d0   
	d8	gatepulse 1
	(p2:sp2	ph0):f1
        d22     grad_ramp{0, 0, Mdeft_PrepSpoilGrad}
        d12	grad_off
}

if(PreparationMode == MDEFT_prep) 
{
        10u     mdeftlist:f1 
        d8	gatepulse 1
        (p1:sp1  ph0):f1
        d0
        d8      gatepulse 1
        (p2:sp2  ph0):f1
        d22     grad_ramp{0, 0, Mdeft_PrepSpoilGrad}
        d12     grad_off
}
 





