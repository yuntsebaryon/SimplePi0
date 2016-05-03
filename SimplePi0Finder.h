/**
 * \file SimplePi0Finder.h
 *
 * \ingroup SimplePi0
 * 
 * \brief Class def header for a class SimplePi0Finder
 *
 * @author yuntse
 */

/** \addtogroup SimplePi0

    @{*/

#ifndef LARLITE_SIMPLEPI0FINDER_H
#define LARLITE_SIMPLEPI0FINDER_H

#include "Analysis/ana_base.h"

// Data format
#include "DataFormat/pfpart.h"
#include "DataFormat/shower.h"
#include "DataFormat/vertex.h"

namespace larlite {
  /**
     \class SimplePi0Finder
     User custom analysis class made by SHELL_USER_NAME
   */
  class SimplePi0Finder : public ana_base{
  
  public:

    /// Default constructor
    SimplePi0Finder();

    /// Default destructor
    virtual ~SimplePi0Finder(){}

    /** IMPLEMENT in SimplePi0Finder.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in SimplePi0Finder.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in SimplePi0Finder.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetInputProducer( std::string name ) { _input_producer = name; }

  protected:

    /// Input Producer's Name
    std::string _input_producer;
    std::string _input_shower_producer;

    /// event variables
    int _event, _subrun, _run;

    struct SinglePi0TreeParams_t {
      int n_recoshowers;
      double RecoPi0Mass;
      double RecoCosTheta;
      double RecoTheta;
      double reco_dca[2];
      double reco_x[2], reco_y[2], reco_z[2];
      double reco_dcosx[2], reco_dcosy[2], reco_dcosz[2];
      double reco_energy[2];
      double reco_energy_U[2];
      double reco_energy_V[2];
      double reco_energy_Y[2];
      double reco_dedx[2];
      double reco_dqdx[2];
      double reco_dqdx_U[2];
      double reco_dqdx_V[2];
      double reco_dqdx_Y[2];
      double reco_dedx_U[2];
      double reco_dedx_V[2];
      double reco_dedx_Y[2];
      int    best_plane_id[2];
    } fSinglePi0TreeParams;

    /// Analysis TTree
    TTree *fSinglePi0Tree;

    /// Function to prepare TTree
    void InitializeSinglePi0Tree();

    /// Functions to fill TTree
    void FillShower( shower& shower, int i, const TVector3& InterPoint );
    void FillPi0();

    /// Function to calculate DCA
    double DCAPointToLine( const TVector3& p, const TVector3& c, const TVector3& m );
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
