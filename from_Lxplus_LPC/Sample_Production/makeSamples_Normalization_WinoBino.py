import re
import time
import subprocess
import sys,os
import pickle
import ROOT
from ROOT import TFile, TTree

#for this script, I assume a naming convention for the cards that goes "N2Mass_C1Mass_(noISR)". I'll give outputdirectories the same name as their cards
N2Mass = sys.argv[1];
pathToCardsISR = sys.argv[2];
pathToCardsNoISR = sys.argv[3];
numJobs = sys.argv[4]; sigmaTheory = sys.argv[5]; sigmaTheory2 = sys.argv[6]; 
massSplittingList = sys.argv[7:]
cardsNameISR = N2Mass + "_" + N2Mass
cardsNameNoISR = N2Mass + "_" + N2Mass + "_noISR"
ISRTarball = cardsNameISR + "_slc7_amd64_gcc820_CMSSW_9_3_16_tarball.tar.xz"
NoISRTarball = cardsNameNoISR + "_slc7_amd64_gcc820_CMSSW_9_3_16_tarball.tar.xz"
Z_MuMu = 0.0366
lifetimeList = ['1', '10', '100', '1000']
sys.stdout.flush()
'''
#make ISR and ISRinclusive gridpacks - note that throughout this code, noISR is synoymous to ISRInclusive
os.system("export PYTHONPATH=/usr/lib64/python2.7/site-packages")
os.system("time ./submit_condor_gridpack_generation.sh {cardsNameISR} {pathToCardsISR}".format(cardsNameISR = cardsNameISR, pathToCardsISR = pathToCardsISR))
print("made ISR tarball")
sys.stdout.flush()
os.system("export PYTHONPATH=/usr/lib64/python2.7/site-packages")
os.system("time ./submit_condor_gridpack_generation.sh {cardsNameNoISR} {pathToCardsNoISR}".format(cardsNameNoISR = cardsNameNoISR, pathToCardsNoISR = pathToCardsNoISR))
print("made noISR tarball")
sys.stdout.flush()
'''
ISRCrossSectionMad = 1; NoISRCrossSectionMad = 1; METCrossSection = 1;
os.system("condor_rm -all")
time.sleep(60)
massSamples = []; gridpackList = []
ISRGridpack = ""; noISRGripack = ""
for massSplitting in massSplittingList:
	massString = cardsNameISR + "_" + str(int(N2Mass)  - int(massSplitting))
	massSamples.append(massString)
	gridpackName = massString + "_slc7_amd64_gcc820_CMSSW_9_3_16_tarball.tar.xz"
	gridpackList.append(gridpackName)
	os.system("cp {ISRTarball} /afs/cern.ch/user/a/alalbert/lxplus_work/iDM-analysis-AODproducer/condor/gridpacks/Production/{newName}".format(ISRTarball = ISRTarball, newName = massString + "_slc7_amd64_gcc820_CMSSW_9_3_16_tarball.tar.xz"))
	if massSplitting == massSplittingList[0]:
		ISRGridpack = gridpackName
		noISRGridpack = massString + "_noISR_slc7_amd64_gcc820_CMSSW_9_3_16_tarball.tar.xz"
		os.system("cp {NoISRTarball} /afs/cern.ch/user/a/alalbert/lxplus_work/iDM-analysis-AODproducer/condor/gridpacks/Production/{newName}".format(NoISRTarball = NoISRTarball, newName = massString + "_noISR_slc7_amd64_gcc820_CMSSW_9_3_16_tarball.tar.xz"))
print("copied gridpacks")
sys.stdout.flush()
print(massSamples)
'''
os.chdir("/afs/cern.ch/user/a/alalbert/lxplus_work/iDM-analysis-AODproducer/condor")
#make noISR events in Pythia
print("about to submit noISR jobs")
sys.stdout.flush()
os.system('./submitPileupLxplus.py {NoISRTarball} 2018 {numJobs} {outputDirectories}| tee "getJobIDnoISR.txt"'.format(NoISRTarball = noISRGridpack, numJobs = numJobs, outputDirectories = massSamples[0]+"_ISRinclusive"))
print("submitted noISR jobs")
sys.stdout.flush()
#make ISR events in Pythia
print("about to submit ISR jobs")
sys.stdout.flush()
for index, gridpack in enumerate(gridpackList):
	os.system('./submitPileupLxplus.py {ISRTarball} 2018 {numJobs} {outputDirectories} | tee "getJobIDISR.txt"'.format(ISRTarball = gridpack, numJobs = numJobs, outputDirectories = massSamples[index]))
	time.sleep(30)
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

'''
os.chdir("/afs/cern.ch/user/a/alalbert/lxplus_work/iDM-analysis-AODproducer/condor")
#get Cross Section info for ISRInclusive events
print("calling helper script noISR")
sys.stdout.flush()
os.system('python getCrossSectionInfo.py noISR {outputDirectories}'.format(outputDirectories=massSamples[0]+"_ISRinclusive"))

print("calling helper script ISR")
os.system('python getCrossSectionInfo.py ISR {outputDirectories}'.format(outputDirectories=massSamples[0]))

#load in cross section info stored in directory
with open("Logs/ISRInfo"+".obj", 'r') as ISR:
        ISRTuple = pickle.load(ISR)

with open("Logs/noISRInfo"+".obj", 'r') as noISR:
        noISRTuple = pickle.load(noISR)

print("maxMean\t", "maxSTD\t", "pythiaMean\t", "pythiaSTD")
sys.stdout.flush()
print("ISRTuple", ISRTuple)
sys.stdout.flush()
print("noISRTuple", noISRTuple)
sys.stdout.flush()

MadgraphEff = ISRTuple[0]/noISRTuple[0]

print("recoGen commands")
sys.stdout.flush()
os.chdir('/afs/cern.ch/user/a/alalbert/iDM-codev2/CMSSW_10_2_18/')
os.system('scram b -j 8')
os.chdir('src/iDMAnalysis/skimmer')
os.system("source /cvmfs/cms.cern.ch/cmsset_default.sh")
os.system("eval `scramv1 runtime -sh`")

for sampleName in massSamples:
	for lifetime in lifetimeList:
		outputFileString = '/eos/user/a/alalbert/iDM/Samples/' + sampleName + '_' + lifetime + 'mm.root'
		os.system('source /afs/cern.ch/user/j/jreicher/public/setupCMS.sh; cmsRun test/run_ntuplizer_cfg2.py test=1 year=2018 inputDirectory={outputDirectoriesISR} outputFile={outputFile} lifetime={lifetime} lifetimeUnits=mm'.format(outputDirectoriesISR = sampleName, outputFile = outputFileString, lifetime = lifetime))
		print("finished " + sampleName + "sample with lifetime", lifetime)
		sys.stdout.flush()
##### code to get number of entries in ntuples
PythiaEffDict = {}
for sampleName in massSamples:
	sampleEffs = []
	for lifetime in lifetimeList:
		outputFileString = '/eos/user/a/alalbert/iDM/Samples/' + sampleName + '_'+lifetime+'mm.root'
		os.system('cd /eos/user/a/alalbert/iDM/Samples')
		print("Attempting to Access NTuple for " + sampleName + " " + lifetime + " mm lifetime")
		sys.stdout.flush()
		dataFile = TFile.Open(outputFileString)
		tree = dataFile.Get("ntuples_gbm/recoT")
		numEntries = tree.GetEntries()
		print(numEntries)
		sys.stdout.flush()
		sampleEffs.append(numEntries/(float(numJobs)*500))
	PythiaEffDict[sampleName] = sampleEffs

sigmaTheory = float(sigmaTheory) + float(sigmaTheory2)
for sampleName in massSamples:
	PythiaEff = PythiaEffDict[sampleName]
	os.chdir('/eos/user/a/alalbert/lxplus_work/hats_2020_gen/CMSSW_10_2_3/src/GEN4HATS/genproductions/bin/MadGraph5_aMCatNLO')
	with open("/eos/user/a/alalbert/lxplus_work/hats_2020_gen/CMSSW_10_2_3/src/GEN4HATS/genproductions/bin/MadGraph5_aMCatNLO/normalizationInfo"+sampleName+".txt", 'w') as f:
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
		f.close()
print("script done")
sys.stdout.flush()
os.system("echo 'Wino-Bino Pythia and NTuples Done for {cardsName}' | mail -s 'LXPLUS' ama288@cornell.edu".format(cardsName = str(N2Mass)+ "N2 Mass Sample"))
