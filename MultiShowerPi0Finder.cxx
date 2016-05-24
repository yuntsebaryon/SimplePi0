#ifndef LARLITE_MULTISHOWERPI0FINDER_CXX
#define LARLITE_MULTISHOWERPI0FINDER_CXX

#include "MultiShowerPi0Finder.h"

namespace larlite {

  MultiShowerPi0Finder::MultiShowerPi0Finder() {
    _name = "MultiShowerPi0Finder";
    _fout = 0;

    _input_producer = "pandoraNu";
    _input_shower_producer = "showerreco";
    fPi0Tree = nullptr;
  }

  bool MultiShowerPi0Finder::initialize() {

    InitializePi0Tree();
    return true;
  }
  
  bool MultiShowerPi0Finder::analyze(storage_manager* storage) {

    Reset();
  
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
        if ( nShowers > 1 ) InterestedNu.push_back( ipfp );
      }
    } 

    if ( InterestedNu.size() > 1 ) {
      print( msg::kWARNING, __FUNCTION__,
             Form("Multiple neutrino PFParticles are found with producer %s",
             _input_producer.c_str() ) );
      return false;
    }
 
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

    // Find the interaction point
    auto pfpart = (*ev_pfpart)[InterestedNu[0]];
    TVector3 InterPoint( 0., 0., 0. );
    if ( ass_vertices.size() > InterestedNu[0] ) {
      auto ivertex = ass_vertices[InterestedNu[0]];
      auto vertex = (*ev_vertex)[ivertex[0]];
      InterPoint.SetXYZ( vertex.X(), vertex.Y(), vertex.Z() );
      fPi0TreeParams.reco_nu_x = vertex.X();
      fPi0TreeParams.reco_nu_y = vertex.Y();
      fPi0TreeParams.reco_nu_z = vertex.Z();
    }

    // Fill the shower list
    std::vector< size_t > showerList;
    for ( size_t ipfp = 0; ipfp < ev_pfpart->size(); ++ipfp ) {
      auto & pfp = (*ev_pfpart)[ipfp];
      if ( ( std::find( InterestedNu.begin(), InterestedNu.end(), pfp.Parent() ) != InterestedNu.end() ) 
           && ( pfp.PdgCode() == 11 ) ) showerList.push_back( ipfp );
    }

    // Order by shower energy
    std::sort( showerList.begin(), showerList.end(), [ &ev_shower, &ass_showers ]( const size_t a, const size_t b )
               { size_t ishowera = ass_showers[a][0]; size_t ishowerb = ass_showers[b][0];
                 return (*ev_shower)[ishowera].Energy(-1) > (*ev_shower)[ishowerb].Energy(-1); } );

    fPi0TreeParams.n_recoshowers = (int)showerList.size();

    // Fill shower information
    for ( size_t i = 0; i < showerList.size(); ++ i ) {
      size_t ipfp = showerList[i];
      size_t ishr = ass_showers[ipfp][0];
      auto shower = ( *ev_shower )[ishr];
      FillShower( shower, i, InterPoint );
    }

    // Loop over the pi0 combination
    size_t npi0 = 0;
    for ( size_t i = 0; i < showerList.size() - 1; ++i ) {
      for ( size_t j = i+1; j < showerList.size(); ++j ) {
        FillPi0( i, j, npi0 );
        npi0 += 1;
      }
    }
    // std::cout << "Number of pi0 combination: " << npi0 << std::endl;

    // Find the best combination by DCA
    fPi0TreeParams.best_pi0 = 0;
    double min = 10000.;
    for ( size_t ipi0 = 0; ipi0 < npi0; ++ipi0 ) {
      TVector3 p( fPi0TreeParams.reco_interp_x[ipi0], fPi0TreeParams.reco_interp_y[ipi0], fPi0TreeParams.reco_interp_z[ipi0] );
      TVector3 diff = p - InterPoint;
      std::cout << "Combination " << ipi0 << ": " << diff.Mag() << std::endl;
      if ( ipi0 == 0 ) { min = diff.Mag(); continue; }
      if ( diff.Mag() < min ) {
        min = diff.Mag();
        fPi0TreeParams.best_pi0 = ipi0;
      }
    }

    fPi0Tree->Fill();

    return true;
  }

  bool MultiShowerPi0Finder::finalize() {

    fPi0Tree->Write();
    return true;
  }

  void MultiShowerPi0Finder::FillShower( shower& shower, size_t i, TVector3& InterPoint ) {

    // Reco vtx
    fPi0TreeParams.reco_x[i] = shower.ShowerStart()[0];
    fPi0TreeParams.reco_y[i] = shower.ShowerStart()[1];
    fPi0TreeParams.reco_z[i] = shower.ShowerStart()[2];

    // Reco angle
    fPi0TreeParams.reco_dcosx[i] = shower.Direction()[0];
    fPi0TreeParams.reco_dcosy[i] = shower.Direction()[1];
    fPi0TreeParams.reco_dcosz[i] = shower.Direction()[2];

    // Reco dca
    const TVector3 c( fPi0TreeParams.reco_x[i], fPi0TreeParams.reco_y[i], fPi0TreeParams.reco_z[i] );
    const TVector3 m( fPi0TreeParams.reco_dcosx[i], fPi0TreeParams.reco_dcosy[i], fPi0TreeParams.reco_dcosz[i] );
    if ( InterPoint.Mag() == 0. ) fPi0TreeParams.reco_dca[i] = -1.;
    else fPi0TreeParams.reco_dca[i] = _pi0_helper.DCAPointToLine( InterPoint, c, m );

    // Reco energy & dedx info
    int best_plane = shower.best_plane();
    if ((best_plane < 0) or (best_plane > 2))
      best_plane = 2;
    fPi0TreeParams.best_plane_id[i] = best_plane;

    fPi0TreeParams.reco_energy[i]   = shower.Energy_v().at(best_plane);
    fPi0TreeParams.reco_energy_U[i] = shower.Energy_v().at(0);
    fPi0TreeParams.reco_energy_V[i] = shower.Energy_v().at(1);
    fPi0TreeParams.reco_energy_Y[i] = shower.Energy_v().at(2);

    fPi0TreeParams.reco_dqdx[i]     = shower.dQdx_v().at(best_plane);
    fPi0TreeParams.reco_dqdx_U[i]   = shower.dQdx_v().at(0);
    fPi0TreeParams.reco_dqdx_V[i]   = shower.dQdx_v().at(1);
    fPi0TreeParams.reco_dqdx_Y[i]   = shower.dQdx_v().at(2);

    fPi0TreeParams.reco_dedx[i]     = shower.dEdx_v().at(best_plane);
    fPi0TreeParams.reco_dedx_U[i]   = shower.dEdx_v().at(0);
    fPi0TreeParams.reco_dedx_V[i]   = shower.dEdx_v().at(1);
    fPi0TreeParams.reco_dedx_Y[i]   = shower.dEdx_v().at(2);

    return;
  }

  void MultiShowerPi0Finder::FillPi0( size_t i, size_t j, size_t ipi0 ) {

    fPi0TreeParams.reco_showera[ipi0] = (int)i;
    fPi0TreeParams.reco_showerb[ipi0] = (int)j;

    TVector3 dir1( fPi0TreeParams.reco_dcosx[i], fPi0TreeParams.reco_dcosy[i], fPi0TreeParams.reco_dcosz[i] );
    TVector3 dir2( fPi0TreeParams.reco_dcosx[j], fPi0TreeParams.reco_dcosy[j], fPi0TreeParams.reco_dcosz[j] );
    fPi0TreeParams.RecoTheta[ipi0] = dir1.Angle( dir2 );
    fPi0TreeParams.RecoCosTheta[ipi0] = cos( fPi0TreeParams.RecoTheta[ipi0] );
    std::cout << "Reco'ed energy: ( " << fPi0TreeParams.reco_energy[i] << ", " <<
                 fPi0TreeParams.reco_energy[j] << " )" << std::endl;
    fPi0TreeParams.RecoPi0Mass[ipi0] = sqrt( 2.*fPi0TreeParams.reco_energy[i]*fPi0TreeParams.reco_energy[j] * ( 1. - fPi0TreeParams.RecoCosTheta[ipi0] ) );

    TVector3 c1( fPi0TreeParams.reco_x[i], fPi0TreeParams.reco_y[i], fPi0TreeParams.reco_z[i] );
    TVector3 c2( fPi0TreeParams.reco_x[j], fPi0TreeParams.reco_y[j], fPi0TreeParams.reco_z[j] );
    TVector3 p;
    fPi0TreeParams.RecoPairDCA[ipi0] = _pi0_helper.DCALineToLine( c1, dir1, c2, dir2, p );
    fPi0TreeParams.reco_interp_x[ipi0] = p[0];
    fPi0TreeParams.reco_interp_y[ipi0] = p[1];
    fPi0TreeParams.reco_interp_z[ipi0] = p[2];

    fPi0TreeParams.reco_showera_dca_interp[ipi0] = _pi0_helper.DCAPointToLine( p, c1, dir1 );
    fPi0TreeParams.reco_showerb_dca_interp[ipi0] = _pi0_helper.DCAPointToLine( p, c2, dir2 );

    return;
  }

  void MultiShowerPi0Finder::InitializePi0Tree() {

    if ( fPi0Tree ) delete fPi0Tree;
    fPi0Tree = new TTree("fPi0Tree", "");

    // event information
    fPi0Tree->Branch( "event", &_event, "event/I");
    fPi0Tree->Branch( "run", &_run, "run/I");
    fPi0Tree->Branch( "subrun", &_subrun, "subrun/I");
    fPi0Tree->Branch( "n_recoshowers", &fPi0TreeParams.n_recoshowers, "n_recoshowers/I");
    fPi0Tree->Branch( "RecoPi0Mass", &fPi0TreeParams.RecoPi0Mass, "RecoPi0Mass[3]/D" );
    fPi0Tree->Branch( "RecoCosTheta", &fPi0TreeParams.RecoCosTheta, "RecoCosTheta[3]/D" );
    fPi0Tree->Branch( "RecoTheta", &fPi0TreeParams.RecoTheta, "RecoTheta[3]/D" );
    fPi0Tree->Branch( "RecoPairDCA", &fPi0TreeParams.RecoPairDCA, "RecoPairDCA[3]/D" );
    fPi0Tree->Branch( "reco_showera", &fPi0TreeParams.reco_showera, "reco_showera[3]/I" );
    fPi0Tree->Branch( "reco_showerb", &fPi0TreeParams.reco_showerb, "reco_showerb[3]/I" );
    fPi0Tree->Branch( "reco_type", &fPi0TreeParams.reco_type, "reco_type[3]/I" );
    fPi0Tree->Branch( "best_pi0", &fPi0TreeParams.best_pi0, "best_pi0/I" );
    fPi0Tree->Branch( "reco_interp_x", &fPi0TreeParams.reco_interp_x, "reco_interp_x[3]/D" );
    fPi0Tree->Branch( "reco_interp_y", &fPi0TreeParams.reco_interp_y, "reco_interp_y[3]/D" );
    fPi0Tree->Branch( "reco_interp_z", &fPi0TreeParams.reco_interp_z, "reco_interp_z[3]/D" );
    fPi0Tree->Branch( "reco_showera_dca_interp", &fPi0TreeParams.reco_showera_dca_interp, "reco_showera_dca_interp[3]/D" );
    fPi0Tree->Branch( "reco_showerb_dca_interp", &fPi0TreeParams.reco_showerb_dca_interp, "reco_showerb_dca_interp[3]/D" );
    fPi0Tree->Branch( "reco_nu_x", &fPi0TreeParams.reco_nu_x, "reco_nu_x/D" );
    fPi0Tree->Branch( "reco_nu_y", &fPi0TreeParams.reco_nu_y, "reco_nu_y/D" );
    fPi0Tree->Branch( "reco_nu_z", &fPi0TreeParams.reco_nu_z, "reco_nu_z/D" );

    fPi0Tree->Branch( "reco_dca", &fPi0TreeParams.reco_dca, "reco_dca[3]/D" );
    fPi0Tree->Branch( "reco_x", &fPi0TreeParams.reco_x, "reco_x[3]/D");
    fPi0Tree->Branch( "reco_y", &fPi0TreeParams.reco_y, "reco_y[3]/D");
    fPi0Tree->Branch( "reco_z", &fPi0TreeParams.reco_z, "reco_z[3]/D");
    fPi0Tree->Branch( "reco_dcosx", &fPi0TreeParams.reco_dcosx, "reco_dcosx[3]/D");
    fPi0Tree->Branch( "reco_dcosy", &fPi0TreeParams.reco_dcosy, "reco_dcosy[3]/D");
    fPi0Tree->Branch( "reco_dcosz", &fPi0TreeParams.reco_dcosz, "reco_dcosz[3]/D");
    fPi0Tree->Branch( "reco_energy", &fPi0TreeParams.reco_energy, "reco_energy[3]/D");
    fPi0Tree->Branch( "reco_energy_U", &fPi0TreeParams.reco_energy_U, "reco_energy_U[3]/D");
    fPi0Tree->Branch( "reco_energy_V", &fPi0TreeParams.reco_energy_V, "reco_energy_V[3]/D");
    fPi0Tree->Branch( "reco_energy_Y", &fPi0TreeParams.reco_energy_Y, "reco_energy_Y[3]/D");
    fPi0Tree->Branch( "reco_dqdx", &fPi0TreeParams.reco_dqdx, "reco_dqdx[3]/D");
    fPi0Tree->Branch( "reco_dqdx_U", &fPi0TreeParams.reco_dqdx_U, "reco_dqdx_U[3]/D");
    fPi0Tree->Branch( "reco_dqdx_V", &fPi0TreeParams.reco_dqdx_V, "reco_dqdx_V[3]/D");
    fPi0Tree->Branch( "reco_dqdx_Y", &fPi0TreeParams.reco_dqdx_Y, "reco_dqdx_Y[3]/D");

    fPi0Tree->Branch( "reco_dedx", &fPi0TreeParams.reco_dedx, "reco_dedx[3]/D");
    fPi0Tree->Branch( "reco_dedx_U", &fPi0TreeParams.reco_dedx_U, "reco_dedx_U[3]/D");
    fPi0Tree->Branch( "reco_dedx_V", &fPi0TreeParams.reco_dedx_V, "reco_dedx_V[3]/D");
    fPi0Tree->Branch( "reco_dedx_Y", &fPi0TreeParams.reco_dedx_Y, "reco_dedx_Y[3]/D");
    fPi0Tree->Branch( "best_plane_id", &fPi0TreeParams.best_plane_id, "best_plane_id[3]/I" );

    return;
  }

  void MultiShowerPi0Finder::Reset() {
    fPi0TreeParams.n_recoshowers = -1;
    fPi0TreeParams.best_pi0 = -1;
    fPi0TreeParams.reco_nu_x = -10000.;
    fPi0TreeParams.reco_nu_y = -10000.;
    fPi0TreeParams.reco_nu_z = -10000.;
    for ( int i = 0; i < 3; ++i ) {
      fPi0TreeParams.RecoPi0Mass[i] = -2.;
      fPi0TreeParams.RecoCosTheta[i] = -2.;
      fPi0TreeParams.RecoTheta[i] = -1.;
      fPi0TreeParams.RecoPairDCA[i] = -1.;
      fPi0TreeParams.reco_showera[i] = -1;
      fPi0TreeParams.reco_showerb[i] = -1;
      fPi0TreeParams.reco_type[i] = -1;
      fPi0TreeParams.reco_interp_x[i] = -10000.;
      fPi0TreeParams.reco_interp_y[i] = -10000.;
      fPi0TreeParams.reco_interp_z[i] = -10000.;
      fPi0TreeParams.reco_showera_dca_interp[i] = -1.;
      fPi0TreeParams.reco_showerb_dca_interp[i] = -1.;
      fPi0TreeParams.reco_dca[i] = -1.;
      fPi0TreeParams.reco_x[i] = -10000.;
      fPi0TreeParams.reco_y[i] = -10000.;
      fPi0TreeParams.reco_z[i] = -10000.;
      fPi0TreeParams.reco_dcosx[i] = 0.;
      fPi0TreeParams.reco_dcosy[i] = 0.;
      fPi0TreeParams.reco_dcosz[i] = 0.;
      fPi0TreeParams.reco_energy[i] = -1.;
      fPi0TreeParams.reco_energy_U[i] = -1.;
      fPi0TreeParams.reco_energy_V[i] = -1.;
      fPi0TreeParams.reco_energy_Y[i] = -1.;
      fPi0TreeParams.reco_dedx[i] = -1.;
      fPi0TreeParams.reco_dqdx[i] = -1.;
      fPi0TreeParams.reco_dqdx_U[i] = -1.;
      fPi0TreeParams.reco_dqdx_V[i] = -1.;
      fPi0TreeParams.reco_dqdx_Y[i] = -1.;
      fPi0TreeParams.reco_dedx_U[i] = -1.;
      fPi0TreeParams.reco_dedx_V[i] = -1.;
      fPi0TreeParams.reco_dedx_Y[i] = -1.;
      fPi0TreeParams.best_plane_id[i] = -1;
    }
    return;
  }
}
#endif
