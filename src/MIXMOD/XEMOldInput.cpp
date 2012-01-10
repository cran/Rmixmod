/***************************************************************************
                             SRC/MIXMOD/XEMOldInput.cpp  description
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
#include "XEMOldInput.h"
#include "XEMStrategy.h"
#include "XEMGaussianData.h"
#include "XEMBinaryData.h"
#include "XEMError.h"

//------------
// Constructor
//------------
XEMOldInput::XEMOldInput(){
  _finalized = false;

  _nbSample               = 0;
  _pbDimension            = 0;

  _data                   = NULL;
  _deleteData             = false;

    
  _tabKnownPartition      = NULL;
  _nbKnownPartition       = 0;
  _nbNbCluster            = 0;
  _tabNbCluster           = NULL;
  _deleteTabNbCluster     = false;

  _nbCriterionName        = 1;
  _tabCriterionName       = new XEMCriterionName[_nbCriterionName];
  _tabCriterionName[0]    = defaultCriterionName;
  _deleteTabCriterionName = true;

  _nbModelType            = 1;

  _tabModelType           = new XEMModelType*[_nbModelType];
  _tabModelType[0]        = new XEMModelType();
  _deleteTabModelType     = true;

  _nbStrategy             = 1;
  _tabStrategy            = new XEMStrategy*[_nbStrategy];
  _tabStrategy[0]         = new XEMStrategy();
  
  _binaryDataType         = false;
  
  _DCVinitBlocks          = defaultDCVinitBlocks;
  _numberOfDCVBlocks      = defaultDCVnumberOfBlocks;
	
  _numberOfCVBlocks       = defaultCVnumberOfBlocks;
  _CVinitBlocks           = defaultCVinitBlocks;
}




//------------
// Constructor
//------------
XEMOldInput::XEMOldInput(int64_t iNbSample, int64_t iPbDimension, int64_t iNbNbCluster, int64_t * iTabNbCluster, XEMData * iData, bool binaryDataType){

  _finalized = false;

  _nbSample               = iNbSample;
  _pbDimension            = iPbDimension;
	
  _nbNbCluster            = iNbNbCluster;
  //_tabNbCluster           = iTabNbCluster;

  _tabNbCluster = new int64_t [ _nbNbCluster ];
  recopyTab( iTabNbCluster, _tabNbCluster, _nbNbCluster );

  _deleteTabNbCluster     = true;

  _tabKnownPartition      = NULL;
  _nbKnownPartition       = _nbNbCluster;
  _data                   = iData->clone();

  //_deleteData             = false;
  _deleteData             = true;

  _nbCriterionName        = defaultNbCriterion;
  _tabCriterionName       = new XEMCriterionName[_nbCriterionName];
  _tabCriterionName[0]    = defaultCriterionName;
  _deleteTabCriterionName = true;

  _nbModelType            = defaultNbModel;

  if (binaryDataType==false){
    _tabModelType           = new XEMModelType * [_nbModelType];
    _tabModelType[0]        =  new XEMModelType();
    _deleteTabModelType     = true;
    _binaryDataType         = false;
  }
  else{
    _tabModelType           = new XEMModelType * [_nbModelType];
    _tabModelType[0]        =  new XEMModelType(defaultBinaryModelName);
    _deleteTabModelType     = true;
    _binaryDataType         = true;
  }
  _nbStrategy             = defaultNbStrategy;
  _tabStrategy            = new XEMStrategy*[_nbStrategy];
  _tabStrategy[0]         = new XEMStrategy();  
  
  _DCVinitBlocks          = defaultDCVinitBlocks;
  _numberOfDCVBlocks      = defaultDCVnumberOfBlocks;
	
  _numberOfCVBlocks       = defaultCVnumberOfBlocks;
  _CVinitBlocks           = defaultCVinitBlocks;

}



//---------------------------------
// Constructor used in DCV context
//--------------------------------
XEMOldInput::XEMOldInput(XEMOldInput * originalInput, XEMCVBlock & learningBlock){
  _nbSample      = learningBlock._nbSample ;  
  _pbDimension   = originalInput->_pbDimension;

  //---------------------- Nb cluster  
  _nbNbCluster   = originalInput->_nbNbCluster ;  
  _tabNbCluster  = originalInput->_tabNbCluster;
  _deleteTabNbCluster = false;
  
  //---------------------- criterion type
  _nbCriterionName = 1;
  _tabCriterionName = new XEMCriterionName[1];
  _tabCriterionName[0]  = CV;
  _deleteTabCriterionName = true;
  
  
  _nbStrategy  = originalInput->_nbStrategy ;
  
  int64_t k ;
      
  //------------------------- Known labels
  if(originalInput->_tabKnownPartition){
    _tabKnownPartition = new XEMPartition*[_nbNbCluster];
    for(k=0; k<_nbNbCluster; k++){
      _tabKnownPartition[k] = new XEMPartition(originalInput->_tabKnownPartition[k], learningBlock) ;
    }
  }
  else{
    _tabKnownPartition       = NULL;
  }
  
  //------------------------- Model types
  _nbModelType  = originalInput->_nbModelType ;
  _tabModelType = originalInput->_tabModelType ;
  _deleteTabModelType = false;
  
  //------------------------- Strategy types
  _tabStrategy = new XEMStrategy*[_nbStrategy] ;
  for(k=0; k<_nbStrategy; k++){
    _tabStrategy[k] = new XEMStrategy(originalInput->_tabStrategy[k], learningBlock) ;
  }
    	
  _numberOfCVBlocks  = originalInput->_numberOfCVBlocks;
  _CVinitBlocks = originalInput->_CVinitBlocks;
  
  // will be not used but initalized
  _numberOfDCVBlocks  = originalInput->_numberOfDCVBlocks;
  _DCVinitBlocks = originalInput->_DCVinitBlocks;
  
  
  //---------------------- data
  //----------------------------
  if (originalInput->_binaryDataType){
    // _data is not used by XEMMain... -> not updated
    _deleteData = false;
    _data = originalInput->_data;
    _binaryDataType = true;

  }
  else{
    // Gaussian case
    _deleteData = true;
    _data = new XEMGaussianData(_nbSample, _pbDimension, originalInput->_data , learningBlock);
    _binaryDataType = false;
  }
	
  _finalized = true;
}



//-----------
// Destructor
//-----------
XEMOldInput::~XEMOldInput(){
  int64_t i,k;

  if (_tabCriterionName && _deleteTabCriterionName){
    delete[] _tabCriterionName;
    _tabCriterionName = NULL;
  }
  if (_tabNbCluster && _deleteTabNbCluster){
    delete[] _tabNbCluster;
    _tabNbCluster = NULL; 
  }

  if (_tabModelType  && _deleteTabModelType){
    for (i=0; i<_nbModelType; i++){
      delete _tabModelType[i];
      _tabModelType[i] = NULL;
    }
    delete[] _tabModelType;
    _tabModelType = NULL;	
  }
  
  if (_data && _deleteData){
    delete _data;
    _data = NULL;
  }


  if (_tabKnownPartition){
    for(k=0; k<_nbNbCluster;k++){
      delete _tabKnownPartition[k];
      _tabKnownPartition[k] = NULL;
    }
    delete [] _tabKnownPartition;
    _tabKnownPartition = NULL;
  }
  
  if (_tabStrategy){
    for (i=0; i<_nbStrategy; i++){
      delete _tabStrategy[i];
    }
    delete [] _tabStrategy;
  }
}



//-----------------
// setCriterionName
//-----------------
void XEMOldInput::setCriterionName(int64_t iNbCriterionName, XEMCriterionName * iTabCriterionName){
  if (_tabCriterionName && _deleteTabCriterionName){
    delete[] _tabCriterionName;
    _tabCriterionName= NULL;	
  }
  _nbCriterionName = iNbCriterionName;
  _tabCriterionName = new XEMCriterionName[_nbCriterionName];
  recopyTab<XEMCriterionName>(iTabCriterionName, _tabCriterionName, iNbCriterionName);
  _deleteTabCriterionName = true;
  _finalized = false;
}



void  XEMOldInput::setCriterionName(XEMCriterionName criterionName, int64_t position){
  if ((position>(_nbCriterionName-1))
       ||(position<0) ){
    throw wrongCriterionPositionInSet;
       }
       else{
         _tabCriterionName[position] = criterionName;
         _deleteTabCriterionName = true;
         _finalized = false;
       }
}



// insertCriterionName
void XEMOldInput::insertCriterionName(XEMCriterionName criterionName, int64_t position){
  if ((position>_nbCriterionName)
       || (position<0)){
    throw wrongCriterionPositionInInsert;
       }
       else{
         XEMCriterionName * tabCriterionName = new XEMCriterionName[_nbCriterionName+1];
         recopyTab<XEMCriterionName>(_tabCriterionName, tabCriterionName, position);
         tabCriterionName[position] = criterionName;
         for (int64_t i=position; i<_nbCriterionName;i++){
           tabCriterionName[i+1] = _tabCriterionName[i];
         }
         delete [] _tabCriterionName;
         _tabCriterionName = NULL;
         _nbCriterionName++;
         _tabCriterionName = tabCriterionName;

         _deleteTabCriterionName = true;
         _finalized = false;
       }
}



// removeCriterionName
void XEMOldInput::removeCriterionName(int64_t position){
  if ((position>(_nbCriterionName-1)) 
       || ((position==0) && (_nbCriterionName==1)) 
       || (position<0)) {
    throw wrongCriterionPositionInRemove;
       }
       else{
         XEMCriterionName * tabCriterionName = new XEMCriterionName[_nbCriterionName+1];
         recopyTab<XEMCriterionName>(_tabCriterionName, tabCriterionName, position);
         for (int64_t i=position; i<(_nbCriterionName-1);++i){
           tabCriterionName[i] = _tabCriterionName[i+1];
         }
         _nbCriterionName--;
         delete [] _tabCriterionName;
         _tabCriterionName = NULL;
         _tabCriterionName = tabCriterionName;

         _deleteTabCriterionName = true;
         _finalized = false;

       }
}




//-------------
// setModelType
//-------------
void XEMOldInput::setModelType(int64_t iNbModelType, XEMModelType ** iTabModelType){
  if (_tabModelType && _deleteTabModelType){
    delete[] _tabModelType;
    _tabModelType= NULL;	
  }
  _nbModelType = iNbModelType;
  _tabModelType = new XEMModelType*[_nbModelType];
  for (int64_t i=0; i<_nbModelType; ++i){
    _tabModelType[i] = new XEMModelType(*(iTabModelType[i]));
  }
  _deleteTabModelType = true;
  _finalized = false;
}


void XEMOldInput::setModelType(XEMModelName nameModel, int64_t position){
  if(position>(_nbModelType-1)){
    throw wrongModelPositionInSet;
  }
  else{
    XEMModelType * newModel;
    if (isHD(nameModel)){
      newModel = new XEMModelType(nameModel,_tabNbCluster[0]);
    }
    else{
      newModel = new XEMModelType(nameModel);
    }
    delete _tabModelType[position];
    _tabModelType[position] = NULL;
    _tabModelType[position] = new XEMModelType(*newModel);

    _deleteTabModelType = true;
    _finalized = false;

    delete newModel;
  }
}

// insertModelType


void XEMOldInput::insertModelType(XEMModelName nameModel, int64_t position){
  if(position>(_nbModelType)){
    throw wrongModelPositionInInsert;
  }
  else{
    XEMModelType * newModel;
    if (isHD(nameModel)){
      newModel = new XEMModelType(nameModel,_tabNbCluster[0]);
    }
    else{
      newModel = new XEMModelType(nameModel);
    }
    XEMModelType** tabModel = new XEMModelType*[_nbModelType+1];
    for (int64_t i=0;i<position;++i){
      tabModel[i] = new XEMModelType(*_tabModelType[i]);
    }
    tabModel[position] = new XEMModelType(*newModel);
    for (int64_t i=position ; i<_nbModelType;++i){
      tabModel[i+1] = new XEMModelType(*(_tabModelType[i]));
    }

    for (int64_t i=0; i<_nbModelType;++i){
      delete _tabModelType[i];
      _tabModelType[i] = NULL;
    }
    delete[] _tabModelType;
    _tabModelType = NULL;

    _nbModelType++;
    _tabModelType = tabModel;

    _deleteTabModelType = true;
    _finalized = false;

    delete newModel;

  }
}


// removeModelType
void XEMOldInput::removeModelType(int64_t position){

  if(position>(_nbModelType-1) || ((position==0) && (_nbModelType==1) )){
    throw wrongModelPositionInRemove;
  }
  else{
    XEMModelType** tabModel = new XEMModelType*[_nbModelType-1];

    for (int64_t i=0 ; i<position;++i){
      tabModel[i] = new XEMModelType(*(_tabModelType[i]));
    }
    for (int64_t i=position ; i<(_nbModelType-1);++i){
      tabModel[i] = new XEMModelType(*(_tabModelType[i+1] ));
    }

    for (int64_t i=0; i<_nbModelType;++i){
      delete _tabModelType[i];
      _tabModelType[i] = NULL;
    }
    delete[] _tabModelType;
    _tabModelType = NULL;
    _nbModelType--;
    _tabModelType = tabModel;

    _deleteTabModelType = true;
    _finalized = false;

  }
}



//-------------
// setStrategy
//-------------
void XEMOldInput::setStrategy(int64_t iNbStrategy, XEMStrategy ** iTabStrategy){
  int64_t i;

  if (_tabStrategy){
    for (i=0; i<_nbStrategy; i++){
      delete _tabStrategy[i];
    }
    delete[] _tabStrategy;
    _tabStrategy = NULL;
  }
  _nbStrategy   = iNbStrategy;
  _tabStrategy  = new XEMStrategy*[_nbStrategy];
  recopyTab<XEMStrategy*>(iTabStrategy,_tabStrategy,_nbStrategy);
  _finalized = false;
}



//--------------
// setKnownPartition : copyTab in XEMPartition
//--------------
void XEMOldInput::setKnownPartition(int64_t iNbKnownPartition, XEMPartition ** iTabKnownPartition){
  int64_t k; 
  if (_tabKnownPartition){
    for (k=0; k<_nbNbCluster; k++){
      delete _tabKnownPartition[k];
    }
    delete[] _tabKnownPartition;
    _tabKnownPartition = NULL;
  }
  
  if (iNbKnownPartition != _nbNbCluster){
    throw wrongNbKnownPartition;
  }
  else{
    _tabKnownPartition = new XEMPartition *[_nbNbCluster];
    for (k=0; k<_nbNbCluster; k++){
      _tabKnownPartition[k] = new XEMPartition(iTabKnownPartition[k]);
    }
  }
  _finalized = false;
}


void XEMOldInput::setKnownPartition(string & fileName, int64_t position){
  if(position>(_nbNbCluster-1)){
    throw wrongKnownPartitionPositionInSet;
  }
  else{
     // _nbKnownPartition = _nbNbCluster;
    if (_tabKnownPartition){
      if (_tabKnownPartition[position]){
        delete _tabKnownPartition[position];
        _tabKnownPartition[position] = NULL;
      }
    }
    else{
      _tabKnownPartition = new XEMPartition*[_nbNbCluster]; 
    }
    try{
        XEMNumericPartitionFile partitionFile;
        partitionFile._fileName = fileName;
        partitionFile._format = FormatNumeric::defaultFormatNumericFile;
        partitionFile._type = TypePartition::defaultTypePartition;
      _tabKnownPartition[position] = new XEMPartition(_nbSample, _tabNbCluster[position], partitionFile);
    }
    catch (XEMError error){
      delete _tabKnownPartition[position];
      _tabKnownPartition[position] = NULL;
      throw badKnownPartition;
    }
    _finalized = false;
  }
}


// removeKnownPartition

void XEMOldInput::removeKnownPartition(int64_t position, bool decrease){
  if (decrease){
    _nbKnownPartition--;
  }
  if(position>(_nbNbCluster-1)){
    throw wrongKnownPartitionPositionInRemove;
  }
  else{
    if (_tabKnownPartition){
      if (_tabKnownPartition[position]){
        delete _tabKnownPartition[position];
        _tabKnownPartition[position] = NULL;
      }
    }
    if (_nbKnownPartition==0){
      if (_tabKnownPartition){
        delete [] _tabKnownPartition;
        _tabKnownPartition = NULL;
      }
    }
    _finalized = false;
  }
}

// insertKnownPartition
// void XEMOldInput::insertKnownPartition(string & fileName, int64_t position){
//   if( position > _nbNbCluster || position < 0 ){
//     throw wrongKnownPartitionPositionInInsert ;
//   }else{
//     if( _tabKnownPartition ){
//       bool error = false;
//       _tabKnownPartition[position] = new XEMPartition(_nbSample, _tabNbCluster[position],fileName, error);
//       if (error){
//         delete _tabKnownPartition[position];
//         _tabKnownPartition[position] = NULL;
//         throw badKnownPartition;
//       }
//     }else{
//       _tabKnownPartition = new XEMPartition*[_nbNbCluster];
//       _nbKnownPartition = _nbNbCluster;
//       for( int64_t i = 0 ; i < _nbNbCluster ; i++ ){
//         _tabKnownPartition[i] = new XEMPartition();
//       }
//       delete _tabKnownPartition[position] ;
//       bool error = false;
//       _tabKnownPartition[position] = new XEMPartition(_nbSample, _tabNbCluster[position],fileName, error);
//       if (error){
//         delete _tabKnownPartition[position];
//         _tabKnownPartition[position] = NULL;
//         throw badKnownPartition;
//       }
//     }
//     _finalized = false;
//   }
// }


//setNbTryInStrategy
void XEMOldInput::setNbTryInStrategy(int64_t strategyPosition, int64_t nbTry){
  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    _tabStrategy[strategyPosition]->setNbTry(nbTry);
  }
}

  
//---------------
// getNbTryInStrategy
int64_t XEMOldInput::getNbTryInStrategy(int64_t strategyPosition){
  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    return _tabStrategy[strategyPosition]->getNbTry(); 
  }
}
  
  
//--------------
//setNbTryInInit
void XEMOldInput::setNbTryInInit(int64_t strategyPosition, int64_t nbTry){
  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    _tabStrategy[strategyPosition]->setNbTryInInit(nbTry);
  }
}


//---------------
// getNbTryInInit
int64_t XEMOldInput::getNbTryInInit(int64_t strategyPosition){
  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    return _tabStrategy[strategyPosition]->getNbTryInInit(); 
  }
}


//------------------
// getStopNameInInit
//------------------
const XEMAlgoStopName XEMOldInput::getStopNameInInit(int64_t strategyPosition) const{
  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    return _tabStrategy[strategyPosition]->getStopNameInInit(); 
  }
}
  
  
//------------------
// setStopNameInInit
//----------------
void XEMOldInput::setStopNameInInit(int64_t strategyPosition, XEMAlgoStopName stopName){
  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    _tabStrategy[strategyPosition]->setStopNameInInit(stopName); 
  }
}


//--------------------
//setNbIterationInInit
void XEMOldInput::setNbIterationInInit(int64_t strategyPosition, int64_t nbIteration){
  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    _tabStrategy[strategyPosition]->setNbIterationInInit(nbIteration);
  }
}

//---------------------
// getNbIterationInInit
int64_t XEMOldInput::getNbIterationInInit(int64_t strategyPosition){
  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    return _tabStrategy[strategyPosition]->getNbIterationInInit(); 
  }
}


//--------------
//setEpsilonInInit
void XEMOldInput::setEpsilonInInit(int64_t strategyPosition, double epsilon){
  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    _tabStrategy[strategyPosition]->setEpsilonInInit(epsilon);
  }
}


//---------------
// getEpsilonInInit
double XEMOldInput::getEpsilonInInit(int64_t strategyPosition){
  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    return _tabStrategy[strategyPosition]->getEpsilonInInit(); 
  }
}

// setStrategyInitType
void  XEMOldInput::setStrategyInitName(XEMStrategyInitName  initName, int64_t position){
  if(position>(_nbStrategy-1)){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    _tabStrategy[position]->setStrategyInit(initName,_data,_nbNbCluster,_tabNbCluster,_tabModelType);
    _finalized = false;
  }
}
 
// set init parameter
void XEMOldInput::setInitParam(int64_t strategyPosition, string & paramFileName, int64_t position){
  if(strategyPosition>(_nbStrategy-1)){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    _tabStrategy[strategyPosition]->setInitParam(paramFileName, position);
    _finalized = false;
  }
}

// set init partition
void XEMOldInput::setInitPartition(int64_t strategyPosition, string & partitionFileName, int64_t position){
  if(strategyPosition>(_nbStrategy-1)){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{   
    _tabStrategy[strategyPosition]->setInitPartition(partitionFileName, position);
    _finalized = false;
  }
}


// set algo name
void XEMOldInput::setAlgoName(int64_t strategyPosition, XEMAlgoName algoName, int64_t position){
  if(strategyPosition>(_nbStrategy-1)){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    _tabStrategy[strategyPosition]->setAlgo(algoName, position);
    _finalized = false;
  }
}

// remove algo type
void XEMOldInput::removeAlgo(int64_t strategyPosition, int64_t position){

  if(strategyPosition>(_nbStrategy-1)){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    _tabStrategy[strategyPosition]->removeAlgo(position);
    _finalized = false;
  }
}

/*
// insert algo type
void XEMOldInput::insertAlgoName(int64_t strategyPosition, XEMAlgoName algoName, int64_t position){

    if(strategyPosition>_nbStrategy){
      throw wrongStrategyPositionInInsertAlgoType;
}
    else{
      _tabStrategy[strategyPosition]->insertAlgo(algoName,position);
      _finalized = false;
}
}*/

// insert default algo
void XEMOldInput::insertAlgo(int64_t strategyPosition, int64_t position){

  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    XEMAlgo * defaultAlgo = createDefaultAlgo();
    _tabStrategy[strategyPosition]->insertAlgo(defaultAlgo,position);
    _finalized = false;
  }
}

// set algoStopRuleValue
void XEMOldInput::setAlgoStopRule(int64_t strategyPosition, int64_t position, XEMAlgoStopName stopName){
  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    _tabStrategy[strategyPosition]->setAlgoStopRule(stopName,position);
  }
}

void XEMOldInput::setAlgoIteration(int64_t strategyPosition, int64_t position,int64_t nbIterationValue){
  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    _tabStrategy[strategyPosition]->setAlgoIteration(position,nbIterationValue);
  }
}

void XEMOldInput::setAlgoEpsilon(int64_t strategyPosition, int64_t position, double epsilonValue){
  if(strategyPosition>_nbStrategy){
    throw wrongStrategyPositionInSetOrGetMethod;
  }
  else{
    _tabStrategy[strategyPosition]->setAlgoEpsilon(position,epsilonValue);
  }
}


// set subDimensionEqual
void XEMOldInput::setSubDimensionEqual(int64_t position, int64_t subDimensionValue){
  if(position>_nbModelType){
    throw wrongModelPositionInSetSubDimensionEqual;
  }
  else{
    _tabModelType[position]->setSubDimensionEqual(subDimensionValue);
  }
}


// set subDimensionFree
void XEMOldInput::setTabSubDimensionFree(int64_t positionModel, int64_t subDimensionValue, int64_t position){
  if(positionModel>_nbModelType){
    throw wrongModelPositionInSetSubDimensionFree;
  }
  else{
    _tabModelType[positionModel]->setTabSubDimensionFree(subDimensionValue, position);
  }
}





//---------------
// Friend Methods
//---------------

ifstream & operator >> (ifstream & fi, XEMOldInput & input){
  int64_t i;
  int64_t * tabNbModality = NULL;
  bool nbFind = false;
 
  ////////////////////////
  //                    //
  // Compulsory Options //
  //                    //
  ////////////////////////
 
  // Number of sample(s) //
  //---------------------//
  moveUntilReach(fi,"nblines");
  if(!fi.eof()){
    fi >> input._nbSample;
    if (input._nbSample > maxNbSample){
      throw nbLinesTooLarge;
    }
    else if (input._nbSample <= 0){
      throw nbLinesTooSmall;
    }
  }
  else{
    throw errorNbLines;
  }

  // Problem dimension //
  //-------------------//
  
  
  moveUntilReach(fi,"pbdimension"); 
  if(!fi.eof()){
    fi >> input._pbDimension;
    if (input._pbDimension > maxPbDimension){
      throw pbDimensionTooLarge;
    }
    else if (input._pbDimension <= 0){
      throw pbDimensionTooSmall;
    }
  }
  else{
    throw errorPbDimension;
  }





  // Cluster(s) //
  //------------//

  /* Number of number of cluster(s) */
  moveUntilReach(fi,"nbnbcluster");
  if(!fi.eof()){
    fi>>input._nbNbCluster;
    if (input._nbNbCluster > maxNbNbCluster){
      throw nbNbClusterTooLarge;
    }
    else if (input._nbNbCluster <= 0){
      throw nbNbClusterTooSmall;
    }
    input._tabNbCluster = new int64_t[input._nbNbCluster];
    input._deleteTabNbCluster = true;
  }
  else{
    throw errorNbNbCluster;
  }
  
  /* List of number of cluster(s) */
  moveUntilReach(fi,"listnbcluster");
  if(!fi.eof()){
    for (i=0; i<input._nbNbCluster; i++){
      fi>>input._tabNbCluster[i];
    }
  }
  else{
    throw errorListNbCluster;
  }

  // tab modality //
  //--------------//
  moveUntilReach(fi,"nbmodality");
  if(!fi.eof()){
    input._binaryDataType = true;
    delete input._tabModelType[0];
    input._tabModelType[0] = new XEMModelType(defaultBinaryModelName);
    tabNbModality = new int64_t[input._pbDimension];
    for (i=0; i<input._pbDimension; i++){
      fi >> tabNbModality[i];
      if (tabNbModality[i]<2)
        throw errorNbModality;
    }
  }
  



  //------//
  // Data //
  //------//

  string dataFileName = "";
  moveUntilReach(fi);
  if(fi.eof()){
    throw  errorDataFile;
  }
  fi >> dataFileName;
/*  XEMNumericDataFile dataFile;
  dataFile._fileName = dataFileName;
  dataFile._format = FormatNumeric::defaultFormatNumericFile;*/
  if (input._binaryDataType){
    input._data = new XEMBinaryData(input._nbSample, input._pbDimension, dataFileName, tabNbModality); 
  }
  else{
    input._data = new XEMGaussianData(input._nbSample, input._pbDimension, dataFileName);
  }
  input._deleteData  = true;

  if (tabNbModality){
    delete [] tabNbModality;
  }
  
  ////////////////////////
  //                    //
  //  Optional Options  //
  //                    //
  ////////////////////////


  //---------//
  // Weight  //
  //---------//
  string weightFileName = "";
  moveUntilReach(fi,"weightfile");
  if(!fi.eof()){
    fi >> weightFileName;
    if(input._data){
      input._data->setWeight(weightFileName);
    }
    else{
      throw wrongWeightFileName;
    }
  }


  // Model type(s) //
  //---------------//

  /* Number of model(s) */
  moveUntilReach(fi,"nbmodel");
  if(!fi.eof()){
    nbFind = true;
    fi >> input._nbModelType;

// delete model data if necessary
    if (input._tabModelType  && input._deleteTabModelType){
      delete input._tabModelType[0];
      delete[] input._tabModelType;
      input._tabModelType= NULL;
    }
    if (input._nbModelType > maxNbModel){
      throw nbModelTypeTooLarge;
    }
    else if (input._nbModelType <= 0){
      throw nbModelTypeTooSmall;
    }
    input._tabModelType = new XEMModelType*[input._nbModelType];
    input._deleteTabModelType = true;

    moveUntilReach(fi,"listmodel");
    if( (!fi.eof()) && (nbFind) ){
      for (i=0; i<input._nbModelType;i++){
        input._tabModelType[i] = new XEMModelType();
        input._tabModelType[i]->input(fi, input._tabNbCluster[0]);
      }
    }

  }


  // init reading at the beginning of file //
  fi.clear();
  fi.seekg(0, ios::beg);
  nbFind = false;


  // tabKnownLabel //
  //---------------//
  string keyWord = "";
  moveUntilReach(fi,"partitionfile");
  if(!fi.eof()){
    input._tabKnownPartition = new XEMPartition*[input._nbNbCluster];
    string * tabFileName = new string[input._nbNbCluster];
    int64_t k;
    for (k=0; k<input._nbNbCluster; k++){
      tabFileName[k] = "";
    }
    readTabFileName(fi, input._nbNbCluster, tabFileName, keyWord);

    for (k=0; k<input._nbNbCluster; k++){
      try{
        XEMNumericPartitionFile partitionFile;
        partitionFile._fileName = tabFileName[k];
        partitionFile._format = FormatNumeric::defaultFormatNumericFile;
        partitionFile._type = TypePartition::defaultTypePartition;
        input._tabKnownPartition[k] = new XEMPartition(input._nbSample, input._tabNbCluster[k], partitionFile);
      }
      catch(XEMError){
        delete input._tabKnownPartition[k];
        input._tabKnownPartition[k] = NULL;
        throw badKnownPartition;
      }
    }
    delete[] tabFileName;
  }
  

  // Criterion(ia) //
  //---------------//

  /* Number of criterion(ia) */
  moveUntilReach(fi,"nbcriterion");
  if(!fi.eof()){
    nbFind = true;
    fi >> input._nbCriterionName;
  
    // delete Criterion data if necessary
    if (input._tabCriterionName && input._deleteTabCriterionName){
      delete[] input._tabCriterionName;
      input._tabCriterionName= NULL;	
    }
    if (input._nbCriterionName > maxNbCriterion){
      throw nbCriterionTooLarge;
    }
    else if (input._nbCriterionName <= 0){
      throw nbCriterionTooSmall;
    }
    moveUntilReach(fi,"listcriterion");
    if( (!fi.eof()) && (nbFind) ){
      delete[] input._tabCriterionName;
      input._tabCriterionName = new XEMCriterionName[input._nbCriterionName];
      input._deleteTabCriterionName = true;
      for (i=0; i<input._nbCriterionName; i++){
        inputCriterion(fi, input._tabCriterionName[i]);
      }
    }
    else if ( (!fi.eof()) && (!nbFind) ){
      throw errorNbCriterion;
    }
    else if ( (fi.eof()) && (nbFind) ){
      throw errorListCriterion;
    }
  }

  

  // Strategy(ies) //
  //---------------//

  /* Number of strategy(ies) */
  moveUntilReach(fi,"nbstrategy");
  if(!fi.eof()){

    fi>>input._nbStrategy;

    // delete strategy Data
    if (input._tabStrategy){
      delete input._tabStrategy[0];
      delete[] input._tabStrategy;
      input._tabStrategy= NULL;
    }

    if (input._nbStrategy > maxNbStrategy){
      throw nbStrategyTypeTooLarge;
    }
    else if (input._nbStrategy <= 0){
      throw nbStrategyTypeTooSmall;
    }

    //delete[] input._tabStrategy;
    input._tabStrategy       = new XEMStrategy*[input._nbStrategy];

    for (i=0; i<input._nbStrategy; i++){
      // initialisation : default
      input._tabStrategy[i] = new XEMStrategy(); // default value
      input._tabStrategy[i]->input(fi, input._data, input._nbNbCluster, input._tabNbCluster, input._tabModelType);
    }
  }

  
  moveUntilReach(fi,"nbcvblocks");
  if(!fi.eof()){
    fi >> input._numberOfCVBlocks ;
  }
  moveUntilReach(fi,"cvinitblocks");
  if(!fi.eof()){
    inputCVinitBlocks( fi,input._CVinitBlocks);
  }
  
  moveUntilReach(fi,"nbdcvblocks");
  if(!fi.eof()){
    fi >> input._numberOfDCVBlocks ;
  }
   
  moveUntilReach(fi,"dcvinitblocks");
  if(!fi.eof()){
    inputDCVinitBlocks( fi,input._DCVinitBlocks);
  }

  input.finalize();
  return fi;
}






//-----------
//-----------
// ostream <<
//-----------
//-----------
ostream & operator << (ostream & fo, XEMOldInput & input){
  
#ifdef VERBOSE
  int64_t i;

  fo<<"nbSample : "<<input._nbSample<<endl;
  fo<<"nbDimension : "<<input._pbDimension<<endl;
  fo<<endl;

  /* Number of mixture components (clusters) */
  fo<<"nbNbCluster : "<<input._nbNbCluster<<endl;
  for (i=0; i<input._nbNbCluster; i++){
    fo<<"tabNbCluster["<<i<<"] : "<<input._tabNbCluster[i]<<endl;
  }
  fo<<endl;

  /* Criterion type */
  fo<<"nbCriterionName : "<<input._nbCriterionName<<endl;
  for (i=0; i<input._nbCriterionName; i++){
    fo<<"tabCriterionName["<<i<<"] : ";
    edit(input._tabCriterionName[i]);
    cout<<endl;
  }
  fo<<endl;
	
  /* Type of model */
  fo<<"nbModelType : "<<input._nbModelType<<endl;
  for (i=0; i<input._nbModelType; i++){
    fo<<"tabModelType["<<i<<"] : ";
    fo<<*(input._tabModelType[i])<<endl;
  }
  fo<<endl;

  //CV and DCV information		
  fo<<"numberOfCVBlocks : "<<input._numberOfCVBlocks<<endl ; 
  fo<<"CVinitBlocks : "<<input._CVinitBlocks<<endl ; 
  fo<<"numberOfDCVBlocks : "<<input._numberOfDCVBlocks<<endl ; 
  fo<<"DCVinitBlocks : "<<input._DCVinitBlocks<<endl<<endl ; 
        
  if (input._tabKnownPartition == NULL){
    fo<<"pas de knownPartition"<<endl;
  }
  else{
    for (int64_t k=0; k<input._nbNbCluster; k++){
      fo<<k+1<<" eme konwn partition :"<<endl;
      if (input._tabKnownPartition[k]){
        fo<<*(input._tabKnownPartition[k])<<endl<<endl;
      }
    }
  }
        
  /* Strategy */
  fo<<"nbStrategyType : "<<input._nbStrategy<<endl;
  for (i=0; i<input._nbStrategy; i++){
    fo<<"Strategy n°"<<i+1<<endl;
    fo<<*(input._tabStrategy[i]);
  }
  fo<<endl;
	
  /* Data */
  fo<<"_data : "<<endl;;
  input._data->output(fo);
  fo<<endl;
  
  fo<<endl;

  fo<<endl<<endl;
  fo<<"-------- END INPUT FILE -----------\n";
  fo<<endl;
#endif
  return fo;
}




// --------
// finalize
// --------
void XEMOldInput::finalize(){
  if (!_finalized){

    _finalized = verif();
  }
}









// ----------------
// Verif
//-----------------
bool XEMOldInput::verif(){
  bool res = true;


  // verif 0 : required inputs
  if (   _nbSample     == 0 
         || _pbDimension  == 0
         || _data         == NULL
         || _nbNbCluster  == 0
         || _tabNbCluster == NULL
     ) {
    res = false;
    throw missingRequiredInputs;
     }
  
 
  // verif 1
  //--------
  //weightTotal must be an integer
     double weightTotal = _data->_weightTotal;
     int64_t iWeightTotal = (int64_t)weightTotal;
     if (weightTotal - iWeightTotal != 0){
       res = false;
       throw weightTotalIsNotAnInteger;
     }


  // verif 2
  //--------
  // DCV context : verify _nbDCVBlock
  // each learning block must contains at least 10 individuals
     int64_t iCriterion=0;
     while (iCriterion<_nbCriterionName && res){
       if (_tabCriterionName[iCriterion] == DCV){
         if (_numberOfDCVBlocks *10 >= iWeightTotal){
           _numberOfDCVBlocks = iWeightTotal / 10;
         }
      // _nbDCVBlock must be > 1
         if (_numberOfDCVBlocks <= 1){
           res = false;
           throw NbDCVBlocksTooSmall;
         }
       }
       iCriterion++;
     }



//verif 3 HDDA models and MAP require given sub dimension
//-------
     bool HDDA_context = false;
     int64_t iModelType = 0;
     while (iModelType<_nbModelType && HDDA_context==false){
       if (isHD(_tabModelType[iModelType]->_nameModel)){
         HDDA_context = true;
       }
       iModelType++;
     }


 // int64_t nbAlgoType = _tabStrategy[k]->_nbAlgoType;
     int64_t inbAlgoType = 0;
     bool MAPAlgo = false;
     for (int64_t k=0;k<_nbStrategy;k++){
       int64_t nbAlgoType = _tabStrategy[k]->getNbAlgo();
       while (inbAlgoType<nbAlgoType && MAPAlgo==false){
         const XEMAlgo * algo = _tabStrategy[k]->getAlgo(inbAlgoType);
         if (algo->getAlgoName() == MAP){
           MAPAlgo = true;
         }
         inbAlgoType++; 
       }
     }

  /*if ((MAPAlgo==true) && (HDDA_context==true) && (_tabSubDimensionFree==NULL) &&(_subDimensionEqual==NULL) ){
     throw ungivenSubDimension;
     res = false;
}*/


     if ((MAPAlgo==true) && (HDDA_context==true)) {
       for (int64_t k=0;k<_nbModelType;k++){
         if ((_tabModelType[k]->_tabSubDimensionFree==NULL) && (_tabModelType[k]->_subDimensionEqual==0)){
           res = false;
           throw ungivenSubDimension;
         }
       }
     }

//verif 4 if subDimensionFree only Gaussian_HDDA_AkjBkQkDk and Gaussian_HDDA_AkBkQkDk
     for (int64_t k=0;k<_nbModelType;k++){

       if (isHD(_tabModelType[k]->_nameModel))
         if (isFreeSubDimension(_tabModelType[k]->_nameModel)
             && (_tabModelType[k]->_tabSubDimensionFree==NULL)){
         res = false;
         throw wrongSubDimension;
             }
             else if (!isFreeSubDimension(_tabModelType[k]->_nameModel) 
                       && (_tabModelType[k]->_subDimensionEqual==0)){
               res = false;
               throw wrongSubDimension;
                       }
     }

	
		 	
    //--------------------------------
    // verif  : Verify all strategies
    //-------------------------------
     int64_t iStrategy;
     for (iStrategy=0; iStrategy<_nbStrategy; iStrategy++){
       res = _tabStrategy[iStrategy]->verify();
     }
		 
     //--------------------------------
     // verif : 
     // If one strategy is M or MAP, one only strategy is allowed
		 //-------------------------------
     iStrategy = 0;
     bool isOneStrategyUseMOrMAP = false;
     while (iStrategy<_nbStrategy && !isOneStrategyUseMOrMAP){
       isOneStrategyUseMOrMAP = (_tabStrategy[iStrategy]->isMAlgo() || _tabStrategy[iStrategy]->isMAPAlgo());
       iStrategy++;
     }
     if (isOneStrategyUseMOrMAP){
       if (_nbStrategy > 1){
         res = false;
         throw nbStrategyMustBe1;
       }
     }
		 
	 
	 //-------
	 // verif
	 // if HDDA model : only M or MAP could be choosen
	 //------
     if (HDDA_context && !isOneStrategyUseMOrMAP){
       res = false;
       throw badAlgorithmInHDContext;
     }
		 
		
	//--------
	// Verif
	// if CV (or DCV) :
	// - algo must be M 
	//--------
     iCriterion=0;
     int64_t k;
     bool CV_DCV_criterion = false;
     while (iCriterion<_nbCriterionName && !CV_DCV_criterion){
       if ((_tabCriterionName[iCriterion] == CV) || (_tabCriterionName[iCriterion] == DCV)){
         CV_DCV_criterion = true;
       }
       iCriterion++;
     }
	
	// CV or DCV
     if (CV_DCV_criterion){
		// algo must be M
       res = _tabStrategy[0]->isMAlgo(); // v�rifier sur le 1er suffit car on a d�j� v�rifi� que 'il y avait M, il n'y a qu'une strat�gie
       if (!res){
         throw algorithmMustBeM;
       }
     }
		
		
	
	//---------------------------------------------------------------------
	// if M : 
	// knowLabel is required and knownLabel=initLabel (for each knonwLabel)
	//---------------------------------------------------------------------
     if (_tabStrategy[0]->isMAlgo()){ // v�rifier sur le 1er suffit car on a d�j� v�rifi� que 'il y avait M, il n'y a qu'une strat�gie
	// knwonLabel is required and initLabel=knownLabel
       if (_tabKnownPartition == NULL){
         res = false;
         throw knownPartitionNeeded;
       }
       else{
         if (_nbNbCluster != _tabStrategy[0]->getStrategyInit()->getNbPartition()){
           res = false;
           throw wrongNbKnownPartitionOrInitPartition;
         }
         k = 0;
         while (k<_nbNbCluster && res){ 
           res = *(_tabKnownPartition[k]) == *(_tabStrategy[0]->getStrategyInit()->getPartition(k));
           if (!res){
             throw knownPartitionAndInitPartitionMustBeEqual;
           }
           k++;
         }
       }
     } 

	
	
		 
  //verif  MAP & CV or DCV impossible
     if ((MAPAlgo==true) && (CV_DCV_criterion==true)){
       res=false;
       throw wrongCriterionName;
     }


     return res;
}


