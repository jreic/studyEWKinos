#! /usr/bin/env python

from builtins import range
import ROOT
import sys,os
from DataFormats.FWLite import Events, Handle

outputdir = "~/public_html/EWKino/newdata"
outputdir += "/" # in case we forget it...
os.system("mkdir -p "+outputdir)

# This is all stolen from https://github.com/cms-sw/cmssw/blob/CMSSW_10_6_12/DataFormats/FWLite/examples/patZpeak.py
# and https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookFWLitePython
# run via:
# python plot_gen.py inputFiles=DYToLL_M-50_13TeV_pythia8_cff_py_GEN.root 
# (or the path to the file, if you aren't in the same dir as the file)

# Make VarParsing object
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideAboutPythonConfigFile#VarParsing_Example
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')
options.parseArguments()

# Events takes either
# - single file name
# - list of file names
# - VarParsing options

# use Varparsing object
events = Events (options)

# create handle and labels outside of loop
handle  = Handle ("std::vector<reco::GenParticle>")

# different file formats contain different types of collections
if "MINIAODSIM" in options.inputFiles[0] :
    label = ("prunedGenParticles")
else :
    label = ("genParticles")

# Create histograms, etc.
ROOT.gROOT.SetBatch()        # don't pop up canvases

# create an output root file
outfile = ROOT.TFile(outputdir+"out.root", "RECREATE")

# Define histograms here, see https://root.cern.ch/doc/master/classTH1.html but the general format is:
# ROOT.TH1F("name of the object", "title of the histogram;x-axis title;y-axis title", nbins, xmin, xmax)
# also note 2D histograms exist as well and are often useful (https://root.cern.ch/doc/master/classTH2.html)
h_zmass = ROOT.TH1F ("zmass", "Z Candidate Mass;mass [GeV];entries", 50, 20, 220)
h_zpt = ROOT.TH1F ("zpt", "Z Candidate pT;pT [GeV];entries", 50, 0, 250)
h_lep0pt = ROOT.TH1F ("lep0pt", "Leading lepton pT;pT [GeV];entries", 50, 0, 250)
h_lep1pt = ROOT.TH1F ("lep1pt", "Subleading lepton pT;pT [GeV];entries", 50, 0, 250)
h_N2_mass = ROOT.TH1F ("N2_Mass", "Neutralino 2;mass [GeV];entries", 50, 20, 220)
h_N2_Pt = ROOT.TH1F ("N2_Pt", "Neutralino 2 pT;pT [GeV];entries", 50, 0, 250)
h_N2_Eta = ROOT.TH1F ("N2_Eta", "Neutralino 2 Pseudorapidity;entries",10,-4,4)
h_N2_Phi = ROOT.TH1F ("N2_Phi", "Neutralino 2 Phi;entries",10,-4,4)
h_N2_status = ROOT.TH1F ("N2_Status", "Neutralino 2 Status;entries", 100,0,100)

h_N1_mass = ROOT.TH1F ("N1_Mass", "Neutralino 1;mass [GeV];entries", 50, 20, 220)
h_N1_Pt = ROOT.TH1F ("N1_Pt", "Neutralino 1 pT;pT [GeV];entries", 50, 0, 250)
h_N1_Eta = ROOT.TH1F ("N1_Eta", "Neutralino 1 Pseudorapidity;entries",10,-4,4)
h_N1_Phi = ROOT.TH1F ("N1_Phi", "Neutralino 1 Phi;entries",10,-4,4)
h_N1_status = ROOT.TH1F ("N1_Status", "Neutralino 1 Status;entries", 100,0,100)

h_C1_mass = ROOT.TH1F ("C1_Mass", "Chargino 1;mass [GeV];entries", 50, 20, 220)
h_C1_Pt = ROOT.TH1F ("C1_Pt", "Chargino 1 pT;pT [GeV];entries", 50, 0, 250)
h_C1_Eta = ROOT.TH1F ("C1_Eta", "Chargino 1 Pseudorapidity;entries",10,-4,4)
h_C1_Phi = ROOT.TH1F ("C1_Phi", "Chargino 1 Phi;entries",10,-4,4)
h_C1_status = ROOT.TH1F ("C1_Status", "Chargino 1 Status;entries", 100,0,100)

# add more histograms here for other kinematics!
# most of the interesting quantities for a single particle come from
# https://github.com/cms-sw/cmssw/blob/master/DataFormats/Candidate/interface/LeafCandidate.h#L105
# https://github.com/cms-sw/cmssw/blob/master/DataFormats/HepMCCandidate/interface/GenParticle.h#L50
# https://github.com/cms-sw/cmssw/blob/master/DataFormats/Candidate/interface/CompositeRefCandidateT.h#L50
# as well as properties of the particle 4-vector itself

# a counter for printouts
nevents_processed = 0
count=0
count1=0
count2=0
count3=0
count4=0
count5=0
count6=0
typesofparticles = {}
leptonhistograms = {}
listoflists = []
groupdict = {}
# loop over events
def leptoninfo(particle,number,numberofdaughters):
	if not leptonhistograms.has_key("histograms_lepton_"+str(number)):
		globals()["h_lepton_Pt_"+str(number)] = ROOT.TH1F ("lep_pt", "Lepton" + str(number) + " pT;pT [GeV];entries", 50, 0, 250)
		globals()["h_lepton_Eta_"+str(number)] = ROOT.TH1F ("lep_Eta", "Lepton" + str(number) + " Eta;Eta;entries", 10,-4,4)
		globals()["h_lepton_Phi_"+str(number)] = ROOT.TH1F ("lep_Phi", "Lepton" + str(number) + " Phi;Phi;entries", 10,-4,4)
		globals()["h_lepton_pdg_"+str(number)] = ROOT.TH1F ("lep_pdg", "Lepton" + str(number) + " pdg;pdg;entries", 41,-20,20)
		groupdict[number] = []
		globals()["h_lepton_Pt_list_"+str(number)] = []
		globals()["h_lepton_Eta_list_"+str(number)] = []
		globals()["h_lepton_Phi_list_"+str(number)] = []
		globals()["h_lepton_pdg_list_"+str(number)] = []
		listoflists.append([globals()["h_lepton_Pt_list_"+str(number)],globals()["h_lepton_Eta_list_"+str(number)],globals()["h_lepton_Phi_list_"+str(number)],globals()["h_lepton_pdg_list_"+str(number)]])	
		histogramlist = [globals()["h_lepton_Pt_"+str(number)], globals()["h_lepton_Eta_"+str(number)], globals()["h_lepton_Phi_"+str(number)], globals()["h_lepton_pdg_"+str(number)]]		
		leptonhistograms["histograms_lepton_"+str(number)] = histogramlist
		groupdict[number]=[]
	temptuple = []
	lorentzparticle = particle.p4();
	'''globals()["h_lepton_Pt_list_"+str(number)].append(lorentzparticle.Pt())

	globals()["h_lepton_Eta_list_"+str(number)].append(lorentzparticle.eta())

	globals()["h_lepton_Phi_list_"+str(number)].append(lorentzparticle.phi())

	globals()["h_lepton_pdg_list_"+str(number)].append(particle.pdgId())'''
	
	temptuple = [lorentzparticle.Pt(),lorentzparticle.eta(),lorentzparticle.phi(),particle.pdgId(),lorentzparticle]
	groupdict[number].append(temptuple)

	
		
def neutralino1info(particle):
	lorentzParticle = particle.p4() 
	N1_status = particle.status(); N1_mass = lorentzParticle.M(); N1_Pt = lorentzParticle.Pt();
        N1_Eta = lorentzParticle.eta(); N1_Phi = lorentzParticle.phi()
	h_N1_status.Fill(N1_status); h_N1_mass.Fill(N1_mass); h_N1_Pt.Fill(N1_Pt); 
        h_N1_Eta.Fill(N1_Eta); h_N1_Phi.Fill(N1_Phi)
def neutralino2info(particle):
	lorentzParticle = particle.p4() 
	N2_status = particle.status(); N2_mass = lorentzParticle.M(); N2_Pt = lorentzParticle.Pt();
        N2_Eta = lorentzParticle.eta(); N2_Phi = lorentzParticle.phi()
	h_N2_status.Fill(N2_status); h_N2_mass.Fill(N2_mass); h_N2_Pt.Fill(N2_Pt); 
        h_N2_Eta.Fill(N2_Eta); h_N2_Phi.Fill(N2_Phi)
def charginoinfo(particle):
	lorentzParticle = particle.p4() 
	C1_status = particle.status(); C1_mass = lorentzParticle.M(); C1_Pt = lorentzParticle.Pt();
        C1_Eta = lorentzParticle.eta(); C1_Phi = lorentzParticle.phi()
	h_C1_status.Fill(C1_status); h_C1_mass.Fill(C1_mass); h_C1_Pt.Fill(C1_Pt); 
        h_C1_Eta.Fill(C1_Eta); h_C1_Phi.Fill(C1_Phi)
for event in events:
    event.getByLabel (label, handle)

    if nevents_processed % 1000 == 0:
        print "Processing event #%s" % (nevents_processed)

    # get the product: these are the particles that we'll access!
    genParticles = handle.product()
    #print "Number of genParticles is %s" % len(genParticles)

    # loop over the particles
    for initial_idx in range(0, len(genParticles)) :
	if initial_idx > 0:
	    count+=1
        initial = genParticles[initial_idx]

        # I think status 3 is the main one for particles from the hard process, but 
        # GEN4HATS/GenExercise/python/ZjetsAnalysis_cfi.py includes 62 as well so we'll do the same here
       # if not (initial.status() == 3 or initial.status() == 62) :
        #    continue
	count1+=1
        # only want the Z boson--note we may need to adjust this when looking at SUSY, because
        # the PDG ID for very off-shell Z bosons can be different
        if not abs(initial.pdgId()) == 23 : 
            if typesofparticles.has_key(initial.pdgId()):
		typesofparticles[initial.pdgId()]+=1
	    else:
		 typesofparticles[initial.pdgId()]=1
	    #here, find info about SUSY particles for histograms
	    #first, neutralino1
	    particle = initial
	    leptoncount =  0
	    if abs(initial.pdgId())==1000023:
		neutralino2info(particle)
		#accumulate info of daughters, for leptons asssume all N2 have same number of daught
		for y in range (0,particle.numberOfDaughters()):
		    x = particle.daughter(y)
		    if (abs(x.pdgId()))== 1000022:
			neutralino1info(x)
		    if (abs(x.pdgId()))== 11 or (abs(x.pdgId()))== 13:
			leptoncount+=1
			leptoninfo(x,leptoncount,particle.numberOfDaughters())
	    if abs(initial.pdgId())==1000024:
		charginoinfo(particle)
	    
	    continue
	count2+=1
        # only look at decays to two particles
        if initial.numberOfDaughters() < 2 :
            continue
	count3+=1
        lep0 = initial.daughter(0)
        lep1 = initial.daughter(1)

        # Let's look for Z boson decays to e+e-f typesofparticle or mu+mu- pairs:
        # only want pairs of electrons or pairs of muons: pdgId 11 or 13
        pdgId0 = abs(lep0.pdgId())
        pdgId1 = abs(lep1.pdgId())
        if pdgId0 != pdgId1 : continue
	count4+=1
        if not (pdgId0 == 11 or pdgId1 == 13) : continue
	count5+=1
        # only want oppositely charged electrons or muons
        if lep0.charge() == lep1.charge() : continue
	count6+=1
        # these are the momentum/energy 4-vectors
        # https://root.cern.ch/doc/master/classTLorentzVector.html
	
        lep0_tlv = lep0.p4()
        lep1_tlv = lep1.p4()
	
        # TLorentzVectors can be added, and then we can get its invariant mass!
        z_tlv    = lep0_tlv + lep1_tlv

        zmass = z_tlv.M()

        # fill the histogram
        h_zmass.Fill(zmass)

        # some other example plots
        lep0pt = lep0_tlv.Pt()
        lep1pt = lep1_tlv.Pt()
        zpt    = z_tlv.Pt()

        h_lep0pt.Fill(lep0pt)
        h_lep1pt.Fill(lep1pt)
        h_zpt.Fill(zpt)
        
    nevents_processed += 1

print((count,count1,count2,count3,count4,count5,count6))
for key,value in typesofparticles.items():
    print(key, value)
# make a canvas, draw, and save it
c1 = ROOT.TCanvas()
h_N2_status.Draw()
c1.Print (outputdir+"N2_status.png")
h_N2_mass.Draw()
c1.Print (outputdir+"N2_mass.png")
h_N2_Pt.Draw()
c1.Print (outputdir+"N2_Pt.png")
h_N2_Eta.Draw()
c1.Print (outputdir+"N2_Eta.png")
h_N2_Phi.Draw()
c1.Print (outputdir+"N2_Phi.png")

h_N1_status.Draw()
c1.Print (outputdir+"N1_status.png")
h_N1_mass.Draw()
c1.Print (outputdir+"N1_mass.png")
h_N1_Pt.Draw()
c1.Print (outputdir+"N1_Pt.png")
h_N1_Eta.Draw()
c1.Print (outputdir+"N1_Eta.png")
h_N1_Phi.Draw()
c1.Print (outputdir+"N1_Phi.png")

h_C1_status.Draw()
c1.Print (outputdir+"C1_status.png")
h_C1_mass.Draw()
c1.Print (outputdir+"C1_mass.png")
h_C1_Pt.Draw()
c1.Print (outputdir+"C1_Pt.png")
h_C1_Eta.Draw()
c1.Print (outputdir+"C1_Eta.png")
h_C1_Phi.Draw()
c1.Print (outputdir+"C1_Phi.png")
outfile.Write()
"""
h_zmass.Draw()
c1.Print (outputdir+"h_zmass.png")
h_zpt.Draw()
c1.Print (outputdir+"h_zpt.png")
h_lep0pt.Draw()
c1.Print (outputdir+"h_lep0pt.png")
h_lep1pt.Draw()
c1.Print (outputdir+"h_lep1pt.png")
outfile.Write()
"""
print("Number of entries in LeptonHistograms:",len(leptonhistograms))
print("number of histograms in value:", len(leptonhistograms["histograms_lepton_1"]))
# sort lepton histograms by leading and nonleading, create reconstructed Z-boson plots
'''for x in enumerate(listoflists[0][0]):
    tempdict = {}
    templist = []
    for y in range(0,len(listoflists)):
	templist=[]
	for yy in range(0,len(listoflists[0])
	    templist.append(listoflists[y][yy][x])
	    tempdict[yy]=templist
    templist = templist.sort()
    for z in range(0, len(listoflists)):
	for zz in range(0,len(listoflists[0])
	listoflists[z][zz][x] = templist[z]
'''
zlist = []
for x in range(0,len(groupdict[1])):
    tempdict = {} 
    for key,value in groupdict.items():
	tempdict[value[x][0]] = value[x]
    sortedPt = sorted(tempdict,reverse=True)
    sortedTuples = []
    for num in range(0,len(tempdict)):
	sortedTuples.append(tempdict[sortedPt[num]])
    zefftup = []
    for num2 in range(0,len(sortedTuples)):
        whichhist = 0
	for a in range(0,5):
	    
	    if a==0:
                histtype = "Pt"
            if a==1:
                histtype = "Eta"
            if a==2:
                histtype = "Phi"
	    if a==3:
                histtype = "pdg"
	    if a==4:
		zefftup.append(sortedTuples[num2][4])
		continue
            globals()["h_lepton_"+histtype+"_"+str(num2+1)].Fill(sortedTuples[num2][a])
    zlist.append(zefftup)




for number in range(1,len(leptonhistograms)+1):
    histogramlist = [globals()["h_lepton_Pt_"+str(number)], globals()["h_lepton_Eta_"+str(number)], globals()["h_lepton_Phi_"+str(number)], globals()["h_lepton_pdg_"+str(number)]]		
    leptonhistograms["histograms_lepton_"+str(number)] = histogramlist



# find relevant z info
for x in range(0,len(zlist)):
    for y in range(0,len(zlist[x])):
	if y ==0:
	    vectorsum = zlist[x][y]
	    continue
	vectorsum = vectorsum + zlist[x][y]
    h_zmass.Fill(vectorsum.M())
    h_zpt.Fill(vectorsum.Pt())




h_zmass.Draw()
outfile.Write()
c1.Print (outputdir+"Reconstructed Z Mass"+".png")

h_zpt.Draw()
outfile.Write()
c1.Print (outputdir+"Reconstructed Z pT"+".png")






for key in leptonhistograms:
    histcount=0
    for x in leptonhistograms[key]:
	histcount+=1
	if histcount==1:
	    histtype = "Pt"
	if histcount==2:
	    histtype = "Eta"
	if histcount==3:
	    histtype = "Phi"
	if histcount==4:
	    histtype = "pdg"
	x.Draw()
	outfile.Write()
	c1.Print (outputdir+"Lepton"+str(key)+histtype+".png")
