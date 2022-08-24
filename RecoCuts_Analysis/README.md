The majority of this code is taken from CMS/s IDM analysis group, and can be used to apply reconstruction-level cuts with plots.
Before using root, run:
```
source setup.sh
```

To compile directory, run:
```
cd build
cmake ..
make -j8
make install
```
To apply reconstruction-level cutflows and plot the output after each cut, run:
```
source runCutflow_Plots.sh <json configuration file name (no extension)> <output file/directory named (no extension)>
```

To determine the number of events in the signal region, run:
```
source getIntegralValues_Higgsino.sh
```
or the corresponding file for the Wino/Bino scenrio or to find the total number of events that pass cuts.

To plot the events in the signal region, total events that pass cuts, or reco-level filter efficiency, run the line of code below. Note that the values data is hard-coded into the script, and will have tobe modified accordingly.
```
root -l -b -q "plotSignalEvents_<Higgsino/WinoBino>(\"<quantity to overlay for plots (enter lifetime or mass)>\", \"<output directory>\")" 
