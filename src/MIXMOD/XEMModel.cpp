/***************************************************************************
                             SRC/MIXMOD/XEMModel.cpp  description
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
#include "XEMModel.h"
#include "XEMGaussianDiagParameter.h"
#include "XEMGaussianSphericalParameter.h"
#include "XEMGaussianGeneralParameter.h"
#include "XEMGaussianHDDAParameter.h"
#include "XEMBinaryEParameter.h"
#include "XEMBinaryEkParameter.h"
#include "XEMBinaryEjParameter.h"
#include "XEMBinaryEkjParameter.h"
#include "XEMBinaryEkjhParameter.h"
#include "XEMRandom.h"
#include "XEMGaussianData.h"
#include "XEMBinaryData.h"
#include <string.h>



//------------
// Constructor
//------------
XEMModel::XEMModel(){
	throw internalMixmodError;
}



//------------
// Constructor
//------------
XEMModel::XEMModel(XEMModel * iModel){
  _deleteData = true;
  _nbCluster              = iModel->getNbCluster();
  _nbSample               = iModel->getNbSample();
  _algoName = iModel->_algoName;
	XEMModelType * iModelType = iModel->getParameter()->getModelType();
  XEMModelName iModelName = iModelType->_nameModel;

  if ( isBinary(iModelName )){
    XEMBinaryData * bD = (XEMBinaryData*)(iModel->getData());
    _data = new XEMBinaryData(*bD);
  }
  else{
    XEMGaussianData * gD = (XEMGaussianData*)(iModel->getData());
    _data = new XEMGaussianData(*gD);
  }

  _tabFik      = copyTab(iModel->getTabFik()     , _nbSample, _nbCluster);
	_tabSumF     = copyTab(iModel->getTabSumF()    , _nbSample);
	_tabTik      = copyTab(iModel->getTabTik()     , _nbSample, _nbCluster);
  _tabZikKnown = copyTab(iModel->getTabZikKnown(), _nbSample,_nbCluster);
  _tabZiKnown = copyTab(iModel->getTabZiKnown(), _nbSample);
  _tabCik      = copyTab(iModel->getTabCik()     , _nbSample, _nbCluster);
  _tabNk       = copyTab(iModel->getTabNk()      , _nbCluster);

  _parameter   = (iModel->getParameter())->clone();
  _parameter->setModel(this);

}



//------------
// Constructor
//------------
XEMModel::XEMModel(XEMModelType * modelType, int64_t  nbCluster, XEMData *& data, XEMPartition *& knownPartition){
  int64_t  k;
  int64_t  i;
  _deleteData = false;
  _nbCluster     = nbCluster;
  _data          = data;
  _nbSample      = _data->_nbSample;
  _algoName = UNKNOWN_ALGO_NAME;

  _tabFik = new double*[_nbSample];
  _tabCik = new double*[_nbSample];
  _tabSumF = new double[_nbSample];
	_tabTik = new double*[_nbSample];
  _tabZikKnown = new int64_t *[_nbSample];
  _tabZiKnown = new bool[_nbSample];
  _tabNk = new double[_nbCluster];

  for (i=0; i<_nbSample; i++){
		_tabFik[i] = new double[_nbCluster];
		_tabTik[i] = new double[_nbCluster];
    _tabZikKnown[i] = new int64_t [_nbCluster];
		_tabCik[i] = new double[_nbCluster];
		for (k=0;k<_nbCluster;k++){
			_tabFik[i][k] = 0.0;
			_tabTik[i][k] = 0.0;
			_tabZikKnown[i][k] = 0;
			_tabCik[i][k] = 0.0;
    }
		_tabZiKnown[i] = false;
		_tabSumF[i]     = 0.0;
	}
	
	// _tabNk[k] = 0 even if knownPartition because this partition could be partial
	for (k=0;k<_nbCluster;k++){
		_tabNk[k] = 0.0;
	}

  FixKnownPartition(knownPartition);
	XEMModelName modelName = modelType->_nameModel;
  // create Param
	if (isSpherical(modelName)){
      _parameter = new XEMGaussianSphericalParameter(this, modelType); 
	}
	if (isDiagonal(modelName)){
		_parameter = new XEMGaussianDiagParameter(this, modelType); 
	}
	if (isGeneral(modelName)){
		_parameter = new XEMGaussianGeneralParameter(this, modelType); 
	}
	

//HDDA models
	if (isHD(modelName)){
		_parameter = new XEMGaussianHDDAParameter(this, modelType);
	}
	
	
	switch(modelName){
      // Binary models //
    case (Binary_p_E):
      _parameter = new XEMBinaryEParameter(this, modelType, ((XEMBinaryData*)data)->getTabNbModality());
      break;
    case (Binary_p_Ek):
      _parameter = new XEMBinaryEkParameter(this, modelType, ((XEMBinaryData*)data)->getTabNbModality());
      break;
    case (Binary_p_Ej):
      _parameter = new XEMBinaryEjParameter(this, modelType, ((XEMBinaryData*)data)->getTabNbModality());
      break;
    case (Binary_p_Ekj):
      _parameter = new XEMBinaryEkjParameter(this, modelType, ((XEMBinaryData*)data)->getTabNbModality());
      break;
    case (Binary_p_Ekjh):
      _parameter = new XEMBinaryEkjhParameter(this, modelType, ((XEMBinaryData*)data)->getTabNbModality());
      break;
    case (Binary_pk_E):
      _parameter = new XEMBinaryEParameter(this, modelType, ((XEMBinaryData*)data)->getTabNbModality());
      break;
    case (Binary_pk_Ek):
      _parameter = new XEMBinaryEkParameter(this, modelType, ((XEMBinaryData*)data)->getTabNbModality());
      break;
    case (Binary_pk_Ej):
      _parameter = new XEMBinaryEjParameter(this, modelType, ((XEMBinaryData*)data)->getTabNbModality());
      break;
    case (Binary_pk_Ekj):
      _parameter = new XEMBinaryEkjParameter(this, modelType, ((XEMBinaryData*)data)->getTabNbModality());
      break;
    case (Binary_pk_Ekjh):
      _parameter = new XEMBinaryEkjhParameter(this, modelType, ((XEMBinaryData*)data)->getTabNbModality());
      break;
		}
}


//-----------
// Destructor
//-----------
XEMModel::~XEMModel(){
  int64_t  i;
  
  if (_tabFik){
    for (i=0; i<_nbSample; i++){
      delete[] _tabFik[i];
      _tabFik[i] = NULL;
    }
    delete[] _tabFik;
    _tabFik = NULL;
  }

  
  if (_tabCik){
    for (i=0; i<_nbSample; i++){
      delete[] _tabCik[i];
      _tabCik[i] = NULL;
    }
    delete[] _tabCik;
    _tabCik = NULL;
  }
  
  
  if (_tabTik){
    for (i=0; i<_nbSample; i++){
      delete[] _tabTik[i];
      _tabTik[i] = NULL;
    }
    delete[] _tabTik;
    _tabTik = NULL;
  }

  if (_tabZikKnown){
    for (i=0; i<_nbSample; i++){
      delete[] _tabZikKnown[i];
      _tabZikKnown[i] = NULL;
    }
    delete[] _tabZikKnown;
    _tabZikKnown = NULL;
  }
  
  if (_tabZiKnown){
    delete[] _tabZiKnown;
    _tabZiKnown = NULL;
  }

  if (_tabNk){
    delete[] _tabNk;
    _tabNk = NULL;
  }

  if (_tabSumF){
    delete[] _tabSumF;
    _tabSumF = NULL;
  }


  if (_parameter){
    delete _parameter;
    _parameter = NULL;
  }

  if (_deleteData){
    delete _data;
  }

}



//------------
// updateForCV
//------------
void XEMModel::updateForCV(XEMModel * originalModel, XEMCVBlock & CVBlock){  
  int64_t  k,i;  

  // _data
  //------
  // _data=originalData but weight will be different
  // _data has already been updated (copy constructor)
  _data->_weightTotal = originalModel->_data->_weightTotal - CVBlock._weightTotal;  
  recopyTab(originalModel->_data->_weight, _data->_weight, _nbSample);
  for (int64_t  ii=0; ii<CVBlock._nbSample; ii++){
    i = CVBlock._tabWeightedIndividual[ii].val;
    _data->_weight[i]  -= CVBlock._tabWeightedIndividual[ii].weight;
  }
  
  /* new version : Dec 2006*/
  /* recopy fik, tik and zik*/
  recopyTab(originalModel->_tabFik, _tabFik, _nbSample, _nbCluster);
  recopyTab(originalModel->_tabSumF, _tabSumF, _nbSample);
  recopyTab(originalModel->_tabTik, _tabTik, _nbSample, _nbCluster);
  recopyTab(originalModel->_tabCik, _tabCik,_nbSample, _nbCluster); 
  

  // already done : (with the copy constructor
//  recopyTab(originalModel->_tabZikKnown, _tabZik, _nbSample);   
  
  //--------------
  // update _tabNk
  //--------------
  recopyTab(originalModel->_tabNk, _tabNk, _nbCluster);
  for (int64_t  ii=0; ii<CVBlock._nbSample; ii++){
    i = CVBlock._tabWeightedIndividual[ii].val;
    for (k=0; k<_nbCluster; k++){
      _tabNk[k] -=  CVBlock._tabWeightedIndividual[ii].weight * _tabCik[i][k];
    }
  } 
  //----------
  // parameter
  //----------
  _parameter->updateForCV(originalModel, CVBlock);
}






//--------------
// getKnownLabel 
//--------------
// return a value in : 0, ..., K-1
// i = 0 ... nbSample-1
int64_t  XEMModel::getKnownLabel(int64_t  i){
  int64_t  res=-1;
  int64_t  k;
	if (_tabZiKnown[i]){
		for (k=0;k<_nbCluster;k++){
			if (_tabZikKnown[i][k]==1){
				res = k;
			}
		}
	}
	else{
		throw internalMixmodError;
	}
	return res;
}
	


//-------------------------------------------
// getLabelAndPartitionByMAPOrKnownPartition
// label[i] = 1 ... nbSample
//------------------------------------------
void XEMModel::getLabelAndPartitionByMAPOrKnownPartition(int64_t  * label, int64_t  ** partition){
  //if (!_isCikEqualToMapOrKnownPartition){ 
   if (_algoName == UNKNOWN_ALGO_NAME) 
     throw;
  
  if (_algoName==MAP || _algoName==CEM || _algoName==M ){
    // _tabCik contains the result
    int64_t  i;
    int64_t  k;
    for (i=0; i<_nbSample; i++){
      for (k=0; k<_nbCluster; k++){
        partition[i][k] = (int64_t )_tabCik[i][k];  // cast double to int64_t  (_tabCik[i][k] = 0.0 or 1.0)
        if (partition[i][k] == 1){
          label[i] = k+1;
        }
      }
    }
  } 
  else{
    int64_t  k, kMax;
    int64_t  i;
    double tikMax = 0;
    for (i=0; i<_nbSample; i++){
      if (_tabZiKnown[i]){
        //-----------------
        for (k=0; k<_nbCluster; k++){
          partition[i][k] = _tabZikKnown[i][k];
          if (_tabZikKnown[i][k] == 1){
            label[i] = k+1;
          }
        }
      }
      else{
        // !ziKnown
        //---------
        kMax = 0;
        tikMax = _tabTik[i][0];
        for (k=1; k<_nbCluster; k++){
          if (_tabTik[i][k] > tikMax){
            tikMax = _tabTik[i][k];
            kMax = k;
          }
        }
        for (k=0; k<_nbCluster; k++){
          partition[i][k] = 0;
          partition[i][kMax] = 1;
          label[i] = kMax+1;
        }
      }
    }
  }
}
  

//------------------------------
// getLabelByMAPOrKnownPartition
//------------------------------
// return a value in : 0, ..., K-1
// i = 0 ... nbSample-1
int64_t  XEMModel::getLabelByMAPOrKnownPartition(int64_t  i){
  
  int64_t  k, kMax;
  double tikMax = 0;
  int64_t  res = -1; 
	
   if (_algoName == UNKNOWN_ALGO_NAME) 
     throw;
//   
  if (_algoName==CEM || _algoName==MAP || _algoName==M){
    //_tabCik[i] gives to result
    for (k=0; k<_nbCluster; k++){
      if (_tabCik[i][k]==1){
        res = k;
      }
    }
  }
  
  else{
    // must be computed
    // This method is called by getCompletedLogLikelihood
    // In this case (!_isCikEqualToMapOrKnownPartition), it's called by XEMICLCriterion or XEMLikelihoodOutput, so an Estep have been done before to upadte fik and tik used in this section
    if (_tabZiKnown[i]){
      for (k=0;k<_nbCluster;k++){
        if (_tabZikKnown[i][k]==1){
          res = k;
        }
      }
    }
    else {
      tikMax = _tabTik[i][0];
      kMax = 0;
      for (k=0;k<_nbCluster;k++){
        if (_tabTik[i][k] > tikMax){
          tikMax = _tabTik[i][k];
          kMax = k;
        }
      }
      res = kMax;
    }
  }
  
  if (res == -1){
#ifdef VERBOSE
  	// label couldn't be found
    cout<<"internalMixmodError ds XEMModel::getLabelByMAPOrKnownPartition, i="<<i<<endl;
#endif
    throw internalMixmodError;
  }
	
	
  return res;
}





//-------------------------------------
// get log-likelihood one (one cluster)
//-------------------------------------
double XEMModel::getLogLikelihoodOne(){
  return _parameter->getLogLikelihoodOne();
}





//-------------------
// get log-likelihood
//-------------------
double XEMModel::getLogLikelihood(bool fikMustBeComputed){
  // Compute the log-likelihood (observed) //
	
	if (fikMustBeComputed){
		computeFik();
	}

  int64_t  i;
  double logLikelihood = 0.0 ;
  double ** p_tabFik;
  double *  p_tabFik_i;
  double *  weight = _data->_weight;
  p_tabFik = _tabFik;
  for (i=0; i<_nbSample; i++){
    p_tabFik_i = *p_tabFik;
    if (_tabZiKnown[i]){
      int64_t  ki = getKnownLabel(i);// la classe de l'individu i
      logLikelihood += log(p_tabFik_i[ki]) * weight[i];
    }
    else{
      if (_tabSumF[i] > 0)
        logLikelihood += log(_tabSumF[i]) * weight[i];
    }
      //cout<<"compute LL, with ind "<<i<<", LL = "<<logLikelihood<<endl;
    p_tabFik++;
  }

  return logLikelihood;
}


//-----------------------------------------
// get completed LL (if CEM) or LL (elseif)
//-----------------------------------------
double XEMModel::getCompletedLogLikelihoodOrLogLikelihood(){
  if (_algoName == UNKNOWN_ALGO_NAME){
    throw internalMixmodError;
  }
  else{
    if (_algoName == CEM){
      return getCompletedLogLikelihood();
    }
    else{
      return getLogLikelihood(true);
    }
  }

}




//-----------------------------
// get completed log-likelihood
//-----------------------------
double XEMModel::getCompletedLogLikelihood(){
  // Compute the observed completed log-likelihood //

  int64_t  i;
  double cLogLikelihood = 0.0;
  int64_t  ki ;// la classe de l'individu i
  for (i=0; i<_nbSample; i++){
    ki = getLabelByMAPOrKnownPartition(i);// la classe de l'individu i
    if (_tabFik[i][ki] > 0){
      cLogLikelihood += log(_tabFik[i][ki]) * _data->_weight[i];
    }
  }

  return cLogLikelihood;
}



//------------
// get entropy
//------------
double XEMModel::getEntropy(){
  // Entropy //

  // Initialization //
  int64_t  i;
  int64_t  k;
  double entropy = 0;

  // Compute entropy: - sum[-tik * log(tik)] //
  for (i=0; i<_nbSample; i++){
// ajout du 16/06/2004 : ligne suivante : on ajoute uniqt si le label est inconnu
    if (!_tabZiKnown[i]){
      for (k=0; k<_nbCluster; k++){
        if (_tabTik[i][k]>0 && _tabTik[i][k]!=1) {
          entropy += _tabTik[i][k] * log(_tabTik[i][k]) * _data->_weight[i];
        }
      }
    }
  }

  return -entropy;
}



void XEMModel::setAlgoName(XEMAlgoName algoName){
  _algoName = algoName;
}



//----------------------------------
// compute the probabilities _tabFik
//----------------------------------
void XEMModel::computeFik(){
// // updates _tabSumF, _tabFik

  int64_t  k;
  int64_t  i;
  double * tabProportion = _parameter->getTabProportion();
  double *  p_tabSumF    = _tabSumF ; // parcours de _tabSum
  double ** p_tabFik     = _tabFik;   // parcours de _tabFik
  
  _parameter->getAllPdf(_tabFik,tabProportion);

  for (i=0; i<_nbSample; i++){
    *p_tabSumF = 0.0;
    for (k=0; k<_nbCluster; k++){
      *p_tabSumF  += (*p_tabFik)[k];
    }
    p_tabFik++;
    p_tabSumF++;
  }
}



//-------------------------------------
// compute number of element by cluster
//-------------------------------------
void XEMModel::computeNk(){
  int64_t  k;
  int64_t  i;
  double    ** p_tabCik      = _tabCik ;      // parcours le tableau _tabCik
  double  * w             = _data->_weight;// parcours des poids
  double wi;                               // poids courant  
  double     * p_tabCik_i;

  // initialisation
  initToZero(_tabNk,_nbCluster);
  for(i=0 ; i<_nbSample ; i++){
    wi         = *w;
    p_tabCik_i = *p_tabCik;
      for(k=0 ; k<_nbCluster ; k++){
        _tabNk[k] += p_tabCik_i[k] * wi;
      }

    p_tabCik++;
    w++;
  }
  // verification
	for(k=0 ; k<_nbCluster ; k++){
    if (_tabNk[k]==0.0){
      throw nullNk;
    }
  }
}



//----------------
//getFreeParameter
//----------------
int64_t  XEMModel::getFreeParameter(){
  return _parameter->getFreeParameter();
}




// compute label of samples in x  (res : 0 -> _nbCluster-1)
//------------------------------
int64_t  XEMModel::computeLabel(XEMSample * x){
    
  int64_t  k, res = 0;
  double sumfk = 0.0;
  double * tk = new double[_nbCluster];
  double * fk = new double[_nbCluster];
  double max = 0.0;
  double * tabProportion = _parameter->getTabProportion();
  double tmp;
  
  // Compute the probabilities _tabFik //
  for (k=0; k<_nbCluster; k++){
    tmp   = tabProportion[k] * _parameter->getPdf(x,k);
    fk[k] = tmp;
    // Compute the sum of probabilities sumfk //
    sumfk += tmp;
  }

  // Compute the conditional probabilities (posteriori probabilities) //
  for (k=0; k<_nbCluster; k++){
    tk[k] = fk[k] / sumfk;
  }

  // get the max of tk[k] //
  for (k=0 ; k<_nbCluster; k++){
    tmp = tk[k];
    if (tmp > max){
      max = tmp;
      res = k;
    }
  }

  delete[] fk;
  delete[] tk;

  return res;

}



//-------------
// computeLabel
//-------------
// compute the label of the i0-th point of the sample
// i0 : 0 -> _nbSample-1  and res : 0 -> _nbCluster-1)
int64_t  XEMModel::computeLabel(int64_t  i0){
  int64_t  k, res = 0;
  double max = 0.0;
  double * Tik_i0 = _tabTik[i0];


  for (k=0; k<_nbCluster; k++){
    if (Tik_i0[k] > max){
      max = Tik_i0[k];
      res = k;
    }
  }

  return res;
}




//-----------------
// Fix label Known
//----------------

void XEMModel::FixKnownPartition(XEMPartition *& knownPartition){
  // update Nk if knownLabel
	if (knownPartition!=NULL){
    int64_t     ** knownPartitionValue = knownPartition->_tabValue;
    int64_t      * knownPartition_i;
		double    ** p_cik = _tabCik;
    int64_t  ** p_zikKnown = _tabZikKnown;
		//double ** p_tik = _tabTik;
    int64_t  k;
    int64_t  i;
		double sumLabel = 0.0;
		for (i=0; i<_nbSample; i++){
			sumLabel     = 0.0;
			knownPartition_i = *knownPartitionValue;
      
			for (k=0; k<_nbCluster; k++){
				sumLabel += knownPartition_i[k];
			}
			if (sumLabel != 0.0){
				_tabZiKnown[i] = true;
				recopyTab(knownPartition_i,*p_cik,_nbCluster);
				recopyTab(knownPartition_i,*p_zikKnown,_nbCluster);
				//recopyTab(knownLabel_i,*p_tik,_nbCluster);
			}
			knownPartitionValue++;
			p_cik++;
			//p_tik++;
			p_zikKnown++;
		}

//
        computeNk();
	}//endif
}








//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
// 										Initialization methods
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------


/*-------------------------------------------
 		initRANDOM   
 		----------
 		
  updated in this method : 
	- _parameter
	- _tabFik and _tabSumF (because bestParameter is choose with the best LL which is computed with fik (and sumF)
	Note : _tabFik and sumF wil be 're'computed in the following EStep
	So only _parameter have to be updated in this method	
-------------------------------------------*/
void XEMModel::initRANDOM(int64_t  nbTry){
 // cout<<"init RANDOM, nbTryInInit="<<nbTry<<endl;
  _algoName = UNKNOWN_ALGO_NAME;
  int64_t  i,k;
  double logLikelihood, bestLogLikelihood;
  XEMParameter * bestParameter = _parameter->clone();
  bool * tabIndividualCanBeUsedForInitRandom = new bool[_nbSample];
  for (i=0; i<_nbSample; i++){
    tabIndividualCanBeUsedForInitRandom[i] = true;
  }
  bool * tabClusterToInitialize = new bool[_nbCluster];
  for (k=0; k<_nbCluster; k++){
    tabClusterToInitialize[k] = true;
  }
	
  // 1. InitForInitRandom
  //---------------------
  _parameter->initForInitRANDOM();
		
  // 1rst RANDOM
  //-------------
  randomForInitRANDOMorUSER_PARTITION(tabIndividualCanBeUsedForInitRandom, tabClusterToInitialize);
  // Compute log-likelihood 
  logLikelihood = getLogLikelihood(true);  // true : to compute fik
  bestLogLikelihood = logLikelihood;
  bestParameter->recopy(_parameter);
  /*cout<<"initRandom"<<endl<<"1er essai : "<<endl<<"Parameter : "<<endl;
  _parameter->edit();
  cout<<"LL : "<< bestLogLikelihood<<endl;*/
		
  // Others RANDOM
  for (i=1; i<nbTry; i++){
    randomForInitRANDOMorUSER_PARTITION(tabIndividualCanBeUsedForInitRandom, tabClusterToInitialize);
    // Compute log-likelihood 
    logLikelihood = getLogLikelihood(true);  // true : to compute fik
    if (logLikelihood > bestLogLikelihood){
      bestLogLikelihood = logLikelihood;
      bestParameter->recopy(_parameter);
    }
  /*cout<<endl<<"initRandom"<<endl<<i+1<<" eme essai : "<<endl<<"Parameter : "<<endl;
    _parameter->edit();
    cout<<"LL : "<< logLikelihood<<endl;*/
  }
		
  // set best parameter
  delete _parameter;
  _parameter = bestParameter;
  _parameter->setModel(this);
  /*cout<<endl<<"initRandom"<<endl<<"meilleur essai : "<<endl<<"Parameter : "<<endl;
  _parameter->edit();
  cout<<"LL : "<< bestLogLikelihood<<endl;*/
	
  //cout<<"fin de init RANDOM, nb d'essais effectues="<<i<<endl;
  delete [] tabIndividualCanBeUsedForInitRandom;
  delete [] tabClusterToInitialize;
}





//-----------------------------------------------
// random step for init RANDOM or USER_PARTITION
//----------------------------------------------
void XEMModel::randomForInitRANDOMorUSER_PARTITION(bool * tabIndividualCanBeUsedForInitRandom, bool * tabClusterToInitialize){
  int64_t  * tabIdxSampleForInit     = new int64_t  [_nbCluster];
	XEMSample ** tabSampleForInit  = new XEMSample*[_nbCluster];
	double totalWeight             = _data->_weightTotal;
	XEMSample ** tabSample         = _data->_matrix;
	double *  tabWeight            = _data->_weight;
  int64_t  k;

	for (k=0; k<_nbCluster; k++){
		if (tabClusterToInitialize[k]){
			tabIdxSampleForInit[k] = generateRandomIndex(tabIndividualCanBeUsedForInitRandom, tabWeight, totalWeight);
			tabSampleForInit[k] = tabSample[tabIdxSampleForInit[k]];
		}
	} // end for k
	
	_parameter->updateForInitRANDOMorUSER_PARTITION(tabSampleForInit, tabClusterToInitialize);
	
	// update tabIndividualCanBeUsedForInitRandom for others runs
	for (k=0; k<_nbCluster; k++){
		if (tabClusterToInitialize[k]){
			tabIndividualCanBeUsedForInitRandom[tabIdxSampleForInit[k]] = true;
		}
	}
	delete [] tabIdxSampleForInit;
	delete [] tabSampleForInit;
}








/*----------------------------------------
			initUSER
			--------
	updated in this method :
	- _parameter
-----------------------------------------*/
void XEMModel::initUSER(XEMParameter * initParameter){
  _algoName = UNKNOWN_ALGO_NAME;
  if (initParameter){
    _parameter->initUSER(initParameter);
  }
  else{
    throw errorInitParameter;
  }
}






/*----------------------------------------
 			initUSER_PARTITION
 			------------------
 			
 	updated in this method :
 	- _parameter
 		
 		Note : this method is only called in Classification context (not in Discriminant context).
 		So, an Estep follows
 	
-----------------------------------------*/
/*
Les partitions donnees servent a calculer 
- les cik, les nk (dans fixKnownLabel) appel� dans le constructeur
- les centres lorsque l'on a au moins un representant de la classe dans la initPartition (sinon on tire au hasard)
En revanche, on ne les utilise pas pour les dispersions.
On pourrait le faire si on a beaucoup d'information mais dans ce le cas ou l'on a peu d'information (ex : un seul individu pour une des classes), on ne peut pas calculer de disperion.
On calcule donc la dispersion autour du centre (comme s'il y a vait une seule classe) dans le cas gaussien et on tire la dispersion au hasard dans le cas binaire.
*/
void XEMModel::initUSER_PARTITION(XEMPartition * initPartition, int64_t nbTryInInit){
  
  
  _algoName = UNKNOWN_ALGO_NAME;
  int64_t  nbInitializedCluster;
	bool * tabNotInitializedCluster = new bool[_nbCluster];
	
	// 1. InitForUSER_PARTITION
	//-------------------------
	_parameter->initForInitUSER_PARTITION(nbInitializedCluster, tabNotInitializedCluster, initPartition);
	
	// 2.init random if needed
	//------------------------
	if (nbInitializedCluster != _nbCluster){
		// upadte tabIndividualCanBeUsedForInitRandom
    int64_t  i, k;
    int64_t ** initLabelValue = initPartition->_tabValue;
    int64_t  nbSampleCanBeUsedForInitRandom = _nbSample;
		bool * tabIndividualCanBeUsedForInitRandom = new bool[_nbSample];
		for (i=0; i<_nbSample; i++){
			tabIndividualCanBeUsedForInitRandom[i] = true;
			k=0;
			while (k<_nbCluster && tabIndividualCanBeUsedForInitRandom[i]){
				if (initLabelValue[i][k]==1){
					tabIndividualCanBeUsedForInitRandom[i] = false;
					nbSampleCanBeUsedForInitRandom--;
				}
				k++;
			}
		}
		if (nbSampleCanBeUsedForInitRandom < (_nbCluster - nbInitializedCluster)){
			throw tooManySampleInInitPartitionAndTooManyClusterNotRepresented;
		}
		
		double logLikelihood, bestLogLikelihood;
		XEMParameter * bestParameter = _parameter->clone();
		
		// 1rst random
		//-------------
		//cout<<"1rst random"<<endl;
		randomForInitRANDOMorUSER_PARTITION(tabIndividualCanBeUsedForInitRandom, tabNotInitializedCluster);
		// Compute log-likelihood 
		logLikelihood = getLogLikelihood(true);  // true : to compute fik
		bestLogLikelihood = logLikelihood;
		bestParameter->recopy(_parameter);
	/*cout<<"initRandom"<<endl<<"1er essai : "<<endl<<"Parameter : "<<endl;
		_parameter->edit();
		cout<<"LL : "<< bestLogLikelihood<<endl;*/
	
	
		// Others RANDOM
		//-------------
    for (i=1; i<nbTryInInit; i++){
	//		cout<<i+1<<" random"<<endl;
			randomForInitRANDOMorUSER_PARTITION(tabIndividualCanBeUsedForInitRandom, tabNotInitializedCluster);
		// Compute log-likelihood 
			logLikelihood = getLogLikelihood(true);  // true : to compute fik
			if (logLikelihood > bestLogLikelihood){
				bestLogLikelihood = logLikelihood;
				bestParameter->recopy(_parameter);
			}
		/*cout<<endl<<"initRandom"<<endl<<i+1<<" eme essai : "<<endl<<"Parameter : "<<endl;
			_parameter->edit();
			cout<<"LL : "<< logLikelihood<<endl;*/
		}
		
	// set best parameter
		delete _parameter;
		_parameter = bestParameter;
		_parameter->setModel(this);
	/*cout<<endl<<"initRandom"<<endl<<"meilleur essai : "<<endl<<"Parameter : "<<endl;
		_parameter->edit();
		cout<<"LL : "<< bestLogLikelihood<<endl;*/

		delete [] tabIndividualCanBeUsedForInitRandom;
	}
		
	delete [] tabNotInitializedCluster;
}







/*------------------------------------------------------
 					initSMALL_EM
 					------------
 					
  updated in this method : 
	- _parameter
	- _tabFik, _tabSumF, _tabCik, _tabTik, _tabNk (because an Estep is called to choose the bestParameter)
	Note : _tabFik, _tabSumF, _tabCik, _tabTik, _tabNk wil be 're'computed in the following EStep
	So only _parameter have to be updated in this method
	
-------------------------------------------------------*/
void XEMModel::initSMALL_EM(XEMClusteringStrategyInit * clusteringStrategyInit){
//  cout<<"init SMALL_EM, nbTryInInit="<<strategyInit->getNbTry()<<", nbIteration = "<<strategyInit->getNbIteration()<<", epsilon = "<<strategyInit->getEpsilon()<<endl;
  _algoName = EM;
  double logLikelihood, bestLogLikelihood;
  XEMParameter * bestParameter = _parameter->clone();
  int64_t  i, nbRunOfSmallEMOk = 0; 
  bestLogLikelihood = 0.0;
  for (i=0; i<clusteringStrategyInit->getNbTry(); i++){
    nbRunOfSmallEMOk++;
    try{
      // one run of small EM
      _parameter->reset();
      oneRunOfSmallEM(clusteringStrategyInit, logLikelihood);
//cout<<"sortie de oneRunOfSmallEM, LL = "<<logLikelihood<<endl;
      if ((nbRunOfSmallEMOk == 1) || (logLikelihood > bestLogLikelihood)){
        bestLogLikelihood = logLikelihood;
        bestParameter->recopy(_parameter);
//       cout<<"best LL dans SMALL_EM : " <<bestLogLikelihood<<endl;
      }
    }
    catch(XEMErrorType errorType){
      nbRunOfSmallEMOk--;
    }
  }
  
  if (nbRunOfSmallEMOk == 0){
    throw SMALL_EM_error;
  }

  // set best parameter
  delete _parameter;
  _parameter = bestParameter;
  _parameter->setModel(this);
//  cout<<"fin de init SMALL_EM, nb d'essais effectues="<<i<<endl;
}



void XEMModel::initSMALL_EM(XEMStrategyInit * strategyInit){
  //cout<<"init SMALL_EM, nbTryInInit="<<strategyInit->getNbTry()<<", nbIteration = "<<strategyInit->getNbIteration()<<", epsilon = "<<strategyInit->getEpsilon()<<endl;
  _algoName = EM;
  double logLikelihood, bestLogLikelihood;
  XEMParameter * bestParameter = _parameter->clone();
  int64_t  i, nbRunOfSmallEMOk = 0; 
  bestLogLikelihood = 0.0;
  for (i=0; i<strategyInit->getNbTry(); i++){
    nbRunOfSmallEMOk++;
    try{
      // one run of small EM
      _parameter->reset();
      oneRunOfSmallEM(strategyInit, logLikelihood);
//cout<<"sortie de oneRunOfSmallEM, LL = "<<logLikelihood<<endl;
      if ((nbRunOfSmallEMOk == 1) || (logLikelihood > bestLogLikelihood)){
        bestLogLikelihood = logLikelihood;
        bestParameter->recopy(_parameter);
//       cout<<"best LL dans SMALL_EM : " <<bestLogLikelihood<<endl;
      }
    }
    catch(XEMErrorType errorType){
      nbRunOfSmallEMOk--;
    }
  }
  
  if (nbRunOfSmallEMOk == 0){
    throw SMALL_EM_error;
  }

  // set best parameter
  delete _parameter;
  _parameter = bestParameter;
  _parameter->setModel(this);
 // cout<<"fin de init SMALL_EM, nb d'essais effectues="<<i<<endl;
}



//---------------------
// one run if small EM
//--------------------
void XEMModel::oneRunOfSmallEM(XEMStrategyInit * strategyInit, double & logLikelihood){
  double lastLogLikelihood, eps;
  eps = 1000;
  initRANDOM(1);
  Estep();
  Mstep();
  logLikelihood = getLogLikelihood(true);  // true : to compute fik
  int64_t  nbIteration = 1;
  bool continueAgain = true;
  while (continueAgain){
//    cout<<"while de oneRunOfSmallEM, nbIteration = "<<nbIteration<<endl;
         //(nbIteration < strategyInit->getNbIteration()) && (eps > strategyInit->getEpsilon())){
    lastLogLikelihood = logLikelihood;
    Estep();
    Mstep();
    nbIteration++; 
    // update continueAgain
    switch (strategyInit->getStopName()) {
      case NBITERATION :
        continueAgain = (nbIteration < strategyInit->getNbIteration());
        break;
      case EPSILON :
        logLikelihood = getLogLikelihood(true);  // true : to compute fik
        eps = fabs(logLikelihood - lastLogLikelihood);
        //continueAgain = (eps > strategyInit->getEpsilon());
        continueAgain = (eps > strategyInit->getEpsilon() && (nbIteration < maxNbIterationInInit)); // on ajoute un test pour ne pas faire trop d'iterations quand meme ....
        break;
      case NBITERATION_EPSILON :
        logLikelihood = getLogLikelihood(true);  // true : to compute fi
        eps = fabs(logLikelihood - lastLogLikelihood);
        continueAgain = ((eps > strategyInit->getEpsilon()) && (nbIteration < strategyInit->getNbIteration()));
        break;
        default : throw internalMixmodError;
    }
  }
  if (strategyInit->getStopName() == NBITERATION){ // logLikelihood is an output
    logLikelihood = getLogLikelihood(true);  // true : to compute fi
  }
//cout<<"Fin de oneRunOfSmallEM, nb d'iterations effectuees = "<<nbIteration<<", logLikelihood = "<<logLikelihood<<endl;
}



void XEMModel::oneRunOfSmallEM(XEMClusteringStrategyInit * clusteringStrategyInit, double & logLikelihood){
  double lastLogLikelihood, eps;
  eps = 1000;
  initRANDOM(1);
  Estep();
  Mstep();
  logLikelihood = getLogLikelihood(true);  // true : to compute fik
  int64_t  nbIteration = 1;
  bool continueAgain = true;
  while (continueAgain){
//    cout<<"while de oneRunOfSmallEM, nbIteration = "<<nbIteration<<endl;
         //(nbIteration < strategyInit->getNbIteration()) && (eps > strategyInit->getEpsilon())){
    lastLogLikelihood = logLikelihood;
    Estep();
    Mstep();
    nbIteration++; 
    // update continueAgain
    switch (clusteringStrategyInit->getStopName()) {
      case NBITERATION :
        continueAgain = (nbIteration < clusteringStrategyInit->getNbIteration());
        break;
      case EPSILON :
        logLikelihood = getLogLikelihood(true);  // true : to compute fik
        eps = fabs(logLikelihood - lastLogLikelihood);
        //continueAgain = (eps > strategyInit->getEpsilon());
        continueAgain = (eps > clusteringStrategyInit->getEpsilon() && (nbIteration < maxNbIterationInInit)); // on ajoute un test pour ne pas faire trop d'iterations quand meme ....
        break;
      case NBITERATION_EPSILON :
        logLikelihood = getLogLikelihood(true);  // true : to compute fi
        eps = fabs(logLikelihood - lastLogLikelihood);
        continueAgain = ((eps > clusteringStrategyInit->getEpsilon()) && (nbIteration < clusteringStrategyInit->getNbIteration()));
        break;
        default : throw internalMixmodError;
    }
  }
  if (clusteringStrategyInit->getStopName() == NBITERATION){ // logLikelihood is an output
    logLikelihood = getLogLikelihood(true);  // true : to compute fi
  }
//cout<<"Fin de oneRunOfSmallEM, nb d'iterations effectuees = "<<nbIteration<<", logLikelihood = "<<logLikelihood<<endl;
}





/*---------------------------------------------------
 initCEM_INIT
/*---------------------------------------------------
  updated in this method : 
	- _parameter
	- _tabFik, _tabSumF, _tabCik, _tabTik, _tabNk (because an Estep and a CStep are called to choose the bestParameter)
	Note : _tabFik, _tabSumF, _tabCik, _tabTik, _tabNk wil be 're'computed in the following EStep
	So only _parameter have to be updated in this method

---------------------------------------------------*/
void XEMModel::initCEM_INIT(XEMClusteringStrategyInit * clusteringStrategyInit){
  //cout<<"init CEM, nbTryInInit="<<strategyInit->getNbTry()<<endl;
  _algoName = CEM;
  int64_t  i;
  double cLogLikelihood, oldLogLikelihood, bestCLogLikelihood;
  XEMParameter * bestParameter = _parameter->clone();
  int64_t  nbRunOfCEMOk = 0;
  bestCLogLikelihood = 0.0;
  
  for (i=0; i<clusteringStrategyInit->getNbTry(); i++){
    nbRunOfCEMOk++;
    try{
      _parameter->reset(); // reset to default values
      initRANDOM(1);
      _algoName = CEM;
      int64_t  nbIter = 0;
      bool fin = false;
      while (!fin && nbIter<=maxNbIterationInCEM_INIT){
        Estep();
        Cstep();
        Mstep();
        nbIter++;
        if (nbIter == 1){
          oldLogLikelihood = getCompletedLogLikelihood();
        }
        else{
          cLogLikelihood = getCompletedLogLikelihood();
          if (cLogLikelihood == oldLogLikelihood){
            fin = true;
          }
          else{
            oldLogLikelihood = cLogLikelihood;
          }
        }
      }
      //cout<<"dans init CEM, nb d'iterations effectuées : "<<nbIter<<endl;
    // Compute log-likelihood 
      cLogLikelihood = getCompletedLogLikelihood();
    // Comparaison of log-likelihood between step p and p-1 
      if ((nbRunOfCEMOk==1) || (cLogLikelihood > bestCLogLikelihood)){
        bestCLogLikelihood = cLogLikelihood;
        bestParameter->recopy(_parameter);
      }
      //cout<<"nbIter : "<<nbIter<<endl;
    }
    catch (XEMErrorType errorType){
      nbRunOfCEMOk--;
    }
  }

  if (nbRunOfCEMOk==0){
    delete _parameter;
    _parameter = bestParameter;
    _parameter->setModel(this);
    throw CEM_INIT_error;
  }
  
  //cout<<"fin de init CEM, nb d'essais effectues="<<i<<endl;
  // set Best parameter
  delete _parameter;
  _parameter = bestParameter;
  _parameter->setModel(this);

}

void XEMModel::initCEM_INIT(XEMStrategyInit * strategyInit){
  //cout<<"init CEM, nbTryInInit="<<strategyInit->getNbTry()<<endl;
  _algoName = CEM;
  int64_t  i;
  double cLogLikelihood, oldLogLikelihood, bestCLogLikelihood;
  XEMParameter * bestParameter = _parameter->clone();
  int64_t  nbRunOfCEMOk = 0;
  bestCLogLikelihood = 0.0;
  
  for (i=0; i<strategyInit->getNbTry(); i++){
    nbRunOfCEMOk++;
    try{
      _parameter->reset(); // reset to default values
      initRANDOM(1);
      _algoName = CEM;
      int64_t  nbIter = 0;
      bool fin = false;
      while (!fin && nbIter<=maxNbIterationInCEM_INIT){
        Estep();
        Cstep();
        Mstep();
        nbIter++;
        if (nbIter == 1){
          oldLogLikelihood = getCompletedLogLikelihood();
        }
        else{
          cLogLikelihood = getCompletedLogLikelihood();
          if (cLogLikelihood == oldLogLikelihood){
            fin = true;
          }
          else{
            oldLogLikelihood = cLogLikelihood;
          }
        }
      }
      //cout<<"dans init CEM, nb d'iterations effectuées : "<<nbIter<<endl;
    // Compute log-likelihood 
      cLogLikelihood = getCompletedLogLikelihood();
    // Comparaison of log-likelihood between step p and p-1 
      if ((nbRunOfCEMOk==1) || (cLogLikelihood > bestCLogLikelihood)){
        bestCLogLikelihood = cLogLikelihood;
        bestParameter->recopy(_parameter);
      }
      //cout<<"nbIter : "<<nbIter<<endl;
    }
    catch (XEMErrorType errorType){
      nbRunOfCEMOk--;
    }
  }

  if (nbRunOfCEMOk==0){
    delete _parameter;
    _parameter = bestParameter;
    _parameter->setModel(this);
    throw CEM_INIT_error;
  }
  
  //cout<<"fin de init CEM, nb d'essais effectues="<<i<<endl;
  // set Best parameter
  delete _parameter;
  _parameter = bestParameter;
  _parameter->setModel(this);

}





/*---------------------------------------------------------
 					Initialization by SEM
 					---------------------
 					
  updated in this method : 
		- _parameter
		- _tabFik, _tabSumF, _tabCik, _tabTik, _tabNk (because an Estep and a SStep are called to choose the bestParameter)
	Note : _tabFik, _tabSumF, _tabCik, _tabTik, _tabNk wil be 're'computed in the following EStep
	So, only _parameter have to be updated in this method
	
-------------------------------------------------------*/
void XEMModel::initSEM_MAX(XEMClusteringStrategyInit * clusteringStrategyInit){
  //cout<<"init SEM_MAX, nbTryInInit="<<strategyInit->getNbIteration()<<endl;
  _algoName = SEM;
  int64_t  j;
  double logLikelihood, bestLogLikelihood;
  XEMParameter * bestParameter = _parameter->clone();
  int64_t  nbRunOfSEMMAXOk = 0;
  bestLogLikelihood = 0.0;
//  int64_t  bestIndex=0;
  
  for (j=0; j<clusteringStrategyInit->getNbIteration(); j++){
    nbRunOfSEMMAXOk++;
    try{
      _parameter->reset();
      initRANDOM(1);
      Estep();
      Sstep();
      Mstep();
      // Compute log-likelihood 
      logLikelihood = getLogLikelihood(true);  // true : to compute fik
      if ((nbRunOfSEMMAXOk==1) || (logLikelihood > bestLogLikelihood)){
        bestLogLikelihood = logLikelihood;
        bestParameter->recopy(_parameter);
//        bestIndex = j;
      }
    }
    catch (XEMErrorType errorType){
      nbRunOfSEMMAXOk--;
    }
  }
  
  if (nbRunOfSEMMAXOk==0){
    throw SEM_MAX_error;
  }
  
  //cout<<"fin de init SEM_MAX, nb d'iterations effectuees="<<j<<" meilleure solution : "<<bestIndex<<endl;
  // set best parameter
  delete _parameter;
  _parameter = bestParameter;
  _parameter->setModel(this);
}

void XEMModel::initSEM_MAX(XEMStrategyInit * strategyInit){
  //cout<<"init SEM_MAX, nbTryInInit="<<strategyInit->getNbIteration()<<endl;
  _algoName = SEM;
  int64_t  j;
  double logLikelihood, bestLogLikelihood;
  XEMParameter * bestParameter = _parameter->clone();
  int64_t  nbRunOfSEMMAXOk = 0;
  bestLogLikelihood = 0.0;
//  int64_t  bestIndex=0;
  
  for (j=0; j<strategyInit->getNbIteration(); j++){
    nbRunOfSEMMAXOk++;
    try{
      _parameter->reset();
      initRANDOM(1);
      Estep();
      Sstep();
      Mstep();
      // Compute log-likelihood 
      logLikelihood = getLogLikelihood(true);  // true : to compute fik
      
      if ((nbRunOfSEMMAXOk==1) || (logLikelihood > bestLogLikelihood)){
        bestLogLikelihood = logLikelihood;
        bestParameter->recopy(_parameter);
//        bestIndex = j;
      }
    }
    catch (XEMErrorType errorType){
      nbRunOfSEMMAXOk--;
    }
  }
  
  if (nbRunOfSEMMAXOk==0){
    throw SEM_MAX_error;
  }
  
  //cout<<"fin de init SEM_MAX, nb d'iterations effectuees="<<j<<" meilleure solution : "<<bestIndex<<endl;
  // set best parameter
  delete _parameter;
  _parameter = bestParameter;
  _parameter->setModel(this);
}









//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// 																	Algorithms
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------




/*--------------------------------------------------------------------------------------------
 	MAP step : Maximum a Posteriori procedure
 	---------
  MAP procedure consists in assigning a point to the group maximing this conditional probabiliy  
	 
	Note : MAP follows an USER initialisation
	----

	already updated : 
	- _parameter (by USER initialisation)
	
	updated in this method :
	- _tabFik, _tabCik, _tabTik, _tabNk (in Estep called in this method)
	- _tabCik, _tabNk (called by Cstep in this method)
----------------------------------------------------------------------------------------------*/
void XEMModel::MAPstep(){
  Estep();  // to compute _tabFik, _tabTik, _tabCik, _tabNk
  Cstep();  // to compute _tabCik (by MAP procedure) and _tabNk 	
}





/*--------------------------------------------------------------------------------------------
 	E step : Expectation
 	------

	Note : Estep follows a Mstep or an initialsation (so _parameter is updated)
	----
	
	already updated :
	- _parameter
	
	updated in this method :
	- _tabFik, _tabCik, _tabTik, _tabNk

--------------------------------------------------------------------------------------------*/
void XEMModel::Estep(){

	// 1. compute fik
	//---------------
	computeFik();
	
	//2. compute tik (conditional probabilities (posteriori probabilities)) 
	//---------------
  int64_t  k;
  int64_t  i;
  for (i=0; i<_nbSample; i++){
    if (_tabSumF[i]==0.0){
      _parameter->computeTikUnderflow(i, _tabTik);
    }
    else{
      for (k=0; k<_nbCluster; k++){
        _tabTik[i][k] = _tabFik[i][k] / _tabSumF[i] ;
      }
    }

		// 3. compute cik
		//---------------
    if (!_tabZiKnown[i]){
      for (k=0;k<_nbCluster;k++){
        _tabCik[i][k] = _tabTik[i][k];
      }
    }
  }
 //4. compute nk
	//------------
	computeNk();
}
 




/*--------------------------------------------------------------------------------------------
 	M step
	------

	Note : Mstep follows an Estep, Cstep, Step, or USER_PARTITION initialisation
	----
	
	already updated :
	- _tabFik, _tabCik, _tabTik, _tabNk  
	
	updated in this method :
	- _parameter
--------------------------------------------------------------------------------------------*/
void XEMModel::Mstep(){
  _parameter->MStep();
}





/*--------------------------------------------------------------------------------------------
	S step
	------
  // S Step : Stochastic Classification

	Note : Sstep follows an Estep
	----
	
	already updated :
	- _tabFik, _tabCik, _tabTik, _tabNk, _parameter  
	
	updated in this method :
	- _tabCik, _tabNk
--------------------------------------------------------------------------------------------*/
void XEMModel::Sstep(){
  int64_t  i;
  int64_t  k;
  double ** cumTabT = new double*[_nbSample];

  for (i=0; i<_nbSample; i++){
    cumTabT[i]    = new double[_nbCluster];
    cumTabT[i][0] = _tabTik[i][0];
  }
  for (k=1; k<_nbCluster; k++){
    for (i=0; i<_nbSample; i++){
      cumTabT[i][k] = _tabTik[i][k] + cumTabT[i][k-1];
    }
  }

  double * tabRnd = new double[_nbSample];
  for (i=0; i<_nbSample; i++){
    tabRnd[i] = rnd();
  }

  for (i=0; i<_nbSample; i++){
    if (!_tabZiKnown[i]){
      for (k=0; k<_nbCluster; k++){
        _tabCik[i][k] = 0;
      }
      k=0;
      while ((k<_nbCluster) && (tabRnd[i] > cumTabT[i][k])){
        k++;
      }
      if (tabRnd[i] <= cumTabT[i][k]){
        _tabCik[i][k] = 1;
      }
      else{
        throw internalMixmodError;
      }
    } 
  }

  for (i=0; i<_nbSample; i++){
    delete[] cumTabT[i];
  }
  delete[] cumTabT;
  delete[] tabRnd;

 //update _tabNk
	computeNk();
}





/*--------------------------------------------------------------------------------------------
	C step
	------
  Classification Step
	
	Note : Cstep follows an Estep
	----
	
	already updated :
	- _tabFik, _tabCik, _tabTik, _tabNk, _parameter  
	
	updated in this method :
	- _tabCik, _tabNk
--------------------------------------------------------------------------------------------*/
void XEMModel::Cstep(){
  int64_t  k, kMax;
  int64_t  i;
  double tikMax = 0;
  for (i=0;i<_nbSample;i++){
    if (!_tabZiKnown[i]){
      kMax = 0;
      tikMax = _tabTik[i][0];
      for (k=1; k<_nbCluster; k++){
        if (_tabTik[i][k] > tikMax){
          tikMax = _tabTik[i][k];
          kMax = k;
        }
      }
      for (k=0;k<_nbCluster;k++){
        _tabCik[i][k] = 0;
      }
      _tabCik[i][kMax] = 1;
    }
 }
 
 
 //update _tabNk
 if(_algoName == UNKNOWN_ALGO_NAME)	
    throw;

 if(_algoName != MAP){
   computeNk();
 }
}






//-----------------------
//  debug information
//-----------------------
void XEMModel::editDebugInformation(){
#if DEBUG > 0
  _parameter->edit();
  #if DEBUG > 1
    editFik();
    editTik();
  #endif
#endif
}



//---------
// edit Fik
//---------
void XEMModel::editFik(){
  
#ifdef DEBUG
  int64_t  i,k;
  for (i=0; i<_nbSample; i++){
    for (k=0; k<_nbCluster; k++){
      cout<<"\tfik["<<i<<"]["<<k<<"]="<<_tabFik[i][k];
    }
    cout<<"\n";
  }
#endif
}


//---------
// edit Tik
//---------
void XEMModel::editTik(){
  
#ifdef DEBUG
  int64_t  i,k;
  for (i=0; i<_nbSample; i++){
    for (k=0; k<_nbCluster; k++){
      cout<<"\ttik["<<i<<"]["<<k<<"]="<<_tabTik[i][k];
    }
    cout<<"\n";
  }
#endif
}


//---------
// edit Cik
//---------
void XEMModel::editCik(){
#ifdef DEBUG
  int64_t  i,k;
	for (i=0; i<_nbSample; i++){
		for (k=0; k<_nbCluster; k++){
			cout<<"\tcik["<<i<<"]["<<k<<"]="<<_tabCik[i][k];
		}
		cout<<"\n";
	}
#endif
}



//-----------
// edit tabNk
//-----------
void XEMModel::editNk(){
#ifdef DEBUG
  int64_t  k;
	for (k=0; k<_nbCluster; k++){
		cout<<"\tnk["<<k<<"]="<<_tabNk[k]<<"\n";
	}
#endif
}

