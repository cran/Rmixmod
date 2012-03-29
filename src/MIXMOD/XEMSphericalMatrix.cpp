/***************************************************************************
                             SRC/MIXMOD/XEMSphericalMatrix.cpp  description
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

#include "XEMSphericalMatrix.h"
#include "XEMGeneralMatrix.h"
#include "XEMDiagMatrix.h"

//------------
// Constructor
//------------
XEMSphericalMatrix::XEMSphericalMatrix(){
 throw wrongConstructorType;
}


XEMSphericalMatrix::XEMSphericalMatrix(int64_t pbDimension, double initValue):XEMMatrix(pbDimension){
  _store = initValue;
}

XEMSphericalMatrix::XEMSphericalMatrix(XEMSphericalMatrix * A):XEMMatrix(A){
  _store = A->getStore();
}


//----------
//Destructor
//----------
XEMSphericalMatrix::~XEMSphericalMatrix(){
}






double XEMSphericalMatrix::determinant(XEMErrorType errorType){
  double det;
#ifdef __APPLE__
  det = pow(_store, (int)_s_pbDimension);
#else
  det = pow(_store, (int64_t)_s_pbDimension);
#endif
  if(det < minDeterminantValue){
    throw errorType;
  }
  return det;
}

double* XEMSphericalMatrix::getDiagonalStore(){
  throw wrongMatrixType;
}


double* XEMSphericalMatrix::getSymmetricStore(){
  throw wrongMatrixType;
}

double* XEMSphericalMatrix::getGeneralStore(){
  throw wrongMatrixType;
}

double XEMSphericalMatrix::getSphericalStore(){
  return(_store);
}

void XEMSphericalMatrix::computeSVD(XEMDiagMatrix* & S, XEMGeneralMatrix* & O){
  throw nonImplementedMethod;
}


void XEMSphericalMatrix::equalToMatrixMultiplyByDouble(XEMMatrix* D, double d){
  //throw nonImplementedMethod;
	double store_D = D->putSphericalValueInStore(_store);
	_store= store_D*d;
}


void XEMSphericalMatrix::inverse(XEMMatrix * &  Inv){
 
/*1.
  if (Inv == NULL){
    Inv = new XEMSphericalMatrix(_s_pbDimension);
  }
  else{
    if (typeof(Inv) != XEMSphericalMatrix){
      delete Inv;
      Inv = new XEMSphericalMatrix(_s_pbDimension);
    }
  }
*/

  if (Inv == NULL){
    Inv = new XEMSphericalMatrix(_s_pbDimension);
  }

// Inv = new XEMSphericalMatrix(_s_pbDimension);
  double store_Inv = 1.0 / _store;// = A->getSphericalStore();
  Inv->setSphericalStore(store_Inv); // virtual
} 

void XEMSphericalMatrix::compute_product_Lk_Wk(XEMMatrix* Wk, double  L){
  throw nonImplementedMethod;
}

double XEMSphericalMatrix::computeTrace(){
	double trace = _s_pbDimension * _store;
	return trace;
}



void XEMSphericalMatrix::addToValue(double a){
  _store += a;
} 

double XEMSphericalMatrix::norme(double * xMoinsMean){
  int64_t p;
  double termesDiag     = 0.0;
  double xMoinsMean_p;
  
  for(p=0 ; p<_s_pbDimension ; p++){
    xMoinsMean_p = xMoinsMean[p];
    termesDiag += xMoinsMean_p * xMoinsMean_p ;
  }
  termesDiag *= _store;
  return termesDiag;
  
}


// (this) will be A / d
void XEMSphericalMatrix::equalToMatrixDividedByDouble(XEMMatrix * A, double d){
  _store = (A->getSphericalStore()) / d;
} 


  void XEMSphericalMatrix::compute_as__multi_O_S_O(double multi, XEMGeneralMatrix* & O, XEMDiagMatrix *& S){
    throw nonImplementedMethod;
  }
  double XEMSphericalMatrix::trace_this_O_Sm1_O(XEMGeneralMatrix* & O, XEMDiagMatrix* & S){
    throw nonImplementedMethod;
  }
  double XEMSphericalMatrix::compute_trace_W_C(XEMMatrix * C){
    throw nonImplementedMethod;
  }
  void XEMSphericalMatrix::computeShape_as__diag_Ot_this_O(XEMDiagMatrix* & Shape, XEMGeneralMatrix* & Ori, double diviseur){
    throw nonImplementedMethod;
  }









// add :  cik * xMoinsMean * xMoinsMean'  to this
void XEMSphericalMatrix::add(double * xMoinsMean, double cik){
  
  int64_t p;
  double xMoinsMean_p, tmp;

  tmp = 0.0;
  for(p=0; p<_s_pbDimension ; p++){
    xMoinsMean_p = xMoinsMean[p];
    tmp         += xMoinsMean_p * xMoinsMean_p;
  }//end for p
  
  _store += tmp / _s_pbDimension * cik;

}

// add : diag( cik * xMoinsMean * xMoinsMean' )  to this
void XEMSphericalMatrix::addDiag(double * xMoinsMean, double cik){
  
  int64_t p;
	double xMoinsMean_p, tmp;

	tmp = 0.0;
	for(p=0; p<_s_pbDimension ; p++){
	xMoinsMean_p = xMoinsMean[p];
	tmp         += xMoinsMean_p * xMoinsMean_p;
	}//end for p
  
	_store += tmp / _s_pbDimension * cik;
  
}


// set the value of (d x Identity) to this  
void XEMSphericalMatrix::operator=(const double& d){
  _store = d;
}

// divide each element by d
void XEMSphericalMatrix::operator/=(const double& d){
  _store /=d;
}


// multiply each element by d
void XEMSphericalMatrix::operator*=(const double& d){
  _store *=d;
}



//add M to this
void XEMSphericalMatrix::operator+=(XEMMatrix* M){
  M->addSphericalValueInStore(_store);
}

void XEMSphericalMatrix::operator=(XEMMatrix* M){
    M->putSphericalValueInStore(_store);
}


double XEMSphericalMatrix::putSphericalValueInStore(double & store){
        store = _store;
       return(store);
}

double XEMSphericalMatrix::addSphericalValueInStore(double & store){
        store += _store;
       return(store);
}

double* XEMSphericalMatrix::putDiagonalValueInStore(double * store){
    throw wrongMatrixType;
}

double* XEMSphericalMatrix::addDiagonalValueInStore(double * store){
    throw wrongMatrixType;
}

double* XEMSphericalMatrix::putSymmetricValueInStore(double * store){
    throw wrongMatrixType;
}

double* XEMSphericalMatrix::addSymmetricValueInStore(double * store){
    throw wrongMatrixType;
}



double* XEMSphericalMatrix::putGeneralValueInStore(double * store){
   throw wrongMatrixType;
} 

double* XEMSphericalMatrix::addGeneralValueInStore(double * store){
   throw wrongMatrixType;
} 



void XEMSphericalMatrix::input(ifstream & fi){
  int64_t p,q;
  double garbage;
  
    for (p=0; p<_s_pbDimension; p++){
      // useless because all are 0
      for (q=0; q<_s_pbDimension; q++){
        if (p==0 && q==0){
          fi >> _store;
        }
        else{
          fi >> garbage;
        }
      }
    }
  
}
void XEMSphericalMatrix::input(double ** variances){
  int64_t p,q;
  
  for (p=0; p<_s_pbDimension; p++){
    // useless because all are 0
    for (q=0; q<_s_pbDimension; q++){
      if (p==0 && q==0){
        _store=variances[p][q];
      }
    }
  }
  
}

double XEMSphericalMatrix::detDiag(XEMErrorType errorType){
  return determinant(errorType);
}



double**  XEMSphericalMatrix::storeToArray() const{

  int64_t i,j;
  double** newStore = new double*[_s_pbDimension];
  for (i=0; i<_s_pbDimension ; ++i){
    newStore[i] = new double[_s_pbDimension];
  
    for (j=0; j<_s_pbDimension ; ++j){
      if (i == j){
        newStore[i][j] = _store;
      }
      else {
        newStore[i][j] = 0;
      }
    }
  }

  return newStore;

}







