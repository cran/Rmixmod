/***************************************************************************
                             SRC/MIXMOD/XEMDiagMatrix.h  description
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
#ifndef XEMDIAGMATRIX_H
#define XEMDIAGMATRIX_H

#include "XEMUtil.h"
#include "XEMMatrix.h"
#include "XEMGeneralMatrix.h"

/**
  @brief class XEMDiagMatrix
  @author F Langrognet & A Echenim
*/

class XEMDiagMatrix : public XEMMatrix{

public:

  /// Default constructor
  XEMDiagMatrix();

	/// contructor : d*Id
	/// default value  = Id
  XEMDiagMatrix(int64_t pbDimension, double d=1.0);

  XEMDiagMatrix(XEMDiagMatrix * A);


  /// Desctructor
  virtual ~XEMDiagMatrix();

  /// compute determinant of diagonal matrix
  double determinant(XEMErrorType errorType);
  /// return store of diagonal matrix
  double * getStore();
  /// compute inverse of diagonal matrix
  void inverse(XEMMatrix *  & A);

  void compute_product_Lk_Wk(XEMMatrix* Wk, double  L);

  /// compute (x - mean)' this (x - mean) 
  double norme(double * xMoinsMean);

  /// (this) will be A / d
  void equalToMatrixDividedByDouble(XEMMatrix * A, double d);

  /// this = matrix * d
  void equalToMatrixMultiplyByDouble(XEMMatrix*D, double d);

///compute singular vector decomposition
   void computeSVD(XEMDiagMatrix* & S, XEMGeneralMatrix* & O);

  /// compute trace of general matrix
 double computeTrace();

  /// add :  cik * xMoinsMean * xMoinsMean'  to this
  void add(double * xMoinsMean, double cik);

  // add : diag( cik * xMoinsMean * xMoinsMean' )  to this
  //void addDiag(double * xMoinsMean, double cik);

  /// set the value of (d x Identity) to this  
  void operator=(const double& d);
  /// this = this / (d * Identity)
  void operator/=(const double& d);
  /// this = this * (d * Identity)
  void operator*=(const double& d);
  /// this = this + matrix
  void operator+=(XEMMatrix* M);
  /// this = matrix
  void operator=(XEMMatrix* M);

  /// Return store of a spherical matrix in a diagonal one
  double putSphericalValueInStore(double & store);
  /// Add store of a spherical matrix in a diagonal one
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

  /// read general matrix in an input file
  void input(ifstream & fi);

  ///set store
   void setSymmetricStore(double * store);
   void setGeneralStore(double * store);
   void setDiagonalStore(double * store);
   void setSphericalStore(double store);
  double** storeToArray() const;

  /// gives : det(diag(this))
  double detDiag(XEMErrorType errorType);

  void compute_as__multi_O_S_O(double multi, XEMGeneralMatrix* & O, XEMDiagMatrix *& S);
  double trace_this_O_Sm1_O(XEMGeneralMatrix* & O, XEMDiagMatrix* & S);
  double compute_trace_W_C(XEMMatrix * C);
  void computeShape_as__diag_Ot_this_O(XEMDiagMatrix* & Shape, XEMGeneralMatrix* & Ori, double diviseur = 1.0);

  ///sort diagonal matrix in decreasing order
  void sortDiagMatrix();
protected :

  double * _store;

};

inline double * XEMDiagMatrix::getStore(){
  return _store;
}

inline void XEMDiagMatrix::setSymmetricStore(double * store){
 throw wrongMatrixType;
}

inline void XEMDiagMatrix::setGeneralStore(double * store){
 throw wrongMatrixType;
}

inline void XEMDiagMatrix::setDiagonalStore(double * store){
  //_store = store;
recopyTab(store,_store,_s_pbDimension);
}

inline void XEMDiagMatrix::setSphericalStore(double store){
 throw wrongMatrixType;
}
#endif
