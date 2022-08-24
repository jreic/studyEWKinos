
from builtins import range
from ROOT import TText
from ROOT import TPaveText
from ROOT import TGraph
from array import array

import re
import time
import subprocess
import sys,os
import ROOT
import pickle

ROOT.gROOT.SetBatch()        # don't pop up canvases
ROOT.gStyle.SetOptFit(0100)
#outputdir = "Logs/crossSectionPlots/v6ISR/"
#os.system("mkdir -p " +outputdir)
#logID = sys.argv[1]
subDir = sys.argv[2] 
os.chdir("Logs/" + subDir)

#outfile = ROOT.TFile(outputdir+"out.root", "RECREATE")
#listOutput = subprocess.check_output(['ls', '-ltr', '|','grep','/".out/"'], shell=True,stderr=subprocess.STDOUT)
listOutput= subprocess.check_output(['ls'])
longList = re.split('\n',listOutput)
index = 0
longList2 = []
for x in longList:
	if ".out" in x:
		longList2.append(x)
print(longList2)

relevantLines = longList2
PythiaLogs = list(map(lambda x: x[x.rfind(" ")+1:], relevantLines))
crossSectionList = []
for PythiaLog in PythiaLogs:
        with open(PythiaLog, 'r') as w:
                lines = w.readlines()
                for line in lines:
                        if "Les Houches User Process(es)" in line and "500" in line:
				modLine = line[:len(line)-2]
				modLine = modLine[modLine.rfind("|") + 1: modLine.rfind(" ")]
				modLine = modLine[:modLine.rfind(" ")]
                                modLine.strip()
				if 'e' in modLine:
					crossSectionList.append(float(modLine))
                                break
        w.close()
#print("Post Pythia Cross Section List", crossSectionList)
PythiaCrossSection = sum(crossSectionList)/len(crossSectionList)
#PythiaEff = PythiaCrossSection/noISRCrossSectionMad

#print("Average Cross-Section", PythiaCrossSection)

crossSectionList = list(map(lambda x: x*10**12, crossSectionList))


maxCrossSectionList = []
for PythiaLog in PythiaLogs:
        with open(PythiaLog, 'r') as w:
                lines = w.readlines()
                for line in lines:
                        if "Les Houches User Process(es)" in line and "500" not in line and "->" not in line:
				modLine = line[line[:len(line)-2].rfind('|')+1:line.rfind('|')]
				modLine.strip()
				maxCrossSectionList.append(float(modLine))
                                break
        w.close()

#print("Max Process Cross Section List", maxCrossSectionList)
MadgraphCrossSection = sum(maxCrossSectionList)/len(maxCrossSectionList)
maxCrossSectionList = list(map(lambda x: x*10**12, maxCrossSectionList))
#print("Average Max Cross Section", MadgraphCrossSection)


h_maxCrossSection = ROOT.TH1F("MaxCrossSection", "Max Cross Section Distribution;cross section [pb];entries", 50, min(maxCrossSectionList), max(maxCrossSectionList))
h_CrossSectionPythia = ROOT.TH1F("PythiaCrossSection", "Post-Pythia Cross Section Distribution;cross section [pb];entries", 50, min(crossSectionList), max(crossSectionList))


for x in range(len(maxCrossSectionList)):
	h_maxCrossSection.Fill(maxCrossSectionList[x])
	h_CrossSectionPythia.Fill(crossSectionList[x])

maxMean = h_maxCrossSection.GetMean(); maxSTD = h_maxCrossSection.GetRMS()
pythiaMean = h_CrossSectionPythia.GetMean(); pythiaSTD = h_CrossSectionPythia.GetRMS()

print("Mean of Max Cross Section: ", maxMean, " pb")
print("Standard Deviation of Max Cross Section: ", maxSTD, " pb")
print("Mean of Pythia Cross Section: ", pythiaMean, " pb")
print("Standard Deviation of Pythia Cross Section: ", pythiaSTD, " pb")

os.system("cd  ..")
os.system("cd ..")
if sys.argv[1] == 'ISR':
	with open("/afs/cern.ch/user/a/alalbert/lxplus_work/iDM-analysis-AODproducer/condor/Logs/ISRInfo.obj", 'wb') as f:
		print("test1")	
		pickle.dump([maxMean, maxSTD, pythiaMean, pythiaSTD], f)


else:
	with open("/afs/cern.ch/user/a/alalbert/lxplus_work/iDM-analysis-AODproducer/condor/Logs/noISRInfo.obj", 'wb') as f:
		print("test2")
		pickle.dump([maxMean, maxSTD, pythiaMean, pythiaSTD], f)
'''
C1 = ROOT.TCanvas()
h_maxCrossSection.Draw()
C1.Print (outputdir+"h_maxCrossSection.png")

h_CrossSectionPythia.Draw()
C1.Print (outputdir+"h_CrossSectionPythia.png")

outfile.Write()
'''
