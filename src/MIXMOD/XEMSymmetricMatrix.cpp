/***************************************************************************
                             SRC/MIXMOD/XEMSymmetricMatrix.cpp  description
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

#include "XEMSymmetricMatrix.h"
#include "XEMDiagMatrix.h"
#include "XEMGeneralMatrix.h"

//------------
// Constructor
//------------
XEMSymmetricMatrix::XEMSymmetricMatrix(){
  _value = NULL;
  _store = NULL;
  throw wrongConstructorType;
}

XEMSymmetricMatrix::XEMSymmetricMatrix(int64_t pbDimension, double d):XEMMatrix(pbDimension){  
  _value      = new SymmetricMatrix(_s_pbDimension);
  _store      = _value->Store();

  _s_storeDim = _s_pbDimension * (_s_pbDimension + 1) / 2; 
	(*this)=d;
}



// copy constructor
XEMSymmetricMatrix::XEMSymmetricMatrix(XEMSymmetricMatrix * A):XEMMatrix(A){
  _value      = new SymmetricMatrix(_s_pbDimension);
  _store      = _value->Store();  
    
  _s_storeDim = _s_pbDimension * (_s_pbDimension + 1) / 2; 

  recopyTab(A->getStore(), _store, _s_storeDim);
}



//----------
//Destructor
//----------
XEMSymmetricMatrix::~XEMSymmetricMatrix(){
  if(_value){
    delete _value;
  }
  _value = NULL;
}



double XEMSymmetricMatrix::determinant(XEMErrorType errorType){
  double det=0;
  det = LogDeterminant(*_value).Value();
  if (det < minDeterminantValue){
      throw errorType;
      }
  return det;  
}


void XEMSymmetricMatrix::equalToMatrixMultiplyByDouble(XEMMatrix* D, double d){
 throw nonImplementedMethod;
}


double* XEMSymmetricMatrix::getDiagonalStore(){
  throw wrongMatrixType;
}


double* XEMSymmetricMatrix::getSymmetricStore(){
  return(_store);
}

double* XEMSymmetricMatrix::getGeneralStore(){
  return(_store);
}

double XEMSymmetricMatrix::getSphericalStore(){
  throw wrongMatrixType;
}


void XEMSymmetricMatrix::compute_M_tM(double* V, int64_t l){
  int64_t indice1=l-1, indice2;
  int64_t indiceStoreGammak = _s_storeDim-1;
  int64_t dim = l / _s_pbDimension;
  while (indice1 >0){
    for (int64_t j=0;j<dim;j++){
             _store[indiceStoreGammak] += V[(indice1-j)]*V[(indice1-j)];
             }
             indiceStoreGammak-=1;
         indice2 = indice1 - dim;
         while (indice2 > 0){
           for (int64_t j=0;j<dim;j++){
                  _store[indiceStoreGammak] += V[(indice1-j)]*V[(indice2-j)]; 
             }
             indice2 -= dim;
             indiceStoreGammak-=1;
         } 
         indice1 -= dim;
  }
}


void XEMSymmetricMatrix::compute_product_Lk_Wk(XEMMatrix* Wk, double  L){
  double * Wk_store;
  Wk_store = Wk->getSymmetricStore();
  for(int64_t p=0; p<_s_storeDim ;p++){
      _store[p] += Wk_store[p] / L;
  }
}



double XEMSymmetricMatrix::compute_trace_W_C(XEMMatrix * C){
  double tabLambdak_k   = 0.0;
  double termesHorsDiag;
  int64_t p,q,r;
    double * C_store = C->getSymmetricStore();
        termesHorsDiag = 0.0;
        for(p=0,r=0 ; p<_s_pbDimension ; p++,r++){
          for(q=0; q<p ; q++,r++){
            termesHorsDiag += _store[r] * C_store[r];
          }
          tabLambdak_k += _store[r] * C_store[r];
        }
        tabLambdak_k += 2.0 * termesHorsDiag;
        return tabLambdak_k;
}


void XEMSymmetricMatrix::inverse(XEMMatrix * &  Inv){
//cout<<"Inv Symm :  "<<Inv<<endl;
  if (Inv == NULL){
    Inv = new XEMSymmetricMatrix(_s_pbDimension);
  }

  SymmetricMatrix* value_Inv = new SymmetricMatrix(_s_pbDimension);
  *value_Inv<<_value->i();

  Inv->setSymmetricStore(value_Inv->Store());
//cout<<"Inv Symm :  "<<Inv<<endl;
  delete value_Inv;
}


double XEMSymmetricMatrix::norme(double * xMoinsMean){
  int64_t p,q,r;
  double termesHorsDiag = 0.0;
  double termesDiag     = 0.0;
  double xMoinsMean_p;

  for(p=0,r=0 ; p<_s_pbDimension ; p++,r++){
    xMoinsMean_p = xMoinsMean[p];
    for(q=0 ; q<p ; q++,r++){
      termesHorsDiag += xMoinsMean_p * xMoinsMean[q] * _store[r];
    }
    termesDiag += xMoinsMean_p * xMoinsMean_p * _store[r];
  }
  termesDiag += 2.0 * termesHorsDiag;
  return termesDiag;
}

  double XEMSymmetricMatrix::putSphericalValueInStore(double & store){
    int64_t p,r;
    int64_t increment = 2;
    store = 0.0;

    for(p=0,r=0; p<_s_pbDimension ; p++){
      store += _store[r];
      r+= increment;
      increment++;
    }
  store /= _s_pbDimension;
  return(store);

  }

  double XEMSymmetricMatrix::addSphericalValueInStore(double & store){
    int64_t p,r;
    int64_t increment = 2;
    for(p=0,r=0; p<_s_pbDimension ; p++){
      store += _store[r];
      r+= increment;
      increment++;
    }
  store /= _s_pbDimension;
  return(store);

  }


  double* XEMSymmetricMatrix::putDiagonalValueInStore(double * store){
    int64_t p,r;
    int64_t increment = 2;
    for(p=0,r=0; p<_s_pbDimension ; p++){
      store[p] = _store[r];
      r+= increment;
      increment++;
    }
    return(store);
  }


  double* XEMSymmetricMatrix::addDiagonalValueInStore(double * store){
    int64_t p,r;
    int64_t increment = 2;
    for(p=0,r=0; p<_s_pbDimension ; p++){
      store[p] += _store[r];
      r+= increment;
      increment++;
    }
    return(store);
  }


  double* XEMSymmetricMatrix::putSymmetricValueInStore(double * store){
    for (int64_t p=0 ; p<_s_storeDim ; p++){
      store[p] = _store[p];
    }
    return(store);
  }

  double* XEMSymmetricMatrix::addSymmetricValueInStore(double * store){
    for (int64_t p=0;p<_s_storeDim;p++){
      store[p] += _store[p];
    }
    return(store);
  }


   double* XEMSymmetricMatrix::putGeneralValueInStore(double * store){
   throw wrongMatrixType;
  }

  double* XEMSymmetricMatrix::addGeneralValueInStore(double * store){
   throw wrongMatrixType;
  }





// (this) will be A / d
void XEMSymmetricMatrix::equalToMatrixDividedByDouble(XEMMatrix * A, double d){
  A->putSymmetricValueInStore(_store);

  int64_t p;
  for(p=0; p<_s_storeDim; p++){
    _store[p] /=  d ;
  }
} 



// add :  cik * xMoinsMean * xMoinsMean'  to this
void XEMSymmetricMatrix::add(double * xMoinsMean, double cik){

  int64_t p,q,r;
  double xMoinsMean_p;

  for(p=0,r=0 ; p<_s_pbDimension ; p++,r++){
    xMoinsMean_p = xMoinsMean[p];
    for(q=0 ; q<p ; q++,r++){
      _store[r]  +=  cik * xMoinsMean_p * xMoinsMean[q];
    } // end for q
    _store[r]  +=  cik * xMoinsMean_p * xMoinsMean_p;
  }//end for p
}

// add : diag( cik * xMoinsMean * xMoinsMean' )  to this
/*void XEMSymmetricMatrix::addDiag(double * xMoinsMean, double cik){
  int64_t p,q,r;
  double xMoinsMean_p;

  for(p=0,r=0 ; p<_s_pbDimension ; p++,r++){
    xMoinsMean_p = xMoinsMean[p];
    for(q=0 ; q<p ; q++,r++) ;
    _store[r]  +=  cik * xMoinsMean_p * xMoinsMean_p;
  }
  
}*/



  // set the value of (d x Identity) to this  
void XEMSymmetricMatrix::operator=(const double& d){

  int64_t p,q,r;
  for(p=0,r=0; p<_s_pbDimension; p++,r++){
    for(q=0; q<p ; q++,r++){
      _store[r] = 0.0;
    }
    _store[r] = d;
  }
}

// divide each element by d
void XEMSymmetricMatrix::operator/=(const double& d){
  int64_t p;
  for(p=0; p<_s_storeDim; p++){
    _store[p] /= d;
  }
}

// multiply each element by d
void XEMSymmetricMatrix::operator*=(const double& d){
  int64_t p;
  for(p=0; p<_s_storeDim; p++){
    _store[p] *= d;
  }
}


void XEMSymmetricMatrix::operator=(XEMMatrix* M){
    M->putSymmetricValueInStore(_store);
}


//add M to this
void XEMSymmetricMatrix::operator+=(XEMMatrix* M){

    M->addSymmetricValueInStore(_store);
}



// compute Shape as diag(Ot . this . O ) / diviseur
void XEMSymmetricMatrix::computeShape_as__diag_Ot_this_O(XEMDiagMatrix* & Shape, XEMGeneralMatrix* & Ori, double diviseur){
  
  
  int64_t i_index, j_index;

  int64_t p,q,r,j;
  double * O_store     = Ori->getStore();
  double * Shape_store = Shape->getStore();
  
  double termesDiag, termesHorsDiag;
  double tmp;
  
  for(j=0; j<_s_pbDimension; j++){
    // computation of the [j,j] term of the diagonal

    //-- reset
    termesDiag     = 0.0;
    termesHorsDiag = 0.0;

    i_index = j;
    for(p=0,r=0; p<_s_pbDimension; p++,r++,i_index+=_s_pbDimension){
      j_index = j;
      for(q=0; q<p; q++,r++,j_index+= _s_pbDimension){
        termesHorsDiag += O_store[i_index] * O_store[j_index] * _store[r];
      }
      tmp = O_store[i_index];
      termesDiag += tmp * tmp * _store[r];
    }

    termesDiag     += 2.0 * termesHorsDiag;
    termesDiag     /= diviseur;
    Shape_store[j]  = termesDiag;

  }

}

// compute this as : multi * (O * S * O' )
void XEMSymmetricMatrix::compute_as__multi_O_S_O(double multi, XEMGeneralMatrix* & O, XEMDiagMatrix* & S){

  int64_t i_index = 0;
  int64_t j_index;
  int64_t p,q,r,l;

  double * O_store = O->getStore();
  double * S_store = S->getStore();
  double tmp;

  for(p=0,r=0; p<_s_pbDimension; p++,i_index+=_s_pbDimension){
    j_index = 0;
    for(q=0; q<=p ; q++,r++,j_index+=_s_pbDimension){    
      // compute this[i,j] = \multi * sum_{l} ( O[i,l] * 0[j,l] * S|l] ) 
      tmp = 0.0;
      for(l=0; l<_s_pbDimension ;l++){
        tmp += O_store[i_index+l] * O_store[j_index+l] * S_store[l];
      }
      tmp       *= multi;
      _store[r]  = tmp;

    }
  }

}


// compute this as : (O * S * O' )

void XEMSymmetricMatrix::compute_as_O_S_O(XEMGeneralMatrix* & O, double* & S_store){

  int64_t i_index = 0;
  int64_t j_index;
  int64_t p,q,r,l;

  for (int64_t i=0;i<_s_storeDim;i++){
   _store[i] = 0;
  }

  double * O_store = O->getStore();
  double tmp;
  for(p=0,r=0; p<_s_pbDimension; p++,i_index+=_s_pbDimension){
    j_index = 0;
    for(q=0; q<=p ; q++,r++,j_index+=_s_pbDimension){    
      tmp = 0.0;
      for(l=0; l<_s_pbDimension ;l++){
        tmp += O_store[i_index+l] * O_store[j_index+l] * S_store[l];
      }
      _store[r]  = tmp;
    }
  }
}

//compute trace of a symmetric matrix
double XEMSymmetricMatrix::computeTrace(){
  int64_t i;
  int64_t indice = 0;
 double trace = 0.0;


  i = 0;
  while(indice<_s_storeDim){
    trace += _store[indice];
    i++;
    indice+=i+1;
  }
 return trace;
}



void XEMSymmetricMatrix::computeSVD(XEMDiagMatrix* & S, XEMGeneralMatrix* & O){
  int64_t dim = O->getPbDimension();
  DiagonalMatrix * tabShape_k = new DiagonalMatrix(dim);
  Matrix * tabOrientation_k   = new Matrix(dim,dim);
  SVD((*_value),(*tabShape_k),(*tabOrientation_k));
 
  double * storeS = S->getStore();
  double * storeO = O->getStore();


  double * storeTabShape_k = (*tabShape_k).Store();
  double * storeTabOrientation_k = (*tabOrientation_k).Store();

  recopyTab(storeTabShape_k, storeS, dim);
  recopyTab(storeTabOrientation_k, storeO, dim*dim);

  delete tabShape_k;
  delete tabOrientation_k;
}



void XEMSymmetricMatrix::compute_as_M_tM(XEMGeneralMatrix* M, int64_t d){

  int64_t indiceStoreM1=0, indiceStoreM2;
  int64_t indice = 0;
  int64_t k1 = 0, k2;
  int64_t DimStoreM = _s_pbDimension*_s_pbDimension;
  double * storeM = M->getStore();

  for (int64_t i =0;i<_s_storeDim;i++){
   _store[i] = 0;
  }

   while (indiceStoreM1 < DimStoreM){
       k2 = k1;
       indiceStoreM2 = indiceStoreM1;
        while (indiceStoreM2 < DimStoreM){

          for (int64_t j=0;j<d;j++){
               _store[indice] += storeM[(indiceStoreM1+j)]*storeM[(indiceStoreM2+j)]; // attention vecteur contenant la matrice triangulaire supérieure
           }
           indiceStoreM2 = (k2+1)*_s_pbDimension;
           k2+=1;
           indice+=1;
        } 
        indiceStoreM1 = (k1+1)*_s_pbDimension;
        k1+=1;
   }
}


void XEMSymmetricMatrix::compute_as_M_V(XEMSymmetricMatrix* M, double * V){

  for (int64_t i=0;i<_s_pbDimension;i++){
	   _store[i] = 0;
       }
       int64_t indiceV = 0, k=0, indice=0;
       int64_t indiceM = 0;
       double* storeM = M->getStore();

       while (indice < _s_pbDimension){
         for (int64_t i=0;i<(_s_pbDimension-k);i++){
                 _store[indice] += V[indiceV+i]*storeM[indiceM + i];
             }

             for (int64_t j=1;j<(_s_pbDimension-k);j++){
                 _store[indice+j] += V[indiceV]*storeM[indiceM+j];
             }
             indiceM += (_s_pbDimension-k);
             k += 1;
             indiceV += 1;
             indice += 1;
       }
}






// compute M as : M = ( O * S^{-1} * O' ) * this
void XEMSymmetricMatrix::compute_M_as__O_Sinverse_Ot_this(XEMGeneralMatrix & M, XEMGeneralMatrix* & O, XEMDiagMatrix* & S){

  double * M_store = M.getStore();
  double * O_store = O->getStore();
  double * S_store = S->getStore();
  
  int64_t i,j,l, p,r;
  int64_t O1_index = 0;
  int64_t O2_index;
  int64_t r_decalage ;
  double tmp, omega;
  int64_t fillindex = 0 ;
  

  for(i=0; i<_s_pbDimension; i++){
    for(j=0; j<_s_pbDimension; j++, fillindex++){
      // filling tabMtmpk_store[i,j]
      
      tmp        = 0.0;
      r_decalage = _s_pbDimension - j + 1 ;
      
      r = j;
      p = 0;
      O2_index = 0;
      
      
      while(p<j){
        omega = 0.0;
        for(l=0; l<_s_pbDimension; l++){
          
          omega += O_store[O1_index+l] * O_store[O2_index+l] / S_store[l];
        }
        tmp += omega * _store[r];
        r   += r_decalage;
        r_decalage--;
        p++;
        O2_index += _s_pbDimension;
      }
      
      while(p<_s_pbDimension){
        omega = 0.0;
        
        for(l=0; l<_s_pbDimension; l++){
          omega += O_store[O1_index+l] * O_store[O2_index+l] / S_store[l];
        }
        tmp += omega * _store[r];
        r++;
        O2_index += _s_pbDimension;
        p++;
      }
      
      
      M_store[fillindex] = tmp;
    }
    O1_index += _s_pbDimension;
  }


}


void XEMSymmetricMatrix::input(std::ifstream & fi){
  int64_t i,j, r=0;  
  double garbage; 
    
  for (i=0; i<_s_pbDimension; i++){
    for (j=0; j<i+1; j++){
      fi >> _store[r];
      r++;
    }
    for (j=i+1; j<_s_pbDimension; j++){
      fi >> garbage;
    }
  }
}


void XEMSymmetricMatrix::input( double ** variances ){
  int64_t i,j, r=0;
  
  for (i=0; i<_s_pbDimension; i++){
    for (j=0; j<i+1; j++){
      _store[r] = variances[i][j];
      r++;
    }
    for (j=i+1; j<_s_pbDimension; j++){}
  }
}


// gives : det(diag(this))
double XEMSymmetricMatrix::detDiag(XEMErrorType errorType){
  int64_t p,q,r;
  double det = 1.0;
  
  for(p=0,r=0; p<_s_pbDimension; p++,r++){
    for(q=0;q<p;q++,r++) ;
    det *= _store[r];
  }
  if (det < minDeterminantValue)
      throw errorType;
  return det;  

}

// trace( this * O * S^{-1} * O' )
double XEMSymmetricMatrix::trace_this_O_Sm1_O(XEMGeneralMatrix* & O, XEMDiagMatrix* & S){
  double * O_store = O->getStore();
  double * S_store = S->getStore();
    
  double trace          = 0.0;
  double termesHorsDiag = 0.0;
  double tmp,tmp2;
  
  int64_t i_index = 0;
  int64_t j_index;
  int64_t p,q,r,l;

  for(p=0,r=0; p<_s_pbDimension ;p++,r++,i_index+=_s_pbDimension){
    j_index = 0;
    for(q=0; q<p; q++,r++,j_index+=_s_pbDimension){
      tmp = 0.0;
            
      for(l=0 ; l<_s_pbDimension ;l++){
        tmp += O_store[i_index+l] * O_store[j_index+l] / S_store[l];
      }
      tmp *= _store[r];
      termesHorsDiag += tmp;
    }
    tmp = 0.0;
    for(l=0 ; l<_s_pbDimension ;l++){
      tmp2 = O_store[i_index+l];
      tmp += tmp2 * tmp2 / S_store[l];
    }
    tmp *= _store[r];
    trace += tmp;
  }
  trace += 2.0 * termesHorsDiag;

  return trace;
  
}

double**  XEMSymmetricMatrix::storeToArray() const{

  int64_t i,j, k=(_s_storeDim-1);
  double** newStore = new double*[_s_pbDimension];

  for (i=0; i<_s_pbDimension ; ++i){
    newStore[i] = new double[_s_pbDimension];
  }
  for (i=(_s_pbDimension-1); i>-1 ; --i){
    newStore[i][i] = _store[k];
    k--;
    for (j=(i-1); j>-1 ; --j){
      newStore[i][j] = _store[k];
      newStore[j][i] = _store[k];
      k--;
    }

  }

  return newStore;

}










