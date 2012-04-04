/***************************************************************************
                             SRC/MIXMOD/XEMGeneralMatrix.cpp  description
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

#include "XEMGeneralMatrix.h"
#include "XEMDiagMatrix.h"

//------------
// Constructor
//------------
XEMGeneralMatrix::XEMGeneralMatrix(){
  _value = NULL;
  _store = NULL;
  throw wrongConstructorType;
}

XEMGeneralMatrix::XEMGeneralMatrix(int64_t pbDimension, double d):XEMMatrix(pbDimension){  
  _value      = new Matrix(pbDimension, pbDimension);
  _store      = _value->Store();

  _s_storeDim = pbDimension * pbDimension; 
	(*this)=1.0;
}



// copy constructor
XEMGeneralMatrix::XEMGeneralMatrix(XEMGeneralMatrix * A):XEMMatrix(A){
  _value      = new Matrix(_s_pbDimension, _s_pbDimension);
 _store      = _value->Store();
    
  _s_storeDim = _s_pbDimension * _s_pbDimension; 

  recopyTab(A->getStore(), _store, _s_storeDim);
}



//----------
//Destructor
//----------
XEMGeneralMatrix::~XEMGeneralMatrix(){
  if(_value){
    delete _value;
  }
  _value = NULL;
}



double XEMGeneralMatrix::determinant(XEMErrorType errorType){
  throw nonImplementedMethod;
}




void XEMGeneralMatrix::equalToMatrixMultiplyByDouble(XEMMatrix* D, double d){
  throw nonImplementedMethod;
}



void XEMGeneralMatrix::multiply(double * V, int64_t nk, XEMGeneralMatrix * Q){
  int64_t indiceV, indiceQ, indice = 0;
  double * storeQ = Q->getStore();

  for (indiceV = 0; indiceV<_s_pbDimension; indiceV++){
    for (indiceQ = 0; indiceQ<nk; indiceQ++){
      _store[indice] = V[indiceV] * storeQ[indiceQ] + V[indiceV + _s_pbDimension] * storeQ[indiceQ+ _s_pbDimension];
      indice++;
    }
  }
}

double* XEMGeneralMatrix::getDiagonalStore(){
  throw wrongMatrixType;
}


double* XEMGeneralMatrix::getSymmetricStore(){
  throw wrongMatrixType;
}

double* XEMGeneralMatrix::getGeneralStore(){
  return(_store);
}

double XEMGeneralMatrix::getSphericalStore(){
  throw wrongMatrixType;
}


double XEMGeneralMatrix::compute_trace_W_C(XEMMatrix * C){
  throw nonImplementedMethod;
}
void XEMGeneralMatrix::computeShape_as__diag_Ot_this_O(XEMDiagMatrix* & Shape, XEMGeneralMatrix* & Ori, double diviseur){
  throw nonImplementedMethod;
}

void XEMGeneralMatrix::inverse(XEMMatrix * & A){
  throw nonImplementedMethod;
}

void XEMGeneralMatrix::compute_product_Lk_Wk(XEMMatrix* Wk, double  L){
  throw nonImplementedMethod;
}

double XEMGeneralMatrix::norme(double * xMoinsMean){
  throw nonImplementedMethod;
}

// (this) will be A / d
void XEMGeneralMatrix::equalToMatrixDividedByDouble(XEMMatrix * A, double d){
  throw nonImplementedMethod;
} 



// add :  cik * xMoinsMean * xMoinsMean'  to this
void XEMGeneralMatrix::add(double * xMoinsMean, double cik){
  throw nonImplementedMethod;
}

// add : diag( cik * xMoinsMean * xMoinsMean' )  to this
/*void XEMGeneralMatrix::addDiag(double * xMoinsMean, double cik){
  throw nonImplementedMethod;
}*/


  double XEMGeneralMatrix::putSphericalValueInStore(double & store){
   throw wrongMatrixType;
  }

  double XEMGeneralMatrix::addSphericalValueInStore(double & store){
   throw wrongMatrixType;
  }

  double * XEMGeneralMatrix::putDiagonalValueInStore(double * store){
   throw wrongMatrixType;
  }

  double *  XEMGeneralMatrix::addDiagonalValueInStore(double * store){
   throw wrongMatrixType;
  }

  double * XEMGeneralMatrix::putSymmetricValueInStore(double * store){
   throw wrongMatrixType;
  }

  double *  XEMGeneralMatrix::addSymmetricValueInStore(double * store){
   throw wrongMatrixType;
  }


  double* XEMGeneralMatrix::putGeneralValueInStore(double * store){
    for (int64_t p = 0; p<_s_storeDim;p++){
        store[p] = _store[p];
    }
   return(store);
  }


  double* XEMGeneralMatrix::addGeneralValueInStore(double * store){
    for (int64_t p = 0; p<_s_storeDim;p++){
        store[p] += _store[p];
    }
   return(store);
  }
 
  // set the value of (d x Identity) to this  
void XEMGeneralMatrix::operator=(const double& d){
  //throw nonImplementedMethod;
  int64_t indice = 0;
	while (indice<_s_storeDim){
    for (int64_t i=0;i<_s_pbDimension;i++){
      for (int64_t j=0;j<_s_pbDimension;j++){
				if (i==j){
					_store[indice] = d;
				}
				else{
					_store[indice] = 0.0;
				}
				indice++;
			}
		}
	}
}

// divide each element by d
void XEMGeneralMatrix::operator/=(const double& d){
  throw nonImplementedMethod;
}

// multiply each element by d
void XEMGeneralMatrix::operator*=(const double& d){
  throw nonImplementedMethod;
}

void XEMGeneralMatrix::operator=(XEMMatrix * M){
    M->putGeneralValueInStore(_store);
}

//add M to this
void XEMGeneralMatrix::operator+=(XEMMatrix* M){
    M->addGeneralValueInStore(_store);
}


void XEMGeneralMatrix::edit(std::ostream& flux, std::string before, std::string sep,int64_t dim){

  for (int64_t p=0;p<_s_pbDimension;p++){
    flux <<before<< (*_value).SubMatrix((p+1),(p+1),1,dim)<<sep;
   }
}




/// compute this as : multi * (O * S * O' )
void XEMGeneralMatrix::compute_as__multi_O_S_O(double multi, XEMGeneralMatrix* & O, XEMDiagMatrix* & S){
  throw nonImplementedMethod;
}


/// compute this as : (O * S * O' )
//void XEMGeneralMatrix::compute_as_O_S_O(Matrix & O, double* & S_store){
void XEMGeneralMatrix::compute_as_O_S_O(XEMGeneralMatrix* & O, double* & S_store){
  throw nonImplementedMethod;
}

//compute trace of a symmetric matrix
double XEMGeneralMatrix::computeTrace(){
  throw nonImplementedMethod;
}


void XEMGeneralMatrix::computeSVD(XEMDiagMatrix* & S, XEMGeneralMatrix* & O){
  throw nonImplementedMethod;
}



void XEMGeneralMatrix::compute_as_M_tM(XEMGeneralMatrix* M, int64_t d){
  throw nonImplementedMethod;
}


void XEMGeneralMatrix::compute_as_M_V(XEMGeneralMatrix* M, double * V){
  throw nonImplementedMethod;
}






// compute M as : M = ( O * S^{-1} * O' ) * this
void XEMGeneralMatrix::compute_M_as__O_Sinverse_Ot_this(XEMGeneralMatrix & M, XEMGeneralMatrix* & O, XEMDiagMatrix* & S){
  throw nonImplementedMethod;
}


void XEMGeneralMatrix::input(std::ifstream & fi){
  int64_t i,j,r = 0;
 
  for (i=0; i<_s_pbDimension; i++){
    for (j=0; j<_s_pbDimension; j++,r++){
      fi >> _store[r];
    }
  }
}

void XEMGeneralMatrix::input(double ** variances){
  int64_t i,j,r = 0;
  
  for (i=0; i<_s_pbDimension; i++){
    for (j=0; j<_s_pbDimension; j++,r++){
      _store[r]=variances[i][j];
    }
  }
}

void XEMGeneralMatrix::input(std::ifstream & fi, int64_t dim){
  int64_t i,j,r = 0;
 
  for (i=0; i<_s_pbDimension; i++){
    for (j=0; j<dim; j++,r++){
      fi >> _store[r];
    }
    for (j=dim;j<_s_pbDimension;j++,r++){
      _store[r] = 0.0;
    }
  }

}

// gives : det(diag(this))
double XEMGeneralMatrix::detDiag(XEMErrorType errorType){
  throw nonImplementedMethod;
}

// trace( this * O * S^{-1} * O' )
double XEMGeneralMatrix::trace_this_O_Sm1_O(XEMGeneralMatrix* & O, XEMDiagMatrix* & S){
  throw nonImplementedMethod;
}


double**  XEMGeneralMatrix::storeToArray() const{

  int64_t i,j, k=0;
  double** newStore = new double*[_s_pbDimension];
  for (i=0; i<_s_pbDimension ; ++i){
    newStore[i] = new double[_s_pbDimension];
    for (j=0; j<_s_pbDimension ; ++j){
        newStore[i][j] = _store[k];
        k++;
    }
  }

  return newStore;
}

