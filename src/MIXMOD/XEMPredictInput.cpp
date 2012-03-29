/***************************************************************************
 SRC/MIXMOD/XEMPredictInput.cpp  description
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
#include "XEMPredictInput.h"
#include "XEMPredictStrategy.h"
#include "XEMParameter.h"
#include "XEMParameterDescription.h"
#include "XEMModelType.h"

//--------------------
// Default Constructor
//--------------------
XEMPredictInput::XEMPredictInput() : XEMInput()
{}


//-----------------
//  Copy constructor
//-----------------
XEMPredictInput::XEMPredictInput( const XEMPredictInput & cInput ) 
                                : XEMInput(cInput)
                                , _classificationRule(cInput.getClassificationRule())
{}



//---------------------------
// Initialisation Constructor
//---------------------------
XEMPredictInput::XEMPredictInput( XEMDataDescription * predictData
                                , XEMParameterDescription * classificationRule
                                ) 
                                : XEMInput(std::vector<int64_t>(1,classificationRule->getNbCluster()), *predictData)
                                , _classificationRule(classificationRule->getParameter())
{
  // replace default model type by the input model type
  delete _modelType[0];
  _modelType[0] = new XEMModelType(*classificationRule->getModelType());
}




//-----------
// Destructor
//-----------
XEMPredictInput::~XEMPredictInput()
{}


//------------
//------------
// Modifiers
//------------
//------------

//------ Criterion  ----//

//getCriterion[i]
//-------------------
XEMCriterionName XEMPredictInput::getCriterionName(unsigned int index) const{
  throw notAvailableForPrediction;
}

// removeCriterionName
//--------------------
void XEMPredictInput::removeCriterion(unsigned int index){
  throw notAvailableForPrediction;
}


//setCriterionName
//----------------
void XEMPredictInput::setCriterion(std::vector<XEMCriterionName> const & criterionName){
  throw notAvailableForPrediction;
}


//setCriterionName
//----------------
void XEMPredictInput::setCriterion(const XEMCriterionName criterionName, unsigned int index){
  throw notAvailableForPrediction;
}


// insertCriterionName
//-----------------
void XEMPredictInput::insertCriterion(const XEMCriterionName criterionName, unsigned int index){
  throw notAvailableForPrediction;
}


// add Criterion
//-----------------
void XEMPredictInput::addCriterion(const XEMCriterionName criterionName){
  throw notAvailableForPrediction;
}

//------ modelType  ----//

//setModelType
//----------------
void XEMPredictInput::setModelType(const XEMModelType * modelType, unsigned int index){
  throw notAvailableForPrediction;
}


// insertModelType
//-----------------
void XEMPredictInput::insertModelType(const XEMModelType * modelType, unsigned int index){
  throw notAvailableForPrediction;
}

// add new model type
void XEMPredictInput::addModel( XEMModelName const modelName ){
  throw notAvailableForPrediction;
}

// removeModelType
//--------------------
void XEMPredictInput::removeModelType(unsigned int index){
  throw notAvailableForPrediction;
}



// ----------------
// Verif
//-----------------
bool XEMPredictInput::verif(){
  bool res = XEMInput::verif();

  return res;
}

