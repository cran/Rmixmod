/***************************************************************************
                             SRC/MIXMOD/XEMGeneralMatrix.h  description
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
#ifndef XEMGENERALMATRIX_H
#define XEMGENERALMATRIX_H


#include "XEMUtil.h"
#include "XEMMatrix.h"

/**
  @brief class GeneralMatrix
  @author F Langrognet & A Echenim
*/

class XEMDiagMatrix;

class XEMGeneralMatrix : public XEMMatrix{

public:

  /// Default constructor
  XEMGeneralMatrix();

	/// cosntructor : d*Id
	///default value = Id
  XEMGeneralMatrix(int64_t pbDimension, double d=1.0);

  XEMGeneralMatrix(XEMGeneralMatrix * A);


  /// Desctructor
  virtual ~XEMGeneralMatrix();

  /// compute determinant of general matrix
  double determinant(XEMErrorType errorType);  

  /// return store of general matrix
  double * getStore();

  /// return newmat general matrix
  Matrix * getValue();

  /// return dimension of store
  int64_t getStoreDim();

  /// inverse general matrix
  void inverse(XEMMatrix *  & A);

  void compute_product_Lk_Wk(XEMMatrix* Wk, double  L);

  /// compute (x - mean)' this (x - mean) 
  double norme(double * xMoinsMean);

  /// this =  A / d
  void equalToMatrixDividedByDouble(XEMMatrix * A, double d);
  /// this =   A * d
  void equalToMatrixMultiplyByDouble(XEMMatrix*D, double d);


  /// add :  cik * xMoinsMean * xMoinsMean'  to this
  void add(double * xMoinsMean, double cik);

  // add : diag( cik * xMoinsMean * xMoinsMean' )  to this
  //void addDiag(double * xMoinsMean, double cik);

  /// return store of a spherical matrix in a general one
  double putSphericalValueInStore(double & store);
  /// add store of a spherical matrix in a general one
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

    /// this =  (d x Identity)
  void operator=(const double& d);

    /// this =  this / (d x Identity)
  void operator/=(const double& d);

    /// this =  this * (d x Identity)
  void operator*=(const double& d);

    /// this =  this + matrix
  void operator+=(XEMMatrix* M);

    /// this =  matrix
  void operator=(XEMMatrix*  M);


  /// edit general matrix
  void edit(ostream& flux, string before,string sep,int64_t dim);

  /// read general matrix from input file
  void input(ifstream & fi);
  /// read general matrix from input file
  void input(ifstream & fi, int64_t dim);

  /// compute general matrix SVD decomposition
  void computeSVD(XEMDiagMatrix* & S, XEMGeneralMatrix* & O);




  /// compute Shape as diag(Ot . this . O ) / diviseur
  void computeShape_as__diag_Ot_this_O(XEMDiagMatrix* & Shape, XEMGeneralMatrix* & Ori, double diviseur = 1.0);

  /// compute this as : multi * (O * S * O' )
  void compute_as__multi_O_S_O(double multi, XEMGeneralMatrix* & O, XEMDiagMatrix *& S);

  /// compute this as O * S *O'
  void compute_as_O_S_O(XEMGeneralMatrix* & O, double* & S_store);

  /// compute trace of this
  double computeTrace();

  /// compute this as matrix * matrix'
  void compute_as_M_tM(XEMGeneralMatrix* M,int64_t d);

  /// compute this as matrix * vector
  void compute_as_M_V(XEMGeneralMatrix* M, double * V);
  /// compute this as vector multiplied by matrix
  void multiply(double * V, int64_t nk, XEMGeneralMatrix * Q);

  /// compute M as : M = ( O * S^{-1} * O' ) * this
  void compute_M_as__O_Sinverse_Ot_this(XEMGeneralMatrix & M, XEMGeneralMatrix* & O, XEMDiagMatrix* & S);
  double compute_trace_W_C(XEMMatrix * C);
//  void computeShape_as__diag_Ot_this_O(XEMDiagMatrix* & Shape, XEMGeneralMatrix* & Ori, double diviseur = 1.0);
  /// gives : det(diag(this))
  double detDiag(XEMErrorType errorType);

  /// trace( this * O * S^{-1} * O' )
  double trace_this_O_Sm1_O(XEMGeneralMatrix* & O, XEMDiagMatrix* & S);

  //void refreshStore();  

   void setSymmetricStore(double * store);
   void setGeneralStore(double * store);
   void setDiagonalStore(double * store);
   void setSphericalStore(double store);

  double** storeToArray() const;


protected :

  Matrix * _value;

  double * _store;

  int64_t  _s_storeDim;  

};
// TODO static :
// int64_t XEMGeneralMatrix::_s_storeDim = 0;

inline double * XEMGeneralMatrix::getStore(){
  return _store;
}


inline Matrix * XEMGeneralMatrix::getValue(){
  return _value;
}

inline int64_t XEMGeneralMatrix::getStoreDim(){
  return _s_storeDim;
}

inline void XEMGeneralMatrix::setSymmetricStore(double * store){
  throw wrongMatrixType;
}
inline void XEMGeneralMatrix::setSphericalStore(double store){
  throw wrongMatrixType;
}
inline void XEMGeneralMatrix::setGeneralStore(double * store){
  //_store = store;
recopyTab(store,_store,_s_storeDim);
}
inline void XEMGeneralMatrix::setDiagonalStore(double * store){
  throw wrongMatrixType;
}



/* TODO static
inline void XEMGeneralMatrix::initiate(){
  _s_storeDim = _s_pbDimension * _s_pbDimension / 2; 
} 
*/


#endif
