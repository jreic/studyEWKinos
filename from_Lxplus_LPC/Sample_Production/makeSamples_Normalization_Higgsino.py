import re
import time
import subprocess
import sys,os
import pickle
import ROOT
from ROOT import TFile, TTree
pathToCardsISR = sys.argv[1]; cardsNameISR=sys.argv[2];
pathToCardsNoISR = sys.argv[3]; cardsNameNoISR = sys.argv[4];
pathToCardsN2N1ISR = sys.argv[5]; cardsNameN2N1ISR = sys.argv[6];
pathToCardsN2N1NoISR = sys.argv[7]; cardsNameN2N1NoISR = sys.argv[8];
numJobs = sys.argv[9]; sigmaTheory = sys.argv[10]; sigmaTheory2 = sys.argv[11]; sigmaTheoryN2N1 = sys.argv[12]
outputDirectoriesISR = sys.argv[13]; outputDirectoriesNoISR = sys.argv[14]
outputDirectoriesN2N1ISR = sys.argv[15]; outputDirectoriesN2N1NoISR = sys.argv[16]
ISRTarball = cardsNameISR + "_slc7_amd64_gcc820_CMSSW_9_3_16_tarball.tar.xz"
NoISRTarball = cardsNameNoISR + "_slc7_amd64_gcc820_CMSSW_9_3_16_tarball.tar.xz"
N2N1ISRTarball = cardsNameN2N1ISR + "_slc7_amd64_gcc820_CMSSW_9_3_16_tarball.tar.xz"
N2N1NoISRTarball = cardsNameN2N1NoISR + "_slc7_amd64_gcc820_CMSSW_9_3_16_tarball.tar.xz"
Z_MuMu = 0.0366
lifetimeList = ['1', '10', '100', '1000']
print(pathToCardsN2N1NoISR)
sys.stdout.flush()
print(cardsNameN2N1NoISR)
sys.stdout.flush()
#make ISR and ISRinclusive gridpacks - note that throughout this code, noISR is synoymous to ISRInclusive
os.system("export PYTHONPATH=/usr/lib64/python2.7/site-packages")
os.system("time ./submit_condor_gridpack_generation.sh {cardsNameISR} {pathToCardsISR}".format(cardsNameISR = cardsNameISR, pathToCardsISR = pathToCardsISR))
print("made ISR tarball")
sys.stdout.flush()
os.system("export PYTHONPATH=/usr/lib64/python2.7/site-packages")
os.system("time ./submit_condor_gridpack_generation.sh {cardsNameNoISR} {pathToCardsNoISR}".format(cardsNameNoISR = cardsNameNoISR, pathToCardsNoISR = pathToCardsNoISR))
print("made noISR tarball")
sys.stdout.flush()
os.system("export PYTHONPATH=/usr/lib64/python2.7/site-packages")
os.system("time ./submit_condor_gridpack_generation.sh {cardsNameN2N1ISR} {pathToCardsN2N1ISR}".format(cardsNameN2N1ISR = cardsNameN2N1ISR, pathToCardsN2N1ISR = pathToCardsN2N1ISR))
print("made ISR N2N1 tarball")
sys.stdout.flush()
os.system("export PYTHONPATH=/usr/lib64/python2.7/site-packages")
os.system("time ./submit_condor_gridpack_generation.sh {cardsNameN2N1NoISR} {pathToCardsN2N1NoISR}".format(cardsNameN2N1NoISR = cardsNameN2N1NoISR, pathToCardsN2N1NoISR = pathToCardsN2N1NoISR))
print("made noISR N2N1 tarball")
sys.stdout.flush()
ISRCrossSectionMad = 1; NoISRCrossSectionMad = 1; METCrossSection = 1;
os.system("condor_rm -all")
time.sleep(60)
os.system("cp {ISRTarball} /afs/cern.ch/user/a/alalbert/lxplus_work/iDM-analysis-AODproducer/condor/gridpacks/Production".format(ISRTarball = ISRTarball))
os.system("cp {NoISRTarball} /afs/cern.ch/user/a/alalbert/lxplus_work/iDM-analysis-AODproducer/condor/gridpacks/Production".format(NoISRTarball = NoISRTarball))
os.system("cp {N2N1ISRTarball} /afs/cern.ch/user/a/alalbert/lxplus_work/iDM-analysis-AODproducer/condor/gridpacks/Production".format(N2N1ISRTarball = N2N1ISRTarball))
os.system("cp {N2N1NoISRTarball} /afs/cern.ch/user/a/alalbert/lxplus_work/iDM-analysis-AODproducer/condor/gridpacks/Production".format(N2N1NoISRTarball = N2N1NoISRTarball))
print("copied gridpacks")
sys.stdout.flush()
os.chdir("/afs/cern.ch/user/a/alalbert/lxplus_work/iDM-analysis-AODproducer/condor")

#make noISR events in Pythia
print("about to submit noISR jobs")
sys.stdout.flush()
os.system('./submitPileupLxplus.py {NoISRTarball} 2018 {numJobs} {outputDirectories}| tee "getJobIDnoISR.txt"'.format(NoISRTarball = NoISRTarball, numJobs = numJobs, outputDirectories = outputDirectoriesNoISR))
print("submitted noISR jobs")
sys.stdout.flush()


#make ISR events in Pythia
print("about to submit ISR jobs")
sys.stdout.flush()
os.system('./submitPileupLxplus.py {ISRTarball} 2018 {numJobs} {outputDirectories} | tee "getJobIDISR.txt"'.format(ISRTarball = ISRTarball, numJobs = numJobs, outputDirectories = outputDirectoriesISR))
print("submitted ISR jobs")
sys.stdout.flush()

#make N2N1 noISR events in Pythia
print("about to submit N2N1 noISR jobs")
sys.stdout.flush()
os.system('./submitPileupLxplus.py {N2N1NoISRTarball} 2018 {numJobs} {outputDirectories}| tee "getJobIDnoISR.txt"'.format(N2N1NoISRTarball = N2N1NoISRTarball, numJobs = numJobs, outputDirectories = outputDirectoriesN2N1NoISR))
print("submitted N2N1 noISR jobs")
sys.stdout.flush()


#make ISR events in Pythia
print("about to submit N2N1 ISR jobs")
sys.stdout.flush()
os.system('./submitPileupLxplus.py {N2N1ISRTarball} 2018 {numJobs} {outputDirectories} | tee "getJobIDISR.txt"'.format(N2N1ISRTarball = N2N1ISRTarball, numJobs = numJobs, outputDirectories = outputDirectoriesN2N1ISR))
print("submitted N2N1 ISR jobs")
sys.stdout.flush()
print("checking periodically if jobs are done")
sys.stdout.flush()
jobStatus=-1
while True:
        os.system("condor_q | tee \"condorQLog.txt\"")
        time.sleep(1)
        #with open("condorQLog.txt", 'r') as h:
        h = open("condorQLog.txt", "r")
        lines = h.readlines()
        for line in lines:
                index = line.find("Total for alalbert")
                if index != -1:
                        substring = line[index + len("Total for alalbert")+2: line.find("jobs")]
                        substring.strip()
                        print("Jobs Remaining: ", substring)
                        sys.stdout.flush()
                        substring.strip()
                        jobStatus = int(substring)
                        break
        h.close()
        if jobStatus == 0:
            break
        time.sleep(300)

os.chdir("/afs/cern.ch/user/a/alalbert/lxplus_work/iDM-analysis-AODproducer/condor")
#get Cross Section info for ISRInclusive events
print("calling helper script noISR")
sys.stdout.flush()
os.system('python getCrossSectionInfo.py noISR {outputDirectories}'.format(outputDirectories=outputDirectoriesNoISR))

print("calling helper script ISR")
os.system('python getCrossSectionInfo.py ISR {outputDirectories}'.format(outputDirectories=outputDirectoriesISR))

#load in cross section info stored in directory
with open("Logs/ISRInfo"+".obj", 'r') as ISR:
        print("test")
        ISRTuple = pickle.load(ISR)

with open("Logs/noISRInfo"+".obj", 'r') as noISR:
        print("test2")
        noISRTuple = pickle.load(noISR)

print("maxMean\t", "maxSTD\t", "pythiaMean\t", "pythiaSTD")
sys.stdout.flush()
print("ISRTuple", ISRTuple)
sys.stdout.flush()
print("noISRTuple", noISRTuple)
sys.stdout.flush()

MadgraphEff = ISRTuple[0]/noISRTuple[0]
print("calling helper script N2N1 noISR")
sys.stdout.flush()
os.system('python getCrossSectionInfo.py noISR {outputDirectories}'.format(outputDirectories=outputDirectoriesN2N1NoISR))

print("calling helper script N2N1 ISR")
os.system('python getCrossSectionInfo.py ISR {outputDirectories}'.format(outputDirectories=outputDirectoriesN2N1ISR))

#load in cross section info stored in directory
with open("Logs/ISRInfo"+".obj", 'r') as ISR:
        print("test")
        N2N1ISRTuple = pickle.load(ISR)

with open("Logs/noISRInfo"+".obj", 'r') as noISR:
        print("test2")
        N2N1noISRTuple = pickle.load(noISR)

print("maxMean\t", "maxSTD\t", "pythiaMean\t", "pythiaSTD")
sys.stdout.flush()
print("ISRTuple", N2N1ISRTuple)
sys.stdout.flush()
print("noISRTuple", N2N1noISRTuple)
sys.stdout.flush()
MadgraphEffN2N1 = N2N1ISRTuple[0]/N2N1noISRTuple[0]
#PythiaEff = noISRTuple[2]/noISRTuple[0]
#make ntuples and determine number of "surviving" events to get Pythia Efficiency - only for ISR
#os.system('recoGen')
print("recoGen commands")
sys.stdout.flush()
os.chdir('/afs/cern.ch/user/a/alalbert/iDM-codev2/CMSSW_10_2_18/')
print("command 1")
sys.stdout.flush()
#os.system('source /afs/cern.ch/user/j/jreicher/public/setupCMS.sh')
print("command 2")
sys.stdout.flush()
os.system('scram b -j 8')
print("command 3")
sys.stdout.flush()
os.chdir('src/iDMAnalysis/skimmer')
print("command 4")
sys.stdout.flush()
os.system("source /cvmfs/cms.cern.ch/cmsset_default.sh")
#os.system("cmsenv")
os.system("eval `scramv1 runtime -sh`")
for lifetime in lifetimeList:
#for lifetime in ['1']:
        outputFileString = '/eos/user/a/alalbert/iDM/Samples/' +cardsNameISR + '_' + lifetime + 'mm.root'
        os.system('source /afs/cern.ch/user/j/jreicher/public/setupCMS.sh; cmsRun test/run_ntuplizer_cfg2.py test=1 year=2018 inputDirectory={outputDirectoriesISR} outputFile={outputFile} lifetime={lifetime} lifetimeUnits=mm'.format(outputDirectoriesISR = outputDirectoriesISR, outputFile = outputFileString, lifetime = lifetime))
        print("finished sample with lifetime", lifetime)
        sys.stdout.flush()
	N2N1outputFileString = '/eos/user/a/alalbert/iDM/Samples/' +cardsNameN2N1ISR + '_' + lifetime + 'mm.root'
        os.system('source /afs/cern.ch/user/j/jreicher/public/setupCMS.sh; cmsRun test/run_ntuplizer_cfg2.py test=1 year=2018 inputDirectory={N2N1outputDirectoriesISR} outputFile={outputFile} lifetime={lifetime} lifetimeUnits=mm'.format(N2N1outputDirectoriesISR = outputDirectoriesN2N1ISR, outputFile = N2N1outputFileString, lifetime = lifetime))
        print("finished sample with lifetime", lifetime)
        sys.stdout.flush()
##### code to get number of entries in ntuples
PythiaEff = []; PythiaEffN2N1 = []
for lifetime in lifetimeList:
	outputFileString = '/eos/user/a/alalbert/iDM/Samples/' +cardsNameISR + '_'+lifetime+'mm.root'
	os.system('cd /eos/user/a/alalbert/iDM/Samples')
	print("Attempting to Access NTuple for " + lifetime + " mm lifetime")
	sys.stdout.flush()
	dataFile = TFile.Open(outputFileString)
	tree = dataFile.Get("ntuples_gbm/recoT")
	numEntries = tree.GetEntries()
	print(numEntries)
	sys.stdout.flush()
	PythiaEff.append(numEntries/(float(numJobs)*500))

for lifetime in lifetimeList:
        outputFileString = '/eos/user/a/alalbert/iDM/Samples/' +cardsNameN2N1ISR + '_'+lifetime+'mm.root'
        os.system('cd /eos/user/a/alalbert/iDM/Samples')
        print("Attempting to Access NTuple for " + lifetime + " mm lifetime")
        sys.stdout.flush()
        dataFile = TFile.Open(outputFileString)
        tree = dataFile.Get("ntuples_gbm/recoT")
        numEntries = tree.GetEntries()
        print(numEntries)
        sys.stdout.flush()
        PythiaEffN2N1.append(numEntries/(float(numJobs)*500))
print(PythiaEffN2N1)

sigmaTheory = float(sigmaTheory) + float(sigmaTheory2)
sigmaTheoryN2N1 = float(sigmaTheoryN2N1)
os.chdir('/eos/user/a/alalbert/lxplus_work/hats_2020_gen/CMSSW_10_2_3/src/GEN4HATS/genproductions/bin/MadGraph5_aMCatNLO')
with open("/eos/user/a/alalbert/lxplus_work/hats_2020_gen/CMSSW_10_2_3/src/GEN4HATS/genproductions/bin/MadGraph5_aMCatNLO/normalizationInfo"+cardsNameISR+".txt", 'w') as f:
        f.write("N2C1 Normalization Info")
	f.write('\n')
	f.write("Process Cross Section from Theory: "+ str(sigmaTheory)+ " pb" + '\n')
        f.write("Madgraph ISR Efficiency: "+str(MadgraphEff)+ '\n')
        f.write('\n')
	for index in range(len(lifetimeList)):
		f.write('\n')
		f.write("Pythia Cut Efficiency for " + lifetimeList[index] + " mm lifetime: "+ str(PythiaEff[index])+ '\n')
		f.write("Total Filter Efficiency for  " + lifetimeList[index] + " mm lifetime: " + str(MadgraphEff * PythiaEff[index]) + '\n')
		f.write("Total Cross Section is: " + str(sigmaTheory * MadgraphEff * PythiaEff[index] * Z_MuMu) + " pb")
	f.write('\n')
	f.write('\n')
	f.write("N2N1 Normalization Info")
	f.write('\n')
	f.write("Process Cross Section from Theory: "+ str(sigmaTheoryN2N1)+ " pb " +'\n')
        f.write("Madgraph ISR Efficiency: "+str(MadgraphEffN2N1)+ '\n')
        f.write('\n')
        for index in range(len(lifetimeList)):
                f.write('\n')
                f.write("Pythia Cut Efficiency for " + lifetimeList[index] + " mm lifetime: "+ str(PythiaEffN2N1[index])+ '\n')
                f.write("Total Filter Efficiency for  " + lifetimeList[index] + " mm lifetime: " + str(MadgraphEffN2N1 * PythiaEffN2N1[index]) + '\n')
		f.write("Total Cross Section is: " + str(sigmaTheoryN2N1 * MadgraphEffN2N1 * PythiaEffN2N1[index] * Z_MuMu) + " pb")
	f.close()
print("script done")
sys.stdout.flush()
os.system("echo 'Pythia and NTuples Done for {cardsName}' | mail -s 'LXPLUS' ama288@cornell.edu".format(cardsName = cardsNameISR))
