#macroRun -m configs/macros/RunCutflow.json -p configs/plots/Nominal2D.json -c configs/cuts/SR.json -s configs/samples/fourteenthrun/2018/bkgs_all.json -s configs/samples/fourteenthrun/2018/signal_EWKino.json -o plots_SR_fourteenthrun_2018_EWKino_200_190_180_10mm.root


#macroRun -m configs/macros/RunCutflow.json -p configs/plots/Nominal2D.json -c configs/cuts/SR.json -s configs/samples/fourteenthrun/2018/signal_EWKino_1mm.json -o plots_SR_fourteenthrun_2018_EWKino_200_190_180_1mm.root
#macroRun -m configs/macros/RunCutflow.json -p configs/plots/Nominal2D.json -c configs/cuts/SR.json -s configs/samples/fourteenthrun/2018/signal_EWKino_10mm.json -o plots_SR_fourteenthrun_2018_EWKino_200_190_180_10mm.root


macroRun -m configs/macros/RunCutflow.json -p configs/plots/Nominal1D.json -c configs/cuts/SR.json -s configs/samples/fourteenthrun/2018/bkgs_all.json -s configs/samples/fourteenthrun/2018/signal_EWKino.json -o plots_SR_fourteenthrun_2018_EWKino_200_190_180.root
