# studyEWKinos

Usage w/ our example files:

`python plot_gen.py inputFiles=/nfs/cms/mc1/jpr255/EWKino/hats_2020_gen/data/GEN4HATS/DYToLL_M-50_13TeV_pythia8_cff_py_GEN.root maxEvents=1000`

or

`python plot_gen.py inputFiles=/nfs/cms/mc1/jpr255/EWKino/hats_2020_gen/data/GENHATS2020/RunIIAutumn18MiniAOD/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/0038605E-C94B-574F-AF1F-000435E9A26E.root maxEvents=1000`

We can also remove `maxEvents=1000` (or set it to `maxEvents=-1`) to process all events, or change the number to any other number of events (e.g. `maxEvents=10000`). For most simple checks, 1000 is probably sufficient though.
