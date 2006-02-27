#ifndef EcalDigiTask_H
#define EcalDigiTask_H

/*
 * \file EcalDigiTask.h
 *
 * $Date: 2006/01/11 11:56:47 $
 * $Revision: 1.1 $
 * \author F. Cossutti
 *
*/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQMServices/Daemon/interface/MonitorDaemon.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/Track/interface/EmbdSimTrack.h"
#include "SimDataFormats/Track/interface/EmbdSimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/EmbdSimVertex.h"
#include "SimDataFormats/Vertex/interface/EmbdSimVertexContainer.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "SimDataFormats/EcalValidation/interface/PEcalValidInfo.h"

#include "DataFormats/EcalDigi/interface/EBDataFrame.h"
#include "DataFormats/EcalDigi/interface/EEDataFrame.h"
#include "DataFormats/EcalDigi/interface/ESDataFrame.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace cms;
using namespace edm;
using namespace std;

class EcalDigiTask: public EDAnalyzer{

    typedef map<uint32_t,float,less<uint32_t> >  MapType;

public:

/// Constructor
EcalDigiTask(const ParameterSet& ps);

/// Destructor
~EcalDigiTask();

protected:

/// Analyze
void analyze(const Event& e, const EventSetup& c);

// BeginJob
void beginJob(const EventSetup& c);

// EndJob
void endJob(void);

private:

 string HepMCLabel;
 string SimTkLabel;
 string SimVtxLabel;
 
 bool verbose_;
 
 DaqMonitorBEInterface* dbe_;
 
 string outputFile_;

 MonitorElement* meGunEnergy_;
 MonitorElement* meGunEta_;
 MonitorElement* meGunPhi_;   

 MonitorElement* meEBDigiOccupancy_;
 MonitorElement* meEEDigiOccupancy_;

 MonitorElement* meEBDigiADCGlobal_;
 MonitorElement* meEEDigiADCGlobal_;

 MonitorElement* meEBDigiADC_[10];
 MonitorElement* meEEDigiADC_[10];
 MonitorElement* meESDigiADC_[3];

 MonitorElement* meEBDigiGain_[10];
 MonitorElement* meEEDigiGain_[10];

 MonitorElement* meEBPedestal_;
 MonitorElement* meEEPedestal_;

 MonitorElement* meEBMaximum_;
 MonitorElement* meEEMaximum_; 

 MonitorElement* meEBDigiSimRatio_;
 MonitorElement* meEEDigiSimRatio_;

};

#endif
