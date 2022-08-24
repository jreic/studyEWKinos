## Hadronizer fragment for iDM LLP
## To set decay length of heavier dark matter particle
## (ID 1000023), change line '1000023:tau0 = 1' in
## "processParameters" to whatever decay length is desired (in mm).
## There is also a gen-level filter on both MET and jet pT 
## (80 GeV for both) which can be enabled/disabled in the last 
## line, by commenting out one definition of ProductionFilterSequence 
## and uncommenting the other.
##
## Note to self: remember 2017 samples are all e+mu, and 2018 ones
## are only mu

import FWCore.ParameterSet.Config as cms
import sys,os
import subprocess
import re
from Configuration.Generator.Pythia8CommonSettings_cfi import *
# from Configuration.Generator.Pythia8CUEP8M1Settings_cfi import *
# from Configuration.Generator.Pythia8aMCatNLOSettings_cfi import *
from Configuration.Generator.MCTunes2017.PythiaCP5Settings_cfi import *

# Avoid to have multiple lumi in the same job and therefore auto-cleaning
# This means that it need an argument to be run
# from FWCore.ParameterSet.VarParsing import VarParsing
# options = VarParsing ('analysis')
# options.register('jobNum', 0, VarParsing.multiplicity.singleton,VarParsing.varType.int,"jobNum")
# options.parseArguments ()
# firstLumi=10*options.jobNum+1 ## eventsPerJob/eventsPerLumi*jobNum +1
# source = cms.Source("EmptySource",
#         firstLuminosityBlock  = cms.untracked.uint32(firstLumi),
#         numberEventsInLuminosityBlock = cms.untracked.uint32(100)
#         )


# External LHE producer configuration
#externalLHEProducer = cms.EDProducer("ExternalLHEProducer",
#    args = cms.vstring( 'THISDIR' + '/GRIDPACKNAME'),
#    nEvents = cms.untracked.uint32(5000),
#    numberOfParameters = cms.uint32(1),
#    outputFile = cms.string('cmsgrid_final.lhe'),
#    scriptName = cms.FileInPath('GeneratorInterface/LHEInterface/data/run_generic_tarball_cvmfs.sh')
#)
neutralino2_ctau = 100 # mm
neutralino2_width = 0.0197e-11 / float(neutralino2_ctau)

N2Mass = 0.0
N1Mass = 0.0
C1Mass = 0.0
listOutput = subprocess.check_output(["ls","-lt","/afs/cern.ch/user/a/alalbert/lxplus_work/iDM-analysis-AODproducer/condor/submissions"])
longList = re.split('\n',listOutput)
fileName = longList[1]
pos1 = fileName.index("submit")
fileName = fileName[pos1+7:]
dashList=[]
for x in range(0,len(fileName)):
    if fileName[x:x+1]=="_":
        dashList.append(x)

N2Mass = float(fileName[0:dashList[0]])
C1Mass=float(fileName[dashList[0]+1:dashList[1]])
N1Mass=float(fileName[dashList[1]+1:dashList[2]])

# Hadronizer configuration
generator = cms.EDFilter("Pythia8HadronizerFilter",
        maxEventsToPrint = cms.untracked.int32(1),
        pythiaPylistVerbosity = cms.untracked.int32(1),
        filterEfficiency = cms.untracked.double(1.0),
        pythiaHepMCVerbosity = cms.untracked.bool(False),
        comEnergy = cms.double(13000.),
        PythiaParameters = cms.PSet(
            pythia8CommonSettingsBlock,
            pythia8CP5SettingsBlock,
            # pythia8CUEP8M1SettingsBlock,
            # pythia8aMCatNLOSettingsBlock,
            JetMatchingParameters = cms.vstring(
                'JetMatching:setMad = off',
                'JetMatching:scheme = 1',
                'JetMatching:merge = on',
                'JetMatching:jetAlgorithm = 2',
                'JetMatching:etaJetMax = 5.',
                'JetMatching:coneRadius = 1.',
                'JetMatching:slowJetPower = 1',
                'JetMatching:qCut = 25', #this is the actual merging scale
                'JetMatching:nQmatch = 4', #5 for 5-flavour scheme (matching of b-quarks)
                'JetMatching:nJetMax = 2', #number of partons in born matrix element for highest multiplicity
                'JetMatching:doShowerKt = off', #off for MLM matching, turn on for shower-kT matching
                '6:m0 = 172.5', 
                '24:mMin = 0.1', 
                '23:onMode = off', 
                '23:onIfAny = 13', 
                '23:mMin = 0.1', 
                'Check:abortIfVeto = on'
                ),
            processParameters = cms.vstring(
                'SLHA:keepSM = on',
                'SLHA:minMassSM = 10.',
                # Very important to enable override!
                'SLHA:allowUserOverride = on',
                'RHadrons:allow = on',
                'RHadrons:allowDecay = on',
                'ParticleDecays:limitTau0 = off',
                #'ParticleDecays:tau0Max = 1000.1',
                'LesHouches:setLifetime = 2',
                'ParticleDecays:allowPhotonRadiation = on',
                # Set decay channel of dark photon to chi2+chi1
                #'32:mayDecay = true',
                #'32:oneChannel = 1 1.0 0 1000023 1000022',
                # Set decay channels of chi2 (only mu or e+mu)
                #'1000023:mayDecay = true',
                #'1000023:oneChannel = 1 1.0 0 1000022 13 -13',
                #'1000023:oneChannel = 1 0.5 0 1000022 13 -13',
                #'1000023:addChannel = 1 0.5 0 1000022 11 -11'	
                #'1000023:addChannel = 1 0.5 0 1000022 11 -11',
                'SUSY:qqbar2chi+-chi0 = on',
                'SUSY:idA = 1000023',
                'SUSY:idB = 1000024',
                # Set decay length of chi2 and chi1pm
                '1000023:tau0 = %f' % neutralino2_ctau,
                #'1000024:tau0 = 100', # FIXME this could be improved! we should decay it too...
                'Tune:preferLHAPDF = 2', 
                'Main:timesAllowErrors = 10000', 
                'Check:epTolErr = 0.01', 
                'Beams:setProductionScalesFromLHEF = off', 
                #'SLHA:useDecayTable = on'
                ),
            parameterSets = cms.vstring('pythia8CommonSettings',
                'pythia8CP5Settings',
                # 'pythia8CUEP8M1SettingsBlock',
                # 'pythia8aMCatNLOSettingsBlock',
                'JetMatchingParameters',
                'processParameters',
                )
            ),
            SLHATableForPythia8 = cms.string('\n#\nBLOCK MASS  # Mass Spectrum\n# PDG code           mass       particle\n        35     1.00000000E+05\n        36     1.00000000E+05\n        37     1.00000000E+05\n        6      1.72500000E+02\n   1000001     1.00000000E+05    # ~d_L\n   2000001     1.00000000E+05   # ~d_R\n   1000002     1.00000000E+05    # ~u_L\n   2000002     1.00000000E+05   # ~u_R\n   1000003     1.00000000E+05    # ~s_L\n   2000003     1.00000000E+05   # ~s_R\n   1000004     1.00000000E+05    # ~c_L\n   2000004     1.00000000E+05   # ~c_R\n   1000005     1.00000000E+05   # ~b_1\n   2000005     1.00000000E+05   # ~b_2\n   1000006     1.00000000E+05   # ~t_1\n   2000006     1.00000000E+05   # ~t_2\n   1000011     1.00000000E+05   # ~e_L\n   2000011     1.00000000E+05   # ~e_R\n   1000012     1.00000000E+05   # ~nu_eL\n   1000013     1.00000000E+05   # ~mu_L\n   2000013     1.00000000E+05   # ~mu_R\n   1000014     1.00000000E+05   # ~nu_muL\n   1000015     1.00000000E+05   # ~tau_1\n   2000015     1.00000000E+05   # ~tau_2\n   1000016     1.00000000E+05   # ~nu_tauL\n   1000021     1.00000000E+05   # ~g\n   1000022     %E   # ~chi_10\n   1000023     %E   # ~chi_20\n   1000025     1.00000000E+05   # ~chi_30\n   1000035     1.00000000E+05   # ~chi_40\n   1000024     %E   # ~chi_1+\n   1000037     1.00000000E+05   # ~chi_2+\n#\n#\n#\n#         PDG            Width\nDECAY         6     1.134E+00        # top decays\nDECAY   2000006     0.00000000E+00   # stop2 decays\nDECAY   1000005     0.00000000E+00   # sbottom1 decays\nDECAY   2000005     0.00000000E+00   # sbottom2 decays\n#\n#         PDG            Width\nDECAY   1000011     0.00000000E+00   # selectron_L decays\nDECAY   2000011     0.00000000E+00   # selectron_R decays\nDECAY   1000013     0.00000000E+00   # smuon_L decays\nDECAY   2000013     0.00000000E+00   # smuon_R decays\nDECAY   1000015     0.00000000E+00   # stau_1 decays\nDECAY   2000015     0.00000000E+00   # stau_2 decays\n#\n#         PDG            Width\nDECAY   1000012     0.00000000E+00   # snu_elL decays\nDECAY   1000014     0.00000000E+00   # snu_muL decays\nDECAY   1000016     0.00000000E+00   # snu_tauL decays\nDECAY   1000006     0.00000000E+00   # stop1 decays\nDECAY   1000021     0.00000000E+00   # gluino decays\nDECAY   1000022     0.00000000E+00   # neutralino1 decays\nDECAY   1000023     %E   # neutralino2 decays\n    0.00000000E+00   3    1000022   11   -11  #dummy decay\n    1.00000000E+00   2    1000022   23\nDECAY   1000024     %E   # chargino1+ decays\n    0.00000000E+00   3    1000022   12   -11\n    1.00000000E+00   2    1000022   24\n' % (N1Mass, N2Mass, C1Mass, neutralino2_width, neutralino2_width))
        )

#     Filter setup
# ------------------------
# https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_X/PhysicsTools/HepMCCandAlgos/python/genParticles_cfi.py

tmpGenParticles = cms.EDProducer("GenParticleProducer",
        saveBarCodes = cms.untracked.bool(True),
        src = cms.InputTag("generator", "unsmeared"),
        abortOnUnknownPDGCode = cms.untracked.bool(False)
        )

# https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_X/RecoJets/Configuration/python/GenJetParticles_cff.py
# https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_X/RecoMET/Configuration/python/GenMETParticles_cff.py
tmpGenParticlesForJetsNoNu = cms.EDProducer("InputGenJetsParticleSelector",
        src = cms.InputTag("tmpGenParticles"),
        ignoreParticleIDs = cms.vuint32(
            1000022, 1000012, 1000014, 1000016,
            2000012, 2000014, 2000016, 1000039,
            5100039, 4000012, 4000014, 4000016,
            9900012, 9900014, 9900016,
            39,12,14,16),
        partonicFinalState = cms.bool(False),
        excludeResonances = cms.bool(False),
        excludeFromResonancePids = cms.vuint32(12, 13, 14, 16),
        tausAsJets = cms.bool(False)
        )

# https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_X/RecoJets/JetProducers/python/AnomalousCellParameters_cfi.py
AnomalousCellParameters = cms.PSet(
        maxBadEcalCells         = cms.uint32(9999999),
        maxRecoveredEcalCells   = cms.uint32(9999999),
        maxProblematicEcalCells = cms.uint32(9999999),
        maxBadHcalCells         = cms.uint32(9999999),
        maxRecoveredHcalCells   = cms.uint32(9999999),
        maxProblematicHcalCells = cms.uint32(9999999)
        )

# https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_X/RecoJets/JetProducers/python/GenJetParameters_cfi.py
GenJetParameters = cms.PSet(
        src            = cms.InputTag("tmpGenParticlesForJetsNoNu"),
        srcPVs         = cms.InputTag(''),
        jetType        = cms.string('GenJet'),
        jetPtMin       = cms.double(3.0),
        inputEtMin     = cms.double(0.0),
        inputEMin      = cms.double(0.0),
        doPVCorrection = cms.bool(False),
        # pileup with offset correction
        doPUOffsetCorr = cms.bool(False),
        # if pileup is false, these are not read:
        nSigmaPU = cms.double(1.0),
        radiusPU = cms.double(0.5),  
        # fastjet-style pileup     
        doAreaFastjet = cms.bool(False),
        doRhoFastjet = cms.bool(False),
        # if doPU is false, these are not read:
        Active_Area_Repeats = cms.int32(5),
        GhostArea = cms.double(0.01),
        Ghost_EtaMax = cms.double(6.0),
        Rho_EtaMax = cms.double(4.5),
        useDeterministicSeed = cms.bool(True),
        minSeed = cms.uint32( 14327 )
        )

# https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_X/RecoJets/JetProducers/python/ak4GenJets_cfi.py
tmpAk4GenJetsNoNu = cms.EDProducer("FastjetJetProducer",
        GenJetParameters,
        AnomalousCellParameters,
        jetAlgorithm = cms.string("AntiKt"),
        rParam       = cms.double(0.4)
        )

genHTFilter = cms.EDFilter("GenHTFilter",
        src = cms.InputTag("tmpAk4GenJetsNoNu"), #GenJet collection as input
        jetPtCut = cms.double(80.0), #GenJet pT cut for HT
        jetEtaCut = cms.double(5.0), #GenJet eta cut for HT
        genHTcut = cms.double(80.0) #genHT cut
        )

tmpGenMetTrue = cms.EDProducer("GenMETProducer",
        src = cms.InputTag("tmpGenParticlesForJetsNoNu"),
        onlyFiducialParticles = cms.bool(False), ## Use only fiducial GenParticles
        globalThreshold = cms.double(0.0), ## Global Threshold for input objects
        usePt   = cms.bool(True), ## using Pt instead Et
        applyFiducialThresholdForFractions = cms.bool(False),
        )
genMETfilter1 = cms.EDFilter("CandViewSelector",
        src = cms.InputTag("tmpGenMetTrue"),
        cut = cms.string("pt > 80")
        )

genMETfilter2 = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag("genMETfilter1"),
        minNumber = cms.uint32(1),
        )

## Choose to enable or disable the MET and jet gen-level filters
#ProductionFilterSequence = cms.Sequence(generator)
ProductionFilterSequence = cms.Sequence(generator*tmpGenParticles *
        tmpGenParticlesForJetsNoNu * tmpAk4GenJetsNoNu * genHTFilter *
        tmpGenMetTrue * genMETfilter1 * genMETfilter2)
