#include "SimCalorimetry/EcalSimProducers/interface/EcalDigiTester.h"


EcalDigiTester::EcalDigiTester (const edm::ParameterSet& params) 
{
  theParameterMap = new EcalSimParameterMap () ;
  std::cout << "[EcalDigiTester][ctor] etering\n" ;
}


EcalDigiTester::~EcalDigiTester () 
{
  delete theParameterMap ;
}


void EcalDigiTester::analyze (const edm::Event& event, const edm::EventSetup& eventSetup) 
{
//  edm::ESHandle<EcalPedestals> pedHandle ;
//  eventSetup.get<EcalPedestalsRcd> ().get ( pedHandle ) ;
//  theCoder->setPedestals (pedHandle.product ()) ;

  std::cout << "[EcalDigiTester][analyze] ----------------------------\n" ;

  checkCalibrations (eventSetup) ;
  checkGeometry (eventSetup) ;

  // Get the hits
  // ------------
  
  edm::Handle<CrossingFrame> crossingFrame ;
  event.getByType (crossingFrame) ;

  // access to SimHits
  const std::string barrelHitsName ("EcalHitsEB") ;
  const std::string endcapHitsName ("EcalHitsEE") ;

  std::auto_ptr<MixCollection<PCaloHit> > 
    barrelHits (new MixCollection<PCaloHit>(crossingFrame.product (), barrelHitsName)) ;
//  std::auto_ptr<MixCollection<PCaloHit> > 
//    endcapHits (new MixCollection<PCaloHit> (crossingFrame.product (), endcapHitsName)) ;

  // Get the digis
  // -------------
     
  edm::Handle<EBDigiCollection> barrelResultHandle ;
  event.getByType (barrelResultHandle) ;
 
  // pring results
  // -------------

  const EBDigiCollection * barrelResult = barrelResultHandle.product () ;

  edm::LogInfo ("EcalDigiTester") << "EB Digis: " << barrelResult->size () ;

  CaloDigiCollectionSorter sorter (5) ;
  std::vector<EBDataFrame> sortedDigis = sorter.sortedVector (*barrelResult) ;
  std::cout << "Top 10 EB digis" << std::endl ;
  for (int i = 0 ; i < std::min (10, (int) sortedDigis.size ()) ; ++i) 
   {
    std::cout << "[EcalDigiTester][analyze] digi " << i
              << "\t" << sortedDigis[i] ;
   }

}


void  EcalDigiTester::checkCalibrations (const edm::EventSetup & eventSetup) 
{}


void EcalDigiTester::checkGeometry (const edm::EventSetup & eventSetup) 
{
  // TODO find a way to avoid doing this every event
  edm::ESHandle<CaloGeometry> geometry ;
  eventSetup.get<IdealGeometryRecord> ().get (geometry) ;

  theBarrelDets.clear () ;
  theEndcapDets.clear () ;

  theBarrelDets =  geometry->getValidDetIds (DetId::Ecal, EcalBarrel) ;
  theEndcapDets =  geometry->getValidDetIds (DetId::Ecal, EcalEndcap) ;

  //PG FIXME
  std::cout << "deb geometry: "
            << "\t barrel: " << theBarrelDets.size () 
            << "\t endcap: " << theEndcapDets.size () 
            << std::endl ;

}


