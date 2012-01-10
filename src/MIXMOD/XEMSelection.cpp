/***************************************************************************
                             SRC/MIXMOD/XEMSelection.cpp  description
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

#include "XEMSelection.h"
#include "XEMBICCriterion.h"
#include "XEMCVCriterion.h"
#include "XEMICLCriterion.h"
#include "XEMNECCriterion.h"
#include "XEMDCVCriterion.h"
#include "XEMError.h"
#include "XEMUtil.h"


//------------
// Constructor
//------------
XEMSelection::XEMSelection(){
  _tabEstimation = NULL;
  _criterion  = NULL;
  _criterionName = UNKNOWN_CRITERION_NAME;
  _errorType     = noError;
  _tabCriterionValueForEachEstimation = NULL;
  _tabCriterionErrorForEachEstimation = NULL;
  _CVLabelOfEachEstimation = NULL;
}



//------------
// Constructor
//------------
XEMSelection::XEMSelection(XEMCriterionName criterionName, XEMEstimation **& tabEstimation,
                           int64_t nbEstimation, XEMClusteringInput * input){

  _tabEstimation = tabEstimation;
  _nbEstimation  = nbEstimation;
  _criterionName = criterionName;

  _CVLabelOfEachEstimation = new int64_t*[_nbEstimation]; 
  _tabCriterionValueForEachEstimation = new double[_nbEstimation];
  _tabCriterionErrorForEachEstimation = new XEMErrorType[_nbEstimation];
  
  if (_criterionName == DCV){
    throw XEMDAInput;
    //TODO _nbCriterion = 1;
    //TODO _criterion = new XEMDCVCriterion(tabEstimation, nbEstimation, input);
  }
  else{
      switch (_criterionName) {
        case BIC : _criterion = new XEMBICCriterion(); break;
        case CV :  
          throw XEMDAInput;
    //_tabCriterion[i] = new XEMCVCriterion(input);  break;
        case ICL : _criterion = new XEMICLCriterion(); break;
        case NEC : _criterion = new XEMNECCriterion(); break;
				case UNKNOWN_CRITERION_NAME : throw internalMixmodError;break;
				default : throw internalMixmodError;
    }
  }
  
  _errorType      = noError;
}

XEMSelection::XEMSelection(XEMCriterionName criterionName, XEMEstimation **& tabEstimation,
                           int64_t nbEstimation, XEMOldInput * input){

  _tabEstimation = tabEstimation;
  _nbEstimation  = nbEstimation;
  _criterionName = criterionName;

  _CVLabelOfEachEstimation = new int64_t*[_nbEstimation]; 
  _tabCriterionValueForEachEstimation = new double[_nbEstimation];
  _tabCriterionErrorForEachEstimation = new XEMErrorType[_nbEstimation];
  
  if (_criterionName == DCV){
    _criterion = new XEMDCVCriterion(tabEstimation, nbEstimation, input);
  }
  else{
      switch (_criterionName) {
        case BIC : _criterion = new XEMBICCriterion(); break;
        case CV :_criterion = new XEMCVCriterion(input);  break;
        case ICL : _criterion = new XEMICLCriterion(); break;
        case NEC : _criterion = new XEMNECCriterion(); break;
        case UNKNOWN_CRITERION_NAME : throw internalMixmodError;break;
        default : throw internalMixmodError;
    }
  }
  
  _errorType      = noError;
}


//-----------
// Destructor
//-----------
XEMSelection::~XEMSelection(){
  if (_criterion) {
    delete _criterion;
    _criterion = NULL;
  }
  
  if (_tabCriterionValueForEachEstimation){
    delete [] _tabCriterionValueForEachEstimation;
  }
  if (_tabCriterionErrorForEachEstimation){
    delete [] _tabCriterionErrorForEachEstimation;
  }
}



//---------------------------
// getCVLabelOfBestEstimation
//---------------------------
int64_t * XEMSelection::getCVLabelOfBestEstimation(){
  if ((_criterionName == CV ) || (_criterionName == DCV)){
    return _CVLabelOfEachEstimation[_bestIndexEstimation];;
  }
  else{
    throw internalMixmodError;
  }
}




//---------
//set error
//---------
void XEMSelection::setErrorType(XEMErrorType errorType){
  _errorType = errorType;
}



//--------------------------------------
// getCriterionErrorType for estimation
//-------------------------------------
XEMErrorType XEMSelection::getCriterionErrorType(XEMEstimation *& estimation){
  int64_t i = 0;
  bool continu = true;
  while (i<_nbEstimation && continu){
     if (estimation == _tabEstimation[i]){
       continu = false;
     }
     else{
       i++;
     }
   }
   if (continu)
     throw internalMixmodError;

   return _tabCriterionErrorForEachEstimation[i];
}



//--------------------------------------
// getCriterionErrorType for jth estimation
//-------------------------------------
XEMErrorType XEMSelection::getCriterionErrorType(int64_t j){
  if (j<0 || j>=_nbEstimation){
    throw internalMixmodError;
  }
  else{
    return _tabCriterionErrorForEachEstimation[j];
    }

}



//---------------------------------  
// getCriterionValue for estimation
//----------------------------------
double XEMSelection::getCriterionValue(XEMEstimation *& estimation){
  int64_t i = 0;
  bool continu = true;
  while (i<_nbEstimation && continu){
     if (estimation == _tabEstimation[i]){
       continu = false;
     }
     else{
       i++;
     }
   }
   if (continu)
     throw internalMixmodError;

   return _tabCriterionValueForEachEstimation[i];
}


//----------------------
// getBestCriterionValue
//-----------------------
double XEMSelection::getBestCriterionValue(){
  double res = 0.0;
  
  if (_bestIndexEstimation >=0 && _bestIndexEstimation <_nbEstimation){
    res = _tabCriterionValueForEachEstimation[_bestIndexEstimation];
  }
  else
    throw internalMixmodError;

  return res;
}




//---
//run
//---
void XEMSelection::run(){

  //verify that all estimation have error
  int64_t nbEstimationError = 0;
  int64_t i;
  for (i=0; i<_nbEstimation; i++){
    if (_tabEstimation[i]->getErrorType() != noError){
      nbEstimationError++;
    }
  }
  if (nbEstimationError == _nbEstimation){
  _errorType = errorAllEstimation;
    throw errorAllEstimation;
  }


  //------------------
  // CV, NEC, BIC, ICL
  //------------------
  if (_criterionName != DCV){

    // printing ...  
#ifdef VERBOSE
      int64_t todo;
      switch (_criterionName) {
        case BIC : cout << "BIC |" << flush; break;
        case CV :  cout << "CV  |" << flush; break;
        case ICL : cout << "ICL |" << flush; break;
        case NEC : cout << "NEC |" << flush; break;
				case UNKNOWN_CRITERION_NAME : throw internalMixmodError; break;
				default : throw internalMixmodError;
      }
      todo = _nbEstimation;
      while(todo){
        cout << " " << flush;
        todo--;
      } 
      cout << "|" << flush;
      todo = _nbEstimation+1;
      while(todo){
        cout << "\b" << flush;
        todo--;
      } 
#endif
    
    XEMModel * model = NULL;
    // update criteria
    for (i=0; i<_nbEstimation; i++){
#ifdef VERBOSE
      printShortcutModelType(_tabEstimation[i]->getModelType());
#endif  
      if (_tabEstimation[i]->getErrorType() == noError){
        model = _tabEstimation[i]->getModel(); // the best model
         
        // Criterion : run
        try{
          if ((_criterionName==CV) & (_errorType != noSelectionError))  {
            XEMCVCriterion * cvCriterion = dynamic_cast<XEMCVCriterion*>(_criterion);
            cvCriterion->run(model, _tabCriterionValueForEachEstimation[i],   _CVLabelOfEachEstimation[i], _tabCriterionErrorForEachEstimation[i]);
          }
          else{
          _criterion->run(model, _tabCriterionValueForEachEstimation[i], _tabCriterionErrorForEachEstimation[i]);
          }
        }
        catch(XEMErrorType errorType){
          //_tabCriterion[i]->setErrorType(errorType);
          _tabCriterionErrorForEachEstimation[i] = errorType;
        }
        catch (Exception){
          //_tabCriterion[i]->setErrorType(numericError);
          _tabCriterionErrorForEachEstimation[i] = internalMixmodError;
        }
      }
      else {
        //_tabCriterion[i]->setErrorType(_tabEstimation[i]->getErrorType());
        _tabCriterionErrorForEachEstimation[i] = _tabEstimation[i]->getErrorType();
        _tabCriterionValueForEachEstimation[i] = 0;
      }
#ifdef VERBOSE
      cout << "\b-" << flush;
#endif
    }
    // Select the best Estimation
    selectBestEstimation();

  }

  
  else{
  //----
  //DCV
  //----
    
#ifdef VERBOSE
      cout << "DCV " << flush<<endl; 
#endif
    XEMDCVCriterion * dcvCriterion = (XEMDCVCriterion *) _criterion;
    // criterion->run
    //try{

      // todo : quiet en option du run
      double value = 0; // TODO a enlever
      XEMErrorType error = noError; // TODO a enlever
      dcvCriterion->run(NULL, value, error);

      for (i=0; i<_nbEstimation; i++){
        _tabCriterionValueForEachEstimation[i] = (dcvCriterion->getTabCriterionValueForEachEstimation())[i];
        _tabCriterionErrorForEachEstimation[i] = (dcvCriterion->getTabCriterionErrorForEachEstimation())[i];
      }
      _bestIndexEstimation = dcvCriterion->getBestIndexEstimation();
      
      /*for (i=0; i<_nbEstimation; i++){
        cout<<"cv value for "<<i<<"th estimation : "<<_tabCriterionValueForEachEstimation[i]<<endl;
        cout<<"cv error for "<<i<<"th estimation : "<<_tabCriterionErrorForEachEstimation[i]<<endl;
      }
      cout<<"best Index Estimatimation : "<<_bestIndexEstimation<<endl;*/
      
      if (_bestIndexEstimation == -1){
        _errorType = noSelectionError;
      }/*
    }
    catch(XEMErrorType errorType){
      _tabCriterion[0]->setErrorType(errorType);
    }
    catch (Exception){
      _tabCriterion[0]->setErrorType(numericError);
    }
    */
   //update _CVLabelOfBestEstimation : NON fait car on n'estime pas les labels de tous les individus pour la meilleur estimation (il faudrait relancer un CV si on voulait l'info)
    /*if (_criterionType==DCV){
      copyTab(_CVLabelOfBestEstimation, _tabCriterion[0]->getCVLabelOfBestEstimation(),  _nbSample);
    }*/
  }
  
  
}



//---------------------
// selectBestEstimation
//---------------------
void XEMSelection::selectBestEstimation(){

  int64_t i = 0;
  double valueMin, valueTmp;
  _bestIndexEstimation = -1;

	valueMin = 0.0; 
  // is there an criterion with no error
  while (i<_nbEstimation && _bestIndexEstimation==-1){
    if (_tabCriterionErrorForEachEstimation[i] == noError){
      _bestIndexEstimation = i;
      valueMin = _tabCriterionValueForEachEstimation[i];
    }
    i++;
  }

  if (_bestIndexEstimation == -1){
    _errorType = noSelectionError;
  }
  else{
    for (i=_bestIndexEstimation+1; i<_nbEstimation; i++){
      if (_tabCriterionErrorForEachEstimation[i] == noError){
        valueTmp = _tabCriterionValueForEachEstimation[i];
        if (valueTmp < valueMin){
          // if this Estimation is better
          valueMin = valueTmp;
          _bestIndexEstimation= i;
       	}
       	else if (valueTmp == valueMin){
          // the model with less parameter is choosen
          int64_t freeParameter = _tabEstimation[i]->getModel()->getFreeParameter();
          int64_t freeParameter2 = _tabEstimation[_bestIndexEstimation]->getModel()->getFreeParameter();
          if (freeParameter < freeParameter2){
            // if this Estimation is better	
            _bestIndexEstimation = i;
          }
        }
      }
    }
  }
}

