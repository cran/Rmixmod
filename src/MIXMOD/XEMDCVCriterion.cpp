/***************************************************************************
                             SRC/MIXMOD/XEMDCVCriterion.cpp  description
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
#include "XEMDCVCriterion.h"
#include "XEMGaussianSample.h"
#include "XEMMain.h"
#include <list>

// Defaul constructor : never used
XEMDCVCriterion::XEMDCVCriterion(){
  _tabLearningBlock = NULL;
  _tabTestBlock = NULL;
  _originalInput = NULL;
  _tabCriterionValueForEachEstimation = NULL;
  _tabCriterionErrorForEachEstimation = NULL;
  _tabDCVErrorForEachEstimation = NULL;
  _tabEstimation = NULL;
  _tabIndexOfBestEstimation = NULL;
  _tabCVValueOnBestModel = NULL;
  
  _data = NULL;
  _weight = NULL;
  _weightTotal = 0;
  _value = 0;
  
  _DCVinitBlocks = defaultDCVinitBlocks;
}


XEMDCVCriterion::XEMDCVCriterion(XEMEstimation **& tabEstimation, int64_t nbEstimation, XEMOldInput * originalInput){
  
  _value = 0;
  _originalInput = originalInput;
  _tabEstimation = tabEstimation,
  _nbEstimation  =  nbEstimation;

  _tabCriterionValueForEachEstimation = new double[_nbEstimation];
  _tabCriterionErrorForEachEstimation = new XEMErrorType[_nbEstimation];


  _DCVinitBlocks = originalInput->_DCVinitBlocks;
  _nbDCVBlock = originalInput->_numberOfDCVBlocks;
  _tabDCVErrorForEachEstimation = new XEMErrorType[_nbDCVBlock];  
  // _nbDCVBlock is initialzed to the numberOfDCVBlocks that the user wanted (issued from input)(or the default value)
  // this value could chane if nbSample is too small (will be done in createDCVBlock) 

  if (nbEstimation > 0){
    _data = tabEstimation[0]->getData();
    _weight = _data->_weight;
    _weightTotal =  _data->_weightTotal;
    if (_weightTotal-_data->_weightTotal != 0){
      throw weightTotalIsNotAnInteger;
    }
  }
  else{
    throw internalMixmodError;
  }
  
  
  _tabIndexOfBestEstimation = new int64_t [_nbDCVBlock];
  _tabCVValueOnBestModel = new double[_nbDCVBlock];
}



XEMDCVCriterion::~XEMDCVCriterion(){
  if (_tabTestBlock){
    for (int64_t v=0; v<_nbDCVBlock; v++){
      delete [] _tabTestBlock[v]._tabWeightedIndividual;
    }
      delete [] _tabTestBlock;
      _tabTestBlock = NULL;
  }
  
  if (_tabLearningBlock){
    for (int64_t v=0; v<_nbDCVBlock; v++){
      delete [] _tabLearningBlock[v]._tabWeightedIndividual;
    }
      delete [] _tabLearningBlock;
      _tabLearningBlock = NULL;
  }
  
  if (_tabIndexOfBestEstimation){
    delete [] _tabIndexOfBestEstimation;
  }
  
  if (_tabCVValueOnBestModel){
    delete [] _tabCVValueOnBestModel;
  }  
  
  if (_tabCriterionValueForEachEstimation){
    delete [] _tabCriterionValueForEachEstimation;
  }
  
  if (_tabCriterionErrorForEachEstimation){
    delete [] _tabCriterionErrorForEachEstimation;
  }

  if (_tabDCVErrorForEachEstimation){
    delete [] _tabDCVErrorForEachEstimation;
  }
}




//---------
// setModel
//---------
void XEMDCVCriterion::setModel(XEMModel * model){
  throw internalMixmodError;
}



//-----------
// getCVLabelOfBestEstimation
//-----------
/*int64_t * XEMDCVCriterion::getCVLabelOfBestEstimation(){
  int64_t i;
  int64_t nbSample = _originalInput->getNbSample();
  int64_t * res     = new int64_t[nbSample];  
  for (i=0; i<nbSample; i++)
    res[i] = _CVLabelOfBestEstimation[_bestIndexEstimation] + 1;  
  
  return res;
}*/





//---
//run
//---
void XEMDCVCriterion::run(XEMModel * model, double & value,  XEMErrorType & error){
  
  createDCVBlocks();

  XEMOldInput * inputStepV               = NULL;
  XEMSelection * selectionStepV       = NULL;
  XEMEstimation * bestEstimationStepV = NULL;
  double * tabCriterionValueForEachEstimationStepV = NULL;
  XEMErrorType * tabCriterionErrorForEachEstimationStepV = NULL;
  XEMModel * bestModelStepV           = NULL; // best model for vth block
  XEMPartition * knownPartitionForBestModelV  = NULL;
  int64_t rankOfNbClusterOfBestModelV, NbClusterOfBestModelV           = 0;
  XEMSample * curSample               = NULL;
  int64_t i;
  int64_t v, known_ki;
  int64_t * nbErrorForEachEstimation      = new int64_t[_nbEstimation];
  
  
  // initialisation of _tabCriterionValueForEachEstimation and _tabCriterionErrorForEachEstimation
  for (i=0; i<_nbEstimation; i++){
    _tabCriterionValueForEachEstimation[i] = 0;
    nbErrorForEachEstimation[i] = 0;
    _tabCriterionErrorForEachEstimation[i] = _tabEstimation[i]->getErrorType();
    //cout<<"error for "<<i<<"th estimation : "<<_tabEstimation[i]->getErrorType()<<endl;
  }

  value = 0;
  
  
  
  // For all DCVBlocks :
  //-------------------
  for(v=0; v<_nbDCVBlock; v++){
    //cout<<"DCV : block N : "<<v<<endl;
   // build inputStepV
    inputStepV = new XEMOldInput(_originalInput, _tabLearningBlock[v]);
    XEMErrorType selectionStepVError = noError;

    XEMMain xmain(inputStepV);
    selectionStepV =  (xmain.getTabSelection())[0];

    try{

      xmain.run();

    }
   
    catch(XEMErrorType errorType){
    
      //selectionStepV =  (xmain.getTabSelection())[0];
      selectionStepVError = selectionStepV->getErrorType();
      if (selectionStepVError == errorAllEstimation){
        for (i=0; i<_nbEstimation; i++){
          nbErrorForEachEstimation[i]++;
        }
      }
    }

    _tabDCVErrorForEachEstimation[v] = selectionStepVError;
    //cout<<_tabDCVErrorForEachEstimation[v]<<endl;

    if (selectionStepVError != errorAllEstimation){
    // update _tabCriterionValueForEachEstimation, _tabCriterionErroForEachEstimation 
    tabCriterionValueForEachEstimationStepV = selectionStepV->getTabCriterionValueForEachEstimation();
    tabCriterionErrorForEachEstimationStepV = selectionStepV->getTabCriterionErrorForEachEstimation();
    for (i=0; i<_nbEstimation; i++){
       //cout<<"block "<<v<<"  - estimation "<<i<<" - Error : "<<tabCriterionErrorForEachEstimationStepV[i]<<endl;
      if (tabCriterionErrorForEachEstimationStepV[i] == noError){
        //cout<<_tabCriterionValueForEachEstimation[i]<<endl;
        _tabCriterionValueForEachEstimation[i] += tabCriterionValueForEachEstimationStepV[i];
      }
      else{
        nbErrorForEachEstimation[i]++;
      }
    }
    //compute DCV value with the vth testBlock
    //----------------------------------------

    _tabIndexOfBestEstimation[v] = selectionStepV->getBestIndexEstimation();


    if ( _tabDCVErrorForEachEstimation[v] == noError){
      bestEstimationStepV = _tabEstimation[_tabIndexOfBestEstimation[v]];
    //bestModelTypeStepV = bestEstimationStepV->getModelType();
      bestModelStepV = bestEstimationStepV->getModel(); // the best model
      NbClusterOfBestModelV = bestModelStepV->getNbCluster();
    // search rankOfNbClusterOfBestModelV
      for (rankOfNbClusterOfBestModelV=0; rankOfNbClusterOfBestModelV<_originalInput->_nbNbCluster; rankOfNbClusterOfBestModelV++){
      if (NbClusterOfBestModelV == _originalInput->_tabNbCluster[rankOfNbClusterOfBestModelV]) break;
      }
      knownPartitionForBestModelV = _originalInput->_tabKnownPartition[rankOfNbClusterOfBestModelV];
      _tabCVValueOnBestModel[v] = 0;


      for (int64_t ii=0; ii<_tabTestBlock[v]._nbSample; ii++){
        i = _tabTestBlock[v]._tabWeightedIndividual[ii].val;  // i : ith sample in the original data

        curSample = _data->_matrix[i];
        int64_t computedLabelForCurSampleOnBestModelStepV = bestModelStepV->computeLabel(curSample);
        known_ki = knownPartitionForBestModelV->getGroupNumber(i);

      // cout<<"known label : "<<known_ki<<" - computed label : "<<computedLabelForCurSampleOnBestModelStepV<<endl;

        if (known_ki != computedLabelForCurSampleOnBestModelStepV){
          _tabCVValueOnBestModel[v] += _tabTestBlock[v]._tabWeightedIndividual[ii].weight;  // original
        }
      }// for ii

      _tabCVValueOnBestModel[v] /= _tabTestBlock[v]._weightTotal;
      value += _tabCVValueOnBestModel[v];

#ifdef VERBOSE
    // printing
        cout << "DCV block "<< flush;
        printf("%4d", (int)v);
        cout << " (n=" << _tabLearningBlock[v]._nbSample <<" - poids="<<_tabLearningBlock[v]._weightTotal<<") : "<< flush;
        cout << " k=" << NbClusterOfBestModelV << flush;
        cout << ", model: " << flush;
        printModelType(_tabEstimation[_tabIndexOfBestEstimation[v]]->getModelType());

      //for (i=0 ; i<_nbEstimation; i++){
      //   cout<<"CV error rate for "<<i+1<<"th Estimation : "<<tabCriterionValueForEachEstimationStepV[i]<<endl;
      //}
        cout << ", error rate (DCV)  : " << flush;
        if(_tabCVValueOnBestModel[v]==0.0)
          cout << "0  " << flush;
        else{
          printf("%02.02f", 100*_tabCVValueOnBestModel[v] );
          cout << " %" << flush;
        }
        cout << endl;
      // end printing
#endif
     }
    }

    delete inputStepV;
    //}
  }  // for v





  value = value / _nbDCVBlock;
  _value = value;

#ifdef VERBOSE
  cout<<"DCV average error rate : "<<value<<endl;
#endif
  // update _tabCriterionValueForEachEstimation, _tabCriterionErroForEachEstimation and _bestIndexEstimation
  int64_t nbError = 0;
   for (i=0; i<_nbEstimation; i++){
   //cout<<"nbError for "<<i<<"th estimation : "<<nbErrorForEachEstimation[i]<<endl;
    if (nbErrorForEachEstimation[i] != _nbDCVBlock){
      _tabCriterionValueForEachEstimation[i] /= (_nbDCVBlock - nbErrorForEachEstimation[i]);
 
  //       cout<<endl<<"CV value for "<<i<<"th estimation : "<<_tabCriterionValueForEachEstimation[i];
    }
    else{
      if (_tabCriterionErrorForEachEstimation[i] == noError){
        _tabCriterionErrorForEachEstimation[i] = allCVCriterionErrorForAnEstimationInDCVContext;  
      }
      else{
      //on laisse l'erreur (de l'etape estimation)  
      }
      nbError++;
    }
  }

  // update _bestIndexEstimation
  if (nbError == _nbEstimation){
    _bestIndexEstimation = -1;
  }
  else{
    _bestIndexEstimation = 0;
    for (i=1; i<_nbEstimation; i++){
      if (_tabCriterionErrorForEachEstimation[i] == noError){
        // _tabCriterionValueForEachEstimation[i] a un sens
        if (_tabCriterionValueForEachEstimation[i] < _tabCriterionValueForEachEstimation[_bestIndexEstimation]){
          _bestIndexEstimation = i;
        }
      }
    }
  }
  delete [] nbErrorForEachEstimation;

}




//-------------------
//- CreateDCVBlock
//-------------------
void XEMDCVCriterion::createDCVBlocks(){
  int64_t i, index, nbSample = _data->_nbSample;
 
  // each learning block must contains at least 10 individuals
  if (_nbDCVBlock *10 >= _weightTotal){
    _nbDCVBlock = _weightTotal / 10;
  }
  // _nbDCVBlock must be > 1
  if (_nbDCVBlock <= 1)
    throw NbDCVBlocksTooSmall;
  //cout<<"_nbDCVBlock : "<<_nbDCVBlock<<endl;
  
  _tabLearningBlock = new XEMCVBlock[_nbDCVBlock];
  _tabTestBlock = new XEMCVBlock[_nbDCVBlock];
        
  int64_t sizeTestList = 0;
  int64_t sizeLearningList = 0;
  double sumWeight = 0.0;
  int64_t value;
   
  // DCV_RANDOM  
  if (_DCVinitBlocks == DCV_RANDOM) {
    //cout<<"DCV_RANDOM"<<endl;
   // random 
    double * tabRandom = new double[_weightTotal];
    int64_t * tabIndex    = new int64_t[_weightTotal];
    index = 0;
    for(i=0; i<nbSample; i++){
      sumWeight = 0.0;
      while (sumWeight< _weight[i]){
        tabRandom[index] = rnd();
        //cout<<"XEMDCVCriterion tabRandom["<<index<<"]  "<<tabRandom[index]<<endl;
        tabIndex[index]  = i;
        sumWeight ++;
        index ++;
      }
    }
    if (index != _weightTotal){
        //cout<<"cout erreur 2 ds XEMDCVCriterion"<<endl;
      throw internalMixmodError;
    }
    quickSortWithOrder(tabRandom, tabIndex, 0, (_weightTotal)-1);
    
    int64_t nbElt = (int64_t )( floor( ((double)(_weightTotal)) / (double)(_nbDCVBlock) )) ; 
    int64_t remaining = _weightTotal - nbElt*_nbDCVBlock;
    int64_t firstIndexOfTestBlock = 0;
    int64_t lastIndexOfTestBlock = 0;
    
    for (int64_t v=0; v<_nbDCVBlock; v++){
      int64_t weightTotalInTestBlock = nbElt;
        // Lists
      list<TWeightedIndividual*>  listTestBlock;
      list<TWeightedIndividual*>  listLearningBlock;
      list<TWeightedIndividual*>::iterator listIterator;
      list<TWeightedIndividual*>::iterator listBegin;
      list<TWeightedIndividual*>::iterator listEnd;
      sizeTestList = 0;
      sizeLearningList = 0;
        
      if (remaining > 0){
        weightTotalInTestBlock++ ;
        remaining--;
      }
           
      lastIndexOfTestBlock = firstIndexOfTestBlock + weightTotalInTestBlock -1 ;
      for (i=0; i<_weightTotal; i++){
        //cout<<"individu : "<<tabIndex[i];
        if ((i >= firstIndexOfTestBlock ) && (i <= lastIndexOfTestBlock)){
          // add individual tabIndex[i] in listTestBlock[v]
          //---------------------------------------------
          //cout<<" dans listTestBlock"<<endl;
          value = tabIndex[i];
          
          // Search if sample already exist in list //
          listBegin = listTestBlock.begin();
          listEnd   = listTestBlock.end();
          listIterator = listBegin;    
          while ( (listIterator != listEnd) && (value > (*listIterator)->val)){
            listIterator++;
          }
          // list empty //  
          if (listBegin==listEnd){
            TWeightedIndividual * elem = new TWeightedIndividual;
            elem->val = value;
            elem->weight = 1;
            listTestBlock.push_front(elem);
            sizeTestList++;      
          }
          else{
            // if elemn in end of list //
            if (listIterator==listEnd){
              listIterator--;
              if ((*listIterator)->val==value)
                (*listIterator)->weight += 1;
              else{
                TWeightedIndividual * elem = new TWeightedIndividual;
                elem->val = value;
                elem->weight = 1;
                listTestBlock.push_back(elem);
                sizeTestList++;
              }
            }    
            // elemen in begin or in middle of list //
            else{
              if ((*listIterator)->val==value)
                (*listIterator)->weight += 1;
              else{
                TWeightedIndividual * elem = new TWeightedIndividual;
                elem->val = value;
                elem->weight = 1;

                if (listIterator == listTestBlock.begin())
                  listTestBlock.push_front(elem);
                else
                  listTestBlock.insert(listIterator,1,elem);

                sizeTestList++;
              }
            }
          }
        
        }
        else{
          // add individual tabIndex[i] in listLearningBlock[v]
          //---------------------------------------------------
          //cout<<" dans listLearningBlock"<<endl;
          value = tabIndex[i];
          
          // Search if sample already exist in list //
          listBegin = listLearningBlock.begin();
          listEnd   = listLearningBlock.end();
          listIterator = listBegin;   
          while ( (listIterator != listEnd) && (value > (*listIterator)->val)){
            listIterator++;
          }
          // list empty //  
          if (listBegin==listEnd){
            TWeightedIndividual * elem = new TWeightedIndividual;
            elem->val = value;
            elem->weight = 1;
            listLearningBlock.push_front(elem);
            sizeLearningList++;      
          }
          else{
            // if elemn in end of list //
            if (listIterator==listEnd){
              listIterator--;
              if ((*listIterator)->val==value)
                (*listIterator)->weight += 1;
              else{
                TWeightedIndividual * elem = new TWeightedIndividual;
                elem->val = value;
                elem->weight = 1;
                listLearningBlock.push_back(elem);
                sizeLearningList++;
              }
            }    
            // elemen in begin or in middle of list //
            else{
              if ((*listIterator)->val==value)
                (*listIterator)->weight += 1;
              else{
                TWeightedIndividual * elem = new TWeightedIndividual;
                elem->val = value;
                elem->weight = 1;

                if (listIterator == listLearningBlock.begin())
                  listLearningBlock.push_front(elem);
                else
                  listLearningBlock.insert(listIterator,1,elem);

                sizeLearningList++;
              }
            }
          }
        
        }
      } // end for i
      
      firstIndexOfTestBlock = lastIndexOfTestBlock + 1; 
         
      // update _tabTestBlock
      //---------------------
      _tabTestBlock[v]._nbSample = sizeTestList;
      _tabTestBlock[v]._weightTotal = weightTotalInTestBlock;
      _tabTestBlock[v]._tabWeightedIndividual = new TWeightedIndividual[_tabTestBlock[v]._nbSample];
        
      listBegin = listTestBlock.begin();
      listEnd   = listTestBlock.end();
      listIterator = listBegin;    
      i=0;
      while ( (listIterator != listEnd) && i<_tabTestBlock[v]._nbSample){
        _tabTestBlock[v]._tabWeightedIndividual[i].val =  (*listIterator)->val;
        _tabTestBlock[v]._tabWeightedIndividual[i].weight =  (*listIterator)->weight;
        listIterator++;
        i++;
      }
      if (i != _tabTestBlock[v]._nbSample){
          //cout<<"cout erreur 1 ds XEMDCVCriterion"<<endl;
        throw internalMixmodError;
      }
      
      // update _tabLearningBlock
      //---------------------------
      _tabLearningBlock[v]._nbSample = sizeLearningList;
      _tabLearningBlock[v]._weightTotal = _weightTotal - weightTotalInTestBlock;
      _tabLearningBlock[v]._tabWeightedIndividual = new TWeightedIndividual[_tabLearningBlock[v]._nbSample];
        
      listBegin = listLearningBlock.begin();
      listEnd   = listLearningBlock.end();
      listIterator = listBegin;    
      i=0;
      while ( (listIterator != listEnd) && i<_tabLearningBlock[v]._nbSample){
        _tabLearningBlock[v]._tabWeightedIndividual[i].val =  (*listIterator)->val;
        _tabLearningBlock[v]._tabWeightedIndividual[i].weight =  (*listIterator)->weight;
        listIterator++;
        i++;
      }
      if (i != _tabLearningBlock[v]._nbSample){
          //cout<<"cout erreur 1 ds XEMDCVCriterion"<<endl;
        throw internalMixmodError;
      }
        while(! listTestBlock.empty()){
          TWeightedIndividual * pelem = * listTestBlock.begin();
          listTestBlock.pop_front();
          delete pelem;
        }

        while(! listLearningBlock.empty()){
          TWeightedIndividual * pelem = * listLearningBlock.begin();
          listLearningBlock.pop_front();
          delete pelem;
        }

    } // end for v

  delete[] tabIndex;
  delete[] tabRandom;
  }
  
  
    // DCV_DIAG
    //---------
  else{
    // DCV_DIAG 
    //---------
    //cout<<"DCV_DIAG"<<endl;
    
      // Lists
    list<TWeightedIndividual*>   * listLearningBlock = new list<TWeightedIndividual*> [_nbDCVBlock];
    list<TWeightedIndividual*>   * listTestBlock = new list<TWeightedIndividual*> [_nbDCVBlock];
      
    list<TWeightedIndividual*>::iterator listIterator;
    list<TWeightedIndividual*>::iterator listBegin;
    list<TWeightedIndividual*>::iterator listEnd;
         
    int64_t v = 0;
    int64_t i, w, nbTraited = 0;
    
    // Update listTestBlock[v]
    for (i=0; i<nbSample; i++){
      for (w=0; w<_weight[i]; w++){
        // add i in listTestBlock[v]
        //--------------------------
        listBegin = listTestBlock[v].begin();
        listEnd   = listTestBlock[v].end();
        listIterator = listBegin;   
        while ( (listIterator != listEnd) && (i > (*listIterator)->val)){
          listIterator++;
        }
        // list empty //  
        if (listBegin==listEnd){
          TWeightedIndividual * elem = new TWeightedIndividual;
          elem->val = i;
          elem->weight = 1;
          listTestBlock[v].push_front(elem);
        }
        else{
          // if elemn in end of list //
          if (listIterator==listEnd){
            listIterator--;
            if ((*listIterator)->val==i)
              (*listIterator)->weight += 1;
            else{
              TWeightedIndividual * elem = new TWeightedIndividual;
              elem->val = i;
              elem->weight = 1;
              listTestBlock[v].push_back(elem);
            }
          }    
          // elemen in begin or in middle of list //
          else{
            if ((*listIterator)->val==i)
              (*listIterator)->weight += 1;
            else{
              TWeightedIndividual * elem = new TWeightedIndividual;
              elem->val = i;
              elem->weight = 1;

              if (listIterator == listTestBlock[v].begin())
                listTestBlock[v].push_front(elem);
              else
                listTestBlock[v].insert(listIterator,1,elem);
            }
          }  
          
        } 
          
        v++;
        if (v == _nbDCVBlock){
          v = 0;
        }
        nbTraited++;   
      }
    }
    if (nbTraited != _weightTotal){
        //cout<<"cout erreur 1 ds XEMCVCriterion"<<endl;
      throw internalMixmodError;
    }
      
    // update _tabTestBlock
    //---------------------
    for (v=0; v<_nbDCVBlock; v++){
      /*cout<<"tabTestBlock["<<v<<"]"<<endl;
      cout<<"---------------"<<endl;*/
      _tabTestBlock[v]._nbSample = listTestBlock[v].size();
      _tabTestBlock[v]._tabWeightedIndividual = new TWeightedIndividual[_tabTestBlock[v]._nbSample];
      double weightTotalInTestBlockV = 0;
        
      listBegin = listTestBlock[v].begin();
      listEnd   = listTestBlock[v].end();
      listIterator = listBegin;     
      i=0;
      while ( (listIterator != listEnd) && i<_tabTestBlock[v]._nbSample){
        //cout<<"listTest : indiv : "<<(*listIterator)->val<<" - poids : "<<(*listIterator)->weight<<endl;
        _tabTestBlock[v]._tabWeightedIndividual[i].val =  (*listIterator)->val;
        _tabTestBlock[v]._tabWeightedIndividual[i].weight =  (*listIterator)->weight;
        weightTotalInTestBlockV += _tabTestBlock[v]._tabWeightedIndividual[i].weight;
        listIterator++;
        i++;
      }
      _tabTestBlock[v]._weightTotal = weightTotalInTestBlockV;
      if (i != _tabTestBlock[v]._nbSample){
          //cout<<"cout erreur 2 ds XEMCVCriterion"<<endl;
        throw internalMixmodError;
      }
    }
      
    
    
    // Update listLearningBlock[v] (complementary of listTestBlock[v])
    //---------------------------------------------------------------
    double weightOfCurrentIndividualInCurrentLearningBlock = 0;
    for (v=0; v<_nbDCVBlock; v++){
      //cout<<"Mise a jour de listLearningBlock["<<v<<"]"<<endl;
      for (i=0; i<nbSample; i++){
        // is i in listTestBlock[v] ?
        //cout<<"traitement de l'individu "<<i<<endl;
        listBegin = listTestBlock[v].begin();
        listEnd   = listTestBlock[v].end();
        listIterator = listBegin;    
        /*while ( (listIterator != listEnd) && (i > (*listIterator)->val)){
          cout<<"  ds while : "<<(*listIterator)->val<<endl;
          listIterator++;
          cout<<"   apres listIterator++ "<<endl;
          if (listIterator==listEnd){
            cout<<"ListeEnd"<<endl;
          }
          else{
            cout<<"pas encore ListeEnd"<<endl;
          }
          cout<<"     "<<(*listIterator)->val<<endl;
        }
        */
        bool search=true;
        while ((listIterator != listEnd) && search){
          if (i <= (*listIterator)->val){
            search=false;
          }
          else{
            listIterator++;
          }
        }
        
        if (listIterator==listEnd){
          // i is not in listTestList[v]
          weightOfCurrentIndividualInCurrentLearningBlock = _weight[i];
        }
        else{
          // search=false
          if (i == (*listIterator)->val){
            // i is in listTestList[v]
            weightOfCurrentIndividualInCurrentLearningBlock = _weight[i] - (*listIterator)->weight ;
          }
          else{
          // i is not in listTestList[v]
            weightOfCurrentIndividualInCurrentLearningBlock = _weight[i];
          }
        }
                
        if (weightOfCurrentIndividualInCurrentLearningBlock != 0){
          // add i in listLearningBlock
          TWeightedIndividual * elem = new TWeightedIndividual;
          elem->val = i;
          elem->weight = weightOfCurrentIndividualInCurrentLearningBlock;
          listLearningBlock[v].push_back(elem);
        }  
          
      }// end for i
    }// end for v     
    
    // update _tabLearningBlock
    //-------------------------
    for (v=0; v<_nbDCVBlock; v++){
      _tabLearningBlock[v]._nbSample = listLearningBlock[v].size();
      _tabLearningBlock[v]._tabWeightedIndividual = new TWeightedIndividual[_tabLearningBlock[v]._nbSample];
      double weightTotalInTestBlockV = 0;
        
      listBegin = listLearningBlock[v].begin();
      listEnd   = listLearningBlock[v].end();
      listIterator = listBegin;     
      i=0;
      while ( (listIterator != listEnd) && i<_tabLearningBlock[v]._nbSample){
        /*cout<<"listLearning : indiv : "<<(*listIterator)->val<<" - poids : "<<(*listIterator)->weight<<endl;*/
        _tabLearningBlock[v]._tabWeightedIndividual[i].val =  (*listIterator)->val;
        _tabLearningBlock[v]._tabWeightedIndividual[i].weight =  (*listIterator)->weight;
        weightTotalInTestBlockV += _tabLearningBlock[v]._tabWeightedIndividual[i].weight;
        listIterator++;
        i++;
      }
      _tabLearningBlock[v]._weightTotal = weightTotalInTestBlockV;
      if (i != _tabLearningBlock[v]._nbSample){
          //cout<<"cout erreur 2 ds XEMCVCriterion"<<endl;
        throw internalMixmodError;
      }
    }

     for (v=0; v<_nbDCVBlock; v++){
         while(! listTestBlock[v].empty()){
          TWeightedIndividual * pelem = * listTestBlock[v].begin();
          listTestBlock[v].pop_front();
          delete pelem;
        }
    }

     for (v=0; v<_nbDCVBlock; v++){
         while(! listLearningBlock[v].empty()){
          TWeightedIndividual * pelem = * listLearningBlock[v].begin();
          listLearningBlock[v].pop_front();
          delete pelem;
        }
    }


    delete [] listLearningBlock;
    delete [] listTestBlock;
  }   
    
  
  /*cout<<"XEMDCVCriterion   "<<_nbDCVBlock<<endl;
  
  
    for (int64_t v=0; v<_nbDCVBlock; v++){
      cout<<endl<<"bloc Learning n"<<v<<" - nb d'individus : "<<_tabLearningBlock[v]._nbSample<<" - poids total : "<<_tabLearningBlock[v]._weightTotal<<endl;
  for (int64_t i=0; i<_tabLearningBlock[v]._nbSample; i++){
        cout<<"ind : "<<_tabLearningBlock[v]._tabWeightedIndividual[i].val<<" de poids : "<<_tabLearningBlock[v]._tabWeightedIndividual[i].weight<<endl;
      }
      cout<<endl<<"bloc test n"<<v<<" - nb d'individus : "<<_tabTestBlock[v]._nbSample<<" -  poids total : "<<_tabTestBlock[v]._weightTotal<<endl;
  for (int64_t i=0; i<_tabTestBlock[v]._nbSample; i++){
        cout<<"ind : "<<_tabTestBlock[v]._tabWeightedIndividual[i].val<<" de poids : "<<_tabTestBlock[v]._tabWeightedIndividual[i].weight<<endl;
      }
    }*/
  
 
}




//--------------------------
// edit
//--------------------------
void XEMDCVCriterion::edit(ofstream & fi){
  fi << "----------------------------------------------------------\n"
     << "|        MIXMOD DOUBLE CROSS VALIDATION OUTPUT           |\n"
     << "----------------------------------------------------------\n\n"
     << "number of blocks          : " << _nbDCVBlock << "\n" 
     << "average error rate        : " << _value << "\n"
     //<< "error rate standard error : " << _stderrvalue << "\n\n" 
     << endl;
     
  int64_t v;   
  for(v=0; v<_nbDCVBlock ; v++){
  if (_tabDCVErrorForEachEstimation[v]==noError){
    fi << "----------------------------------------------------\n::::: block " << v << endl ;    
    fi << "number of learning samples : " << _tabLearningBlock[v]._nbSample<< "\n"
       << "number of test samples     : " << _tabTestBlock[v]._nbSample << endl ;
    fi << "selected model             : " << flush;
    printModelType(_tabEstimation[_tabIndexOfBestEstimation[v]]->getModelType(), fi); 
    fi << "\n"<<"selected nbCluster         : " << flush; 
    fi <<_tabEstimation[_tabIndexOfBestEstimation[v]]->getNbCluster() ;
    fi << "\n" 
       << "error rate on test samples : " << _tabCVValueOnBestModel[v] << "\n" << endl;
    fi << "----------------------------------------------------" << endl;
  }
  //fi << "----------------------------------------------------" << endl;
  }
}



//--------------------------
// editNumeric
//--------------------------
void XEMDCVCriterion::editNumeric(ofstream & fi){
  //number of blocks
  fi <<_nbDCVBlock << "\n";
  
  //average error rate  
  fi<< _value << "\n";

  int64_t v;   
  fi<<endl;
  for(v=0; v<_nbDCVBlock ; v++){
    if (_tabDCVErrorForEachEstimation[v]==noError){
    //block number 
    fi << v << endl ;
  
    //number of learning samples 
    fi<< _tabLearningBlock[v]._nbSample<< "\n";
  
    //number of test samples 
    fi<< _tabTestBlock[v]._nbSample << endl ;
  
    //selected estimation 
    fi <<  _tabIndexOfBestEstimation[v]<< "\n";
  
    // error rate on test samples 
    fi << _tabCVValueOnBestModel[v] << "\n" << endl;
  }
  }
}




