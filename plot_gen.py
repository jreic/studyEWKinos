# /usr/bin/env python

from builtins import range
import ROOT
import sys,os
import math
from DataFormats.FWLite import Events, Handle

inputFile = sys.argv[1]
outputdir = sys.argv[2]+"/"
os.system("mkdir -p "+outputdir)

maxEvents = int(sys.argv[3])
maxHist = float(sys.argv[4])

# create handle and labels outside of loop
handle  = Handle ("std::vector<reco::GenParticle>")
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

nevents_processed = 0
origin = (0.,0.,0.)
N2_massglobal = 0.
N2_Pglobal = 0.
c = 299792458

# loop over events
def findLifetimes(vertex,momentum,mass):
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
    return (t,tprime)

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
	h_N2_ctau_labframe.Fill(times[0]*c*1000/(10**12))
	h_N2_ctau_restframe.Fill(times[1]*c*1000/(10**12))

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

def charginoinfo(particle):
	lorentzParticle = particle.p4() 
	C1_status = particle.status(); C1_mass = lorentzParticle.M(); C1_Pt = lorentzParticle.Pt();
        C1_Eta = lorentzParticle.eta(); C1_Phi = lorentzParticle.phi()
	h_C1_status.Fill(C1_status); h_C1_mass.Fill(C1_mass); h_C1_Pt.Fill(C1_Pt); 
        h_C1_Eta.Fill(C1_Eta); h_C1_Phi.Fill(C1_Phi)
	C1_2Dvertex = math.sqrt((particle.vx())**2+(particle.vy())**2);h_C1_2DVertex.Fill(C1_2Dvertex)

events = Events(inputFile)
for num,event in enumerate(events):
    event.getByLabel (label, handle)
    if nevents_processed % 10 == 0:
        print "Processing event #%s" % (nevents_processed)
    
    # get the product: these are the particles that we'll access!
    genParticles = handle.product()

    # loop over the particles
    for initial_idx in range(0, len(genParticles)) :
        initial = genParticles[initial_idx]
        if not abs(initial.pdgId()) == 23 : 
	    particle = initial
	    if abs(initial.pdgId())==1000023 and initial.status()==62: 
		neutralino2info(particle)
		origin = (particle.vx(),particle.vy(),particle.vz())
		for y in range (0,particle.numberOfDaughters()):
		    x = particle.daughter(y)
		    if (abs(x.pdgId()))== 1000022:
			neutralino1info(x)
	    if abs(initial.pdgId())==1000024:
		if initial.status()==62:
		    charginoinfo(particle)
	    continue
        
	#code continues because we have a Z
	lep0 = initial.daughter(0)
        lep1 = initial.daughter(1)

        # Let's look for Z boson decays to e+e-f typesofparticle or mu+mu- pairs:
        # only want pairs of electrons or pairs of muons: pdgId 11 or 13
        pdgId0 = abs(lep0.pdgId())
        pdgId1 = abs(lep1.pdgId())
        
	if pdgId0 != pdgId1 : 
	    continue

        if not (pdgId0 == 11 or pdgId1 == 13) :
	    continue

        # only want oppositely charged electrons or muons
        if lep0.charge() == lep1.charge() : continue

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
