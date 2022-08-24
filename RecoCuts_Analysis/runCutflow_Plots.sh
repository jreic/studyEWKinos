
#N2C1
macroRun -m configs/macros/RunCutflow.json -p configs/plots/Nominal1D_binning.json -c configs/cuts/SR.json -s configs/samples/fourteenthrun/2018/$1.json -o $2.root
macroRun -m configs/macros/MakeCanvases.json -c configs/cuts/SR.json -y 18  -i bkgs_binned_0_35_70.root -i $2.root
macroRun -m configs/macros/SaveCanvases.json -i bkgs_binned_0_35_70.root -i $2.root

mv plot_temp $1_binned

