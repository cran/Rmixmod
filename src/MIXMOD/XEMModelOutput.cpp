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
#include "XEMModel.h"
#include "XEMModelType.h"
#include "XEMProbaDescription.h"
#include "XEMLabelDescription.h"


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
XEMModelOutput::XEMModelOutput(XEMModel * estimation)
{
  if (!estimation){
    throw nullPointerError;
  }
  _modelType = *(estimation->getModelType());
  _nbCluster = estimation->getNbCluster();
  
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
  
  _likelihood = estimation->getLogLikelihood(false);
}




//------------------------------
//  Initialization constructor 2
//------------------------------
XEMModelOutput::XEMModelOutput( XEMModelType & modelType
                              , int64_t nbCluster
                              , std::vector<XEMCriterionOutput> & criterionOutput
                              , double likelihood
                              , XEMParameterDescription & parameterDescription
                              , XEMLabelDescription & labelDescription
                              ,  XEMProbaDescription & probaDescription
                              )
{
  _modelType = modelType;
  _nbCluster = nbCluster;
  _strategyRunError = noError; // TODO ??
  if (_strategyRunError == noError){
    _probaDescription = new XEMProbaDescription(probaDescription);
    _labelDescription = new XEMLabelDescription(labelDescription);
    _parameterDescription = new XEMParameterDescription(parameterDescription);
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
XEMModelOutput::XEMModelOutput(XEMModelType & modelType, int64_t nbCluster, XEMErrorType error)
{
  _modelType = modelType;
  _nbCluster = nbCluster;
  _strategyRunError = error; 
  _probaDescription = NULL;
  _labelDescription = NULL;
  _parameterDescription = NULL;
  _likelihood = 0;
}

//-----------
// Destructor
//-----------
XEMModelOutput::~XEMModelOutput()
{
  if (_labelDescription) delete _labelDescription;
  if (_parameterDescription) delete _parameterDescription;
  if (_probaDescription) delete _probaDescription;
}



//----------------------
/// Comparison operator
//----------------------
bool XEMModelOutput::operator ==(const XEMModelOutput & modelOutput) const{
  
  if ( _nbCluster != modelOutput.getNbCluster() ) return false;
  if ( !(_modelType == modelOutput.getModelType()) ) return false;
  for (int iCriterion=0; iCriterion<maxNbCriterion; iCriterion++ ){
    if ( !(_criterionOutput[iCriterion] == modelOutput.getCriterionOutput(iCriterion)) ) return false;
  }
  if ( !(_parameterDescription == modelOutput.getParameterDescription()) ) return false;
  if ( !(_labelDescription == modelOutput.getLabelDescription()) ) return false;
  if ( !(_probaDescription == modelOutput.getProbaDescription()) ) return false;
  return true;
}

// set criterion output
void XEMModelOutput::setCriterionOutput( XEMCriterionOutput const & criterionOutput )
{
  // get criterion name
  XEMCriterionName criterionName = criterionOutput.getCriterionName();
  _criterionOutput[criterionName].setCriterionName(criterionName);
  _criterionOutput[criterionName].setValue(criterionOutput.getValue());
  _criterionOutput[criterionName].setError(criterionOutput.getError());
}


