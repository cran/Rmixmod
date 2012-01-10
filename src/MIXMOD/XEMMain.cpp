/***************************************************************************
                             SRC/MIXMOD/XEMMain.cpp  description
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
#include "XEMMain.h"
#include "XEMRandom.h"
#include "XEMOldInput.h"
#include "XEMBinaryData.h"
#include "XEMStrategy.h"


//------------
// Constructor
//------------
XEMMain::XEMMain(){
  _tabEstimation      = NULL;
  _tabSelection       = NULL;
}



//------------
// Constructor
//------------
XEMMain::XEMMain(XEMOldInput * input){
	if (! input->_finalized){
		throw inputNotFinalized;
	}
  _nbCriterion = input->_nbCriterionName;
  _nbModelType     = input->_nbModelType;
  _nbNbCluster     = input->_nbNbCluster;
  _nbEstimation = _nbNbCluster * _nbModelType;
  _nbSelection     = _nbCriterion;
  _tabEstimation = new XEMEstimation * [_nbEstimation];
  int64_t iNbCluster, iModelType, nbCluster;
  XEMModelType * modelType;
  
  // data
  XEMData * inputData = input->_data;
  XEMData * workingData = inputData; 
  
  //Strategy
  XEMStrategy * inputStrategy = NULL;
  XEMStrategy * workingStrategy = NULL;
  
  // knownPartition
  XEMPartition * inputKnownPartition = NULL;
  XEMPartition * workingKnownPartition = NULL;
  
  int64_t iEstimation = 0; // counting the estimations
     // TODO nbStrategy
    inputStrategy =  input->_tabStrategy[0];
    workingStrategy = inputStrategy; 
    
    
    for (iNbCluster=0 ; iNbCluster<_nbNbCluster ; iNbCluster++){
      nbCluster = input->_tabNbCluster[iNbCluster];
      
      //inputKnownPartition
      if (input->_tabKnownPartition){
        inputKnownPartition = input->_tabKnownPartition[iNbCluster];
      }else{
        inputKnownPartition = NULL;
      }
      workingKnownPartition = inputKnownPartition ;
      vector<int64_t> correspondenceOriginDataToReduceData;
      
      //Reduce Data
      //------------
      if (input->_binaryDataType){
        XEMBinaryData * bData =  (XEMBinaryData*)(inputData);
  
        // initPartition
        XEMPartition * inputInitPartition = NULL;
        XEMPartition * workingInitPartition = NULL;
        if (inputStrategy->getStrategyInit()->getStrategyInitName() == USER_PARTITION){
          inputInitPartition = inputStrategy->getStrategyInit()->getPartition(iNbCluster);
        } 
        try{
          //TODO RD : data ne doit pas forcément etre recréé
          workingData = bData->reduceData(correspondenceOriginDataToReduceData, inputKnownPartition, inputInitPartition, workingKnownPartition, workingInitPartition);
          workingStrategy = new XEMStrategy(*inputStrategy);
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

      
      // create tabEstmation[iEstimation]
      //--------------------------------
      for (iModelType=0 ; iModelType<_nbModelType ; iModelType++){
        modelType = input->_tabModelType[iModelType];
        _tabEstimation[iEstimation] = new XEMEstimation(workingStrategy, modelType, nbCluster, workingData, workingKnownPartition, correspondenceOriginDataToReduceData);
         iEstimation++;
       }
    } // end for iNbCluster


  //create _tabSelection
  _tabSelection = new XEMSelection * [_nbSelection];

  int64_t iSelection ;
  for (iSelection=0; iSelection<_nbSelection; iSelection++){
    XEMCriterionName criterionName = input->_tabCriterionName[iSelection];
    _tabSelection[iSelection]      = new XEMSelection(criterionName, _tabEstimation, _nbEstimation, input);
  }
}



//-----------
// Destructor
//-----------
XEMMain::~XEMMain(){
  int64_t i;
  
  if (_tabEstimation){
    for (i=0; i<_nbEstimation ; i++){
      delete _tabEstimation[i];
    }
    delete[] _tabEstimation;
    _tabEstimation = NULL;
  }
  
  if (_tabSelection){
    for (i=0; i<_nbSelection ; i++){
      delete _tabSelection[i];
    }
    delete[] _tabSelection;
    _tabSelection = NULL;
  }

}



//----------------
// getDCVCriterion
//----------------
XEMDCVCriterion * XEMMain::getDCVCriterion(){
  XEMDCVCriterion * res = NULL;
  int64_t iSelection=0;
  while (iSelection<_nbSelection && res==NULL){
    if (_tabSelection[iSelection]->getCriterionName() == DCV){
      XEMCriterion * tmp = _tabSelection[iSelection]->getCriterion();
      res = dynamic_cast<XEMDCVCriterion*>(tmp);
    }  
    iSelection++;
  }
  return res;
}




//---
//run
//---
void XEMMain::run(XEMOutput * output){
  int64_t iEstimation, iSelection;

  // Estimation
  //-----------
  //_tabEstimation : run
  iEstimation = 0;
#ifdef VERBOSE
  int64_t todo = _nbEstimation-1;
  int64_t j;
  cout << "...running" << endl;  
#endif

  while (iEstimation<_nbEstimation){
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
      printModelType(_tabEstimation[iEstimation]->getModelType());
      cout << " (k=" << _tabEstimation[iEstimation]->getNbCluster() << ")\r" << flush;
#endif
    

    try{
      _tabEstimation[iEstimation]->run();
    }

    catch (XEMErrorType errorType){
      _tabEstimation[iEstimation]->setErrorType(errorType);
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
    j = _nbEstimation;
    cout << "    |"<< flush;
    while(j){
      cout << "-" << flush;
      j--;
    }
    cout << "|                                       " << flush;
  
    cout << endl;
  
  //Selection
  //---------
  cout << "... selecting"<< endl;
  
  int64_t bestIndexEstimation;
#endif
  
  for (iSelection=0 ; iSelection<_nbSelection; iSelection++){  
    
    _tabSelection[iSelection]->run(); 
#ifdef VERBOSE
      cout << "| ---> ";
      bestIndexEstimation = _tabSelection[iSelection]->getBestIndexEstimation();
      if (bestIndexEstimation != -1){
        printModelType(_tabEstimation[bestIndexEstimation]->getModelType());
        cout << " (k="<< _tabEstimation[bestIndexEstimation]->getNbCluster() << ") :: " << flush;
      }
      else{
        throw selectionErrorWithNoEstimation;
      }
    try{
      cout << _tabSelection[iSelection]->getBestCriterionValue() ;
    }
    catch(XEMErrorType errorType){
      cout << "!" ;
    }
    cout << endl;
#endif
  }
  

  
  bool allSelectionError = true;
  for (iSelection=0 ; iSelection<_nbSelection; iSelection++){
    if (_tabSelection[iSelection]->getErrorType() != noSelectionError){
      allSelectionError = false;
    }
  }
  if (allSelectionError){
     throw noSelectionError;
  }


  //output->update
  if(output){
    output->update(_tabSelection, _nbSelection, _tabEstimation, _nbEstimation, _nbModelType);
  }
}




