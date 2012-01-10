/***************************************************************************
                             SRC/MIXMOD/XEMGaussianDiagParameter.cpp  description
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
#include "XEMGaussianDiagParameter.h"
#include "XEMGaussianGeneralParameter.h"
#include "XEMGaussianEDDAParameter.h"
#include "XEMGaussianData.h"
#include "XEMModel.h"


/****************/
/* Constructors */
/****************/

//----------------------------------------------------
//----------------------------------------------------
XEMGaussianDiagParameter::XEMGaussianDiagParameter(){
  throw wrongConstructorType;
}



//-------------------------------------------------------------------------------------
// constructor called by XEMModel
//-------------------------------------------------------------------------------------
XEMGaussianDiagParameter::XEMGaussianDiagParameter(XEMModel * iModel, XEMModelType * iModelType): XEMGaussianEDDAParameter(iModel, iModelType){
  int64_t k;
  _tabLambda          = new double [_nbCluster];
  _tabShape           = new XEMDiagMatrix*[_nbCluster];
  _W = new XEMDiagMatrix(_pbDimension);  //Id
  for (k=0; k<_nbCluster; k++){
    _tabLambda[k]   = 1.0;
		_tabShape[k]    = new XEMDiagMatrix(_pbDimension); //Id 
		
		_tabSigma[k]    = new XEMDiagMatrix(_pbDimension); //Id
		_tabInvSigma[k] = new XEMDiagMatrix(_pbDimension); //Id
		_tabWk[k]       = new XEMDiagMatrix(_pbDimension); //Id
  }

}



//---------------------------------------------------------------------
// copy Constructor
//---------------------------------------------------------------------
XEMGaussianDiagParameter::XEMGaussianDiagParameter(const XEMGaussianDiagParameter * iParameter):XEMGaussianEDDAParameter(iParameter){
  int64_t k;
  _tabLambda            = copyTab(iParameter->getTabLambda(), _nbCluster);
  _tabShape             = new XEMDiagMatrix*[_nbCluster];
  _W                    = new XEMDiagMatrix(_pbDimension);
  (* _W)                = iParameter->getW();
  
  XEMMatrix ** iTabSigma = iParameter->getTabSigma();
  XEMMatrix ** iTabInvSigma = iParameter->getTabInvSigma();
  XEMMatrix ** iTabWk       = iParameter->getTabWk();
  XEMDiagMatrix ** iTabShape       = iParameter->getTabShape();

	for (k=0; k<_nbCluster; k++){
		_tabSigma[k]        = new XEMDiagMatrix(_pbDimension);
    (* _tabSigma[k])    = iTabSigma[k];
    _tabInvSigma[k]     = new XEMDiagMatrix(_pbDimension);
    (* _tabInvSigma[k]) = iTabInvSigma[k];
    _tabWk[k]           = new XEMDiagMatrix(_pbDimension);
    (* _tabWk[k])       = iTabWk[k];
    _tabShape[k]        = new XEMDiagMatrix(_pbDimension);
    (* _tabShape[k])    = iTabShape[k];
  }
}



/**************/
/* Destructor */
/**************/
XEMGaussianDiagParameter::~XEMGaussianDiagParameter(){
  int64_t k;
  
	if (_tabLambda){
    delete[] _tabLambda;
    _tabLambda = NULL;
  }
  
	if(_tabShape){
    for(k=0; k<_nbCluster; k++){
      delete _tabShape[k];
      _tabShape[k] = NULL;
    }
    delete[] _tabShape;
    _tabShape = NULL;
  }
  
	if (_tabInvSigma){
    for(k=0; k<_nbCluster; k++){
       delete _tabInvSigma[k];
      _tabInvSigma[k] = NULL;
    }
  }

  if (_tabSigma){
   for(k=0; k<_nbCluster; k++){
      delete _tabSigma[k];
      _tabSigma[k] = NULL;
    }
  }
}



//------------------------
// reset to default values
//------------------------
void XEMGaussianDiagParameter::reset(){
  int64_t k;
	for (k=0; k<_nbCluster; k++){
		_tabLambda[k] = 1.0;
		*(_tabShape[k])  = 1.0;
	}
	XEMGaussianEDDAParameter::reset();
}




/*********/
/* clone */
/*********/
XEMParameter * XEMGaussianDiagParameter::clone()  const{
  XEMGaussianDiagParameter * newParam = new XEMGaussianDiagParameter(this);
  return(newParam);
}


/************/
/* initUSER */
/************/
void XEMGaussianDiagParameter::initUSER(XEMParameter * iParam){
     XEMGaussianEDDAParameter::initUSER(iParam);
     updateTabInvSigmaAndDet();
}



/*******************/
/* computeTabSigma */
/*******************/
void XEMGaussianDiagParameter::computeTabSigma(){

  /* Initialization */
  XEMGaussianData * data= (XEMGaussianData*)(_model->getData());
  double * tabNk = _model->getTabNk();
  int64_t k;
  XEMDiagMatrix * B = new XEMDiagMatrix(_pbDimension);
  XEMDiagMatrix * Bk = new XEMDiagMatrix(_pbDimension);
  double detB = 0.0;       // Determinant of matrix B
  int64_t iter          = 5;   // Number of iterations in iterative procedure 
//  double detDiagW;         // Determinant of diagonal matrix W
  double detDiagWk;        // Determinant of diagonal matrix Wk
  double lambda      = 0.0;  // Volume
  double weightTotal = data->_weightTotal;
  double power    = 1.0 / _pbDimension;
  double det     = 0.0;
  double logDet = 0.0;
  double * W_k = new double[_pbDimension];
  double * Shape_k;
  double tmp;
  int64_t p;


  // Compute det[diag(W)]
  logDet   = _W->determinant(minDeterminantDiagWValueError);
//  detDiagW = powAndCheckIfNotNull(logDet,power);

  // Variance estimator for each of diagonal model
  switch(_modelType->_nameModel){

	//---------------------
  case (Gaussian_p_L_B) :
  case (Gaussian_pk_L_B) :
	//---------------------
    for (k=0; k<_nbCluster; k++){
      /*_tabLambda[k]  = detDiagW / weightTotal;
      if ( _tabLambda[k]< minOverflow) {
        throw errorSigmaConditionNumber;
     }*/
      _tabSigma[k]->equalToMatrixDividedByDouble(_W, weightTotal); //_tabSigma[k] = _W/weightTtal
    }
    break;


	//---------------------
  case (Gaussian_p_L_Bk) :
  case (Gaussian_pk_L_Bk) :
	//----------------------
    for (k=0; k<_nbCluster; k++){
      det        = _tabWk[k]->determinant(minDeterminantDiagWkValueError);
      detDiagWk = powAndCheckIfNotNull(det,power);

      _tabShape[k]->equalToMatrixDividedByDouble(_tabWk[k],detDiagWk); //_tabShape[k] = _tabW[k]/detWk
      lambda        += detDiagWk;
    }

    for (k=0; k<_nbCluster; k++){
      _tabLambda[k] = lambda / weightTotal;
      if ( _tabLambda[k]< minOverflow)
        throw errorSigmaConditionNumber;

      _tabSigma[k]->equalToMatrixMultiplyByDouble(_tabShape[k],_tabLambda[k]); //tabSigma[k] = tabShape[k]*somme(lambda[k])
    }
    break;


	//---------------------
  case (Gaussian_p_Lk_Bk) :
  case (Gaussian_pk_Lk_Bk) :
	//----------------------
    for (k=0; k<_nbCluster; k++){

      logDet    = _tabWk[k]->determinant( minDeterminantDiagWkValueError);
      detDiagWk = powAndCheckIfNotNull(logDet, power);

      _tabLambda[k]  = detDiagWk / tabNk[k];
      if ( _tabLambda[k]< minOverflow)
        throw errorSigmaConditionNumber;

      _tabShape[k]->equalToMatrixDividedByDouble(_tabWk[k],detDiagWk); //_tabShape[k] = _tabW[k]/detWk

      _tabSigma[k]->equalToMatrixMultiplyByDouble(_tabShape[k],_tabLambda[k]); //tabSigma[k] = tabShape[k]*tabLambda[k]

    }
    break;


	//--------------------
  case (Gaussian_p_Lk_B) :
  case (Gaussian_pk_Lk_B) :
	//--------------------
    while (iter){
      /* Pb Overflow */
      for (k=0; k<_nbCluster; k++){
        if (_tabLambda[k] < minOverflow)
          throw errorSigmaConditionNumber;
      }

      /* Compute matrix B */
      (*B)=0.0;
      for (k=0; k<_nbCluster; k++){
        Bk->equalToMatrixDividedByDouble(_tabWk[k],_tabLambda[k]); //_tabShape[k] = _tabW[k]/_tabLambda[k]
        (*B) += Bk;  // B->operator+=(Bk) : B=somme sur k des Bk
      }
      /* Compute det(B) */
      logDet = B->determinant(minDeterminantBValueError);
      detB   = powAndCheckIfNotNull(logDet, power);

      /* Compute Shape[k] and Lambda[k] */
      for (k=0; k<_nbCluster; k++){
       // W_k      = _tabWk[k]->getDiagonalValue();
        _tabWk[k]->putDiagonalValueInStore(W_k);
        _tabShape[k]->equalToMatrixDividedByDouble(B,detB);
        Shape_k  = _tabShape[k]->getStore();
        tmp = 0.0;
        for(p=0; p<_pbDimension ; p++){
          tmp += W_k[p] / Shape_k[p];
        }
        tmp /= (_pbDimension*tabNk[k]);

        _tabLambda[k] = tmp; 
        if ( _tabLambda[k] < minOverflow)
          throw errorSigmaConditionNumber;
      }
      iter--;
    }

    for (k=0; k<_nbCluster; k++){
     _tabSigma[k]->equalToMatrixMultiplyByDouble(_tabShape[k],_tabLambda[k]);
    }
    
    break;


  default :
	//------
		throw internalMixmodError;
    break;
  }

  updateTabInvSigmaAndDet() ;
  delete Bk;
  delete B;
  delete []  W_k; 
}





//-------------------
//getLogLikelihoodOne
//-------------------
double XEMGaussianDiagParameter::getLogLikelihoodOne() const{

  /* Compute log-likelihood for one cluster
     useful for NEC criterion */

  /* Initialization */
  int64_t nbSample = _model->getNbSample();
  int64_t i;
  XEMGaussianData * data = (XEMGaussianData*)(_model->getData());
  double logLikelihoodOne;         // Log-likelihood for k=1
  double * Mean = new double[_pbDimension] ;
  double ** y  = data->_yStore;
  double * yi;
  XEMDiagMatrix * Sigma = new XEMDiagMatrix(_pbDimension);
  XEMDiagMatrix * W     = new XEMDiagMatrix(_pbDimension, 0.0);
  double norme ;       
  double * weight = data->_weight;
  //  Mean Estimator (empirical estimator)
  double totalWeight = data->_weightTotal;
  computeMeanOne(Mean,weight,y,nbSample,totalWeight);
  weight = data->_weight;

  /* Compute the Cluster Scattering Matrix W */
  int64_t p; // parcours
  double * xiMoinsMuk = data->getTmpTabOfSizePbDimension();
	
  for(i=0; i<nbSample ; i++){
    yi = y[i];
    for(p=0 ; p<_pbDimension ; p++){
       xiMoinsMuk[p] = yi[p] - Mean[p];
    }
    W->add(xiMoinsMuk, weight[i]);
  }  

  /* Compute determinant of diag(W) */
//  logDet   = W->detDiag(minDeterminantDiagWValueError);  // virtual
  Sigma->equalToMatrixDividedByDouble(W, totalWeight); // virtual
  
  // inverse of Sigma
  //XEMDiagMatrix * SigmaMoins1 = new XEMDiagMatrix(_pbDimension);
  XEMMatrix * SigmaMoins1 = NULL;
//  SigmaMoins1->inverse(Sigma);// virtual
  Sigma->inverse(SigmaMoins1); //construction de SigmaMoins1 dans la fonction inverse

  double detSigma  = Sigma->determinant(minDeterminantSigmaValueError); // virtual

  // Compute the log-likelihood for one cluster (k=1)
  logLikelihoodOne = 0.0;
  for (i=0; i<nbSample; i++){
    yi = y[i];
    for(p=0; p<_pbDimension; p++){
      xiMoinsMuk[p] = yi[p] - Mean[p];
    }
    norme             = SigmaMoins1->norme(xiMoinsMuk);   // virtual
    logLikelihoodOne += norme * weight[i];
  }

 logLikelihoodOne += totalWeight * ( data->getPbDimensionLog2Pi() + log(detSigma)) ;
 logLikelihoodOne *= -0.5 ;
 
 delete W;
 delete Sigma;
 delete SigmaMoins1;
 
 delete[] Mean;
 return logLikelihoodOne;
}



//----------------
//getFreeParameter
//----------------
int64_t XEMGaussianDiagParameter::getFreeParameter() const{
  int64_t nbParameter;       // Number of parameters //
  int64_t k = _nbCluster;    // Sample size          //
  int64_t d = _pbDimension;  // Sample dimension     //

  int64_t alphaR  = k*d;  // alpha for for models with Restrainct proportions (Gaussian_p_...)
  int64_t alphaF  = (k*d) + k-1; // alpha for models with Free proportions (Gaussian_pk_...)

  switch (_modelType->_nameModel) {
    case (Gaussian_p_L_B) :	
      nbParameter = alphaR + d; 
      break;	
    case (Gaussian_p_Lk_B) :	
      nbParameter = alphaR + d + k - 1; 
      break;	
    case (Gaussian_p_L_Bk) :	
      nbParameter = alphaR + (k*d) - k + 1; 
      break;	
    case (Gaussian_p_Lk_Bk) :	
      nbParameter = alphaR + (k*d); 
      break;	
    case (Gaussian_pk_L_B) :	
      nbParameter = alphaF + d;
      break;	
    case (Gaussian_pk_Lk_B) :	
      nbParameter = alphaF + d + k - 1; 
      break;	
    case (Gaussian_pk_L_Bk) :	
      nbParameter = alphaF + (k*d) - k + 1; 
      break;	
    case (Gaussian_pk_Lk_Bk) :
      nbParameter = alphaF + (k*d); 
      break;	
    default :
      throw internalMixmodError;
      break;
  }
  return nbParameter;
}

