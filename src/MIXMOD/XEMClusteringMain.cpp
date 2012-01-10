/***************************************************************************
                             SRC/MIXMOD/XEMClusteringMain.cpp  description
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
#include "XEMError.h"
#include "XEMClusteringMain.h"
#include "XEMRandom.h"
#include "XEMOldInput.h"
#include "XEMBinaryData.h"
#include "XEMClusteringOutput.h"


//------------
// Constructor
//------------
XEMClusteringMain::XEMClusteringMain(){
 throw internalMixmodError;
}



//------------
// Constructor
//------------
XEMClusteringMain::XEMClusteringMain(XEMClusteringInput * input, XEMClusteringOutput * output){
  _input  = input;
  _output = output;
}





//-----------
// Destructor
//-----------
XEMClusteringMain::~XEMClusteringMain(){
 if (_output){
  delete _output;
 }
 if (_input){
  delete _input; 
 }
}



//----------------
// getDCVCriterion
//----------------
//TODO
// XEMDCVCriterion * XEMClusteringMain::getDCVCriterion(){
//   XEMDCVCriterion * res = NULL;
//   int64_t iSelection=0;
//   while (iSelection<_nbSelection && res==NULL){
//     if (_tabSelection[iSelection]->getCriterionName() == DCV){
//       XEMCriterion * tmp = _tabSelection[iSelection]->getCriterion();
//       res =  dynamic_cast< XEMDCVCriterion*> (tmp);
//     }  
//     iSelection++;
//   }
//   return res;
// }















//---
//run
//---
void XEMClusteringMain::run(){
  
  //----------------------
  // 1. Create Estimations
  //----------------------
  if (!_input){
    throw nullPointerError;
  }
  if (! _input->isFinalized()){
    throw inputNotFinalized;
  }
  
  vector<XEMCriterionName> criterionName = _input->getCriterionName();
  
 // _nbCriterion = _input->getNbCriterionName();
  int64_t nbModelType = _input->getNbModelType();
  vector<int64_t> vNbCluster = _input->getNbCluster();
  int64_t nbNbCluster = vNbCluster.size();
  int64_t nbEstimation = nbNbCluster * nbModelType;
  vector<XEMEstimation*> estimations(nbEstimation);
  
  int64_t iNbCluster, iModelType, nbCluster;
  XEMModelType * modelType;
  
  // data
  XEMData * inputData = (_input->getDataDescription()).getData();
  XEMData * workingData = inputData; 
  
  //Strategy
  XEMClusteringStrategy * inputStrategy = NULL;
  XEMClusteringStrategy * workingStrategy = NULL;
  
  // knownPartition
  XEMPartition * inputKnownPartition = NULL;
  XEMPartition * workingKnownPartition = NULL;
  
  int64_t iEstimation = 0; // counting the estimations
  inputStrategy =  _input->getStrategy();
  workingStrategy = inputStrategy; 
    
    
    for (iNbCluster=0 ; iNbCluster<nbNbCluster ; iNbCluster++){
      nbCluster = vNbCluster[iNbCluster];
      
      //inputKnownPartition
      inputKnownPartition = _input->getKnownPartition();
      workingKnownPartition = inputKnownPartition ;
      vector<int64_t> correspondenceOriginDataToReduceData;
      
#if DATA_REDUCE == 1
      //Reduce Data
      //------------
      if (_input->isBinaryData()){
        XEMBinaryData * bData =  dynamic_cast<XEMBinaryData*>(inputData);
  
        // initPartition
        XEMPartition * inputInitPartition = NULL;
        XEMPartition * workingInitPartition = NULL;
        if (inputStrategy->getStrategyInit()->getStrategyInitName() == USER_PARTITION){
          inputInitPartition = inputStrategy->getStrategyInit()->getPartition(iNbCluster);
        } 
        try{
          //TODO RD : data ne doit pas forcément etre recréé
          workingData = bData->reduceData(correspondenceOriginDataToReduceData, inputKnownPartition, inputInitPartition, workingKnownPartition, workingInitPartition);
          workingStrategy = new XEMClusteringStrategy(*inputStrategy);
          if (workingInitPartition
             ){
            workingStrategy->setInitPartition(workingInitPartition, iNbCluster);
          }
          /* TODO ?:
          if inputKnownPartition : delete workingKnownPartition 
          if inputInitPartition : delete workingStrategy, workingInitPartition
          
          */
        }
        catch(XEMErrorType errorType){
          workingData = NULL;
          throw errorType;
        } 
      }
      // fin de ReduceData
#endif
      
      // create tabEstmation[iEstimation]
      //--------------------------------
      for (iModelType=0 ; iModelType<nbModelType ; iModelType++){
        modelType = _input->getModelType(iModelType);
          estimations[iEstimation] = new XEMEstimation(workingStrategy, modelType, nbCluster, workingData, criterionName, workingKnownPartition, correspondenceOriginDataToReduceData);
         iEstimation++;
       }
    } // end for iNbCluster




  
  
  //-------------------
  // 2. run Estimations
  //-------------------
  


  // Estimation
  //-----------
  //_tabEstimation : run
  iEstimation = 0;
#ifdef VERBOSE
  int64_t todo = nbEstimation-1;
  int64_t j;
  cout << "...running" << endl;  
#endif

  while (iEstimation < nbEstimation){
#ifdef VERBOSE

      cout << "    |"<< flush;

      j = iEstimation+1;
      while(j){
        cout << "-" << flush;
        j--;
      }
      j= todo;
      while(j){
        cout << " " << flush;
        j--;
      }


      cout << "|                      \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << flush;
      printModelType(estimations[iEstimation]->getModelType());
      cout << " (k=" << estimations[iEstimation]->getNbCluster() << ")\r" << flush;
#endif
    

    try{
      estimations[iEstimation]->run();
    }

    catch (XEMErrorType errorType){
      estimations[iEstimation]->setErrorType(errorType);
      XEMError error(errorType);
#ifdef VERBOSE
      error.run();
#endif
    }
    iEstimation++;
    
#ifdef VERBOSE
      todo--;
#endif
    
  }
  
  
#ifdef VERBOSE
    j = nbEstimation;
    cout << "    |"<< flush;
    while(j){
      cout << "-" << flush;
      j--;
    }
    cout << "|                                       " << flush;
  
    cout << endl;
#endif
  
  //----------------
  // 3. create Output
  //-----------------
  _output = new XEMClusteringOutput(estimations);
  _output->sort(_input->getCriterionName()[0]);
  
// TODO 
//  _output->sort(_input->getCriterionName(0));
  
  
}




