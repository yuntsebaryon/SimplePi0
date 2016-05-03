#ifndef LARLITE_SIMPLENUFINDER_CXX
#define LARLITE_SIMPLENUFINDER_CXX

#include "SimpleNuFinder.h"

namespace larlite {

  SimpleNuFinder::SimpleNuFinder() {
    _name = "SimpleNuFinder";
    _fout = 0;

    _input_producer = "pandoraNu";
    _output_producer = "selectNu";
  }

  bool SimpleNuFinder::initialize() {

    return true;
  }
  
  bool SimpleNuFinder::analyze(storage_manager* storage) {
  
    event_pfpart *ev_pfpart = storage->get_data< event_pfpart >( _input_producer );

    // Proceed to write an output data product if any cluster data product exists in the input
    if ( !ev_pfpart or ( ev_pfpart->size() ==0 ) ) {
      print( msg::kWARNING, __FUNCTION__,
             Form( "No input pfparticle! Saving empty event to output ttree..." ) );
      return false;
    }

    // set event ID through storage manager
    storage->set_id( ev_pfpart->run(), ev_pfpart->subrun(), ev_pfpart->event_id() );
    std::cout << "Run " << ev_pfpart->run() << ", subrun " << ev_pfpart->subrun() << ", event " 
              << ev_pfpart->event_id() << std::endl;

    event_pfpart *sel_pfpart = storage->get_data< event_pfpart >( _output_producer );
    if ( !sel_pfpart ) {
      print( msg::kERROR, __FUNCTION__,
             Form("Failed initializing the output pfparticles data product with producer name %s!", _output_producer.c_str() ) );
      return false;
    }
    sel_pfpart->clear();

    event_vertex *ev_vertex = nullptr;
    AssSet_t ass_vertex;
    ass_vertex = storage->find_one_ass( ev_pfpart->id(), ev_vertex, ev_pfpart->name() );

    int nMuons = 0, nElectrons = 0, nNus = 0;
    for ( size_t i = 0; i < ev_pfpart->size(); ++i ) {
      auto pfpart = (*ev_pfpart)[i];
      std::cout << "PFparticle " << i << ": " << pfpart.PdgCode() << "/ " << pfpart.Parent() 
                << "/ ( ";
      for ( size_t idaughter = 0; idaughter < pfpart.Daughters().size(); ++idaughter )
        std::cout << pfpart.Daughters()[idaughter] << ", ";
      std::cout << " )" << std::endl;

      if ( pfpart.PdgCode() == 13 ) {
        ++nMuons;
      } else if ( pfpart.PdgCode() == 11 ) ++nElectrons;
      else if ( pfpart.PdgCode() == 12 || pfpart.PdgCode() == 14 ) ++nNus;
      else std::cout << "PdgCode: " << pfpart.PdgCode() << std::endl;

    }
    std::cout << "Total " << ev_pfpart->size() << " pfparticles" << std::endl;
    std::cout << "Total " << nMuons << " muons" << std::endl;
    std::cout << "Total " << nElectrons << " electrons" << std::endl;
    std::cout << "Total " << nNus << " neutrinos" << std::endl;
    return true;
  }

  bool SimpleNuFinder::finalize() {

    return true;
  }

}
#endif
