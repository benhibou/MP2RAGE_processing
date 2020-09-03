# MP2RAGE_processing
MP2RAGE processing for data collected with Bruker MRI instrument (with Paravision)
MP2RAGE is a MRI experiment originally developped by J.P. Marques:

J.P. Marques et Al., https://doi.org/10.1016/j.neuroimage.2009.10.002
J.P. Marques and R. Gruetter,  https://doi.org/10.1371/journal.pone.0069294

I developped a Bruker method to run with Paravision 6 and the associated software
written as a mixture of C program and python scripts to analyze the data. The processing
software requires only to have access to the "method" file of the expreriment and the
2dseq file obtained from a complex Fourier transform of the aquired fid data.

```diff
- Both the Paravision method and the processing software is to be used at the user own risk. 
- I do not take any responsibility whatsoever for the usage of both the method and the processing software.
```
