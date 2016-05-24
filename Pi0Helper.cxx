#ifndef PI0HELPER_CXX
#define PI0HELPER_CXX

#include "Pi0Helper.h"

double Pi0Helper::DCAPointToLine( const TVector3& p, const TVector3& c, const TVector3& m ) {
  TVector3 diff = c - p;
  double t = -1.*diff*m/ m.Mag2();
  TVector3 dca = c + t*m - p;
  return dca.Mag();
}

double Pi0Helper::DCALineToLine( const TVector3& c1, const TVector3& m1, const TVector3& c2, const TVector3& m2, TVector3& p ) {
  TVector3 w0 = c1 - c2;
  double a = m1 * m1;
  double b = m1 * m2;
  double c = m2 * m2;
  double d = m1 * w0;
  double e = m2 * w0;
  double t1, t2;
  if ( ( a*c - b*b ) == 0. ) {
    t1 = 0.;
    t2 = d / b;
  } else {
    t1 = ( b*e - c*d ) / ( a*c - b*b );
    t2 = ( a*e - b*d ) / ( a*c - b*b );
  }

  TVector3 u0 = c1 + t1* m1;
  TVector3 v0 = c2 + t2* m2;
  TVector3 diff = u0 - v0;
  p = 0.5*( u0 + v0 );

  return diff.Mag();
}

#endif
