/**
 * \file MultiShowerPi0Finder.h
 *
 * \ingroup SimplePi0
 * 
 * \brief Class def header for a class MultiShowerPi0Finder
 *
 * @author yuntse
 */

/** \addtogroup SimplePi0

    @{*/

#ifndef LARLITE_MULTISHOWERPI0FINDER_H
#define LARLITE_MULTISHOWERPI0FINDER_H

#include "Analysis/ana_base.h"

// Data format
#include "DataFormat/pfpart.h"
#include "DataFormat/shower.h"
#include "DataFormat/vertex.h"

// Helper
#include "Pi0Helper.h"

namespace larlite {
  /**
     \class MultiShowerPi0Finder
     User custom analysis class made by SHELL_USER_NAME
   */
  class MultiShowerPi0Finder : public ana_base{
  
  public:

    /// Default constructor
    MultiShowerPi0Finder();

    /// Default destructor
    virtual ~MultiShowerPi0Finder(){}

    /** IMPLEMENT in MultiShowerPi0Finder.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MultiShowerPi0Finder.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MultiShowerPi0Finder.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetInputProducer( std::string name ) { _input_producer = name; }
    void SetInputShowerProducer( std::string name ) { _input_shower_producer = name; }

  protected:

    /// Input Producer's Name
    std::string _input_producer;
    std::string _input_shower_producer;

    /// Helper
    Pi0Helper _pi0_helper;

    /// event variables
    int _event, _subrun, _run;

    struct Pi0TreeParams_t {
      int n_recoshowers;
      double RecoPi0Mass[3];
      double RecoCosTheta[3];
      double RecoTheta[3];
      double RecoPairDCA[3];
      int    reco_showera[3];
      int    reco_showerb[3];
      int    reco_type[3]; // 0b00: (0, 1):whether from the same neutrino, (2, 3): whether having a reasonable dca
      int    best_pi0;
      double reco_interp_x[3], reco_interp_y[3], reco_interp_z[3];
      double reco_showera_dca_interp[3];
      double reco_showerb_dca_interp[3];
      double reco_nu_x, reco_nu_y, reco_nu_z;
      double reco_dca[3];
      double reco_x[3], reco_y[3], reco_z[3];
      double reco_dcosx[3], reco_dcosy[3], reco_dcosz[3];
      double reco_energy[3];
      double reco_energy_U[3];
      double reco_energy_V[3];
      double reco_energy_Y[3];
      double reco_dedx[3];
      double reco_dqdx[3];
      double reco_dqdx_U[3];
      double reco_dqdx_V[3];
      double reco_dqdx_Y[3];
      double reco_dedx_U[3];
      double reco_dedx_V[3];
      double reco_dedx_Y[3];
      int    best_plane_id[3];
    } fPi0TreeParams;

    /// Analysis TTree
    TTree *fPi0Tree;

    /// Function to prepare TTree
    void InitializePi0Tree();

    /// Function to reset TTree
    void Reset();

    /// Functions to fill TTree
    void FillShower( shower& shower, size_t i, TVector3& InterPoint );
    void FillPi0( size_t i, size_t j, size_t ipi0 );
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
