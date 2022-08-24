First Run
```
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv
```

To verify Generator-Level Kinematics run:
```
python GenLevel/plot_gen2.py <input string> <output plot directory> <number of events> <ctau> <ctau units> <N2C1 or N2N1>
```

To verify Reconstruction-Level Kinematics run:
```
root -l -b -q "GenToRecoMatchingv2(\"<N2Mass>_<C1Mass>_<N1Mass>\", <ctau>)"
```  
