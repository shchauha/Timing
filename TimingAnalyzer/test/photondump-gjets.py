import os, re
import FWCore.ParameterSet.Config as cms
  
### CMSSW command line parameter parser
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')

## dump trigger menu 
options.register (
	'dumpTriggerMenu',False,VarParsing.multiplicity.singleton,VarParsing.varType.bool,
	'flag to dump trigger menu');

## dump rec hit info
options.register (
	'dumpRHs',False,VarParsing.multiplicity.singleton,VarParsing.varType.bool,
	'flag to dump rec hits info');

## data or MC options
options.register (
	'isMC',True,VarParsing.multiplicity.singleton,VarParsing.varType.bool,
	'flag to indicate data or MC');

options.register (
	'isGMSB',False,VarParsing.multiplicity.singleton,VarParsing.varType.bool,
	'flag to indicate GMSB');

options.register (
	'isBkg',True,VarParsing.multiplicity.singleton,VarParsing.varType.bool,
	'flag to indicate Background MC');

options.register (
	'HTbin','40To100',VarParsing.multiplicity.singleton,VarParsing.varType.string,
	'which GMSB sample to use');

options.register (
	'dumpIds',False,VarParsing.multiplicity.singleton,VarParsing.varType.bool,
	'flag to dump gen particles IDs');

## processName
options.register (
	'processName','TREE',VarParsing.multiplicity.singleton,VarParsing.varType.string,
	'process name to be considered');

## outputFile Name
options.register (
	'outputFileName','photondump.root',VarParsing.multiplicity.singleton,VarParsing.varType.string,
	'output file name created by cmsRun');

## GT to be used    
options.register (
	'globalTag','80X_mcRun2_asymptotic_2016_TrancheIV_v6',VarParsing.multiplicity.singleton,VarParsing.varType.string,
	'gloabl tag to be used');

## do a demo run over only 100k events
options.register (
	'demoMode',False,VarParsing.multiplicity.singleton,VarParsing.varType.bool,
	'flag to run over only 100k events as a demo');

## parsing command line arguments
options.parseArguments()

## reset file name
options.outputFileName = 'photondump-GJets_HT'+options.HTbin+'.root'

print "##### Settings ######"
print "Running with dumpTriggerMenu     = ",options.dumpTriggerMenu
print "Running with dumpRHs             = ",options.dumpRHs
print "Running with dumpIds             = ",options.dumpIds
print "Running with HTbin               = ",options.HTbin
print "Running with processName         = ",options.processName	
print "Running with outputFileName      = ",options.outputFileName	
print "Running with globalTag           = ",options.globalTag	
print "Running with demoMode            = ",options.demoMode
print "#####################"

## Define the CMSSW process
process = cms.Process(options.processName)

## Load the standard set of configuration modules
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

## Message Logger settings
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

## Define the input source
if options.HTbin == '40To100' :
	process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring( 
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/120000/6476E060-86C0-E611-9739-002618FDA208.root',
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/120000/36B1065E-60C0-E611-9C26-0025904B11AA.root',
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/120000/36456C98-59C0-E611-A0E6-A0000420FE80.root'
			))
elif options.HTbin == '100To200' :
	process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring( 
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/60000/04484E17-C7C1-E611-986A-0025904C6416.root',
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/60000/067BCAD2-A6C0-E611-9157-0CC47A4C8ECE.root',
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/60000/16AFB7B9-61C0-E611-90CC-0025905BA736.root'
			))
elif options.HTbin == '200To400' :
	process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring( 
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/50000/1EED535C-66B9-E611-8613-008CFAFBF0BA.root',
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/50000/142507DF-3EB9-E611-BD17-FA163EC95D6B.root',
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/50000/083DE9FB-60B9-E611-8559-3417EBE643F0.root'
			))
elif options.HTbin == '400To600' :
	process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring( 
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/0C4E670C-A9BE-E611-9DFA-0CC47A7C34A0.root',
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/10B12F2B-B0BE-E611-AF24-0CC47A4D7632.root',
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/14C4621F-A9BE-E611-83AE-0025905A6110.root'
			))
elif options.HTbin == '600ToInf' :
	process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/50000/7E62A441-76BC-E611-A02B-A0369F3102F6.root',
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/50000/7A0F4380-8BBC-E611-8591-0090FAA57C74.root',
			'/store/mc/RunIISummer16MiniAODv2/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/50000/92D8849C-92BC-E611-9D9E-0CC47A745298.root'
			))
else : exit

## How many events to process
if   options.demoMode : process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))
else                  : process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.maxEvents))

# Set the global tag depending on the sample type
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.globaltag = options.globalTag  

# EGM Smearing and VID
from Timing.TimingAnalyzer.PhotonTools_cff import PhotonTools
PhotonTools(process,options.isMC)

## Create output file
## Setup the service to make a ROOT TTree
process.TFileService = cms.Service("TFileService", 
		                   fileName = cms.string(options.outputFileName))

# Make the tree 
process.tree = cms.EDAnalyzer("PhotonDump",
   ## gen info			     
   isGMSB     = cms.bool(options.isGMSB),
   isBkg      = cms.bool(options.isBkg),
   dumpIds    = cms.bool(options.dumpIds),
   genevt     = cms.InputTag("generator"),
   pileup     = cms.InputTag("slimmedAddPileupInfo"),
   genparts   = cms.InputTag("prunedGenParticles"),
   genjets    = cms.InputTag("slimmedGenJets"),
   ## triggers
   dumpTriggerMenu = cms.bool(options.dumpTriggerMenu),
   triggerResults  = cms.InputTag("TriggerResults", "", "HLT"),
   ## vertices
   vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
   ## rho
   rhos = cms.InputTag("fixedGridRhoFastjetAll"), #fixedGridRhoAll
   ## MET
   mets = cms.InputTag("slimmedMETs"),
   ## jets			    	
   jets = cms.InputTag("slimmedJets"),
   ## photons		
   loosePhotonID  = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-loose"),
   mediumPhotonID = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-medium"),
   tightPhotonID  = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-tight"),
   photons        = cms.InputTag("calibratedPhotons"),
   ## ecal recHits			      
   dumpRHs            = cms.bool(options.dumpRHs),
   recHitCollectionEB = cms.InputTag("reducedEgamma", "reducedEBRecHits"),
   recHitCollectionEE = cms.InputTag("reducedEgamma", "reducedEERecHits")
)

# Set up the path
process.treePath = cms.Path(process.selectedPhotons * process.calibratedPhotons * process.egmPhotonIDs * process.tree)
