#macroRun -m configs/macros/RunCutflow.json -p configs/plots/Nominal2D.json -c configs/cuts/SR.json -s configs/samples/fourteenthrun/2018/bkgs_all.json -s configs/samples/fourteenthrun/2018/signal_EWKino.json -o plots_SR_fourteenthrun_2018_EWKino_200_190_180_10mm.root
#macroRun -m configs/macros/RunCutflow.json -p configs/plots/Nominal2D.json -c configs/cuts/SR.json -s configs/samples/fourteenthrun/2018/signal_EWKino.json -o plots_SR_fourteenthrun_2018_EWKino_200_190_180.root

macroRun -m configs/macros/MakeCanvases.json -c configs/cuts/SR.json -y 18 -i plots_SR_fourteenthrun_2018_EWKino_200_190_180.root
macroRun -m configs/macros/SaveCanvases.json -i plots_SR_fourteenthrun_2018_EWKino_200_190_180.root
