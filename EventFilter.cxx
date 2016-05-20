#ifndef LARLITE_EVENTFILTER_CXX
#define LARLITE_EVENTFILTER_CXX

#include "EventFilter.h"

namespace larlite {

  EventFilter::EventFilter() {
    _name = "EventFilter";
    _fout = 0;
    _input_producer = "pandoraNu";
    _event_list.clear();
    _nevents = 0;
  }

  bool EventFilter::initialize() {


    return true;
  }
  
  bool EventFilter::analyze(storage_manager* storage) {

    event_pfpart *ev_pfpart = storage->get_data< event_pfpart >( _input_producer );

    // Proceed to write an output data product if any cluster data product exists in the input
    if ( !ev_pfpart or ( ev_pfpart->size() ==0 ) ) {
      print( msg::kWARNING, __FUNCTION__,
             Form( "No input pfparticle! Saving empty event to output ttree..." ) );
      return false;
    }  

    std::string s = std::to_string( ev_pfpart->run() ) + "_" 
                  + std::to_string( ev_pfpart->subrun() ) + "_"
                  + std::to_string( ev_pfpart->event_id() );

    if ( std::find( _event_list.begin(), _event_list.end(), s ) == _event_list.end() ) return false;
    _nevents += 1;

    return true;
  }

  bool EventFilter::finalize() {
    std::cout << "Number of events passed: " << _nevents << std::endl;
  
    return true;
  }

  void EventFilter::AddAnEvent( unsigned int run, unsigned int subrun, unsigned int event ) {

    std::string s = std::to_string( run ) + "_" + std::to_string( subrun ) + "_" 
                  + std::to_string( event );
    _event_list.push_back( s );

    return;
  }

}
#endif
