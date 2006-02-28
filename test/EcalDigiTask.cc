/*
 * \file EcalDigiTask.cc
 *
 * $Date: 2006/02/27 08:42:10 $
 * $Revision: 1.1 $
 * \author F. Cossutti
 *
*/

#include <SimCalorimetry/EcalSimProducers/test/EcalDigiTask.h>
#include <DataFormats/EcalDetId/interface/EBDetId.h>
#include <DataFormats/EcalDetId/interface/EEDetId.h>
#include <DataFormats/EcalDetId/interface/ESDetId.h>

EcalDigiTask::EcalDigiTask(const ParameterSet& ps):
  HepMCLabel(ps.getUntrackedParameter("moduleLabelMC",string("PythiaSource"))),
  SimTkLabel(ps.getUntrackedParameter("moduleLabelTk",string("EmbdSimTrack"))),
  SimVtxLabel(ps.getUntrackedParameter("moduleLabelVtx",string("EmbdSimVertex"))){

 
  // DQM ROOT output
  outputFile_ = ps.getUntrackedParameter<string>("outputFile", "");
 
  if ( outputFile_.size() != 0 ) {
    cout << " Ecal Digi Task histograms will be saved to '" << outputFile_.c_str() << "'" << endl;
  } else {
    cout << " Ecal Digi Task histograms will NOT be saved" << endl;
  }
 
  // verbosity switch
  verbose_ = ps.getUntrackedParameter<bool>("verbose", false);
 
  if ( verbose_ ) {
    cout << " verbose switch is ON" << endl;
  } else {
    cout << " verbose switch is OFF" << endl;
  }
                                                                                                                                          
  dbe_ = 0;
                                                                                                                                          
  // get hold of back-end interface
  dbe_ = Service<DaqMonitorBEInterface>().operator->();
                                                                                                                                          
  if ( dbe_ ) {
    if ( verbose_ ) {
      dbe_->setVerbose(1);
    } else {
      dbe_->setVerbose(0);
    }
  }
                                                                                                                                          
  if ( dbe_ ) {
    if ( verbose_ ) dbe_->showDirStructure();
  }
 
  meGunEnergy_ = 0;
  meGunEta_ = 0;   
  meGunPhi_ = 0;   

  meEBDigiOccupancy_ = 0;
  meEEDigiOccupancy_ = 0;

  meEBDigiADCGlobal_ = 0;
  meEEDigiADCGlobal_ = 0;

  for (int i = 0; i < 10 ; i++ ) {
    meEBDigiADC_[i] = 0;
    meEEDigiADC_[i] = 0;
    meEBDigiGain_[i] = 0;
    meEEDigiGain_[i] = 0;
  }

  for (int i = 0; i < 3 ; i++ ) {
    meESDigiADC_[i] = 0;
  }

  meEBPedestal_ = 0;
  meEEPedestal_ = 0;
                                 
  meEBMaximum_ = 0; 
  meEEMaximum_ = 0; 

  meEBDigiSimRatio_ = 0;
  meEEDigiSimRatio_ = 0;
 
  Char_t histo[20];
 
  
  if ( dbe_ ) {
    dbe_->setCurrentFolder("EcalDigiTask");
  
    sprintf (histo, "EcalDigiTask Gun Momentum" ) ;
    meGunEnergy_ = dbe_->book1D(histo, histo, 100, 0., 1000.);
  
    sprintf (histo, "EcalDigiTask Gun Eta" ) ;
    meGunEta_ = dbe_->book1D(histo, histo, 500, -2.5, 2.5);
  
    sprintf (histo, "EcalDigiTask Gun Phi" ) ;
    meGunPhi_ = dbe_->book1D(histo, histo, 360, 0., 360.);

    sprintf (histo, "EcalDigiTask Barrel occupancy" ) ;
    meEBDigiOccupancy_ = dbe_->book2D(histo, histo, 360, 0., 360., 170, -85., 85.);
  
    sprintf (histo, "EcalDigiTask Endcap occupancy" ) ;
    meEEDigiOccupancy_ = dbe_->book2D(histo, histo, 100, 0., 100., 100, 0., 100.);
    
    sprintf (histo, "EcalDigiTask Barrel global pulse shape" ) ;
    meEBDigiADCGlobal_ = dbe_->bookProfile(histo, histo, 10, 0, 10, 4096, 0., 4096.) ;
    
    sprintf (histo, "EcalDigiTask Endcap global pulse shape" ) ;
    meEEDigiADCGlobal_ = dbe_->bookProfile(histo, histo, 10, 0, 10, 4096, 0., 4096.) ;
  
    for (int i = 0; i < 10 ; i++ ) {

      sprintf (histo, "EcalDigiTask Barrel ADC pulse %02d", i+1) ;
      meEBDigiADC_[i] = dbe_->book1D(histo, histo, 4096, 0., 4096);

      sprintf (histo, "EcalDigiTask Endcap ADC pulse %02d", i+1) ;
      meEEDigiADC_[i] = dbe_->book1D(histo, histo, 4096, 0., 4096);

      sprintf (histo, "EcalDigiTask Barrel gain pulse %02d", i+1) ;
      meEBDigiGain_[i] = dbe_->book1D(histo, histo, 4, 0, 4);

      sprintf (histo, "EcalDigiTask Endcap gain pulse %02d", i+1) ;
      meEEDigiGain_[i] = dbe_->book1D(histo, histo, 4, 0, 4);
    }
    
    for ( int i = 0; i < 3 ; i++ ) {
      
      sprintf (histo, "EcalDigiTask Preshower ADC pulse %02d", i+1) ;
      meESDigiADC_[i] = dbe_->book1D(histo, histo, 4096, 0., 4096.) ;
    }

    sprintf (histo, "EcalDigiTask Barrel pedestal for pre-sample" ) ;
    meEBPedestal_ = dbe_->book1D(histo, histo, 4096, 0., 4096.) ;

    sprintf (histo, "EcalDigiTask Endcap pedestal for pre-sample" ) ;
    meEEPedestal_ = dbe_->book1D(histo, histo, 4096, 0., 4096.) ;

    sprintf (histo, "EcalDigiTask Barrel maximum position" ) ;
    meEBMaximum_ = dbe_->book1D(histo, histo, 10, 0., 10.) ;

    sprintf (histo, "EcalDigiTask Endcap maximum position" ) ;
    meEEMaximum_ = dbe_->book1D(histo, histo, 10, 0., 10.) ;

    sprintf (histo, "EcalDigiTask Barrel maximum Digi over Sim ratio" ) ;
    meEBDigiSimRatio_ = dbe_->book1D(histo, histo, 10, 0., 10.) ;

    sprintf (histo, "EcalDigiTask Endcap maximum Digi over Sim ratio" ) ;
    meEEDigiSimRatio_ = dbe_->book1D(histo, histo, 10, 0., 10.) ;

  }
 
}

EcalDigiTask::~EcalDigiTask(){
 
  if ( outputFile_.size() != 0 && dbe_ ) dbe_->save(outputFile_);

}

void EcalDigiTask::beginJob(const EventSetup& c){

}

void EcalDigiTask::endJob(){

}

void EcalDigiTask::analyze(const Event& e, const EventSetup& c){

  vector<EmbdSimTrack> theSimTracks;
  vector<EmbdSimVertex> theSimVertexes;

  Handle<HepMCProduct> MCEvt;
  Handle<EmbdSimTrackContainer> SimTk;
  Handle<EmbdSimVertexContainer> SimVtx;
  Handle<CrossingFrame> crossingFrame;
  Handle<EBDigiCollection> EcalDigiEB;
  Handle<EEDigiCollection> EcalDigiEE;
  Handle<ESDigiCollection> EcalDigiES;

  e.getByLabel(HepMCLabel, MCEvt);
  e.getByLabel(SimTkLabel,SimTk);
  e.getByLabel(SimVtxLabel,SimVtx);
  e.getByType(crossingFrame);
  e.getByType(EcalDigiEB);
  e.getByType(EcalDigiEE);
  e.getByType(EcalDigiES);

  theSimTracks.insert(theSimTracks.end(),SimTk->begin(),SimTk->end());
  theSimVertexes.insert(theSimVertexes.end(),SimVtx->begin(),SimVtx->end());

  HepMC::GenEvent * myGenEvent = new  HepMC::GenEvent(*(MCEvt->GetEvent()));
   
  for ( HepMC::GenEvent::particle_iterator p = myGenEvent->particles_begin();
        p != myGenEvent->particles_end(); ++p ) {

    Hep3Vector hmom = Hep3Vector((*p)->momentum().vect());
    double htheta = hmom.theta();
    double heta = -log(tan(htheta * 0.5));
    double hphi = hmom.phi();
    hphi = (hphi>=0) ? hphi : hphi+2*M_PI;
    hphi = hphi / M_PI * 180.;
    if (verbose_) {
      cout<< "Particle gun type form MC = "<< abs((*p)->pdg_id()) << endl; 
      cout<< "Energy = "<< (*p)->momentum().e() << " Eta = " << heta << " Phi = " << hphi << endl;  
    }

    if (meGunEnergy_) meGunEnergy_->Fill((*p)->momentum().e());
    if (meGunEta_) meGunEta_->Fill(heta);
    if (meGunPhi_) meGunPhi_->Fill(hphi);

  }

  //for (vector<EmbdSimTrack>::iterator isimtk = theSimTracks.begin();
  //     isimtk != theSimTracks.end(); ++isimtk){
  //  cout<<" Track momentum  x = "<<isimtk->momentum().x() <<" y = "<<isimtk->momentum().y() <<" z = "<< isimtk->momentum().z()<<endl;
  //  cout<<" Track momentum Ptx = "<<isimtk->momentum().perp() <<endl;
  //}

  for (vector<EmbdSimVertex>::iterator isimvtx = theSimVertexes.begin();
       isimvtx != theSimVertexes.end(); ++isimvtx){
    if (verbose_ ) {
      cout<<" Vertex position  x = "<<isimvtx->position().x() <<" y = "<<isimvtx->position().y() <<" z = "<< isimvtx->position().z()<<endl;
    }
  }

  // BARREL

  // loop over simHits

  const std::string barrelHitsName ("EcalHitsEB") ;
  std::auto_ptr<MixCollection<PCaloHit> > 
    barrelHits (new MixCollection<PCaloHit>(crossingFrame.product (), barrelHitsName)) ;

  MapType ebSimMap;

  for (MixCollection<PCaloHit>::MixItr hitItr = barrelHits->begin () ;
       hitItr != barrelHits->end () ;
       ++hitItr) {
     
    EBDetId ebid = EBDetId(hitItr->id()) ;

    if ( verbose_ ) {
      std::cout<<" CaloHit " << hitItr->getName() << " DetID = "<<hitItr->id()<<std::endl;	
      std::cout << "Energy = " << hitItr->energy() << " Time = " << hitItr->time() << std::endl;
      std::cout << "EBDetId = " << ebid.ieta() << " " << ebid.iphi() << std::endl;
    }

    uint32_t crystid = ebid.rawId();
    ebSimMap[crystid] += hitItr->energy();

  }

  // loop over Digis
 
  const EBDigiCollection * barrelDigi = EcalDigiEB.product () ;

  for (std::vector<EBDataFrame>::const_iterator digis = barrelDigi->begin () ;
       digis != barrelDigi->end () ;
       ++digis)
    {
      
      EBDetId ebid = digis->id () ;

      if (meEBDigiOccupancy_) meEBDigiOccupancy_->Fill( ebid.iphi(), ebid.ieta() );
 
      double Emax = -1 ;
      int Pmax = -1 ;
      std::vector<double> ebADCCounts ;
      std::vector<double> ebADCGains ;
      double pedestalPreSample = -1.;
      for (int sample = 0 ; sample < digis->size () ; ++sample)
        {
          ebADCCounts.push_back (digis->sample (sample).adc ()) ;
          ebADCGains.push_back (digis->sample (sample).gainId ()) ;
          if (Emax < ebADCCounts[sample] ) {
            Emax = ebADCCounts[sample] ;
            Pmax = sample ;
          }
          if ( sample < 3 ) {
            pedestalPreSample += ebADCCounts[sample] ;
          }
        }
      pedestalPreSample /= 3. ; 
 
      if (verbose_) {
        std::cout << "Barrel Digi for EBDetId = " << ebid.ieta() << " " << ebid.iphi() << std::endl;
        for ( int i = 0; i < 10 ; i++ ) {
          std::cout << "sample " << i << " ADC = " << ebADCCounts[i] << " gain = " << ebADCGains[i] << std::endl;
        }
        std::cout << "Maximum energy = " << Emax << " in sample " << Pmax << std::endl;
      }

      for ( int i = 0 ; i < 10 ; i++ ) {
        if (meEBDigiADCGlobal_) meEBDigiADCGlobal_->Fill( i , ebADCCounts[i] ) ;
        if (meEBDigiADC_[i]) meEBDigiADC_[i]->Fill( ebADCCounts[i] ) ;
        if (meEBDigiGain_[i]) meEBDigiGain_[i]->Fill( ebADCGains[i] ) ;
      }

      if (meEBPedestal_) meEBPedestal_->Fill ( pedestalPreSample ) ;
      if (meEBMaximum_) meEBMaximum_->Fill( Pmax ) ;

      if (meEBDigiSimRatio_) {
        if ( ebSimMap[ebid.rawId()] != 0. ) meEBDigiSimRatio_->Fill( Emax/ebSimMap[ebid.rawId()] ) ; 
      }

    } 

  // ENDCAP

  // loop over simHits

  const std::string endcapHitsName ("EcalHitsEE") ;
  std::auto_ptr<MixCollection<PCaloHit> > 
    endcapHits (new MixCollection<PCaloHit>(crossingFrame.product (), endcapHitsName)) ;

  MapType eeSimMap;

  for (MixCollection<PCaloHit>::MixItr hitItr = endcapHits->begin () ;
       hitItr != endcapHits->end () ;
       ++hitItr) {
     
    EEDetId eeid = EEDetId(hitItr->id()) ;

    if ( verbose_ ) {
      std::cout<<" CaloHit " << hitItr->getName() << " DetID = "<<hitItr->id()<<std::endl;	
      std::cout << "Energy = " << hitItr->energy() << " Time = " << hitItr->time() << std::endl;
      std::cout << "EEDetId = " << eeid.ix() << " " << eeid.iy() << std::endl;
    }

    uint32_t crystid = eeid.rawId();
    eeSimMap[crystid] += hitItr->energy();

  }

  // loop over Digis
 
  const EEDigiCollection * endcapDigi = EcalDigiEE.product () ;

  for (std::vector<EEDataFrame>::const_iterator digis = endcapDigi->begin () ;
       digis != endcapDigi->end () ;
       ++digis)
    {
      
      EEDetId eeid = digis->id () ;

      if (meEEDigiOccupancy_) meEEDigiOccupancy_->Fill( eeid.ix(), eeid.iy() );
 
      double Emax = -1 ;
      int Pmax = -1 ;
      std::vector<double> eeADCCounts ;
      std::vector<double> eeADCGains ;
      double pedestalPreSample = -1.;
      for (int sample = 0 ; sample < digis->size () ; ++sample)
        {
          eeADCCounts.push_back (digis->sample (sample).adc ()) ;
          eeADCGains.push_back (digis->sample (sample).gainId ()) ;
          if (Emax < eeADCCounts[sample] ) {
            Emax = eeADCCounts[sample] ;
            Pmax = sample ;
          }
          if ( sample < 3 ) {
            pedestalPreSample += eeADCCounts[sample] ;
          }
        }
      pedestalPreSample /= 3. ; 
 
      if (verbose_) {
        std::cout << "Endcap Digi for EEDetId = " << eeid.ix() << " " << eeid.iy() << std::endl;
        for ( int i = 0; i < 10 ; i++ ) {
          std::cout << "sample " << i << " ADC = " << eeADCCounts[i] << " gain = " << eeADCGains[i] << std::endl;
        }
        std::cout << "Maximum energy = " << Emax << " in sample " << Pmax << std::endl;
      }

      for ( int i = 0 ; i < 10 ; i++ ) {
        if (meEEDigiADCGlobal_) meEEDigiADCGlobal_->Fill( i , eeADCCounts[i] ) ;
        if (meEEDigiADC_[i]) meEEDigiADC_[i]->Fill( eeADCCounts[i] ) ;
        if (meEEDigiGain_[i]) meEEDigiGain_[i]->Fill( eeADCGains[i] ) ;
      }

      if (meEEPedestal_) meEEPedestal_->Fill ( pedestalPreSample ) ;
      if (meEEMaximum_) meEEMaximum_->Fill( Pmax ) ;

      if (meEEDigiSimRatio_) {
        if ( eeSimMap[eeid.rawId()] != 0. ) meEEDigiSimRatio_->Fill( Emax/eeSimMap[eeid.rawId()] ) ; 
      }

    } 

  // PRESHOWER

  // loop over Digis
 
  const ESDigiCollection * preshowerDigi = EcalDigiES.product () ;

  for (std::vector<ESDataFrame>::const_iterator digis = preshowerDigi->begin () ;
       digis != preshowerDigi->end () ;
       ++digis)
    {
      
      ESDetId esid = digis->id () ;

      std::vector<double> esADCCounts ;
      for (int sample = 0 ; sample < digis->size () ; ++sample)
        {
          esADCCounts.push_back (digis->sample (sample).adc ()) ;
 
          if (verbose_) {
            std::cout << "Preshower Digi for ESDetId: z side " << esid.zside() << "  plane " << esid.plane() << esid.six() << ',' << esid.siy() << ':' << esid.strip() << std::endl;
            for ( int i = 0; i < 3 ; i++ ) {
              std::cout << "sample " << i << " ADC = " << esADCCounts[i] << std::endl;
            }
          }

          for ( int i = 0 ; i < 3 ; i++ ) {
            if (meESDigiADC_[i]) meESDigiADC_[i]->Fill( esADCCounts[i] ) ;
          }

        }
    } 

}

//define this as a plug-in
DEFINE_FWK_MODULE(EcalDigiTask)
                                                                                                                                                             
