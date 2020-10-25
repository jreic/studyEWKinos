#! /usr/bin/env python

from builtins import range
import ROOT
import sys,os
from DataFormats.FWLite import Events, Handle

outputdir = "~/public_html/EWKino/"
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

# add more histograms here for other kinematics!
# most of the interesting quantities for a single particle come from
# https://github.com/cms-sw/cmssw/blob/master/DataFormats/Candidate/interface/LeafCandidate.h#L105
# https://github.com/cms-sw/cmssw/blob/master/DataFormats/HepMCCandidate/interface/GenParticle.h#L50
# https://github.com/cms-sw/cmssw/blob/master/DataFormats/Candidate/interface/CompositeRefCandidateT.h#L50
# as well as properties of the particle 4-vector itself

# a counter for printouts
nevents_processed = 0

# loop over events
for event in events:
    event.getByLabel (label, handle)

    if nevents_processed % 1000 == 0:
        print "Processing event #%s" % (nevents_processed)

    # get the product: these are the particles that we'll access!
    genParticles = handle.product()
    #print "Number of genParticles is %s" % len(genParticles)

    # loop over the particles
    for initial_idx in range(0, len(genParticles)) :
        initial = genParticles[initial_idx]

        # I think status 3 is the main one for particles from the hard process, but 
        # GEN4HATS/GenExercise/python/ZjetsAnalysis_cfi.py includes 62 as well so we'll do the same here
        if not (initial.status() == 3 or initial.status() == 62) :
            continue

        # only want the Z boson--note we may need to adjust this when looking at SUSY, because
        # the PDG ID for very off-shell Z bosons can be different
        if not abs(initial.pdgId()) == 23 : 
            continue

        # only look at decays to two particles
        if initial.numberOfDaughters() < 2 :
            continue

        lep0 = initial.daughter(0)
        lep1 = initial.daughter(1)

        # Let's look for Z boson decays to e+e- or mu+mu- pairs:
        # only want pairs of electrons or pairs of muons: pdgId 11 or 13
        pdgId0 = abs(lep0.pdgId())
        pdgId1 = abs(lep1.pdgId())
        if pdgId0 != pdgId1 : continue
        if not (pdgId0 == 11 or pdgId1 == 13) : continue

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
        
    nevents_processed += 1


# make a canvas, draw, and save it
c1 = ROOT.TCanvas()
h_zmass.Draw()
c1.Print (outputdir+"h_zmass.png")
h_zpt.Draw()
c1.Print (outputdir+"h_zpt.png")
h_lep0pt.Draw()
c1.Print (outputdir+"h_lep0pt.png")
h_lep1pt.Draw()
c1.Print (outputdir+"h_lep1pt.png")
outfile.Write()
