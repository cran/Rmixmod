/***************************************************************************
                             SRC/MIXMOD/XEMOldModelOutput.cpp  description
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
#include "XEMOldModelOutput.h"
#include "XEMGaussianParameter.h"



//------------
// Constructor
//------------
// Default constructor
XEMOldModelOutput::XEMOldModelOutput(){
  _param = NULL;
  _tabCriterionOutput = NULL;
  _likelihoodOutput = NULL;
  _probaOutput = NULL;
  _errorType = internalMixmodError;  // because the fields are not updated !
	_modelType = NULL;
	_rankOfBestModelType = 0;
}



//-----------
// Destructor
//-----------
XEMOldModelOutput::~XEMOldModelOutput(){
  int64_t  i;

  if (_param){
    delete _param;
  }

  if (_tabCriterionOutput){
    for (i=0; i<_nbCriterionOutput; i++){
       delete _tabCriterionOutput[i];
    }
    delete [] _tabCriterionOutput;
    _tabCriterionOutput = NULL;
  }

  if (_likelihoodOutput){
     delete _likelihoodOutput;
  }

  if (_probaOutput){
     delete _probaOutput;
  }
}



//-----------------
// setAllModelError
//-----------------
void XEMOldModelOutput::setErrorType(XEMErrorType error){
   _errorType = error;
}



//---------
// udpate 1
//---------
void XEMOldModelOutput::update(XEMSelection * selection, XEMEstimation ** tabEstimation, int64_t nbModelType){
  // this method must be called once time only : at the end of calculation
  // If not, memory leaks appear (because of new in this method and delete in the destructor)

  if (_param != NULL)  // 1rst called ?
    throw internalMixmodError;

  _errorType = selection->getErrorType();
  if (_errorType == noError){
    int64_t  bestIndexEstmimation = selection->getBestIndexEstimation();
    XEMEstimation * bestEstimation = tabEstimation[bestIndexEstmimation];
		
		// Begin Bug 12464
		// old version :
		//_rankOfBestModelType = bestIndexEstmimation+1; 
		// correction
		_rankOfBestModelType = (bestIndexEstmimation%nbModelType)+1;
		// End Bug 12464
    
		XEMModel * bestModel = bestEstimation->getModel();
    XEMParameter * bestParam = bestModel->getParameter();

    _modelType = bestEstimation->getModelType();
    _nbCluster = bestEstimation->getNbCluster();
    _strategy = bestEstimation->getStrategy();
    _cStrategy = bestEstimation->getClusteringStrategy();
    _param = bestParam->clone();
//    _param = new XEMGaussianParameter(bestEstimation->getLastModel());

    _likelihoodOutput = new XEMLikelihoodOutput(bestModel);
  
    _probaOutput = new XEMProbaOutput(bestEstimation);
    if (selection->getCriterionName() == CV){
      int64_t * tabCVLabel = selection->getCVLabelOfBestEstimation();
      int64_t * workingTabCVLabel = tabCVLabel;
      const vector<int64_t> & correspondenceOriginDataToReduceData = bestEstimation->getcorrespondenceOriginDataToReduceData();
      int64_t nbSample = correspondenceOriginDataToReduceData.size();
      if (correspondenceOriginDataToReduceData.size() != 0){ // binary cas
        workingTabCVLabel = new int64_t[nbSample];
        for (int64_t i=0; i<nbSample; i++){
          workingTabCVLabel[i] = tabCVLabel[correspondenceOriginDataToReduceData[i]];
        }
      }
      _probaOutput->setCVLabel(workingTabCVLabel);
    }

    //_tabCriterionOutput
    _nbCriterionOutput = 1;
    _tabCriterionOutput = new XEMCriterionOutput*[_nbCriterionOutput];
    XEMCriterionName criterionName = selection->getCriterionName();
    double criterionValue = selection->getCriterionValue(bestEstimation);
    XEMErrorType criterionErrorType = selection->getCriterionErrorType(bestEstimation);
   // XEMCriterion * criterion = selection->getCriterion(bestEstimation);
    _tabCriterionOutput[0] = new XEMCriterionOutput(criterionName, criterionValue, criterionErrorType);
  }
}


//update1 //TODO : tmp
void XEMOldModelOutput::update1(XEMEstimation * bestEstimation, int64_t indexCriterion){
  // this method must be called once time only : at the end of calculation
  // If not, memory leaks appear (because of new in this method and delete in the destructor)

  if (_param != NULL)  // 1rst called ?
    throw internalMixmodError;

  //_errorType = selection->getErrorType();
  _errorType = bestEstimation->getErrorType();
  if (_errorType == noError){

    XEMModel * bestModel = bestEstimation->getModel();
    XEMParameter * bestParam = bestModel->getParameter();

    _modelType = bestEstimation->getModelType();
    _nbCluster = bestEstimation->getNbCluster();
    _strategy = bestEstimation->getStrategy();
    _cStrategy = bestEstimation->getClusteringStrategy();
    _param = bestParam->clone();
//    _param = new XEMGaussianParameter(bestEstimation->getLastModel());

    _likelihoodOutput = new XEMLikelihoodOutput(bestModel);
  
    _probaOutput = new XEMProbaOutput(bestEstimation);
    //TODO
//     if (bestEstimation->getCriterionName() == CV){
//      int64_t * tabCVLabel = selection->getCVLabelOfBestEstimation();
//       int64_t * workingTabCVLabel = tabCVLabel;
//       const vector<int64_t> & correspondenceOriginDataToReduceData = bestEstimation->getcorrespondenceOriginDataToReduceData();
//       int64_t nbSample = correspondenceOriginDataToReduceData.size();
//       if (correspondenceOriginDataToReduceData.size() != 0){ // binary cas
//         workingTabCVLabel = new int64_t[nbSample];
//         for (int64_t i=0; i<nbSample; i++){
//           workingTabCVLabel[i] = tabCVLabel[correspondenceOriginDataToReduceData[i]];
//         }
//       }
//       _probaOutput->setCVLabel(workingTabCVLabel);
//     }

    //_tabCriterionOutput
    _nbCriterionOutput = 1;
    _tabCriterionOutput = new XEMCriterionOutput*[_nbCriterionOutput];
    vector<XEMCriterionOutput> criterionOutput = bestEstimation->getCriterionOutput();
    _tabCriterionOutput[0] = new XEMCriterionOutput(criterionOutput[indexCriterion].getCriterionName(), criterionOutput[indexCriterion].getValue(), criterionOutput[indexCriterion].getError());
  }
}






//---------
// udpate 2
//---------
void XEMOldModelOutput::update(XEMEstimation * estimation, int64_t  nbSelection, XEMSelection ** tabSelection){
  // this method must be called once only : at the end of calculation
  // If not, memory leaks appear (because of new in this method and delete in the destructor)

  int64_t  i;
  
  if (_param != NULL)  // 1rst called ?
    throw internalMixmodError;

  _errorType = estimation->getErrorType();
  if (_errorType == noError){
    XEMModel * bestModel = estimation->getModel();
    XEMParameter * bestParam = bestModel->getParameter();
    
    _modelType = estimation->getModelType();
    _nbCluster = estimation->getNbCluster();
    _strategy = estimation->getStrategy();
    _cStrategy = estimation->getClusteringStrategy();
    _param = bestParam->clone();

//    _param = new XEMGaussianParameter(estimation->getLastModel());

    _likelihoodOutput = new XEMLikelihoodOutput(bestModel);
    _probaOutput = new XEMProbaOutput(estimation);

    //_tabCriterionOutput
    _nbCriterionOutput = nbSelection;
    _tabCriterionOutput = new XEMCriterionOutput*[_nbCriterionOutput];
    for (i=0; i<_nbCriterionOutput; i++){
      XEMCriterionName criterionName = tabSelection[i]->getCriterionName();
      double criterionValue = tabSelection[i]->getCriterionValue(estimation);
      XEMErrorType criterionErrorType = tabSelection[i]->getCriterionErrorType(estimation);
      //XEMCriterion * criterion = tabSelection[i]->getCriterion(estimation);
      _tabCriterionOutput[i] = new XEMCriterionOutput(criterionName, criterionValue, criterionErrorType);
    }
  }
  else{
    _modelType = estimation->getModelType();
    _nbCluster = estimation->getNbCluster();
    _strategy = estimation->getStrategy();
    _cStrategy = estimation->getClusteringStrategy();
  }

}



//////////////////////
//////////////////////
  /// update //TODO
  // update 2 bis
void XEMOldModelOutput::update2(XEMEstimation * estimation){
  // this method must be called once only : at the end of calculation
  // If not, memory leaks appear (because of new in this method and delete in the destructor)

  int64_t  i;

  if (_param != NULL)  // 1rst called ?
    throw internalMixmodError;

  _errorType = estimation->getErrorType();
  if (_errorType == noError){
    XEMModel * bestModel = estimation->getModel();
    XEMParameter * bestParam = bestModel->getParameter();
    
    _modelType = estimation->getModelType();
    _nbCluster = estimation->getNbCluster();
    _strategy = estimation->getStrategy();
    _cStrategy = estimation->getClusteringStrategy();
    _param = bestParam->clone();

//    _param = new XEMGaussianParameter(estimation->getLastModel());

    _likelihoodOutput = new XEMLikelihoodOutput(bestModel);
    _probaOutput = new XEMProbaOutput(estimation);

    //_tabCriterionOutput
    _nbCriterionOutput = estimation->getCriterionSize();
    _tabCriterionOutput = new XEMCriterionOutput*[_nbCriterionOutput];
    vector<XEMCriterionOutput> criterionOutput = estimation->getCriterionOutput();
    for (i=0; i<_nbCriterionOutput; i++){
      _tabCriterionOutput[i] = new XEMCriterionOutput(criterionOutput[i].getCriterionName(), criterionOutput[i].getValue(), criterionOutput[i].getError());
    }
  }
  else{
    _modelType = estimation->getModelType();
    _nbCluster = estimation->getNbCluster();
    _strategy = estimation->getStrategy();
    _cStrategy = estimation->getClusteringStrategy();
  }

}

