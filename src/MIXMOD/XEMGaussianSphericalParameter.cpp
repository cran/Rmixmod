/***************************************************************************
                             SRC/MIXMOD/XEMGaussianSphericalParameter.cpp  description
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
#include "XEMGaussianSphericalParameter.h"
#include "XEMGaussianGeneralParameter.h"
#include "XEMGaussianEDDAParameter.h"
#include "XEMGaussianData.h"
#include "XEMModel.h"
#include "XEMModelType.h"

#include "XEMSphericalMatrix.h"

/****************/
/* Constructors */
/****************/



//----------------------------------------------------
//----------------------------------------------------
XEMGaussianSphericalParameter::XEMGaussianSphericalParameter(){
 throw wrongConstructorType;
}



//-------------------------------------------------------------------------------------
// constructor called by XEMModel
//-------------------------------------------------------------------------------------
XEMGaussianSphericalParameter::XEMGaussianSphericalParameter(XEMModel * iModel, XEMModelType * iModelType): XEMGaussianEDDAParameter(iModel, iModelType){
  int64_t  k;
  _W = new XEMSphericalMatrix(_pbDimension);
  for (k=0; k<_nbCluster; k++){
    _tabSigma[k]    = new XEMSphericalMatrix(_pbDimension);  //Id
		_tabInvSigma[k] = new XEMSphericalMatrix(_pbDimension);  //Id
		_tabWk[k]       = new XEMSphericalMatrix(_pbDimension);  //Id
  }

}




//---------------------------------------------------------------------
// copy constructor
//---------------------------------------------------------------------
XEMGaussianSphericalParameter::XEMGaussianSphericalParameter(const XEMGaussianSphericalParameter * iParameter):XEMGaussianEDDAParameter(iParameter){
  int64_t  k;

  _W = new XEMSphericalMatrix((XEMSphericalMatrix *)(iParameter->getW())); // copy constructor
  
  
  XEMMatrix ** iTabWk       = iParameter->getTabWk();
  XEMMatrix ** iTabSigma    = iParameter->getTabSigma();
  XEMMatrix ** iTabInvSigma = iParameter->getTabInvSigma();

  for(k=0; k<_nbCluster; k++){
    _tabWk[k]           =   new XEMSphericalMatrix(_pbDimension);
    (* _tabWk[k])       =   iTabWk[k];
    _tabSigma[k]        =   new XEMSphericalMatrix(_pbDimension);
    (* _tabSigma[k])    = iTabSigma[k];
    _tabInvSigma[k]     = new XEMSphericalMatrix(_pbDimension);
    (*_tabInvSigma[k])  = iTabInvSigma[k];
  } 
  
}



/**************/
/* Destructor */
/**************/
XEMGaussianSphericalParameter::~XEMGaussianSphericalParameter(){

 if (_tabSigma){
   for (int64_t  k=0;k<_nbCluster;k++){
    delete _tabSigma[k];
    //_tabSigma[k] = NULL;
   }
 } 

 if (_tabInvSigma){
   for (int64_t  k=0;k<_nbCluster;k++){
    delete _tabInvSigma[k];
   // _tabInvSigma[k] = NULL;
   }
 } 
}



/*********/
/* clone */
/*********/
XEMParameter * XEMGaussianSphericalParameter::clone() const{
  XEMGaussianSphericalParameter * newParam = new XEMGaussianSphericalParameter(this);
  return(newParam);
}


/************/
/* initUSER */
/************/
void XEMGaussianSphericalParameter::initUSER(XEMParameter * iParam){
  XEMGaussianEDDAParameter::initUSER(iParam);
  updateTabInvSigmaAndDet();
}



/*******************/
/* computeTabSigma */
/*******************/
void XEMGaussianSphericalParameter::computeTabSigma(){
  
  // Initialization
  XEMGaussianData * data = (XEMGaussianData*)(_model->getData());
  double * tabNk         = _model->getTabNk();
  int64_t  k;
  double sigmaValue;

  double totalWeight = data->_weightTotal;
  
  
  // Variance estimator for each of spherical model
  switch (_modelType->_nameModel) {

  case (Gaussian_p_L_I) :
  case (Gaussian_pk_L_I) :
  //---------------------
    _W->putSphericalValueInStore(sigmaValue);// / totalWeight;
    sigmaValue /= totalWeight;
    if (sigmaValue < minOverflow){
      throw errorSigmaConditionNumber;
    }
    for(k=0; k<_nbCluster; k++){  
        *(_tabSigma[k]) = sigmaValue;
    }

    break;


  case (Gaussian_p_Lk_I) :
  case (Gaussian_pk_Lk_I) :
	//----------------------
    for(k=0; k<_nbCluster; k++){
                        _tabWk[k]->putSphericalValueInStore(sigmaValue);
                        sigmaValue /= tabNk[k];
			if (sigmaValue < minOverflow)
				throw errorSigmaConditionNumber;
			*(_tabSigma[k]) = sigmaValue;
    }

    break;

  default :
		throw internalMixmodError;
    break;
  }

  updateTabInvSigmaAndDet() ;  
  
}




//-------------------
//getLogLikelihoodOne
//-------------------
double XEMGaussianSphericalParameter::getLogLikelihoodOne() const{

  /* Compute log-likelihood for one cluster
     useful for NEC criterion */

  /* Initialization */
  int64_t  nbSample = _model->getNbSample();
  int64_t  i;
  XEMGaussianData * data = (XEMGaussianData*)(_model->getData());
  double logLikelihoodOne;         // Log-likelihood for k=1
  double * Mean = new double[_pbDimension] ;
  double ** y  = data->_yStore;
  double * yi;
  XEMSphericalMatrix * Sigma = new XEMSphericalMatrix(_pbDimension);
  XEMSphericalMatrix * W     = new XEMSphericalMatrix(_pbDimension);
  double norme ;
  double * weight = data->_weight;

  //  Mean Estimator (empirical estimator)
  double totalWeight = data->_weightTotal;
  computeMeanOne(Mean,weight,y,nbSample,totalWeight);
  weight = data->_weight;

  /* Compute the Cluster Scattering Matrix W */
  int64_t  p; // parcours
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
//  detDiagW = powAndCheckIfNotNull(logDet ,1.0/_pbDimension);
  Sigma->equalToMatrixDividedByDouble(W, totalWeight); // virtual

  // inverse of Sigma
  //XEMSphericalMatrix * SigmaMoins1 = new XEMSphericalMatrix( _pbDimension);
  XEMMatrix * SigmaMoins1 = NULL;
  //SigmaMoins1->inverse(Sigma);// virtual
//cout<<"S"<<endl;
//Sigma->edit(cout,"");

  Sigma->inverse(SigmaMoins1);
//cout<<"S-1"<<endl;
//SigmaMoins1->edit(cout,"");
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

 delete[] Mean;

 delete W;
 delete Sigma;
 delete SigmaMoins1;

 return logLikelihoodOne;
}



//----------------
//getFreeParameter
//----------------
int64_t  XEMGaussianSphericalParameter::getFreeParameter() const{
  int64_t  nbParameter;       // Number of parameters //
  int64_t  k = _nbCluster;    // Sample size          //
  int64_t  d = _pbDimension;  // Sample dimension     //

  int64_t  alphaR  = k*d;  // alpha for for models with Restrainct proportions (Gaussian_p_...)
  int64_t  alphaF  = (k*d) + k-1; // alpha for models with Free proportions (Gaussian_pk_...)

  switch (_modelType->_nameModel) {
    case (Gaussian_p_L_I) :
      nbParameter = alphaR + 1; 
      break;	
    case (Gaussian_p_Lk_I) :
      nbParameter = alphaR + k; // correction 09/09/2009 (old version was alphaR + d)
      break;	
    case (Gaussian_pk_L_I) :
      nbParameter = alphaF + 1; 
      break;	
    case (Gaussian_pk_Lk_I) :
      nbParameter = alphaF + k; // correction 09/09/2009 (old version was alphaR + d)
      break;	
    default :
      throw internalMixmodError;
      break;
  }
  return nbParameter;
}

