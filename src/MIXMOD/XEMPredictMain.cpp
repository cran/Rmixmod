/***************************************************************************
 SRC/MIXMOD/XEMPredictMain.cpp  description
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

#include "XEMPredictMain.h"
#include "XEMPredictInput.h"
#include "XEMPredictOutput.h"
#include "XEMPredictStrategy.h"
#include "XEMBinaryData.h"
#include "XEMLabelDescription.h"
#include "XEMModel.h"
#include "XEMBinaryModel.h"
#include "XEMError.h"
#include "XEMPartition.h"

//------------
// Constructor
//------------
XEMPredictMain::XEMPredictMain(){
  throw internalMixmodError;
}



//------------
// Constructor
//------------
XEMPredictMain::XEMPredictMain( XEMPredictInput * input, XEMPredictOutput * output )
                              : _input(input)
                              , _output(output)
{}


//-----------
// Destructor
//-----------
XEMPredictMain::~XEMPredictMain()
{
  if (_output){
    delete _output;
  }
}


//---
//run
//---
void XEMPredictMain::run(){
  
  if (!_input){
    throw nullPointerError;
  }
  if (! _input->isFinalized()){
    throw inputNotFinalized;
  }
  
  XEMModelType * modelType = _input->getModelType(0);
  int64_t nbCluster = _input->getNbCluster(0);
  XEMData * data = (_input->getDataDescription()).getData();
  
  // define a new estimation
  XEMModel * estimation;
  
  // create model for binary data
  if (_input->isBinaryData()){
    
    XEMData * workingData = data; 
    XEMPartition * inputKnownPartition = NULL;
    XEMPartition * workingKnownPartition = NULL;
    
    std::vector<int64_t> correspondenceOriginDataToReduceData;
#if DATA_REDUCE == 1
    //--------------------------------
    //Reduce Data
    //------------
    XEMBinaryData * bData =  dynamic_cast<XEMBinaryData*>(data);
    
    // initPartition
    XEMPartition * inputInitPartition = NULL;
    XEMPartition * workingInitPartition = NULL;

    try{
      //TODO RD : data ne doit pas forcément etre recréé
      workingData = bData->reduceData(correspondenceOriginDataToReduceData, inputKnownPartition, inputInitPartition, workingKnownPartition, workingInitPartition);
      /* TODO ?:
       if inputKnownPartition : delete workingKnownPartition 
       if inputInitPartition : delete workingStrategy, workingInitPartition
       
       */
    }
    catch(XEMErrorType errorType){
      workingData = NULL;
      throw errorType;
    } 
    // fin de ReduceData    
#endif

    // create new estimation
    estimation = new XEMBinaryModel( modelType, nbCluster, workingData, workingKnownPartition, correspondenceOriginDataToReduceData );
  }
  // create model for quantitative data
  else{
    // create new estimation
     estimation = new XEMModel(modelType, nbCluster, data, NULL); 
  }

  // create new strategy
  XEMPredictStrategy strategy(_input->getClassificationRule());
  
  try{
      strategy.run(estimation);
  }
  catch (XEMErrorType errorType){
#ifdef VERBOSE
    XEMError error(errorType);
    error.run();
#endif      
    // set error for that model
    estimation->setError(errorType);
  }
  // create output
  _output = new XEMPredictOutput(estimation);
  
  // release memory
  delete estimation;
}



//------------------------
// return pointer to Input
//------------------------
XEMInput * XEMPredictMain::getInput(){
  if (_input){
    return _input;
  }
  else{
    throw nullPointerError;
  }
}



