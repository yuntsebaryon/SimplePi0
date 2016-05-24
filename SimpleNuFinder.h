/**
 * \file SimpleNuFinder.h
 *
 * \ingroup SimplePi0
 * 
 * \brief Class def header for a class SimpleNuFinder
 *
 * @author yuntse
 */

/** \addtogroup SimplePi0

    @{*/

#ifndef LARLITE_SIMPLENUFINDER_H
#define LARLITE_SIMPLENUFINDER_H

#include "Analysis/ana_base.h"

// Data format
#include "DataFormat/vertex.h"
#include "DataFormat/pfpart.h"

namespace larlite {
  /**
     \class SimpleNuFinder
     User custom analysis class made by SHELL_USER_NAME
   */
  class SimpleNuFinder : public ana_base{
  
  public:

    /// Default constructor
    SimpleNuFinder();

    /// Default destructor
    virtual ~SimpleNuFinder(){}

    /** IMPLEMENT in SimpleNuFinder.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in SimpleNuFinder.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in SimpleNuFinder.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetInputProducer( std::string name ) { _input_producer = name; }
    void SetOutputProducer( std::string name ) { _output_producer = name; }

  protected:
    std::string _input_producer;
    std::string _output_producer;
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
