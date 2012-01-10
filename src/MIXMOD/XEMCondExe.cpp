/***************************************************************************
                             SRC/MIXMOD/XEMCondExe.cpp  description
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
#include "XEMCondExe.h"
#include "XEMUtil.h"
#include "XEMClusteringOutput.h"
#include <fstream>

//------------
// Constructor
//------------
// Default constructor
XEMCondExe::XEMCondExe(){
  _data = NULL;
	_weight = NULL;
	_nbSample = 0;
  _pbDimension = 0;
  _nbNbCluster = 0;
  _tabNbCluster = NULL;
  _nbCriterion = 0;
  _tabCriterionName = NULL;
  _nbModel = 0;
  _tabModel = NULL;
  _strategy = NULL;
  _cStrategy = NULL;
  _knownPartition = NULL;
  _tabEstimationError = NULL;
  _tabCriterionError = NULL;
  _tabSelectionError = NULL;
  _errorMixmod = noError;
}



//------------
// Constructor
//------------
XEMCondExe::XEMCondExe(XEMOldInput * input, XEMEstimation ** tabEstimation, int64_t nbEstimation){
  int64_t i,j;

  if (input == NULL || tabEstimation == NULL || nbEstimation < 1)
    throw internalMixmodError;

	_data = input->_data;
	_weight = _data->_weight;
  _nbSample = input->_nbSample;
  _weightTotal = input->_data->_weightTotal;
  _pbDimension = input->_pbDimension;
  _nbNbCluster = input->_nbNbCluster;
  _tabNbCluster = input->_tabNbCluster;
  _nbCriterion = input->_nbCriterionName;
  _tabCriterionName = input->_tabCriterionName;
  _nbModel = input->_nbModelType;
  _tabModel = input->_tabModelType;
  XEMStrategy ** tabStra = input->_tabStrategy;
  _strategy = tabStra[0];
  _cStrategy = NULL;

	XEMPartition ** tabKnownPartition = input->_tabKnownPartition;
  if (tabKnownPartition){
    _knownPartition = tabKnownPartition[0];
  }
  else{
    _knownPartition = NULL;
  }
	
  // _errorMixmod
  _errorMixmod = noError;

  // _tabEstimationError
  _tabEstimationError = new XEMErrorType[nbEstimation];
  for (i=0; i<nbEstimation; i++){
     _tabEstimationError[i] = noError;
  }

  // _tabSelectionError
  _tabSelectionError = new XEMErrorType[_nbCriterion];
  for (i=0; i<_nbCriterion; i++){
     _tabSelectionError[i] = noError;
  }

  // _tabCriterionError
  _tabCriterionError = new XEMErrorType*[_nbCriterion];
  for(i=0; i<_nbCriterion; i++){
    _tabCriterionError[i] = new XEMErrorType[nbEstimation];
    for (j=0; j<nbEstimation; j++){
      _tabCriterionError[i][j] = noError;
    }
  }
  
}


//------------
// Constructor
//------------
XEMCondExe::XEMCondExe(XEMClusteringInput * input, XEMClusteringOutput * clusteringOutput){
  int64_t i,j;
  int64_t nbEstimation = clusteringOutput->getNbClusteringModelOutput();

//  if (input == NULL || tabEstimation == NULL || nbEstimation < 1)
  //  throw internalMixmodError;

  _data = (input->getDataDescription()).getData();
  _weight = _data->_weight;
  _nbSample = input->getNbSample();
  _weightTotal = _data->_weightTotal;
  _pbDimension = input->getPbDimension();
  vector<int64_t> vNbCluster = input->getNbCluster();
  _nbNbCluster = vNbCluster.size();
  _tabNbCluster = new int64_t[_nbNbCluster];
  for (int64_t i=0; i<_nbNbCluster; i++){
    _tabNbCluster[i] = vNbCluster[i];
  }
  _nbCriterion = input->getNbCriterionName();
  _tabCriterionName = new XEMCriterionName[_nbCriterion];
  for (int64_t i=0; i<_nbCriterion; i++){
    _tabCriterionName[i] = input->getCriterionName(i);
  }
  _nbModel = input->getNbModelType();
  _tabModel = new XEMModelType*[_nbModel];
  for (int64_t i=0; i<_nbModel; i++){
    _tabModel[i] = input->getModelType(i);
  }
  
  _strategy=NULL;
  _cStrategy = input->getStrategy();

  _knownPartition = input->getKnownPartition();
  
  // _errorMixmod
  _errorMixmod = noError;

  // _tabEstimationError
  _tabEstimationError = new XEMErrorType[nbEstimation];
  for (i=0; i<nbEstimation; i++){
    XEMClusteringModelOutput * cMOutput = clusteringOutput->getClusteringModelOutput(i);
     _tabEstimationError[i] = cMOutput->getStrategyRunError();
  }

  // _tabSelectionError
  _tabSelectionError = new XEMErrorType[_nbCriterion];
  for (i=0; i<_nbCriterion; i++){
     _tabSelectionError[i] = noError;
  }

  _tabCriterionError = new XEMErrorType*[_nbCriterion];
  for(i=0; i<_nbCriterion; i++){
    _tabCriterionError[i] = new XEMErrorType[nbEstimation];
    for (j=0; j<nbEstimation; j++){
      XEMClusteringModelOutput * cMOutput = clusteringOutput->getClusteringModelOutput(j);
      _tabCriterionError[i][j] = cMOutput->getStrategyRunError();
      if (_tabCriterionError[i][j] == noError){
        _tabCriterionError[i][j] = cMOutput->getEstimation()->getCriterionOutput(i).getError();
      }
    }
  }
  
}


//----------
//Destructor
//----------
XEMCondExe::~XEMCondExe(){
  int64_t i;

  if (_tabEstimationError){
     delete [] _tabEstimationError;
  }
  if (_tabSelectionError){
     delete [] _tabSelectionError;
  }
  if (_tabCriterionError){
     for(i=0; i<_nbCriterion;i++){
       delete[] _tabCriterionError[i];
       _tabCriterionError[i] = NULL;
     }
     delete [] _tabCriterionError;
  }
}



//-----------------------
// editTabEstimationError
//-----------------------
void XEMCondExe::editTabEstimationError(ofstream & oFile){
  int64_t i;
  int64_t n = _nbNbCluster*_nbModel;
   for (i=0; i<n; i++){
     oFile<<_tabEstimationError[i]<<endl;
   }
}



//----------------------
// editTabCriterionError
//----------------------
void XEMCondExe::editTabCriterionError(ofstream & oFile, int64_t iCrit){
  int64_t j;
  int64_t n = _nbNbCluster*_nbModel;
  for (j=0; j<n; j++){
    oFile<<_tabCriterionError[iCrit][j]<<endl;
  }
}



//------
//update
//------
void XEMCondExe::update(int64_t nbEstimation, XEMEstimation ** tabEstimation, int64_t nbSelection, XEMSelection ** tabSelection){
  int64_t i,j;
  
  for (i=0; i<nbEstimation; i++){
    _tabEstimationError[i] = tabEstimation[i]->getErrorType();
  }
  for (i=0; i<nbSelection; i++){
    _tabSelectionError[i] = tabSelection[i]->getErrorType();
  }
  for (i=0; i<nbSelection; i++){
    for (j=0; j<nbEstimation; j++){
      XEMErrorType criterionErrorType = tabSelection[i]->getCriterionErrorType(j);
      _tabCriterionError[i][j] = criterionErrorType;
    }
  }

}
