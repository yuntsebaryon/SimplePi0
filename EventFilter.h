/**
 * \file EventFilter.h
 *
 * \ingroup SimplePi0
 * 
 * \brief Class def header for a class EventFilter
 *
 * @author yuntse
 */

/** \addtogroup SimplePi0

    @{*/

#ifndef LARLITE_EVENTFILTER_H
#define LARLITE_EVENTFILTER_H

#include "Analysis/ana_base.h"

// Data format
#include "DataFormat/pfpart.h"

namespace larlite {
  /**
     \class EventFilter
     User custom analysis class made by SHELL_USER_NAME
   */
  class EventFilter : public ana_base{
  
  public:

    /// Default constructor
    EventFilter();

    /// Default destructor
    virtual ~EventFilter(){}

    /** IMPLEMENT in EventFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in EventFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in EventFilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetInputProducer( std::string name ) { _input_producer = name; }
    void AddAnEvent( unsigned int run, unsigned int subrun, unsigned int event );

  protected:

    std::string _input_producer;
    std::vector< std::string > _event_list;
    int _nevents;

  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
