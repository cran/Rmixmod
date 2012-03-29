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
#include "XEMData.h"
#include "XEMPartition.h"
#include "XEMLabelDescription.h"
#include "XEMModelType.h"

//--------------------
// Default Constructor
//--------------------
XEMInput::XEMInput(){
  _nbSample         = 0;
  _pbDimension      = 0;
  _nbCluster.clear();
  //_knownPartition   = NULL;
  _knownLabelDescription = NULL;
  _finalized        = false;
  //_dataDescription will be created by default XEMDataDescription constructor
}


//-----------------
// Copy Constructor
//-----------------
XEMInput::XEMInput( const XEMInput & input )
{
  _finalized = input._finalized;

  _nbSample      = input._nbSample;
  _pbDimension   = input._pbDimension;
  _nbCluster   = input._nbCluster; 

  _dataDescription = (input._dataDescription); // op =

  
  _knownPartition   = NULL;
  if (input._knownPartition != NULL){
    _knownPartition = new XEMPartition(input._knownPartition);
  }
  
  if (!_knownLabelDescription)
    delete _knownLabelDescription;
  
  _knownLabelDescription = NULL;
  if (input._knownLabelDescription != NULL){
    _knownLabelDescription = new XEMLabelDescription(*input._knownLabelDescription);
  }
  
  _criterionName = input._criterionName;
  _modelType = input._modelType;
  
}



//---------------------------
// Initialisation Constructor
//---------------------------
XEMInput::XEMInput(const std::vector<int64_t> & iNbCluster, const XEMDataDescription & iDataDescription){
  cloneInitialisation(iNbCluster, iDataDescription);
}

//---------------------
// Clone Initialisation 
//---------------------
void XEMInput::cloneInitialisation(const std::vector<int64_t> & iNbCluster, const XEMDataDescription & iDataDescription){
  _finalized     = false;

  _nbSample      = iDataDescription.getNbSample();
  _pbDimension   = iDataDescription.getPbDimension();
  _nbCluster   = iNbCluster; // copy Constructor
  
  //_data
  _dataDescription = iDataDescription;
  
  _knownPartition   = NULL;
  _knownLabelDescription   = NULL;
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
  if (!_knownLabelDescription){
    delete _knownLabelDescription;
  }
  for (unsigned int i=0; i<_modelType.size(); i++){
    delete _modelType[i];
  }
}



//------------
//------------
// Modifiers
//------------
//------------
  
//------ Criterion  ----//

//getCriterion[i]
//-------------------
XEMCriterionName XEMInput::getCriterionName(unsigned int index) const{
  if (index<_criterionName.size()){
    return _criterionName[index];
  }
  else{
    throw wrongCriterionPositionInGet;
  }
}

// removeCriterionName
//--------------------
void XEMInput::removeCriterion(unsigned int index){
  if (index<_criterionName.size()){
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
XEMModelType * XEMInput::getModelType(unsigned int index) const{
  if (index<_modelType.size()){
    return _modelType[index];
  }
  else{
    throw wrongModelPositionInGet;
  }
}

/// setModel
void XEMInput::setModel(std::vector<XEMModelName> const & modelName){
  // resize vector
  _modelType.resize(modelName.size());
  for (unsigned int iModel=0; iModel<_modelType.size(); iModel++){
    // copy vector contents
    _modelType[iModel] = new XEMModelType(modelName[iModel]);
  }
}

//setModelType
//----------------
void XEMInput::setModelType(const XEMModelType * modelType, unsigned int index){
  if (index<_modelType.size()){
    if ( _modelType[index] ) delete _modelType[index];
    _modelType[index] = new XEMModelType(*modelType);
  }
  else{
    throw wrongModelPositionInSet;
  }
  _finalized = false;
}


// insertModelType
//-----------------
void XEMInput::insertModelType(const XEMModelType * modelType, unsigned int index){
  if (index<=_modelType.size()){
    if ( _modelType[index] ) delete _modelType[index];
    _modelType.insert (_modelType.begin()+index , new XEMModelType(*modelType));
  }
  else{
    throw wrongModelPositionInInsert;
  }
  _finalized = false;
}

// add new model type
void XEMInput::addModel( XEMModelName const modelName ){
  
  if ( (isBinaryData() && isBinary(modelName)) ||  (!isBinaryData() && !isBinary(modelName)) ){
    bool found = false;
    for ( unsigned int iModel=0; iModel<_modelType.size(); iModel++ ){
      if ( _modelType[iModel]->getModelName() == modelName ) found = true;
    }
    if (!found) _modelType.push_back(new XEMModelType(modelName));
  }
}

// removeModelType
//--------------------
void XEMInput::removeModelType(unsigned int index){
  if (index<_modelType.size()){
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
void XEMInput::setWeight(std::string weightFileName){
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
void XEMInput::insertWeight(std::string weightFileName){
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
void XEMInput::setKnownPartition(std::string iFileName){
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
void XEMInput::insertKnownPartition(std::string iFileName){ 
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

void XEMInput::setKnownLabelDescription(XEMLabelDescription & labeldescription)
{
    removeKnownLabelDescription();
    
    _knownLabelDescription = new XEMLabelDescription(labeldescription);    
}

void XEMInput::removeKnownLabelDescription()
{
  if (!_knownLabelDescription)
    delete _knownLabelDescription;
  
  _knownLabelDescription = NULL;
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


// ----------------
// print out input
// ----------------
void XEMInput::edit(std::ostream & out ) const
{
  out << "Models : ";
  for ( unsigned int iModel=0; iModel<_modelType.size(); iModel++ )
    out << XEMModelNameToString(_modelType[iModel]->getModelName()) << " ";
  out << std::endl;
  
  out << "Criterions : ";
  for ( unsigned int iCriterion=0; iCriterion<_criterionName.size(); iCriterion++ )
    out << XEMCriterionNameToString(_criterionName[iCriterion]) << " ";
  out << std::endl;
}

