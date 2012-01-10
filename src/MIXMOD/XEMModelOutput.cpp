/***************************************************************************
                             SRC/MIXMOD/XEMModelOutput.cpp  description
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
#include "XEMModelOutput.h"



//--------------------
// Default Constructor
//--------------------
XEMModelOutput::XEMModelOutput(){
}

  
//-----------------
//  Copy constructor
//-----------------
XEMModelOutput::XEMModelOutput(const XEMModelOutput & modelOutput){
  throw internalMixmodError;
}
  


//------------------------------
//  Initialization constructor 1
//------------------------------
XEMModelOutput::XEMModelOutput(XEMEstimation * estimation){
  if (!estimation){
    throw nullPointerError;
  }
  _estimation = estimation;
  
  _modelType = *(estimation->getModelType());
  _nbCluster = estimation->getNbCluster();
  _criterionOutput = estimation->getCriterionOutput();

  
  _strategyRunError = estimation->getErrorType();
  if (_strategyRunError == noError){
    _probaDescription = new XEMProbaDescription(estimation);
    _labelDescription = new XEMLabelDescription(estimation);
    _parameterDescription = new XEMParameterDescription(estimation);
  }
  else{
    _probaDescription = NULL;
    _labelDescription = NULL;
    _parameterDescription = NULL;
  }
  
  _likelihood = estimation->getModel()->getLogLikelihood(false);
}




//------------------------------
//  Initialization constructor 2
//------------------------------
XEMModelOutput::XEMModelOutput(XEMModelType & modelType, int64_t nbCluster, vector<XEMCriterionOutput> & criterionOutput, double likelihood, XEMParameterDescription & parameterDescription, XEMLabelDescription & labelDescription,  XEMProbaDescription & probaDescription){
  _estimation = NULL;
  //TODO
  
  _modelType = modelType;
  _nbCluster = nbCluster;
  _criterionOutput = criterionOutput;
  _strategyRunError = noError; // TODO ??
  if (_strategyRunError == noError){
    _probaDescription = &probaDescription;
    _labelDescription = &labelDescription;
    _parameterDescription = &parameterDescription;
  }
  else{
    _probaDescription = NULL;
    _labelDescription = NULL;
    _parameterDescription = NULL;
  }
  
  _likelihood = likelihood;
}

//------------------------------
//  Initialization constructor 3
//------------------------------
XEMModelOutput::XEMModelOutput(XEMModelType & modelType, int64_t nbCluster, XEMErrorType error){
  _estimation = NULL;
  
  _modelType = modelType;
  _nbCluster = nbCluster;
  _strategyRunError = error; 
  _probaDescription = NULL;
  _labelDescription = NULL;
  _parameterDescription = NULL;
  _criterionOutput.resize(0);
  _likelihood = 0;
}

//-----------
// Destructor
//-----------
XEMModelOutput::~XEMModelOutput(){

}

  



