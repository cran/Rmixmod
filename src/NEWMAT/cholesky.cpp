/***************************************************************************
                             SRC/NEWMAT/cholesky.cpp  description
                             ------------------------
    copyright            : (C) MIXMOD Team - 2001-2003-2004-2005-2006-2007-2008-2009
    email                : mixmod@univ-fcomte.fr
 ***************************************************************************/

/***************************************************************************
    This file is part of MIXMOD
    
    MIXMOD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MIXMOD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MIXMOD.  If not, see <http://www.gnu.org/licenses/>.     
                                                                          
 ***************************************************************************/
//$$ cholesky.cpp                     cholesky decomposition

// Copyright (C) 1991,2,3,4: R B Davies

#define WANT_MATH
//#define WANT_STREAM

#include "include.h"

#include "newmat.h"

#ifdef use_namespace
namespace NEWMAT {
#endif

#ifdef DO_REPORT
#define REPORT { static ExeCounter ExeCount(__LINE__,14); ++ExeCount; }
#else
#define REPORT {}
#endif

/********* Cholesky decomposition of a positive definite matrix *************/

// Suppose S is symmetrix and positive definite. Then there exists a unique
// lower triangular matrix L such that L L.t() = S;

inline Real square(Real x) { return x*x; }

ReturnMatrix Cholesky(const SymmetricMatrix& S)
{
   REPORT
   Tracer trace("Cholesky");
   int nr = S.Nrows();
   LowerTriangularMatrix T(nr);
   Real* s = S.Store(); Real* t = T.Store(); Real* ti = t;
   for (int i=0; i<nr; i++)
   {
      Real* tj = t; Real sum; int k;
      for (int j=0; j<i; j++)
      {
         Real* tk = ti; sum = 0.0; k = j;
         while (k--) { sum += *tj++ * *tk++; }
         *tk = (*s++ - sum) / *tj++;
      }
      sum = 0.0; k = i;
      while (k--) { sum += square(*ti++); }
      Real d = *s++ - sum;
      if (d<=0.0)  Throw(NPDException(S));
      *ti++ = sqrt(d);
   }
   T.Release(); return T.ForReturn();
}

ReturnMatrix Cholesky(const SymmetricBandMatrix& S)
{
   REPORT
   Tracer trace("Band-Cholesky");
   int nr = S.Nrows(); int m = S.lower;
   LowerBandMatrix T(nr,m);
   Real* s = S.Store(); Real* t = T.Store(); Real* ti = t;

   for (int i=0; i<nr; i++)
   {
      Real* tj = t; Real sum; int l;
      if (i<m) { REPORT l = m-i; s += l; ti += l; l = i; }
      else { REPORT t += (m+1); l = m; }

      for (int j=0; j<l; j++)
      {
         Real* tk = ti; sum = 0.0; int k = j; tj += (m-j);
         while (k--) { sum += *tj++ * *tk++; }
         *tk = (*s++ - sum) / *tj++;
      }
      sum = 0.0;
      while (l--) { sum += square(*ti++); }
      Real d = *s++ - sum;
      if (d<=0.0)  Throw(NPDException(S));
      *ti++ = sqrt(d);
   }

   T.Release(); return T.ForReturn();
}


#ifdef use_namespace
}
#endif

