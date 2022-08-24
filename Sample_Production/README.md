Many of the file paths will have to replaced wherever the code is being run.

Higgsino Samples:
In a tmux in lxplus, run the following:
```
k5reauth -x -f pagsh
```
```
aklog
```
```
source /cvmfs/cms.cern.ch/cmsset_default.sh
```
The following line of code will create Higgsino nTuples. The card names should be of the form <N2Mass>_<C1Mass>_<N1Mass>...:
```
python makeSamples_Normalization_Higgsino.py <N2C1 card path> <N2C1 card name> <N2C1 ISR inclusive card path> <N2C1 ISR inclusive card name> <N2N1 card path> <N2N1 Card Name> <N2N1 ISR inclusive card path> <N2N1 ISR inclusive card name> <Number of Jobs> <N2C1+ Cross Section> <N2C1- Cross Section> <N2N1 Cross Section> <N2C1 Output Directory> <N2C1 ISR Inclusive Output Directory> <N2N1 ISR Output Directory> <N2N1 ISR Inclusive Output Directory> |& tee <logfile>
```

The following line of code will create the WinoBino nTuple. The card names should be of the form <N2Mass>_<C1Mass>...:
```
python makeSamples_Normalization_WinoBino.py <N2 Mass> <card path> <card path ISR inclusive> <Number of Jobs> <N2C1+ Cross Section> <N2C1- Cross Section> <first mass splitting> <second mass splitting> ...| tee <logfile> 
```
