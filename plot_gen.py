# /usr/bin/env python

from builtins import range
import ROOT
import sys,os
import math
from DataFormats.FWLite import Events, Handle



'''
print("outputdirectory: "+ sys.argv[2])
#outputdir = sys.argv[2]
#outputdir += "/"
'''
inputFile = sys.argv[1]
'''
fullName = sys.argv[2]
#cut1 = fullName.replace("outputFile=","")
if ".root" in cut1:
    place = cut1.rindex("/")
    
    outputdir = cut1[0:place+1]
else:
    outputdir = cut1
print(outputdir)
os.system("mkdir -p "+outputdir)
'''
outputdir = sys.argv[2]+"/"
os.system("mkdir -p "+outputdir)

maxEvents = int(sys.argv[3])

maxHist = float(sys.argv[4])

'''
outputdir = "~/public_html/EWKino/1mmwithZnew"
outputdir += "/" # in case we forget it...
os.system("mkdir -p "+outputdir)
'''

# This is all stolen from https://github.com/cms-sw/cmssw/blob/CMSSW_10_6_12/DataFormats/FWLite/examples/patZpeak.py
# and https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookFWLitePython
# run via:
# python plot_gen.py inputFiles=DYToLL_M-50_13TeV_pythia8_cff_py_GEN.root 
# (or the path to the file, if you aren't in the same dir as the file)

# Make VarParsing object
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideAboutPythonConfigFile#VarParsing_Example
'''
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')
options.parseArguments()
'''
# Events takes either
# - single file name
# - list of file names
# - VarParsing options

# use Varparsing object
#events = Events (options)

#output definition
'''
outputFile = EndPath(process.out)
place = outputFile.rindex('/')
outputdir = outputFile.substring(0,place+1)
os.system("mkdir -p "+outputdir)
'''
# create handle and labels outside of loop
handle  = Handle ("std::vector<reco::GenParticle>")

# different file formats contain different types of collections
'''
if "MINIAODSIM" in options.inputFiles[0] :
    label = ("prunedGenParticles")
else :
    label = ("genParticles")
'''
label = "genParticles"
# Create histograms, etc.
ROOT.gROOT.SetBatch()        # don't pop up canvases
ROOT.gStyle.SetOptFit(0100)
# create an output root file
outfile = ROOT.TFile(outputdir+"out.root", "RECREATE")

# Define histograms here, see https://root.cern.ch/doc/master/classTH1.html but the general format is:
# ROOT.TH1F("name of the object", "title of the histogram;x-axis title;y-axis title", nbins, xmin, xmax)
# also note 2D histograms exist as well and are often useful (https://root.cern.ch/doc/master/classTH2.html)
h_zmass = ROOT.TH1F ("zmass", "Z Candidate Mass;mass [GeV];entries", 50, 0, 50)
h_zpt = ROOT.TH1F ("zpt", "Z Candidate pT;pT [GeV];entries", 12, 0, 60)
h_lep0pt = ROOT.TH1F ("lep0pt", "Leading lepton pT;pT [GeV];entries", 50, 0, 80)
h_lep1pt = ROOT.TH1F ("lep1pt", "Subleading lepton pT;pT [GeV];entries", 50, 0, 80)
h_N2_mass = ROOT.TH1F ("N2_Mass", "Neutralino 2;mass [GeV];entries", 50, 20, 220)
h_N2_Pt = ROOT.TH1F ("N2_Pt", "Neutralino 2 pT;pT [GeV];entries", 50, 0, 300)
h_N2_Eta = ROOT.TH1F ("N2_Eta", "Neutralino 2 Pseudorapidity;entries",10,-4,4)
h_N2_Phi = ROOT.TH1F ("N2_Phi", "Neutralino 2 Phi;entries",10,-4,4)
h_N2_status = ROOT.TH1F ("N2_Status", "Neutralino 2 Status;entries", 100,0,100)
h_N2_lifetime_labframe = ROOT.TH1F ("N2_Lifetime_LabFrame", "Neutralino 2 Lifetime in Lab Frame; Time [ps];entries", 100,0,1.0)
h_N2_lifetime_restframe = ROOT.TH1F ("N2_Lifetime_RestFrame", "Neutralino 2 Lifetime in Rest Frame; Time [ps];entries", 100,0,1.0)
h_N2_ctau_labframe = ROOT.TH1F ("N2_cTau_Labframe", "Neutralino 2 cTau in  Lab Frame; Displacement [mm];entries",100,0,2)
h_N2_ctau_restframe = ROOT.TH1F ("N2_cTau_Restframe", "Neutralino 2 cTau in  Rest Frame; Displacement [mm];entries",100,0,2)

h_N1_mass = ROOT.TH1F ("N1_Mass", "Neutralino 1;mass [GeV];entries", 50, 20, 220)
h_N1_Pt = ROOT.TH1F ("N1_Pt", "Neutralino 1 pT;pT [GeV];entries", 50, 0, 300)
h_N1_Eta = ROOT.TH1F ("N1_Eta", "Neutralino 1 Pseudorapidity;entries",10,-4,4)
h_N1_Phi = ROOT.TH1F ("N1_Phi", "Neutralino 1 Phi;entries",10,-4,4)
h_N1_status = ROOT.TH1F ("N1_Status", "Neutralino 1 Status;entries", 100,0,100)
h_N1_disp2D = ROOT.TH1F("N1_disp2D", "Neutralino 1 2D Displacement; Displacement [cm];entries", 100,0,maxHist)
h_N1_disp3D = ROOT.TH1F("N1_disp3D", "Neutralino 1 3D Displacement; Displacement [cm];entries", 100,0,maxHist)

h_C1_mass = ROOT.TH1F ("C1_Mass", "Chargino 1;mass [GeV];entries", 50, 20, 220)
h_C1_Pt = ROOT.TH1F ("C1_Pt", "Chargino 1 pT;pT [GeV];entries", 50, 0, 300)
h_C1_Eta = ROOT.TH1F ("C1_Eta", "Chargino 1 Pseudorapidity;entries",10,-4,4)
h_C1_Phi = ROOT.TH1F ("C1_Phi", "Chargino 1 Phi;entries",10,-4,4)
h_C1_status = ROOT.TH1F ("C1_Status", "Chargino 1 Status;entries", 100,0,100)
h_C1_2DVertex = ROOT.TH1F ("C1_2DVertex", "Chargino 1 2D Vertex [cm];entries", 300,0,0.3)

h_lep0disp2D = ROOT.TH1F("lep0disp2D", "Leading Lepton 2D Displacement; Displacement [cm];entries", 100,0,maxHist)
h_lep1disp2D = ROOT.TH1F("lep1disp2D", "Subleading Lepton 2D Displacement; Displacement [cm];entries", 100,0,maxHist)
h_lep0disp3D = ROOT.TH1F("lep0disp3D", "Leading Lepton 3D Displacement; Displacement [cm];entries", 100,0,maxHist)
h_lep1disp3D = ROOT.TH1F("lep1disp3D", "Subleading Lepton 3D Displacement; Displacement [cm];entries", 100,0,maxHist)
h_zdisp2D = ROOT.TH1F("zdisp2D", "Z boson 2D Displacement; Displacement [cm];entries", 100,0,maxHist)
h_zdisp3D = ROOT.TH1F("zdisp3D", "Z boson 3D Displacement; Displacement [cm];entries", 100,0,maxHist)
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
origin = (0.,0.,0.)
N2_massglobal = 0.
N2_Pglobal = 0.
noZcount = 0
hasZ = False
Zcount = 0
differentPDGs = 0
notLepton = 0
# loop over events


def findLifetimes(vertex,momentum,mass):
    #convert units to SI
    #print("mass", mass)
    #print("momentum", momentum)
    #print("displacement",vertex)
    c = 299792458
    #momentum1 = (momentum/c)*(1.60218*10**(-10))
    #mass1 = (mass/(c**2))*(1.60218*10**(-10))
    #mass given by MC particle is the rest mass
    ps = 10**12
    Beta = math.sqrt(momentum**2/(momentum**2+mass**2))
    #print("Beta", Beta)
    vertex = vertex/100
    t = vertex/(Beta*c)
    y = 1/(math.sqrt(1-(Beta)**2))
    a = (Beta*vertex)/(c)
    b = t-a
    d = y*b
    t = t*ps
    tprime = d*(ps)
    #print("time (ps)", t)
    #print("tprine (ps)", tprime)	
    return (t,tprime)

def leptoninfo(particle,number,numberofdaughters):
	if not leptonhistograms.has_key("histograms_lepton_"+str(number)):
		globals()["h_lepton_Pt_"+str(number)] = ROOT.TH1F ("lep_pt", "Lepton" + str(number) + " pT;pT [GeV];entries", 24, 0, 120)
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
	N1_status = particle.status(); N1_mass = lorentzParticle.M(); N1_Pt = lorentzParticle.Pt();N1_P = lorentzParticle.P()
        N1_Eta = lorentzParticle.eta(); N1_Phi = lorentzParticle.phi()
	h_N1_status.Fill(N1_status); h_N1_mass.Fill(N1_mass); h_N1_Pt.Fill(N1_Pt); 
        h_N1_Eta.Fill(N1_Eta); h_N1_Phi.Fill(N1_Phi)
	disp2 = math.sqrt((particle.vx()-origin[0])**2+(particle.vy()-origin[1])**2)
	disp3 = math.sqrt((particle.vx()-origin[0])**2+(particle.vy()-origin[1])**2+(particle.vz()-origin[2])**2)
	h_N1_disp2D.Fill(disp2)
	h_N1_disp3D.Fill(disp3)
	times = findLifetimes(disp3,N2_Pglobal,N2_massglobal)
	h_N2_lifetime_labframe.Fill(times[0])
	h_N2_lifetime_restframe.Fill(times[1])
	h_N2_ctau_labframe.Fill(times[0]*3*(10**8)*1000/(10**12))
	h_N2_ctau_restframe.Fill(times[1]*3*(10**8)*1000/(10**12))
def neutralino2info(particle):
	lorentzParticle = particle.p4() 
	N2_status = particle.status(); N2_mass = lorentzParticle.M(); N2_Pt = lorentzParticle.Pt(); N2_P = lorentzParticle.P();
        N2_Eta = lorentzParticle.eta(); N2_Phi = lorentzParticle.phi()
	h_N2_status.Fill(N2_status); h_N2_mass.Fill(N2_mass); h_N2_Pt.Fill(N2_Pt); 
        h_N2_Eta.Fill(N2_Eta); h_N2_Phi.Fill(N2_Phi)
	global N2_massglobal
	N2_massglobal = N2_mass 
	global N2_Pglobal
	N2_Pglobal = N2_P
	#print("changed",N2_massglobal,N2_Pglobal)
	#print(N2_status)
def charginoinfo(particle):
	lorentzParticle = particle.p4() 
	C1_status = particle.status(); C1_mass = lorentzParticle.M(); C1_Pt = lorentzParticle.Pt();
        C1_Eta = lorentzParticle.eta(); C1_Phi = lorentzParticle.phi()
	h_C1_status.Fill(C1_status); h_C1_mass.Fill(C1_mass); h_C1_Pt.Fill(C1_Pt); 
        h_C1_Eta.Fill(C1_Eta); h_C1_Phi.Fill(C1_Phi)
	C1_2Dvertex = math.sqrt((particle.vx())**2+(particle.vy())**2);h_C1_2DVertex.Fill(C1_2Dvertex)
	#print("x-vertex:",particle.vx(),"-", "y-vertex:",particle.vy(),"-","z-vertex:",particle.vz())
test = ROOT.TFile.Open(inputFile,"READ")
#myFile = ROOT.TFile(inputFile)
#treeName = myFile.GetListOfKeys()
#print(treeName)
#myTree = myFile.treeName()
tree = test.Get("Events")
events = Events(inputFile)
for num,event in enumerate(events):
#for event in events:
    
    event.getByLabel (label, handle)

    if nevents_processed % 10 == 0:
        print "Processing event #%s" % (nevents_processed)
	
    if nevents_processed > maxEvents:
	break;
    # get the product: these are the particles that we'll access!
    genParticles = handle.product()
    #print "Number of genParticles is %s" % len(genParticles)

    # loop over the particles
    for initial_idx in range(0, len(genParticles)) :
	if initial_idx > 0:
	    count+=1
        initial = genParticles[initial_idx]
	'''if initial.pdgId()==2212:
	    print("MONTE CARLO PROTONS")
	    print("proton status code: ", initial.status())
	    print("x-vertex:", initial.vx(),"-", "y-vertex:",initial.vy(),"-","z-vertex:",initial.vz())
	    print("Energy:", initial.energy())
	    fourvector = initial.p4();
	    print("px:",fourvector.px(),"py:",fourvector.py(),"pz:",fourvector.pz())
	    flags = initial.statusFlags()
	    print("Is it prompt ", flags.isPrompt())
	    print("DecayedLeptonHadron?", flags.isDecayedLeptonHadron())
	    print("Hard Process?", flags.isHardProcess())
	    print("First Copy?", flags.isFirstCopy())
	    print("From hard process?", flags.fromHardProcess())
	    print("\n")
	'''  
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
	    if abs(initial.pdgId())==1000023 and initial.status()==62: 
		neutralino2info(particle)
		origin = (particle.vx(),particle.vy(),particle.vz())
		'''for numMother in range(0, particle.numberOfMothers()):
		    Mother = particle.mother(numMother)
		    print("MOTHER PARTICLES")
		    print("ParticleID:", Mother.pdgId())
	    	    print("Particle status code: ", Mother.status())
	            print("x-vertex:", Mother.vx(),"-", "y-vertex:",Mother.vy(),"-","z-vertex:",Mother.vz())
	            print("Energy:", Mother.energy())
	            vector = Mother.p4()
	            print("px:",vector.px(),"py:",vector.py(),"pz:",vector.pz()) 
	    	    print("\n")
		'''
		#accumulate info of daughters, for leptons asssume all N2 have same number of daught
		if particle.numberOfDaughters()==0 and particle.status()==62:
                    noZcount+=1
	    	    print("noZcount", noZcount)
		for y in range (0,particle.numberOfDaughters()):
		    x = particle.daughter(y)
		    if abs(x.pdgId())==23:
			hasZ = True
		    if not (abs(x.pdgId())==23 or abs(x.pdgId())==1000022 or abs(x.pdgId())==1000023):
			print(x.pdgId())
		    if (abs(x.pdgId()))== 1000022:
			neutralino1info(x)
		    ''' if (abs(x.pdgId()))==23:
			#print("found z")
			for z in range (0, x.numberOfDaughters()):
			    lepton = x.daughter(z)
			    if (abs(lepton.pdgId()))== 11 or (abs(lepton.pdgId()))== 13:
                        	leptoncount+=1
                        	leptoninfo(lepton,leptoncount,x.numberOfDaughters())
		    '''
		    if ((abs(x.pdgId()))==11) or ((abs(x.pdgId()))==13):
		       leptoncount+=1
		       leptoninfo(x,leptoncount,particle.numberOfDaughters())
		#print("Has Z?", hasZ)
		if hasZ:
		    Zcount+=1
	    if abs(initial.pdgId())==1000024:
		if initial.status()==62:
		    charginoinfo(particle)
	    
	    continue
	count2+=1
        # only look at decays to two particles
        #print(initial.numberOfMothers())
	#print((initial.mother(0)).pdgId())
	if initial.numberOfDaughters() !=  2 :
	    print(initial.numberOfDaughters())
	    continue
	count3+=1
        lep0 = initial.daughter(0)
        lep1 = initial.daughter(1)

        # Let's look for Z boson decays to e+e-f typesofparticle or mu+mu- pairs:
        # only want pairs of electrons or pairs of muons: pdgId 11 or 13
        pdgId0 = abs(lep0.pdgId())
        pdgId1 = abs(lep1.pdgId())
        if pdgId0 != pdgId1 : 
	    differentPDGs+=1
	    continue
	count4+=1
        if not (pdgId0 == 11 or pdgId1 == 13) :
	    notLepton+=1
	    print(pdgId0)
	    count5+=1
	    continue
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
	
	#add info about displacement
	a_2Ddisp = math.sqrt((lep0.vx()-origin[0])**2+(lep0.vy()-origin[1])**2)
	a_3Ddisp = math.sqrt((lep0.vx()-origin[0])**2+(lep0.vy()-origin[1])**2+(lep0.vz()-origin[2])**2)
	b_2Ddisp = math.sqrt((lep1.vx()-origin[0])**2+(lep1.vy()-origin[1])**2)
	b_3Ddisp = math.sqrt((lep1.vx()-origin[0])**2+(lep1.vy()-origin[1])**2+(lep1.vz()-origin[2])**2)
	z_2Ddisp = math.sqrt((initial.vx()-origin[0])**2+(initial.vy()-origin[1])**2)
	z_3Ddisp = math.sqrt((initial.vx()-origin[0])**2+(initial.vy()-origin[1])**2+(initial.vz()-origin[2])**2)
	h_lep0disp2D.Fill(a_2Ddisp)
	h_lep0disp3D.Fill(a_3Ddisp)
	h_lep1disp2D.Fill(b_2Ddisp)
        h_lep1disp3D.Fill(b_3Ddisp)
	h_zdisp2D.Fill(z_2Ddisp)
        h_zdisp3D.Fill(z_3Ddisp) 
    nevents_processed += 1
    origin =  (0.,0.,0.,)
print("Zcount", Zcount)
print("Different PDGs", differentPDGs)
print("Not Lepton", notLepton)
#print((count,count1,count2,count3,count4,count5,count6))
#for key,value in typesofparticles.items():
   #print(key, value)
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

fit = h_N2_lifetime_labframe.Fit("expo","S")
h_N2_lifetime_labframe.Draw()
c1.Print (outputdir+"N2_lifetime_labframe.png")
fit = h_N2_lifetime_restframe.Fit("expo","S")
h_N2_lifetime_restframe.Draw()
c1.Print (outputdir+"N2_lifetime_restframe.png")
fit = h_N2_ctau_labframe.Fit("expo","S")
h_N2_ctau_labframe.Draw()
c1.Print (outputdir+"N2_cTau_labframe.png")
fit = h_N2_ctau_restframe.Fit("expo","S")
h_N2_ctau_restframe.Draw()
c1.Print (outputdir+"N2_cTau_restframe.png")

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
fit = h_N1_disp2D.Fit("expo","S")
par = [fit.Get().Parameter(i) for i in range( 2 )]
print(par)
print("Lifetime = ", 1/-par[1])
h_N1_disp2D.Draw()
c1.Print (outputdir+"N1_2DVertex.png")
fit=h_N1_disp3D.Fit("expo","S")
par = [fit.Get().Parameter(i) for i in range( 2 )]
print(par)
print("Lifetime = ", 1/-par[1])
h_N1_disp3D.Draw()
c1.Print (outputdir+"N1_3DVertex.png")



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
h_C1_2DVertex.Draw()
c1.Print (outputdir+"C1_2DVertex.png")
outfile.Write()

h_zmass.Draw()
c1.Print (outputdir+"h_zmass.png")
h_zpt.Draw()
c1.Print (outputdir+"h_zpt.png")
h_lep0pt.Draw()
c1.Print (outputdir+"h_lep0pt.png")
h_lep1pt.Draw()
c1.Print (outputdir+"h_lep1pt.png")

h_lep0disp2D.Draw()
c1.Print (outputdir+"h_lep0disp2D.png")
h_lep0disp3D.Draw()
c1.Print (outputdir+"h_lep0disp3D.png")
h_lep1disp2D.Draw()
c1.Print (outputdir+"h_lep1disp2D.png")
h_lep1disp3D.Draw()
c1.Print (outputdir+"h_lep1disp3D.png")
h_zdisp2D.Draw()
c1.Print (outputdir+"h_zdisp2D.png")
h_zdisp3D.Draw()
c1.Print (outputdir+"h_zdisp3D.png")

outfile.Write()
h_zmass.Reset()
h_zpt.Reset()
#print("Number of entries in LeptonHistograms:",len(leptonhistograms))
#print("number of histograms in value:", len(leptonhistograms["histograms_lepton_1"]))
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
'''

'''
# find relevant z info
for x in range(0,len(zlist)):
    vectorsum = None
   #print("Our number Z boson for x", x)
    for y in range(0,len(zlist[x])):
	#print("Which Lortentz vector are we at", y)

	if y ==0:
	    vectorsum = zlist[x][y]
	    continue
	print(str(type(vectorsum)) ,str(vectorsum.Pt()))
	print(str(type(zlist[x][y])), str(zlist[x][y].Pt())) 
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
'''
