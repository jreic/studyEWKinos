
#N2C1
macroRun -m configs/macros/RunCutflow.json -p configs/plots/Nominal1D_binning.json -c configs/cuts/SR.json -s configs/samples/fourteenthrun/2018/$1.json -o $2.root
macroRun -m configs/macros/MakeCanvases.json -c configs/cuts/SR.json -y 18  -i bkgs_binned_0_35_70.root -i $2.root
macroRun -m configs/macros/SaveCanvases.json -i bkgs_binned_0_35_70.root -i $2.root

mv plot_temp $1_binned
"
#N2N1
macroRun -m configs/macros/RunCutflow.json -p configs/plots/Nominal1D_binning.json -c configs/cuts/SR.json -s configs/samples/fourteenthrun/2018/$3.json -o $4.root
macroRun -m configs/macros/MakeCanvases.json -c configs/cuts/SR.json -y 18 -i bkgs_binned_0_35_70.root -i $4.root
macroRun -m configs/macros/SaveCanvases.json -i bkgs_binned_0_35_70.root -i $4.root
mv plot_temp $3_binned
#N2C1
macroRun -m configs/macros/RunCutflow.json -p configs/plots/Nominal1D.json -c configs/cuts/SR.json -s configs/samples/fourteenthrun/2018/$1 -o $2
macroRun -m configs/macros/MakeCanvases.json -c configs/cuts/SR.json -y 18 -i plots_SR_fourteenthrun_2018_EWKino_bkgs_all.root -i $2
macroRun -m configs/macros/SaveCanvases.json -i plots_SR_fourteenthrun_2018_EWKino_bkgs_all.root -i $2

mv plot_temp plot_temp_N2C1
#N2N1
macroRun -m configs/macros/RunCutflow.json -p configs/plots/Nominal1D.json -c configs/cuts/SR.json -s configs/samples/fourteenthrun/2018/$3 -o $4
macroRun -m configs/macros/MakeCanvases.json -c configs/cuts/SR.json -y 18 -i plots_SR_fourteenthrun_2018_EWKino_bkgs_all.root -i $4
macroRun -m configs/macros/SaveCanvases.json -i plots_SR_fourteenthrun_2018_EWKino_bkgs_all.root -i $4"
"

