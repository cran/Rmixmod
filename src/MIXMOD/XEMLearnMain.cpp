/***************************************************************************
 SRC/MIXMOD/XEMLearnMain.cpp  description
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

#include "XEMLearnMain.h"
#include "XEMLearnInput.h"
#include "XEMLearnOutput.h"
#include "XEMLearnModelOutput.h"
#include "XEMLearnStrategy.h"
#include "XEMBinaryData.h"
#include "XEMLabelDescription.h"
#include "XEMModel.h"
#include "XEMBinaryModel.h"
#include "XEMError.h"
#include "XEMPartition.h"
#include "XEMBICCriterion.h"
#include "XEMCVCriterion.h"
#include "XEMModelOutput.h"

//------------
// Constructor
//------------
XEMLearnMain::XEMLearnMain(){
  throw internalMixmodError;
}



//------------
// Constructor
//------------
XEMLearnMain::XEMLearnMain( XEMLearnInput * input, XEMLearnOutput * output )
                          : _input(input)
                          , _output(output)
{}


//-----------
// Destructor
//-----------
XEMLearnMain::~XEMLearnMain()
{
  if (_output){
    delete _output;
  }
}


//---
//run
//---
void XEMLearnMain::run(){
  
  if (!_input){
    throw nullPointerError;
  }
  if (! _input->isFinalized()){
    throw inputNotFinalized;
  }
  
  // get the number of models
  int64_t nbModelType = _input->getNbModelType();
  // get the list of number of clusters
  std::vector<int64_t> vNbCluster = _input->getNbCluster();
  // get the size of the cluster's list
//  int64_t nbNbCluster = vNbCluster.size();
  // compute the total number of estimation to run
  int64_t nbEstimation = nbModelType;
  
  // get number of cluster
  int64_t nbCluster = vNbCluster[0];
  
  // get data
  XEMData * inputData = (_input->getDataDescription()).getData();
  
  // get labels
  XEMPartition * inputKnownPartition = new XEMPartition(_input->getKnownLabelDescription()->getLabel(), vNbCluster[0]);
  
  // define a new Learn Stratey
  XEMLearnStrategy learnStrategy;
  
  // Define a vector of models
  std::vector<XEMModel*> estimations(nbEstimation);
  
  // create model for binary data
  if (_input->isBinaryData()){
    
    XEMData * workingData = inputData; 
    XEMPartition * workingKnownPartition = inputKnownPartition;
    std::vector<int64_t> correspondenceOriginDataToReduceData;
#if DATA_REDUCE == 1
    //--------------------------------
    //Reduce Data
    //------------
    XEMBinaryData * bData =  dynamic_cast<XEMBinaryData*>(inputData);
    
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
    // create tabEstmation[iEstimation]
    //--------------------------------
    for (int iModelType=0 ; iModelType<nbModelType ; iModelType++)
    {
      XEMModelType * modelType = _input->getModelType(iModelType);
      estimations[iModelType] = new XEMBinaryModel( modelType, nbCluster, workingData, workingKnownPartition, correspondenceOriginDataToReduceData );
    }
  }
  // create model for quantitative data
  else{
    // loop over all models
    for ( int iModelType=0; iModelType<nbModelType; iModelType++ )
    {
      XEMModelType * modelType = _input->getModelType(iModelType);
      // create new estimation
      estimations[iModelType] = new XEMModel(modelType, nbCluster, inputData, inputKnownPartition); 
    }
  }
  // release memory
  delete inputKnownPartition;  
  
  //-------------------
  // 2. run Estimations
  //-------------------
  // loop over the number of estimation to do
  int64_t iEstimation = 0;
  while (iEstimation < nbEstimation){
    try{
      learnStrategy.run(estimations[iEstimation]);
    }
    catch (XEMErrorType errorType){
#ifdef VERBOSE
      XEMError error(errorType);
      error.run();
#endif      
      // set error for that model
      estimations[iEstimation]->setError(errorType);
    }
    iEstimation++;
  }
  //----------------
  // create Output
  //-----------------
  _output = new XEMLearnOutput(estimations);
  //-------------------
  // 3. run Selections
  //-------------------
  // get criterion names
  std::vector<XEMCriterionName> const & criterion = _input->getCriterionName();
  
  // loop over all models
  for (unsigned int iModel=0; iModel<nbEstimation; iModel++){
    // chek whether error occured for that model
    if ( estimations[iModel]->getErrorType() == noError ){
      // loop over criterion name
      for (unsigned int iCriterion=0; iCriterion<criterion.size(); iCriterion++ ){
        switch (criterion[iCriterion]) {
          case BIC : 
          {
            // create BIC criterion
            XEMBICCriterion bic(estimations[iModel]);
            // compute criterion outputs
            bic.run(_output->getLearnModelOutput(iModel)->getCriterionOutput(BIC));
            break;
          }
          case CV : 
          {
            // create CV criterion
            XEMCVCriterion cv(estimations[iModel], _input->getNbCVBlock());
            // compute criterion outputs
            cv.run(_output->getLearnModelOutput(iModel)->getCriterionOutput(CV));
            // add label from CV
            _output->getLearnModelOutput(iModel)->setCVLabel(estimations[iModel], cv.getCVLabel());
            break;
          }
          case ICL : throw badCriterion;
          case NEC :  throw badCriterion;
          case UNKNOWN_CRITERION_NAME : throw internalMixmodError;
          default : throw internalMixmodError;
        }
      } // end iCriterion
    }
    else{ // set criterion error
      // loop over criterion name
      for (unsigned int iCriterion=0; iCriterion<criterion.size(); iCriterion++ ){
        _output->getLearnModelOutput(iModel)->setCriterionOutput(XEMCriterionOutput(criterion[iCriterion], 0.0, estimations[iModel]->getErrorType()));
      } // end iCriterion
    }
  }//end iModel
  
  // release memory
  for (unsigned int iModel=0; iModel<nbEstimation; iModel++){
    delete estimations[iModel];
  }
}



//------------------------
// return pointer to Input
//------------------------
XEMInput * XEMLearnMain::getInput(){
  if (_input){
    return _input;
  }
  else{
    throw nullPointerError;
  }
}



