#ifndef LARLITE_SIMPLEPI0FINDER_CXX
#define LARLITE_SIMPLEPI0FINDER_CXX

#include "SimplePi0Finder.h"

/// ROOT
#include "TVector3.h"

namespace larlite {

  SimplePi0Finder::SimplePi0Finder() {
    _name = "SimplePi0Finder";
    _fout = 0;

    _input_producer = "pandoraNu";
    _input_shower_producer = "showerreco";
    fSinglePi0Tree = nullptr;
  }

  bool SimplePi0Finder::initialize() {

    InitializeSinglePi0Tree();
    return true;
  }
  
  bool SimplePi0Finder::analyze(storage_manager* storage) {
  
    event_pfpart *ev_pfpart = storage->get_data< event_pfpart >( _input_producer );

    // Proceed to write an output data product if any cluster data product exists in the input
    if ( !ev_pfpart or ( ev_pfpart->size() ==0 ) ) {
      print( msg::kWARNING, __FUNCTION__,
             Form( "No input pfparticle! Saving empty event to output ttree..." ) );
      return false;
    }

    // set event ID through storage manager
    _run = ev_pfpart->run();
    _subrun = ev_pfpart->subrun();
    _event = ev_pfpart->event_id();
    storage->set_id( _run, _subrun, _event );
    std::cout << "Run " << _run << ", subrun " << _subrun << ", event "
              << _event << std::endl;  

    std::vector< size_t > InterestedNu;
    InterestedNu.clear();

    for ( size_t ipfp = 0; ipfp < ev_pfpart->size(); ++ipfp ) {
      auto pfpart = (*ev_pfpart)[ipfp];
      if ( pfpart.PdgCode() == 12 || pfpart.PdgCode() == 14 ) {
        int nShowers = 0;
        for ( size_t idaughter = 0; idaughter < pfpart.Daughters().size(); ++idaughter ) {
          size_t daughter = pfpart.Daughters()[idaughter];
          auto daughter_pfpart = (*ev_pfpart)[daughter];
          if ( daughter_pfpart.PdgCode() == 11 ) ++nShowers;
        }
        std::cout << "Number of daughter showers: " << nShowers << std::endl;
        if ( nShowers == 2 ) InterestedNu.push_back( ipfp );
      }
    }

    // std::cout << "Interested pfparticles: ";
    // for ( size_t i = 0; i < InterestedNu.size(); ++i ) std::cout << InterestedNu[i] << ", ";
    // std::cout << std::endl;

    // Retrieve showers
    event_shower *ev_shower = nullptr;
    // associate vector
    AssSet_t ass_showers;
    ass_showers = storage->find_one_ass( ev_pfpart->id(), ev_shower, _input_shower_producer );
    if ( !ev_shower || ev_shower->empty() ) {
      print( msg::kWARNING, __FUNCTION__,
             Form("No shower found associated to PFParticle with producer %s",
             _input_producer.c_str() ) );
      return false;
    }

    // Retrieve vertices
    event_vertex *ev_vertex = nullptr;
    // associate vector
    AssSet_t ass_vertices;
    ass_vertices = storage->find_one_ass( ev_pfpart->id(), ev_vertex, ev_pfpart->name() );
    if ( !ev_vertex || ev_vertex->empty() ) {
      print( msg::kWARNING, __FUNCTION__,
             Form("No vertex found associated to PFParticle with producer %s",
             _input_producer.c_str() ) );
      return false;
    }

    // Loop over InsterestedNu list
    for ( size_t i = 0; i < InterestedNu.size(); ++i ) {
      auto pfpart = (*ev_pfpart)[InterestedNu[i]];
      TVector3 InterPoint( 0., 0., 0. );
      if ( ass_vertices.size() > InterestedNu[i] ) {
        auto ivertex = ass_vertices[InterestedNu[i]];
        auto vertex = (*ev_vertex)[ivertex[0]];
        InterPoint.SetXYZ( vertex.X(), vertex.Y(), vertex.Z() );
      }

      // Loop over the daughters
      int nShowers = 0;
      for ( size_t idaughter = 0; idaughter < pfpart.Daughters().size(); ++idaughter ) {
        size_t daughter = pfpart.Daughters()[idaughter];
        auto daughter_pfpart = (*ev_pfpart)[daughter];
        if ( daughter_pfpart.PdgCode() == 11 ) {
          auto ishowers = ass_showers[daughter];
          if ( ishowers.size() != 1 ) {
            print( msg::kERROR, __FUNCTION__,
                   Form("There are %u showers associated to PFParticle %u",
                         ishowers.size(), daughter ) );
          }
          auto shower = (*ev_shower)[ishowers[0]];
          FillShower( shower, nShowers, InterPoint );
          ++nShowers;
        }
      } // end of idaughter loop

      FillPi0();
    } // end of InterestedNu loop


    return true;
  }

  bool SimplePi0Finder::finalize() {

    fSinglePi0Tree->Write();
    return true;
  }

  void SimplePi0Finder::FillShower( shower& shower, int i, const TVector3& InterPoint ) {

    // Reco vtx
    fSinglePi0TreeParams.reco_x[i] = shower.ShowerStart()[0];
    fSinglePi0TreeParams.reco_y[i] = shower.ShowerStart()[1];
    fSinglePi0TreeParams.reco_z[i] = shower.ShowerStart()[2];

    // Reco angle
    fSinglePi0TreeParams.reco_dcosx[i] = shower.Direction()[0];
    fSinglePi0TreeParams.reco_dcosy[i] = shower.Direction()[1];
    fSinglePi0TreeParams.reco_dcosz[i] = shower.Direction()[2];

    // Reco dca
    const TVector3 c( fSinglePi0TreeParams.reco_x[i], fSinglePi0TreeParams.reco_y[i], fSinglePi0TreeParams.reco_z[i] );
    const TVector3 m( fSinglePi0TreeParams.reco_dcosx[i], fSinglePi0TreeParams.reco_dcosy[i], fSinglePi0TreeParams.reco_dcosz[i] );
    if ( InterPoint.Mag() == 0. ) fSinglePi0TreeParams.reco_dca[i] = -1.;
    else fSinglePi0TreeParams.reco_dca[i] = DCAPointToLine( InterPoint, c, m );

    // Reco energy & dedx info
    int best_plane = shower.best_plane();
    if ((best_plane < 0) or (best_plane > 2))
      best_plane = 2;
    fSinglePi0TreeParams.best_plane_id[i] = best_plane;

    fSinglePi0TreeParams.reco_energy[i]   = shower.Energy_v().at(best_plane);
    fSinglePi0TreeParams.reco_energy_U[i] = shower.Energy_v().at(0);
    fSinglePi0TreeParams.reco_energy_V[i] = shower.Energy_v().at(1);
    fSinglePi0TreeParams.reco_energy_Y[i] = shower.Energy_v().at(2);

    fSinglePi0TreeParams.reco_dqdx[i]     = shower.dQdx_v().at(best_plane);
    fSinglePi0TreeParams.reco_dqdx_U[i]   = shower.dQdx_v().at(0);
    fSinglePi0TreeParams.reco_dqdx_V[i]   = shower.dQdx_v().at(1);
    fSinglePi0TreeParams.reco_dqdx_Y[i]   = shower.dQdx_v().at(2);

    fSinglePi0TreeParams.reco_dedx[i]     = shower.dEdx_v().at(best_plane);
    fSinglePi0TreeParams.reco_dedx_U[i]   = shower.dEdx_v().at(0);
    fSinglePi0TreeParams.reco_dedx_V[i]   = shower.dEdx_v().at(1);
    fSinglePi0TreeParams.reco_dedx_Y[i]   = shower.dEdx_v().at(2);

    return;
  }

  void SimplePi0Finder::FillPi0() {

    fSinglePi0TreeParams.n_recoshowers = 2;
    TVector3 dir1( fSinglePi0TreeParams.reco_dcosx[0], fSinglePi0TreeParams.reco_dcosy[0], fSinglePi0TreeParams.reco_dcosz[0] );
    TVector3 dir2( fSinglePi0TreeParams.reco_dcosx[1], fSinglePi0TreeParams.reco_dcosy[1], fSinglePi0TreeParams.reco_dcosz[1] );
    fSinglePi0TreeParams.RecoTheta = dir1.Angle( dir2 );
    fSinglePi0TreeParams.RecoCosTheta = cos( fSinglePi0TreeParams.RecoTheta );
    std::cout << "Reco'ed energy: ( " << fSinglePi0TreeParams.reco_energy[0] << ", " <<
                 fSinglePi0TreeParams.reco_energy[1] << " )" << std::endl;
    fSinglePi0TreeParams.RecoPi0Mass = sqrt( 2.*fSinglePi0TreeParams.reco_energy[0]*fSinglePi0TreeParams.reco_energy[1] * ( 1. - fSinglePi0TreeParams.RecoCosTheta ) );

    fSinglePi0Tree->Fill();
    return;
  }

  double SimplePi0Finder::DCAPointToLine( const TVector3& p, const TVector3& c, const TVector3& m ) {
    TVector3 diff = c - p;
    double t = -1.*diff*m/ m.Mag2();
    TVector3 dca = c + t*m - p;
    return dca.Mag();
  }

  void SimplePi0Finder::InitializeSinglePi0Tree() {

    if ( fSinglePi0Tree ) delete fSinglePi0Tree;
    fSinglePi0Tree = new TTree("fSinglePi0Tree", "");

    // event information
    fSinglePi0Tree->Branch( "event", &_event, "event/I");
    fSinglePi0Tree->Branch( "run", &_run, "run/I");
    fSinglePi0Tree->Branch( "subrun", &_subrun, "subrun/I");
    fSinglePi0Tree->Branch( "n_recoshowers", &fSinglePi0TreeParams.n_recoshowers, "n_recoshowers/I");
    fSinglePi0Tree->Branch( "RecoPi0Mass", &fSinglePi0TreeParams.RecoPi0Mass, "RecoPi0Mass/D" );
    fSinglePi0Tree->Branch( "RecoCosTheta", &fSinglePi0TreeParams.RecoCosTheta, "RecoCosTheta/D" );
    fSinglePi0Tree->Branch( "RecoTheta", &fSinglePi0TreeParams.RecoTheta, "RecoTheta/D" );

    fSinglePi0Tree->Branch( "reco_dca", &fSinglePi0TreeParams.reco_dca, "reco_dca[2]/D" );
    fSinglePi0Tree->Branch( "reco_x", &fSinglePi0TreeParams.reco_x, "reco_x[2]/D");
    fSinglePi0Tree->Branch( "reco_y", &fSinglePi0TreeParams.reco_y, "reco_y[2]/D");
    fSinglePi0Tree->Branch( "reco_z", &fSinglePi0TreeParams.reco_z, "reco_z[2]/D");
    fSinglePi0Tree->Branch( "reco_dcosx", &fSinglePi0TreeParams.reco_dcosx, "reco_dcosx[2]/D");
    fSinglePi0Tree->Branch( "reco_dcosy", &fSinglePi0TreeParams.reco_dcosy, "reco_dcosy[2]/D");
    fSinglePi0Tree->Branch( "reco_dcosz", &fSinglePi0TreeParams.reco_dcosz, "reco_dcosz[2]/D");
    fSinglePi0Tree->Branch( "reco_energy", &fSinglePi0TreeParams.reco_energy, "reco_energy[2]/D");
    fSinglePi0Tree->Branch( "reco_energy_U", &fSinglePi0TreeParams.reco_energy_U, "reco_energy_U[2]/D");
    fSinglePi0Tree->Branch( "reco_energy_V", &fSinglePi0TreeParams.reco_energy_V, "reco_energy_V[2]/D");
    fSinglePi0Tree->Branch( "reco_energy_Y", &fSinglePi0TreeParams.reco_energy_Y, "reco_energy_Y[2]/D");
    fSinglePi0Tree->Branch( "reco_dqdx", &fSinglePi0TreeParams.reco_dqdx, "reco_dqdx[2]/D");
    fSinglePi0Tree->Branch( "reco_dqdx_U", &fSinglePi0TreeParams.reco_dqdx_U, "reco_dqdx_U[2]/D");
    fSinglePi0Tree->Branch( "reco_dqdx_V", &fSinglePi0TreeParams.reco_dqdx_V, "reco_dqdx_V[2]/D");
    fSinglePi0Tree->Branch( "reco_dqdx_Y", &fSinglePi0TreeParams.reco_dqdx_Y, "reco_dqdx_Y[2]/D");

    fSinglePi0Tree->Branch( "reco_dedx", &fSinglePi0TreeParams.reco_dedx, "reco_dedx[2]/D");
    fSinglePi0Tree->Branch( "reco_dedx_U", &fSinglePi0TreeParams.reco_dedx_U, "reco_dedx_U[2]/D");
    fSinglePi0Tree->Branch( "reco_dedx_V", &fSinglePi0TreeParams.reco_dedx_V, "reco_dedx_V[2]/D");
    fSinglePi0Tree->Branch( "reco_dedx_Y", &fSinglePi0TreeParams.reco_dedx_Y, "reco_dedx_Y[2]/D");

    return;
  }
}
#endif
