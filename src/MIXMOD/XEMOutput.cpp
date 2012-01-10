/***************************************************************************
                             SRC/MIXMOD/XEMOutput.cpp  description
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
#include "XEMOutput.h"
#include "XEMMain.h"
#include "XEMClusteringMain.h"
#include "XEMEstimation.h"
#include "XEMClusteringModelOutput.h"
#include "XEMModelOutput.h"
#include "XEMClusteringOutput.h"

//------------
// Constructor
//------------
XEMOutput::XEMOutput(){
  _condExe            = NULL;
  _tabAllModelOutput  = NULL;
  _tabBestModelOutput = NULL;
  _DCVCriterion = NULL;
}



//------------
// Constructor
//------------
XEMOutput::XEMOutput(XEMOldInput * cInput,  XEMMain& xmain){
  int64_t i;
  _nbEstimation     = xmain.getNbEstimation();
  _nbBestModel = xmain.getNbSelection();
  _condExe     = new XEMCondExe(cInput, xmain.getTabEstimation(), _nbEstimation);
  _tabBestModelOutput = new XEMOldModelOutput*[_nbBestModel];
  for (i=0; i<_nbBestModel; i++){
     _tabBestModelOutput[i] = new XEMOldModelOutput();
  }
  _tabAllModelOutput = new XEMOldModelOutput*[_nbEstimation];
  for (i=0; i<_nbEstimation; i++){
     _tabAllModelOutput[i] = new XEMOldModelOutput();
  }
  _DCVCriterion = xmain.getDCVCriterion();
}



//------------
// Constructor
//------------
XEMOutput::XEMOutput(XEMClusteringMain & xmain){
  int64_t i;
  //_nbEstimation     = xmain.getNbEstimation();
  XEMClusteringInput * cInput = dynamic_cast<XEMClusteringInput*> (xmain.getInput());
  _nbEstimation = cInput->getNbClusterSize() * cInput->getNbModelType();
  //_nbBestModel = xmain.getNbSelection();
  XEMClusteringOutput * cOutput =  xmain.getClusteringOutput();
  const XEMClusteringModelOutput * const cMOutput =  cOutput->getClusteringModelOutput(0);
  _nbBestModel = cMOutput->getCriterionSize();
  _condExe     = new XEMCondExe(cInput, xmain.getClusteringOutput());
  _tabBestModelOutput = new XEMOldModelOutput*[_nbBestModel];
  for (i=0; i<_nbBestModel; i++){
     _tabBestModelOutput[i] = new XEMOldModelOutput();
  }
  _tabAllModelOutput = new XEMOldModelOutput*[_nbEstimation];
  for (i=0; i<_nbEstimation; i++){
     _tabAllModelOutput[i] = new XEMOldModelOutput();
  }
// TODO ??  _DCVCriterion = xmain.getDCVCriterion();
  _DCVCriterion = NULL;
  
  
  // TODO !! pour que les anciens Output fonctionnent : à enlever ASAP
  XEMEstimation * estimation0 = cMOutput->_estimation;
  vector<XEMCriterionOutput> criterionOutput = estimation0->getCriterionOutput();
  for (i=0; i<_nbEstimation; i++){
    const XEMClusteringModelOutput * const cMOutput = cOutput->getClusteringModelOutput(i);
    _tabAllModelOutput[i]->update2(cMOutput->_estimation);
  }
  for (i=0; i<_nbBestModel; i++){
    //TODO _tabBestModelOutput[i]->update(tabSelection[i], tabEstimation);
    XEMCriterionName cN = criterionOutput[i].getCriterionName();
    
    cOutput->sort(cN);
    XEMClusteringModelOutput * cMO2 = cOutput->getClusteringModelOutput(0);
    _tabBestModelOutput[i]->update1(cMO2->_estimation, i);
  }

  // for error update
  //TODO _condExe->update(_nbEstimation, tabEstimation, _nbBestModel, tabSelection);
}



//-----------
// Destructor
//-----------
XEMOutput::~XEMOutput(){
  int64_t i;
  if (_condExe){
    delete _condExe;
  }

  if (_tabAllModelOutput){
     for(i=0; i<_nbEstimation; i++){
        delete _tabAllModelOutput[i];
     }
     delete [] _tabAllModelOutput;
     _tabAllModelOutput = NULL;
  }

  if (_tabBestModelOutput){
     for(i=0; i<_nbBestModel; i++){
        delete _tabBestModelOutput[i];
     }
     delete [] _tabBestModelOutput;
     //_tabBestModelOutput = NULL;
  }
}



//-------------------
// set a mixmod error
//-------------------
void XEMOutput::setErrorMixmod(XEMErrorType errorType){
   _condExe->_errorMixmod = errorType;
}



//-------
// update
//-------
// after calculation, object must be updated
void XEMOutput::update(XEMSelection ** tabSelection, int64_t nbSelection, XEMEstimation ** tabEstimation, int64_t nbEstimation, int64_t nbModeltype){
  int64_t i;
  for (i=0; i<nbSelection; i++){
    _tabBestModelOutput[i]->update(tabSelection[i], tabEstimation, nbModeltype);
  }
  for (i=0; i<nbEstimation; i++){
    _tabAllModelOutput[i]->update(tabEstimation[i], nbSelection, tabSelection);
  }

  // for error update
  _condExe->update(nbEstimation, tabEstimation, nbSelection, tabSelection);
}



//-----
// edit
//-----
void XEMOutput::editFile(ofstream ** oFile){
  int64_t i, iSel, iEst;
  if (_condExe->_errorMixmod == noError){
    
    int64_t nbOutputFileType = 9;
  
  XEMCriterionName criterionName ;
  
  ofstream * currentFile ;
  
  //standardOutput
  for (iSel=0; iSel<_nbBestModel; iSel++){
    if (_tabBestModelOutput[iSel]->_errorType == noError){
      criterionName = _tabBestModelOutput[iSel]->_tabCriterionOutput[0]->getCriterionName();
      //if(criterionType == DCV)  criterionType = CV;
      
      
      i = nbOutputFileType*criterionName + 0;  //index in table oFile
      currentFile = oFile[i] ;
      
      *currentFile<<"---------------------------------\n";
      *currentFile<<"     MIXMOD  Standard Output     \n";
      *currentFile<<"---------------------------------\n";
      *currentFile<<"\n";
      *currentFile<<"Number of samples : "<<_condExe->_weightTotal<<endl<<endl;
      _tabBestModelOutput[iSel]->_tabCriterionOutput[0]->editType(*currentFile);
      if (_tabBestModelOutput[iSel]->_strategy){
        _tabBestModelOutput[iSel]->_strategy->edit(*currentFile);
      }
      else{
        _tabBestModelOutput[iSel]->_cStrategy->edit(*currentFile);
      }
      *currentFile<<"\n";
      *currentFile<<"\t\tNumber of Clusters : "<<_tabBestModelOutput[iSel]->_nbCluster<<endl;
      *currentFile<<"\t\t------------------"<<endl<<endl;
      editModelType(*currentFile, _tabBestModelOutput[iSel]->_modelType);
      _tabBestModelOutput[iSel]->_tabCriterionOutput[0]->editValue(*currentFile, true);
      _tabBestModelOutput[iSel]->_param->edit(*currentFile, true);
    }
  }

  
  
  //_rankOfBestModelTypetandardOutput
  for (iSel=0; iSel<_nbBestModel; iSel++){
    if (_tabBestModelOutput[iSel]->_errorType == noError){
      criterionName = _tabBestModelOutput[iSel]->_tabCriterionOutput[0]->getCriterionName();
      
      i = nbOutputFileType*criterionName + 1;  //index in table oFile
      currentFile = oFile[i] ;
      
			*currentFile<<_tabBestModelOutput[iSel]->_nbCluster<<endl<<endl;
      *currentFile<<_tabBestModelOutput[iSel]->_rankOfBestModelType<<endl<<endl;
			//cout<<"rank : "<<_rankOfBestModelType<<endl;
			//*currentFile<<_tabBestModelOutput[iSel]->_rankOfBestStrategy<<endl<<endl;
			_tabBestModelOutput[iSel]->_tabCriterionOutput[0]->editValue(*currentFile);
      _tabBestModelOutput[iSel]->_param->edit(*currentFile);
    }
  }


  // labelOutput
  for (iSel=0; iSel<_nbBestModel; iSel++){
    if (_tabBestModelOutput[iSel]->_errorType == noError){
      criterionName = _tabBestModelOutput[iSel]->_tabCriterionOutput[0]->getCriterionName();
      //if(criterionType==DCV)  criterionType = CV;
        
      i = nbOutputFileType*criterionName + 2;  //index in table oFile
      _tabBestModelOutput[iSel]->_probaOutput->editLabel(*(oFile[i]));
    }
  }

  // parameterOutput
  for (iSel=0; iSel<_nbBestModel; iSel++){
    if (_tabBestModelOutput[iSel]->_errorType == noError){
      criterionName = _tabBestModelOutput[iSel]->_tabCriterionOutput[0]->getCriterionName();
      //if(criterionType==DCV)  criterionType = CV;
      i = nbOutputFileType*criterionName + 3;  //index in table oFile
      _tabBestModelOutput[iSel]->_param->edit(*(oFile[i]));
    }
  }

  // tikOutput
  for (iSel=0; iSel<_nbBestModel; iSel++){
    if (_tabBestModelOutput[iSel]->_errorType == noError){
      criterionName = _tabBestModelOutput[iSel]->_tabCriterionOutput[0]->getCriterionName();
      //if(criterionType==DCV)  criterionType = CV;
      i = nbOutputFileType*criterionName + 4;  //index in table oFile
      _tabBestModelOutput[iSel]->_probaOutput->editPostProba(*(oFile[i]));
    }
  }

  // zikOutput
  for (iSel=0; iSel<_nbBestModel; iSel++){
    if (_tabBestModelOutput[iSel]->_errorType == noError){
      criterionName = _tabBestModelOutput[iSel]->_tabCriterionOutput[0]->getCriterionName();
      //if(criterionType==DCV)    criterionType = CV;
      i = nbOutputFileType*criterionName + 5;  //index in table oFile
      _tabBestModelOutput[iSel]->_probaOutput->editPartition(*(oFile[i]));
    }
  }

  // likelihoodOutput
  for (iSel=0; iSel<_nbBestModel; iSel++){
    if (_tabBestModelOutput[iSel]->_errorType == noError){
      criterionName = _tabBestModelOutput[iSel]->_tabCriterionOutput[0]->getCriterionName();
      //if(criterionType==DCV)   criterionType = CV;
      i = nbOutputFileType*criterionName + 6;  //index in table oFile
      currentFile = oFile[i];
      
      *currentFile<<"---------------------------------\n";
      *currentFile<<"     MIXMOD  Likelihood Output     \n";
      *currentFile<<"---------------------------------\n";
     	*currentFile<<"\n";
      *currentFile<<"Number of samples : "<<_condExe->_weightTotal<<endl<<endl;
      if ( _tabBestModelOutput[iSel]->_strategy){
        _tabBestModelOutput[iSel]->_strategy->edit(*currentFile);
      }
      else{
        _tabBestModelOutput[iSel]->_cStrategy->edit(*currentFile);
      }
      *currentFile<<"\n";
      *currentFile<<"\t\tNumber of Clusters : "<<_tabBestModelOutput[iSel]->_nbCluster<<endl;
      *currentFile<<"\t\t------------------"<<endl<<endl;
      editModelType(*currentFile, _tabBestModelOutput[iSel]->_modelType);
      _tabBestModelOutput[iSel]->_likelihoodOutput->edit(*currentFile,true);
    }
  }

  // numericLikelihoodOutput
  for (iSel=0; iSel<_nbBestModel; iSel++){
    if (_tabBestModelOutput[iSel]->_errorType == noError){
      criterionName = _tabBestModelOutput[iSel]->_tabCriterionOutput[0]->getCriterionName();
      //if(criterionType==DCV)      criterionType = CV;
      i = nbOutputFileType*criterionName + 8;  //index in table oFile
      //editModelType(*(oFile[i]), _tabBestModelOutput[iSel]->_modelType);
      _tabBestModelOutput[iSel]->_likelihoodOutput->edit(*(oFile[i]));
    }
  }

  // ErrorOutput
  for (iSel=0; iSel<_nbBestModel; iSel++){
    criterionName = _condExe->_tabCriterionName[iSel];
    //if(criterionType==DCV)      criterionType = CV;
    i = nbOutputFileType*criterionName + 7;  //index in table oFile
    _condExe->editTabCriterionError(*(oFile[i]), iSel);
  }

  // completeOutput
  i = (int64_t)completeOutput;
  currentFile = oFile[i];
  *currentFile<<"---------------------------------\n";
  *currentFile<<"     MIXMOD  Complete Output     \n";
  *currentFile<<"---------------------------------\n";
  *currentFile<<"\n";
  *currentFile<<"Number of samples : "<<_condExe->_weightTotal<<endl<<endl;
  for(iEst=0; iEst<_nbEstimation; iEst++){
    if (_tabAllModelOutput[iEst]->_strategy){
      _tabAllModelOutput[iEst]->_strategy->edit(*currentFile);
    }
    else{
      _tabAllModelOutput[iEst]->_cStrategy->edit(*currentFile);
    }
    *currentFile<<"\n";
    *currentFile<<"\t\tNumber of Clusters : "<<_tabAllModelOutput[iEst]->_nbCluster<<endl;
    *currentFile<<"\t\t------------------"<<endl<<endl;
    editModelType(*currentFile, _tabAllModelOutput[iEst]->_modelType);
    if (_tabAllModelOutput[iEst]->_errorType == noError){
      for (iSel=0; iSel<_nbBestModel; iSel++){
        _tabAllModelOutput[iEst]->_tabCriterionOutput[iSel]->editTypeAndValue(*currentFile);
      }
      _tabAllModelOutput[iEst]->_param->edit(*currentFile, true);
    }
    else{
      *currentFile<<"\t\t\tError"<<endl<<endl;
    }
  }

  // numericCompleteOutput
  i = (int64_t)numericCompleteOutput;
  currentFile = oFile[i];
  for(iEst=0; iEst<_nbEstimation; iEst++){
    if (_tabAllModelOutput[iEst]->_errorType == noError){
      for (iSel=0; iSel<_nbBestModel; iSel++){
        _tabAllModelOutput[iEst]->_tabCriterionOutput[iSel]->editValue(*currentFile);
      }
      _tabAllModelOutput[iEst]->_param->edit(*currentFile);
    }
  }

  // labelClassificationOutput
  for (iSel=0; iSel<_nbBestModel; iSel++){
    if (_tabBestModelOutput[iSel]->_errorType == noError){
      criterionName = _tabBestModelOutput[iSel]->_tabCriterionOutput[0]->getCriterionName();
      //if(criterionType==DCV)    criterionType = CV;
      
      if (criterionName == CV){
        i = (int64_t)CVlabelClassificationOutput;
      _tabBestModelOutput[iSel]->_probaOutput->editCVLabel(*(oFile[i]));
      }
    }
  }


  // errorModelOutput
  i = (int64_t)errorModelOutput;
  _condExe->editTabEstimationError(*(oFile[i]));

  // DCV information
  i = (int64_t)DCVinfo ;
  if(_DCVCriterion){
    _DCVCriterion->edit(*(oFile[i]));
  }
 
  i = (int64_t)DCVnumericInfo ;
  if(_DCVCriterion){
    _DCVCriterion->editNumeric(*(oFile[i]));
  }
  
  }// end if (condExe->_errorMixmod == noError)
  
  // Everytime errorMixmodOutputFile must be written 
  // errorMixmodOutput
  i = (int64_t)errorMixmodOutput;
  *(oFile[i])<<_condExe->_errorMixmod<<endl;
  
}



