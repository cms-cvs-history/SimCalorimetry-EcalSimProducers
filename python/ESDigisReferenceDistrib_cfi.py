import FWCore.ParameterSet.Config as cms

ESDigisReferenceDistrib = cms.EDFilter("ESDigisReferenceDistrib",
    verbose = cms.untracked.bool(True),
    ESdigiCollection = cms.InputTag("ecalPreshowerDigis"),
    outputTxtFile = cms.untracked.string('esRefHistosFile.txt'),
    outputRootFile = cms.untracked.string('esRefHistosFile.root')
)


