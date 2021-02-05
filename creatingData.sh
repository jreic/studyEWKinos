#script must be run in clean environment!
PATHTOCARDS=< >; CARDSNAME=< >; LIFETIME=< >; NUMEVENTS=< >; 
PLOTOUTPUT=< >;LIFETIMEHISTMAX=< >;
DATANAME=< >; DATAFULLPATH=< >;  
source ~/.bash_profile
myMadgraphSetup #alias, cd and source relevant script from HATS 2020
time ./gridpack_generation.sh $CARDSNAME $PATHTOCARDS local
mySetup # alias, allows cmsRun to work 
myMadgraphSetup # effectively cd
cmsRun PythiaDecayScript.py $CARDSNAME$"_slc7_amd64_gcc820_CMSSW_9_3_16_tarball.tar.xz" $DATANAME $LIFETIME $NUMEVENTS |tee $DATANAME$".txt"
mySetup # effectively cd
cd studyEWKinos
python plot_gen.py $DATAFULLPATH $PLOTOUTPUT $NUMEVENTS $LIFETIMEHISTMAX
