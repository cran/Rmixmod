/***************************************************************************
                             SRC/MIXMOD/XEMSphericalMatrix.h  description
    copyright            : (C) MIXMOD Team - 2001-2011
    email                : contact@mixmod.org
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

    All informations available on : http://www.mixmod.org                                                                                               
***************************************************************************/
#ifndef XEMSPHERICALMATRIX_H
#define XEMSPHERICALMATRIX_H

#include "XEMMatrix.h"

// pre-declaration
class XEMGeneralMatrix;
class XEMDiagMatrix;

/**
  @brief class XEMSphericalMatrix
  @author F Langrognet & A Echenim
*/

class XEMSphericalMatrix : public XEMMatrix{

public:

  /// Default constructor
  XEMSphericalMatrix();

	/// contructor
	/// default initialisation : Id
  XEMSphericalMatrix(int64_t pbDimension, double initValue=1.0);

  XEMSphericalMatrix(XEMSphericalMatrix * A);

  /// Desctructor
  virtual ~XEMSphericalMatrix();

  /// compute determinant of spherical matrix
  double determinant(XEMErrorType errorType);

  /// return store of spherical matrix
  double getStore();


  /// inverse spherical matrix
  void inverse(XEMMatrix *  & A);

  void compute_product_Lk_Wk(XEMMatrix* Wk, double  L);

  /// add a to the value of this
  void addToValue(double a);

  /// compute (x - mean)' this (x - mean) 
  double norme(double * xMoinsMean);

  /// (this) will be A / d
  void equalToMatrixDividedByDouble(XEMMatrix * A, double d);

  /// (this) will be A * d
  void equalToMatrixMultiplyByDouble(XEMMatrix*D, double d);

  /// compute trace of spherical matrix
  double computeTrace();


  /// add :  cik * xMoinsMean * xMoinsMean'  to this
  void add(double * xMoinsMean, double cik);

  /// add : diag( cik * xMoinsMean * xMoinsMean' )  to this
  void addDiag(double * xMoinsMean, double cik);

  /// this  = d * Identity
  void operator=(const double& d);

  /// this = this / (d * Identity)
  void operator/=(const double& d);

  /// this = this *  (d * Identity)
  void operator*=(const double& d);
  /// this = this + matrix
  void operator+=(XEMMatrix* M);
  /// this = matrix
  void operator=(XEMMatrix* M);


  /// read spherical matrix store in file
  void input(std::ifstream & fi);
  virtual void input( double ** variances );
  
  /// return store of a spherical matrix
  double putSphericalValueInStore(double & store);
  /// add store of a spherical matrix
  double addSphericalValueInStore(double & store);

  double getSphericalStore();

  /// Return store of a diagonal matrix
  double* putDiagonalValueInStore(double * store);
  /// Add store of a diagonal matrix in a diagonal one
  double* addDiagonalValueInStore(double * store);

  double* getDiagonalStore();

  /// Return store of a diagonal matrix
  double* putSymmetricValueInStore(double * store);
  /// Add store of a diagonal matrix in a diagonal one
  double* addSymmetricValueInStore(double * store);

  double* getSymmetricStore();

  /// Return store of a diagonal matrix
  double* putGeneralValueInStore(double * store);
  /// Add store of a diagonal matrix in a diagonal one
  double* addGeneralValueInStore(double * store);

  double* getGeneralStore();


/// compute general matrix SVD decomposition
  void computeSVD(XEMDiagMatrix* & S, XEMGeneralMatrix* & O);

  void compute_as__multi_O_S_O(double multi, XEMGeneralMatrix* & O, XEMDiagMatrix *& S);
  double trace_this_O_Sm1_O(XEMGeneralMatrix* & O, XEMDiagMatrix* & S);
  double compute_trace_W_C(XEMMatrix * C);
  void computeShape_as__diag_Ot_this_O(XEMDiagMatrix* & Shape, XEMGeneralMatrix* & Ori, double diviseur = 1.0);
  /// gives : det(diag(this))
  double detDiag(XEMErrorType errorType);

   void setSymmetricStore(double * store);
   void setGeneralStore(double * store);
   void setDiagonalStore(double * store);
   void setSphericalStore(double store);
  double** storeToArray() const;

protected :

  double _store;
};

inline double XEMSphericalMatrix::getStore(){
  return _store;
}


inline void XEMSphericalMatrix::setSymmetricStore(double * store){
  throw wrongMatrixType;
}
inline void XEMSphericalMatrix::setSphericalStore(double store){
  _store = store;
}
inline void XEMSphericalMatrix::setGeneralStore(double * store){
  throw wrongMatrixType;
}
inline void XEMSphericalMatrix::setDiagonalStore(double * store){
  throw wrongMatrixType;
}

#endif
