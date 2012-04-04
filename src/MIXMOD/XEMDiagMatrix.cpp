/***************************************************************************
                             SRC/MIXMOD/XEMDiagMatrix.cpp  description
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
#include "XEMDiagMatrix.h"
#include "XEMGeneralMatrix.h"
#include "XEMSymmetricMatrix.h"

//------------
// Constructor
//------------
XEMDiagMatrix::XEMDiagMatrix(){
  _store = NULL;
  throw wrongConstructorType;
}

XEMDiagMatrix::XEMDiagMatrix(int64_t pbDimension, double d):XEMMatrix(pbDimension){
  _store = new double[_s_pbDimension];
  for (int64_t i=0; i<_s_pbDimension; i++){
		_store[i] = d;
	}
}

XEMDiagMatrix::XEMDiagMatrix(XEMDiagMatrix * A):XEMMatrix(A){
  _store = copyTab(A->getStore(), _s_pbDimension);
}


//----------
//Destructor
//----------
XEMDiagMatrix::~XEMDiagMatrix(){
  if(_store){
    delete[] _store;
  }
  _store = NULL;
}

double XEMDiagMatrix::determinant(XEMErrorType errorType){
  int64_t p;
  double det = _store[0];
  for(p=1 ; p<_s_pbDimension ; p++){
    det *= _store[p];
  }
  
  if(det<minDeterminantValue) 
    throw errorType;
  
  return det;

}




void XEMDiagMatrix::compute_product_Lk_Wk(XEMMatrix* Wk, double  L){
throw nonImplementedMethod;
}


void XEMDiagMatrix::inverse(XEMMatrix * & Inv){
//cout<<"Inv diag :  "<<Inv<<endl;
  if (Inv == NULL){
    Inv = new XEMDiagMatrix(_s_pbDimension);
  }
  double * Inv_store = new double[_s_pbDimension];
  int64_t p;
  for(p=0; p<_s_pbDimension; p++){
    Inv_store[p] = 1.0 / _store[p];
  }

  Inv->setDiagonalStore(Inv_store);

  delete [] Inv_store;
}


double* XEMDiagMatrix::getDiagonalStore(){
  return(_store);
}


double* XEMDiagMatrix::getSymmetricStore(){
  throw wrongMatrixType;
}

double* XEMDiagMatrix::getGeneralStore(){
  throw wrongMatrixType;
}

double XEMDiagMatrix::getSphericalStore(){
  throw wrongMatrixType;
}

double XEMDiagMatrix::norme(double * xMoinsMean){
  int64_t p;
  double termesDiag     = 0.0;
  double xMoinsMean_p;
  
  for(p=0 ; p<_s_pbDimension ; p++){
    xMoinsMean_p  = xMoinsMean[p];
    termesDiag   += xMoinsMean_p * xMoinsMean_p * _store[p];
  }
  return termesDiag;
  
}

  void XEMDiagMatrix::compute_as__multi_O_S_O(double multi, XEMGeneralMatrix* & O, XEMDiagMatrix *& S){
   throw nonImplementedMethod;
  }
  double XEMDiagMatrix::trace_this_O_Sm1_O(XEMGeneralMatrix* & O, XEMDiagMatrix* & S){
    throw nonImplementedMethod;
  }
  double XEMDiagMatrix::compute_trace_W_C(XEMMatrix * C){
    throw nonImplementedMethod;
  }
  void XEMDiagMatrix::computeShape_as__diag_Ot_this_O(XEMDiagMatrix* & Shape, XEMGeneralMatrix* & Ori, double diviseur){
    throw nonImplementedMethod;
  }



  double XEMDiagMatrix::putSphericalValueInStore(double & store){
    store = 0.0;
    int64_t p;
    for(p=0; p<_s_pbDimension; p++){
      store += _store[p];
    }
    store /= _s_pbDimension;
    return(store);
  }


  double XEMDiagMatrix::addSphericalValueInStore(double & store){
    int64_t p;
    for(p=0; p<_s_pbDimension; p++){
      store += _store[p];
    }
    store /= _s_pbDimension;
    return(store);
  }



  double* XEMDiagMatrix::putDiagonalValueInStore(double * store){
    for (int64_t p=0 ; p<_s_pbDimension; p++){
        store[p] = _store[p]; 
    }
    return(store);
  }

  double* XEMDiagMatrix::addDiagonalValueInStore(double * store){
    for (int64_t p=0 ; p<_s_pbDimension; p++){
        store[p] += _store[p]; 
    }
    return(store);
  }

  double* XEMDiagMatrix::addSymmetricValueInStore(double * store){
   // return the store of of a symmetric matrix with this on the diag
    //int64_t dimStore = _s_pbDimension*(_s_pbDimension+1)/2;
   // double * store = new double[dimStore];

    int64_t p,q,r;
    for(p=0,r=0; p<_s_pbDimension; p++,r++){
      for(q=0; q<p ; q++,r++){
        store[r] = 0.0;
      }
      store[r] += _store[p];
    }
    return(store);
  }


  double* XEMDiagMatrix::putSymmetricValueInStore(double * store){
   // return the store of of a symmetric matrix with this on the diag
   // int64_t dimStore = _s_pbDimension*(_s_pbDimension+1)/2;
   // double * store = new double[dimStore];

    int64_t p,q,r;
    for(p=0,r=0; p<_s_pbDimension; p++,r++){
      for(q=0; q<p ; q++,r++){
        store[r] = 0.0;
      }
      store[r] = _store[p];
    }
    return(store);
  }



  double* XEMDiagMatrix::putGeneralValueInStore(double * store){
   throw wrongMatrixType;
  }

  double* XEMDiagMatrix::addGeneralValueInStore(double * store){
   throw wrongMatrixType;
  }


  void XEMDiagMatrix::computeSVD(XEMDiagMatrix* & S, XEMGeneralMatrix* & O){
    throw nonImplementedMethod;
  }

double XEMDiagMatrix::computeTrace(){

  double trace = 0.0;
  for (int64_t i=0;i<_s_pbDimension;i++){
    trace += _store[i]; 
  }
  return trace;
}



// (this) will be A / d
void XEMDiagMatrix::equalToMatrixDividedByDouble(XEMMatrix * A, double d){
    A->putDiagonalValueInStore(_store);

    int64_t p;
  for(p=0; p<_s_pbDimension; p++){
    _store[p] /= d;
  }
} 


void XEMDiagMatrix::equalToMatrixMultiplyByDouble(XEMMatrix* D, double d){
  D->putDiagonalValueInStore(_store);
  int64_t p;
  for(p=0; p<_s_pbDimension; p++){
    _store[p] *= d;
  } 
}



// add :  cik * xMoinsMean * xMoinsMean'  to this
void XEMDiagMatrix::add(double * xMoinsMean, double cik){
  
  int64_t p;
  double xMoinsMean_p;

  for(p=0; p<_s_pbDimension ; p++){
    xMoinsMean_p = xMoinsMean[p];
    _store[p]   +=  cik * xMoinsMean_p * xMoinsMean_p;
  }//end for p

}

// add : diag( cik * xMoinsMean * xMoinsMean' )  to this
/*void XEMDiagMatrix::addDiag(double * xMoinsMean, double cik){
  
  int64_t p;
  double xMoinsMean_p;

  for(p=0; p<_s_pbDimension ; p++){
    xMoinsMean_p = xMoinsMean[p];
    _store[p]   +=  cik * xMoinsMean_p * xMoinsMean_p;
  }//end for p
  
}*/



// set the value of (d x Identity) to this  
void XEMDiagMatrix::operator=(const double& d){
  int64_t p;
    
  for(p=0; p<_s_pbDimension; p++){
    _store[p] = d;
  }
}

// divide each element by d
void XEMDiagMatrix::operator/=(const double& d){
  int64_t p;
  for(p=0; p<_s_pbDimension; p++){
    _store[p] /= d;
  }
}


// multiply each element by d
void XEMDiagMatrix::operator*=(const double& d){
  int64_t p;
  for(p=0; p<_s_pbDimension; p++){
    _store[p] *= d;
  }
}


//add M to this
void XEMDiagMatrix::operator+=(XEMMatrix* M){
  M -> addDiagonalValueInStore(_store);
}


void XEMDiagMatrix::operator=(XEMMatrix* M){
  M -> putDiagonalValueInStore(_store);
}





void XEMDiagMatrix::input(std::ifstream & fi){
  int64_t p,q;
  double garbage;  
    
    for (p=0; p<_s_pbDimension; p++){
      // useless because all are 0
      for (q=0; q<p; q++){
        fi >> garbage;
      }
      
      // here i==j so we are in the diagonal
      fi >> _store[p];
      
      // useless because all are 0
      for(q=p+1;q<_s_pbDimension;q++){
        fi >> garbage;
      }
    }

}


void XEMDiagMatrix::input(double ** variances){
  int64_t p,q;
  for (p=0; p<_s_pbDimension; p++){
    // useless because all are 0
    for (q=0; q<p; q++){}
    
    // here i==j so we are in the diagonal
     _store[p]=variances[p][q];
    
    // useless because all are 0
    for(q=p+1;q<_s_pbDimension;q++){}
  }
  
}

double XEMDiagMatrix::detDiag(XEMErrorType errorType){
  return determinant(errorType);
}

void XEMDiagMatrix ::sortDiagMatrix(){
  int64_t max;
  for (int64_t i=0; i<_s_pbDimension; i++){
   max = i;
   for (int64_t j=i+1; j<_s_pbDimension; j++){
      if (_store[j] > _store[max]){
         max = j;
      }
    }
      if (max != i ){  // swich
	double tmp = _store[i];
        _store[i] = _store[max];
        _store[max] = tmp;
      }
 }

/*for (int64_t i=1; i<= _s_pbDimension;i++){
		//search the max eigenvalue
		max = i;
 for (int64_t j=i; j<_s_pbDimension; j++){
			if (_store[j-1] > _store[max-1])
					max = j;
			if (max != i){
				// switch
				double tmp = _store[max-1];
				_store[max-1] = _store[i-1];
				_store[i-1] = tmp;
			}
		}
	}*/

}


double**  XEMDiagMatrix::storeToArray() const{

  int64_t i,j;
  double** newStore = new double*[_s_pbDimension];
  for (i=0; i<_s_pbDimension ; ++i){
    newStore[i] = new double[_s_pbDimension];
  }
  for (i=0; i<_s_pbDimension ; ++i){
  
    for (j=0; j<_s_pbDimension ; ++j){
      if (i == j){
        newStore[i][j] = _store[i];
      }
      else {
        newStore[i][j] = 0;
      }
    }
  }

  return newStore;

}














