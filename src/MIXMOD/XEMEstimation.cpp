/***************************************************************************
                             SRC/MIXMOD/XEMEstimation.cpp  description
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
#include "XEMEstimation.h"
#include "XEMStrategy.h"
#include "XEMClusteringStrategy.h"
#include "XEMBICCriterion.h"
#include "XEMICLCriterion.h"
#include "XEMNECCriterion.h"
#include "XEMCVCriterion.h"
#include "XEMDCVCriterion.h"


//------------
// Constructor
//------------
XEMEstimation::XEMEstimation(){
  _strategy      = NULL;
  _cStrategy      = NULL;
  _modelType     = NULL;
  _errorType     = noError;
  _model = NULL;
}


//------------------
// copy constructor
//-----------------
XEMEstimation::XEMEstimation(const XEMEstimation & estimation){
  throw internalMixmodError;
}


//------------
// Constructor
//------------
XEMEstimation::XEMEstimation(XEMClusteringStrategy *& strategy, XEMModelType * modelType, int64_t nbCluster, XEMData *& data ,vector<XEMCriterionName> & criterionName,  XEMPartition *& knownPartition, vector<int64_t> corresp){
  _modelType     = modelType;
  _nbCluster     = nbCluster;
  _cStrategy     = strategy;
  _strategy      = NULL;
  _errorType     = noError;
  _correspondenceOriginDataToReduceData  = corresp;
  _model = new XEMModel(modelType, nbCluster, data, knownPartition);
  
  int64_t nbCriterion = criterionName.size();
  _criterionOutput.resize(nbCriterion);
  _criterion.resize(nbCriterion);
  for (int64_t i=0; i<nbCriterion; i++){
      switch (criterionName[i]) {
        case BIC : _criterion[i] = new XEMBICCriterion(); break;
        case CV :  
          throw XEMDAInput;
    //_tabCriterion[i] = new XEMCVCriterion(input);  break;
        case ICL : _criterion[i] = new XEMICLCriterion(); break;
        case NEC : _criterion[i] = new XEMNECCriterion(); break;
        case UNKNOWN_CRITERION_NAME : throw internalMixmodError;break;
        default : throw internalMixmodError;
      }
  }
}



//------------
// Constructor
//------------
XEMEstimation::XEMEstimation(XEMStrategy *& strategy, XEMModelType * modelType, int64_t nbCluster, XEMData *& data,  XEMPartition *& knownPartition, vector<int64_t> corresp){
  _modelType     = modelType;
  _nbCluster     = nbCluster;
  _strategy      = strategy;
  _cStrategy     = NULL;
  _errorType     = noError;
  _correspondenceOriginDataToReduceData  = corresp;
  _model = new XEMModel(modelType, nbCluster, data, knownPartition);
}



//-----------
// Destructor
//-----------
XEMEstimation::~XEMEstimation(){
//   if (_strategy){
//     delete _strategy;
//     _strategy = NULL;
//   }
  if (_model){
    delete _model;
    _model = NULL;
  }
}



//---
//run
//---
void XEMEstimation::run(){
  try{
    // compute strategy
    //-----------------
    if (_strategy){
      _strategy->run(_model);
    }
    else{
      _cStrategy->run(_model);
    }
  }
  catch(XEMErrorType & e){
     setErrorType(e); 
    for (int64_t i=0; i<_criterion.size(); i++){
      _criterionOutput[i].setCriterionName(_criterion[i]->getCriterionName());
      _criterionOutput[i].setError(errorEstimationStrategyRun);
      _criterionOutput[i].setValue(0);
    }
    return;
  }
  
  // compute criterion
  //------------------
  double value;
  XEMErrorType error;
  for (int64_t i=0; i<_criterion.size(); i++){
    _criterion[i]->run(_model, value, error);
    _criterionOutput[i].setCriterionName(_criterion[i]->getCriterionName());
    _criterionOutput[i].setError(error);
    _criterionOutput[i].setValue(value);
  }
  
}

//---------
//set error
//---------
void XEMEstimation::setErrorType(XEMErrorType errorType){
  _errorType = errorType;
}



//--------------
//Friend Methods
//--------------
ostream & operator << (ostream & fo, XEMEstimation & estimation){
  return fo;
}

