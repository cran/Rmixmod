/***************************************************************************
                             SRC/MIXMOD/XEMInput.cpp  description
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
#include "XEMInput.h"
#include "XEMGaussianData.h"
#include "XEMBinaryData.h"
#include "XEMError.h"
#include "XEMLabelDescription.h"

//--------------------
// Default Constructor
//--------------------
XEMInput::XEMInput(){
  _nbSample         = 0;
  _pbDimension      = 0;
  _nbCluster.clear();
  _knownPartition   = NULL;
  _labelDescription = NULL;
  _finalized        = false;
  //_dataDescription will be created by default XEMDataDescription constructor
}


//-----------------
// Copy Constructor
//-----------------
XEMInput::XEMInput(const XEMInput & input){
  _finalized = input._finalized;

  _nbSample      = input._nbSample;
  _pbDimension   = input._pbDimension;
  _nbCluster   = input._nbCluster; 

  _dataDescription = (input._dataDescription); // op =

  
  _knownPartition   = NULL;
  if (input._knownPartition != NULL){
    _knownPartition = new XEMPartition(input._knownPartition);
  }
  
  if (!_labelDescription)
    delete _labelDescription;
  
  _labelDescription = NULL;
  if (input._labelDescription != NULL){
    _labelDescription = new XEMLabelDescription(*input._labelDescription);
  }
  
  _criterionName = input._criterionName;
  _modelType = input._modelType;
  
}



//---------------------------
// Initialisation Constructor
//---------------------------
XEMInput::XEMInput(const vector<int64_t> & iNbCluster, const XEMDataDescription & iDataDescription){
  cloneInitialisation(iNbCluster, iDataDescription);
}

//---------------------
// Clone Initialisation 
//---------------------
void XEMInput::cloneInitialisation(const vector<int64_t> & iNbCluster, const XEMDataDescription & iDataDescription){
  _finalized     = false;

  _nbSample      = iDataDescription.getNbSample();
  _pbDimension   = iDataDescription.getPbDimension();
  _nbCluster   = iNbCluster; // copy Constructor
  
  //_data
  _dataDescription = iDataDescription;
  
  _knownPartition   = NULL;
  _labelDescription   = NULL;
  _criterionName.push_back(defaultCriterionName);
  
  if ( _dataDescription.isBinaryData()==false ){
    _modelType.push_back(new XEMModelType());
  }
  else{
    _modelType.push_back(new XEMModelType(defaultBinaryModelName));
  }
}


//---------------------------------
// Constructor used in DCV context
//--------------------------------
XEMInput::XEMInput(XEMInput * originalInput, XEMCVBlock & learningBlock){
  throw internalMixmodError;
}



//-----------
// Destructor
//-----------
XEMInput::~XEMInput(){
  if (!_knownPartition){
    delete _knownPartition;
  }
  if (!_labelDescription){
    delete _labelDescription;
  }
  
}



//------------
//------------
// Modifiers
//------------
//------------
  
//------ Criterion  ----//

//getCriterionName[i]
//-------------------
XEMCriterionName XEMInput::getCriterionName(int64_t index) const{
  if (index>=0 && index<_criterionName.size()){
    return _criterionName[index];
  }
  else{
    throw wrongCriterionPositionInGet;
  }
}


//setCriterionName
//----------------
void XEMInput::setCriterionName(XEMCriterionName criterionName, int64_t index){
  if (index>=0 && index<_criterionName.size()){
    _criterionName[index] = criterionName;
  }
  else{
    throw wrongCriterionPositionInSet;
  }
  _finalized = false;
}


// insertCriterionName
//-----------------
void XEMInput::insertCriterionName(XEMCriterionName criterionName, int64_t index){
  if (index>=0 && index<=_criterionName.size()){
    _criterionName.insert (_criterionName.begin()+index , criterionName);
  }
  else{
    throw wrongCriterionPositionInInsert;
  }
  _finalized = false;
}


// removeCriterionName
//--------------------
void XEMInput::removeCriterionName(int64_t index){
  if (index>=0 && index<_criterionName.size()){
    _criterionName.erase(_criterionName.begin()+index);
  }
  else{
    throw wrongCriterionPositionInRemove;
  }
  _finalized = false;
}
	





//------ modelType  ----//

//getModelType[i]
//-------------------
XEMModelType * XEMInput::getModelType(int64_t index) const{
  if (index>=0 && index<_modelType.size()){
    return _modelType[index];
  }
  else{
    throw wrongModelPositionInGet;
  }
}


//setModelType
//----------------
void XEMInput::setModelType(const XEMModelType * modelType, int64_t index){
  if (index>=0 && index<_modelType.size()){
    _modelType[index] = new XEMModelType(*modelType);
  }
  else{
    throw wrongModelPositionInSet;
  }
  _finalized = false;
}


// insertModelType
//-----------------
void XEMInput::insertModelType(const XEMModelType * modelType, int64_t index){
  if (index>=0 && index<=_modelType.size()){
    _modelType.insert (_modelType.begin()+index , new XEMModelType(*modelType));
  }
  else{
    throw wrongModelPositionInInsert;
  }
  _finalized = false;
}


// removeModelType
//--------------------
void XEMInput::removeModelType(int64_t index){
  if (index>=0 && index<_modelType.size()){
    _modelType.erase(_modelType.begin()+index);
  }
  else{
    throw wrongModelPositionInRemove;
  }
  _finalized = false;
}
	





// ---- Weight ----//

//setWeight()
//-----------
//TODO a enlever
void XEMInput::setWeight(string weightFileName){
  //_data->setWeight(weightFileName);
  _finalized = false;
}
void XEMInput::setWeight(double* weight){
  // get pointer to XEMData
  XEMData * data = getData();
  data->setWeight(weight);
  _finalized = false;
}

//TODO a enlever
void XEMInput::insertWeight(string weightFileName){
  //_data->setWeight(weightFileName);
  _finalized = false;
}

//TODO a enlever
void XEMInput::removeWeight(){
  //_data->setWeightDefault();
  _finalized = false;
}



 // ----- KnownPartition ----//

// setKnownPartition
//------------------
void XEMInput::setKnownPartition(string iFileName){
  if (_nbCluster.size() != 1){
    throw badSetKnownPartition;
  }
  else{
    if (_knownPartition){
      delete _knownPartition;
    }
    XEMNumericPartitionFile partitionFile;
    partitionFile._fileName = iFileName;
    partitionFile._format = FormatNumeric::defaultFormatNumericFile;
    partitionFile._type = TypePartition::defaultTypePartition;
    _knownPartition = new XEMPartition(_nbSample, _nbCluster[0], partitionFile);
  }
  _finalized = false;
}
  

  
// insertKnownPartition
//---------------------
void XEMInput::insertKnownPartition(string iFileName){ 
  if (_nbCluster.size() != 1){
    throw badSetKnownPartition;
  }
  else{
    if (_knownPartition){
      delete _knownPartition;
    }
    XEMNumericPartitionFile partitionFile;
    partitionFile._fileName = iFileName;
    partitionFile._format = FormatNumeric::defaultFormatNumericFile;
    partitionFile._type = TypePartition::defaultTypePartition;
    _knownPartition = new XEMPartition(_nbSample, _nbCluster[0], partitionFile);
  }
  _finalized = false;
}
  
// removeKnownPartition
//---------------------
void XEMInput::removeKnownPartition(){
  if (_knownPartition){
    delete _knownPartition;
  }
  _finalized = false;
}

void XEMInput::setLabel(XEMLabelDescription & labeldescription)
{
    removeLabel();
    
    _labelDescription = new XEMLabelDescription(labeldescription);    
}

void XEMInput::removeLabel()
{
  if (!_labelDescription)
    delete _labelDescription;
  
  _labelDescription = NULL;
}

// --------
// finalize
// --------
void XEMInput::finalize(){
  if (!_finalized){
    _finalized = verif();
  }
}




// ----------------
// Verif
//-----------------
bool XEMInput::verif(){
  bool res = true;
  
  // verif 1 : required inputs
  if (   _nbSample     == 0 
         || _pbDimension  == 0
         || _nbCluster.empty()    ) {
    res = false;
    throw missingRequiredInputs;
  }
  
 
  // verif 2 : _data->verify
  res = _dataDescription.verifyData();

  return res;
}


