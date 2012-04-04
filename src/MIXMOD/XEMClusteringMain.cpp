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

#include "XEMClusteringMain.h"
#include "XEMClusteringInput.h"
#include "XEMClusteringOutput.h"
#include "XEMClusteringModelOutput.h"
#include "XEMClusteringStrategy.h"
#include "XEMClusteringStrategyInit.h"
#include "XEMRandom.h"
#include "XEMBinaryData.h"
#include "XEMLabel.h"
#include "XEMBinaryModel.h"
#include "XEMLabelDescription.h"
#include "XEMPartition.h"
#include "XEMError.h"
#include "XEMBICCriterion.h"
#include "XEMICLCriterion.h"
#include "XEMNECCriterion.h"
#include "XEMModelOutput.h"

//------------
// Constructor
//------------
XEMClusteringMain::XEMClusteringMain(){
 throw internalMixmodError;
}



//------------
// Constructor
//------------
XEMClusteringMain::XEMClusteringMain( XEMClusteringInput * input
                                    , XEMClusteringOutput * output
                                    )
                                    : _input(input)
                                    , _output(output)
{}


//-----------
// Destructor
//-----------
XEMClusteringMain::~XEMClusteringMain()
{
  if (_output){
    delete _output;
  }
}


//---
//run
//---
void XEMClusteringMain::run(){
  //--------------------------------------------------------------------
  // Important : Call randomise function to change random seed or not ...
  //---------------------------------------------------------------------
#if RANDOMISE == 1
  randomise();
#else
	antiRandomise();
#endif
	
	
  //----------------------
  // 1. Create Estimations
  //----------------------
  if (!_input){
    throw nullPointerError;
  }
  if (! _input->isFinalized()){
    throw inputNotFinalized;
  }
  
  // get the number of models
  int64_t nbModelType = _input->getNbModelType();
  // get the list of cluster
  std::vector<int64_t> vNbCluster = _input->getNbCluster();
  // get the length of the cluster's list
  int64_t nbNbCluster = vNbCluster.size();
  // compute the number of estimations to run
  int64_t nbEstimation = nbNbCluster * nbModelType;
	std::vector<XEMModel*> estimations(nbEstimation);
  
  int64_t iNbCluster, iModelType, nbCluster;
  XEMModelType * modelType;
  
  // data
  XEMData * inputData = (_input->getDataDescription()).getData();
  XEMData * workingData = inputData; 
  
  //Strategy
  XEMClusteringStrategy * inputStrategy = _input->getStrategy();
  XEMClusteringStrategy * workingStrategy = inputStrategy;
  
  // knownPartition
  XEMPartition * inputKnownPartition = NULL;
  XEMPartition * workingKnownPartition = NULL;
  
	if (_input->getKnownLabelDescription()){
		const XEMLabel * lab = _input->getKnownLabelDescription()->getLabel();
		//inputKnownPartition = new XEMPartition(_input->getKnownLabelDescription()->getLabel(), vNbCluster[0]);
		inputKnownPartition = new XEMPartition(lab, vNbCluster[0]);
		workingKnownPartition = inputKnownPartition;
		if (_input->getKnownPartition()){
			 // _knowPartition and knownLabelDescription can't together not NULL !
			 //_knownPartition will be removed from XEMInput
			throw internalMixmodError;
		}
	}
	else {
		inputKnownPartition = _input->getKnownPartition();
		workingKnownPartition = inputKnownPartition ;
	}
  
  int64_t iEstimation = 0; // counting the estimations
  
  // loop over the number of cluster 
  for (iNbCluster=0 ; iNbCluster<nbNbCluster ; iNbCluster++)
  {
    nbCluster = vNbCluster[iNbCluster];
      
      //KnownPartition
//       inputKnownPartition = _input->getKnownPartition();
//       workingKnownPartition = inputKnownPartition ;
      
    // create model for binary data
    if (_input->isBinaryData()){
    
      std::vector<int64_t> correspondenceOriginDataToReduceData;
#if DATA_REDUCE == 1
      //--------------------------------
      //Reduce Data
      //------------
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
      // fin de ReduceData    
#endif
      // create tabEstmation[iEstimation]
      //--------------------------------
      for (iModelType=0 ; iModelType<nbModelType ; iModelType++){
        modelType = _input->getModelType(iModelType);
        estimations[iEstimation] = new XEMBinaryModel( modelType, nbCluster, workingData, workingKnownPartition, correspondenceOriginDataToReduceData );
        iEstimation++;
      }
    }
    // create model for quantitative data
    else{
      // create tabEstmation[iEstimation]
      //--------------------------------
      for (iModelType=0 ; iModelType<nbModelType ; iModelType++){
        modelType = _input->getModelType(iModelType);
        estimations[iEstimation] = new XEMModel( modelType, nbCluster, workingData, workingKnownPartition );
        iEstimation++;
       }
    }
  } // end for iNbCluster
  // release memory
  delete inputKnownPartition;

  
  //-------------------
  // 2. run Estimations
  //-------------------
  
  // Estimation
  //-----------
#ifdef VERBOSE
  int64_t todo = nbEstimation-1;
  int64_t j;
  cout << "...running" << endl;  
#endif
  
  // loop over the number of estimation to do
  iEstimation = 0;
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
      workingStrategy->run(estimations[iEstimation]);
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
  // create Output
  //-----------------  
  // get criterion names
  std::vector<XEMCriterionName> const & criterion = _input->getCriterionName();
  _output = new XEMClusteringOutput(estimations, criterion);  
  //-------------------
  // 3. run Selections
  //-------------------
  // loop over all models
  for (unsigned int iModel=0; iModel<nbEstimation; iModel++){
    // chek whether error occured for that model
    if ( estimations[iModel]->getErrorType() == noError ){
      // loop over criterion name
      for (unsigned int iCriterion=0; iCriterion<criterion.size(); iCriterion++ ){
        switch (criterion[iCriterion]) {
          case BIC : 
          {
            XEMBICCriterion bic(estimations[iModel]);
            bic.run(_output->getClusteringModelOutput(iModel)->getCriterionOutput(BIC));
            break;
          }
          case CV : throw XEMDAInput;
          case ICL : 
          {
            XEMICLCriterion icl(estimations[iModel]);
            icl.run(_output->getClusteringModelOutput(iModel)->getCriterionOutput(ICL));
            break;
          }
          case NEC :  
          {
            XEMNECCriterion nec(estimations[iModel]);
            nec.run(_output->getClusteringModelOutput(iModel)->getCriterionOutput(NEC));
            break;
          }
          case UNKNOWN_CRITERION_NAME : throw internalMixmodError;
          default : throw internalMixmodError;
        }
      }// end iCriterion
    }
    else{ // set criterion error
      // loop over criterion name
      for (unsigned int iCriterion=0; iCriterion<criterion.size(); iCriterion++ ){
        _output->getClusteringModelOutput(iModel)->setCriterionOutput(XEMCriterionOutput(criterion[iCriterion], 0.0, estimations[iModel]->getErrorType()));
      }
    }
  }//end iModel
  
  // release memory
  for (unsigned int iModel=0; iModel<nbEstimation; iModel++){
    delete estimations[iModel];
  }
}

