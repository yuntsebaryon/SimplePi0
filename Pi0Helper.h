/**
 * \file Pi0Helper.h
 *
 * \ingroup SimplePi0
 * 
 * \brief Class def header for a class Pi0Helper
 *
 * @author yuntse
 */

/** \addtogroup SimplePi0

    @{*/
#ifndef PI0HELPER_H
#define PI0HELPER_H

#include <iostream>
// ROOT
#include "TVector3.h"

/**
   \class Pi0Helper
   User defined class Pi0Helper ... these comments are used to generate
   doxygen documentation!
 */
class Pi0Helper{

public:

  /// Default constructor
  Pi0Helper(){}

  /// Default destructor
  ~Pi0Helper(){}

  /// Function to calculate DCA
  double DCAPointToLine( const TVector3& p, const TVector3& c, const TVector3& m );
  double DCALineToLine( const TVector3& c1, const TVector3& m1, const TVector3& c2, const TVector3& m2, TVector3& p );

};

#endif
/** @} */ // end of doxygen group 

