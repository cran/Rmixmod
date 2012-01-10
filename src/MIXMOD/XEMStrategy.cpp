/***************************************************************************
                             SRC/MIXMOD/XEMStrategy.cpp  description
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

#include "XEMStrategy.h"
#include "XEMAlgo.h"
#include "XEMMAPAlgo.h"
#include "XEMEMAlgo.h"
#include "XEMCEMAlgo.h"
#include "XEMSEMAlgo.h"
#include "XEMMAlgo.h"
#include "XEMUtil.h"
#include "XEMGaussianParameter.h"
#include "XEMGaussianSphericalParameter.h"
#include "XEMGaussianDiagParameter.h"
#include "XEMGaussianGeneralParameter.h"
#include "XEMGaussianHDDAParameter.h"
#include "XEMBinaryEkjhParameter.h"
#include "XEMBinaryData.h"
#include "XEMUtil.h"


//-----------
//Constructor
//-----------
XEMStrategy::XEMStrategy(){
  _nbTry = defaultNbTryInStrategy;
  _strategyInit = new XEMStrategyInit();
  _nbAlgo  = defaultNbAlgo;
  _tabAlgo = new XEMAlgo * [_nbAlgo];
  for (int64_t i=0; i<_nbAlgo; i++){
    _tabAlgo[i] = createDefaultAlgo();
  }
}


XEMStrategy::XEMStrategy(const XEMStrategy & strategy){
  _nbTry = strategy.getNbTry();
  _strategyInit = new XEMStrategyInit(*(strategy.getStrategyInit()));
  _nbAlgo = strategy.getNbAlgo();
  _tabAlgo = new XEMAlgo * [_nbAlgo];
  XEMAlgo ** tabA = strategy.getTabAlgo();
  for (int64_t i=0; i<_nbAlgo; i++){
    _tabAlgo[i] = tabA[i]->clone();
  }
  
}



//------------
// Constructor (used in DCV)
//------------
XEMStrategy::XEMStrategy(XEMStrategy * originalStrategy, XEMCVBlock & block){
  _nbAlgo = originalStrategy->_nbAlgo ;
  _nbTry  = originalStrategy->_nbTry ;
  _tabAlgo= new XEMAlgo*[_nbAlgo];
  XEMAlgo ** oTabAlgo = originalStrategy->_tabAlgo ;
  for (int64_t i=0; i<_nbAlgo; i++){
    _tabAlgo[i] = oTabAlgo[i]->clone();
  }
  _strategyInit  = new XEMStrategyInit(originalStrategy->_strategyInit, block) ;
}



//----------
//Destructor
//----------
XEMStrategy::~XEMStrategy(){

  if (_tabAlgo){
    for (int64_t i=0; i<_nbAlgo ;i++){
      delete _tabAlgo[i];
      _tabAlgo[i] = NULL;
    }
    delete [] _tabAlgo;
    _tabAlgo = NULL;
  }
}



void XEMStrategy::setAlgoEpsilon(int64_t position, double epsilonValue){
  _tabAlgo[position]->setEpsilon(epsilonValue);
}

// setAlgoStopRuleTypeValue
void XEMStrategy::setAlgoStopRule(XEMAlgoStopName stopName, int64_t position){
  _tabAlgo[position]->setAlgoStopName(stopName);
}

void XEMStrategy::setAlgoIteration( int64_t position, int64_t nbIterationValue){
  _tabAlgo[position]->setNbIteration(nbIterationValue);
}


// setAlgo
void XEMStrategy::setAlgo(XEMAlgoName algoName, int64_t position){
  if (_tabAlgo[position] != NULL){
    delete _tabAlgo[position];
  }
  switch (algoName) {
    case M :
      _tabAlgo[position] = new XEMMAlgo();
      break;
    case MAP :
      _tabAlgo[position] = new XEMMAPAlgo();
      break;
    case EM :
      _tabAlgo[position] = new XEMEMAlgo();
      break;
    case CEM :
      _tabAlgo[position] = new XEMCEMAlgo();
      break;
    case SEM :
      _tabAlgo[position] = new XEMSEMAlgo();
      break;
    default :
      throw internalMixmodError;
  }
}



// set init parameter
void XEMStrategy::setInitParam(string & paramFileName, int64_t position){
  _strategyInit->setInitParam(paramFileName, position);
}



// set init partition
void XEMStrategy::setInitPartition(string & partitionFileName, int64_t position){
  _strategyInit->setPartition(partitionFileName, position);
}


// set init partition
void XEMStrategy::setInitPartition(XEMPartition * part, int64_t position){
  _strategyInit->setPartition(part, position);
}


// removeAlgo
void XEMStrategy::removeAlgo(int64_t position){
  XEMAlgo ** tabAlgo = new XEMAlgo*[_nbAlgo-1];
  recopyTab<XEMAlgo*>(_tabAlgo, tabAlgo, position);
  for (int64_t i=position ; i<(_nbAlgo-1);i++){
    tabAlgo[i] = _tabAlgo[i+1];
  }
  _nbAlgo--;

  delete [] _tabAlgo;
  _tabAlgo = NULL;
  _tabAlgo = tabAlgo;

}

//--------------------
// setStrategyInit
//--------------------
void XEMStrategy::setStrategyInit(XEMStrategyInit * iStrategyInit){
  _strategyInit = iStrategyInit; //copy constructor
}


void XEMStrategy::setStrategyInit(XEMStrategyInitName initName, XEMData *& data, int64_t nbNbCluster, int64_t * tabNbCluster, XEMModelType ** tabModelType){
  int64_t nbSample    = data->_nbSample;
  int64_t pbDimension  = data->_pbDimension;
  string fileName = "";
  XEMParameter ** tabInitParameter = NULL;
  XEMPartition ** tabInitPartition = NULL;

  switch (initName){
    case RANDOM :
    case CEM_INIT :
    case SEM_MAX :
    case SMALL_EM :
      _strategyInit->setStrategyInitName(initName);
      break;
      
    case  USER :
      _strategyInit->setStrategyInitName(initName);
      tabInitParameter = new XEMParameter * [nbNbCluster];

      for (int64_t k=0; k<nbNbCluster; k++){
        if (isEDDA(tabModelType[0]->_nameModel)){
          tabInitParameter[k] = new XEMGaussianGeneralParameter(tabNbCluster[k], pbDimension, tabModelType[0], fileName);
        }
        else if (isBinary(tabModelType[0]->_nameModel)){
          int64_t * tabNbModality = ((XEMBinaryData*)data)->getTabNbModality();
          tabInitParameter[k] = new XEMBinaryEkjhParameter(tabNbCluster[k], pbDimension, tabModelType[0], tabNbModality, fileName);
        }
        else if (isHD(tabModelType[0]->_nameModel)){ 
          tabInitParameter[k] = new XEMGaussianHDDAParameter(tabNbCluster[k], pbDimension, tabModelType[0], fileName);
        }
        else throw internalMixmodError;
      }
      _strategyInit->setTabInitParameter(tabInitParameter, nbNbCluster);
      break;
      
    case USER_PARTITION :
      _strategyInit->setStrategyInitName(initName);
      tabInitPartition = new XEMPartition * [nbNbCluster];
      for (int64_t k=0; k<nbNbCluster; k++){
        XEMNumericPartitionFile partitionFile;
        partitionFile._fileName = fileName;
        partitionFile._format = FormatNumeric::defaultFormatNumericFile;
        partitionFile._type = TypePartition::defaultTypePartition;
        tabInitPartition[k] = new XEMPartition(nbSample, tabNbCluster[k], partitionFile);
      }
      _strategyInit->setTabPartition(tabInitPartition, nbNbCluster);
      break;
  }
}

//-------------
// setNbTry
//-------------
void XEMStrategy::setNbTry(int64_t nbTry){
  if ((_strategyInit->getStrategyInitName() == USER) ||
(_strategyInit->getStrategyInitName() == USER_PARTITION)){
    throw badSetNbTry;
  }
  if (nbTry<minNbTryInStrategy){
    throw nbTryInStrategyTooSmall;
  }
  else
    if (nbTry>maxNbTryInStrategy){
      throw nbTryInStrategyTooLarge;
    }
    else{
      _nbTry = nbTry;
    }
}






//---
//run
//---
void XEMStrategy::run(XEMModel *& model){ 
  //cout<<"XEMStrategy Init, nbTry="<<_nbTry<<endl;
  int64_t i;
  if (_nbTry == 1){
    oneTry(model);
  }
  else{
    XEMModel * currentModel = new XEMModel(model);
    oneTry(currentModel);
    XEMModel * bestModel = new XEMModel(currentModel);
    double bestLLorCLL = currentModel->getCompletedLogLikelihoodOrLogLikelihood();
    // others tries
    for (i=1; i<_nbTry; i++){
      delete currentModel;
      currentModel = new XEMModel(model);
      oneTry(currentModel);
      double lastLLorCLL = currentModel->getCompletedLogLikelihoodOrLogLikelihood();
      if (lastLLorCLL > bestLLorCLL){
        delete bestModel;
        bestModel = new XEMModel(currentModel);
        bestLLorCLL = currentModel->getCompletedLogLikelihoodOrLogLikelihood();
      }
    }
    //cout<<"fin de XEMStrategy Init, nb d'essais effectues="<<i<<endl<<endl;
    delete currentModel;
    delete model;
    model = bestModel;
  }
}



//------
//oneTry
//------
void XEMStrategy::oneTry(XEMModel *& model){
  if (isMAlgo()){
    // 1rst step of Discriminant analysis : USER_PARTITION with complete partition and M algorithm : only M step is done
    _tabAlgo[0]->run(model);
  }
  else{
    //init model
    switch (_strategyInit->getStrategyInitName()){
      case RANDOM :
        model->initRANDOM(_strategyInit->getNbTry()); 
        break;
        
      case USER :{ // get initPartition
        int64_t nbCluster = model->getNbCluster();
        int64_t index=0;
          bool ok = false;
          int64_t nbInitParameter = _strategyInit->getNbInitParameter();
          while (ok==false && index<nbInitParameter){
            int64_t nbClusterOfInitParameter = _strategyInit->getInitParameter(index)->getNbCluster();
            if (nbCluster==nbClusterOfInitParameter){
              ok = true;
            }
            else{
              index++;
            }
          }
          if (!ok)
            throw internalMixmodError;
          XEMParameter * initParameter =_strategyInit->getInitParameter(index);
          model->initUSER(initParameter);
        }
        break;
        
      case USER_PARTITION :{
        // get initPartition
        int64_t nbCluster = model->getNbCluster();
        int64_t index=0;
          bool ok = false;
          int64_t nbPartition = _strategyInit->getNbPartition();
          while (ok==false && index<nbPartition){
            int64_t nbClusterOfInitPartition = _strategyInit->getPartition(index)->_nbCluster;
            if (nbCluster == nbClusterOfInitPartition){
              ok = true;
            }
            else{
              index++;
            }
          }
          if (!ok)
            throw internalMixmodError;
          XEMPartition * initPartition = _strategyInit->getPartition(index);
          int64_t nbTyInInit = _strategyInit->getNbTry();
          model->initUSER_PARTITION(initPartition, nbTyInInit); 
        }
        break;
        
      case SMALL_EM :
        model->initSMALL_EM(_strategyInit); break;
        
      case CEM_INIT :
        model->initCEM_INIT(_strategyInit); break;
        
      case SEM_MAX :
        model->initSEM_MAX(_strategyInit); break;
        
      default :
#ifdef DEBUG
        cout << "XEMAlgo Error: Strategy Initialization Type Unknown";
#endif
        break;
    }
    
    model->setAlgoName(UNKNOWN_ALGO_NAME);

 //model->getParameter()->edit();
 
#if DEBUG > 0
    cout<<"After initialization :"<<endl;
    model->editDebugInformation();
#endif
    
    // runs algos
    _tabAlgo[0]->run(model);
    for (int64_t i=1; i<_nbAlgo ;i++){
      _tabAlgo[i]->run(model);
    }
  }
}




bool XEMStrategy::isMAlgo(){
  bool res = false;
  if (_tabAlgo[0]->getAlgoName() == M){
    res = true;
  }
  return res;
}



bool XEMStrategy::isMAPAlgo(){
  bool res = false;
  if (_tabAlgo[0]->getAlgoName() == MAP){
    res = true;
  }
  return res;
}



// insert algo
void XEMStrategy::insertAlgo(XEMAlgo * algo,int64_t position){
  XEMAlgo ** tabAlgo = new XEMAlgo*[_nbAlgo+1];
  recopyTab<XEMAlgo*>(_tabAlgo, tabAlgo, position);
  tabAlgo[position] = algo;

  for (int64_t k=position; k<_nbAlgo;++k){
    tabAlgo[k+1] = _tabAlgo[k];
  }

  _nbAlgo++;

  delete [] _tabAlgo;
  _tabAlgo = NULL;
  _tabAlgo = tabAlgo;
}




//-------
// verify
//-------
bool XEMStrategy::verify(){

  int64_t i;
  bool res = true;

  // Test 
  //-----
  // nbAlogType > 0
  if (_nbAlgo<1 || _tabAlgo==NULL){
    res = false;
    throw  nbAlgoTypeTooSmall;
  }
  if (_nbTry<minNbTryInStrategy){
    res=false;
    throw nbTryInStrategyTooSmall;
  }
  if (_nbTry>maxNbTryInStrategy){
    res=false;
    throw nbTryInStrategyTooLarge;
  } 

	
  // Test
  //------
  // If the 1rst algo is M or MAP, this algo must be the only one !
  if (((_tabAlgo[0]->getAlgoName() == M) || (_tabAlgo[0]->getAlgoName() == MAP))
    && _nbAlgo>1){
      res = false;
      throw wrongNbAlgoWhenMorMAP;
  }

  // Test
  //------
  // If the algo is M or MAP, nbTry must be 1
  if (((_tabAlgo[0]->getAlgoName() == M) || (_tabAlgo[0]->getAlgoName() ==
MAP)) && _nbTry != 1){
    res = false;
    throw wrongNbStrategyTryValue;
  }

	
  // Test 
  //-----
  // if algoType is M and MAP, the stopName must be nbIteration with nbIteration=1
  for (i=0; i<_nbAlgo; i++){
    if (_tabAlgo[i]->getAlgoName() == M || _tabAlgo[i]->getAlgoName() == MAP){
      if (_tabAlgo[i]->getAlgoStopName() == NBITERATION && _tabAlgo[i]->getNbIteration() == 1){
           // OK
      }
      else {
        _tabAlgo[i]->setAlgoStopName(NBITERATION);
        _tabAlgo[i]->setNbIteration(1);
        
#ifdef VERBOSE
        cout<<" MIXMOD WARNING : if M or MAP is used, the stopName must be nbIteration and NbIteration must be 1"<<endl;
#endif
      }
    }
  }	
	
  // Test 
  //-----
  // if algoType is SEM, the stopName must be nbIteration
  for (i=0; i<_nbAlgo; i++){
    if (_tabAlgo[i]->getAlgoName() == SEM){
      if (_tabAlgo[i]->getAlgoStopName() != NBITERATION){
        throw badStopNameWithSEMAlgo;
      }
    }
  }	
	
  //if init is USER or PARTITION, nbTry must be 1
  if ((_strategyInit->getStrategyInitName() == USER_PARTITION ||
       _strategyInit->getStrategyInitName() == USER_PARTITION) && _nbTry != 1){
    throw wrongNbStrategyTryValue;
  }
  
	
  // Test
  //----- 
  // if the algo is M, the initialization must be  USER_PARTITION and the partion must be complete
  if (_tabAlgo[0]->getAlgoName() == M){
    if (_strategyInit->getStrategyInitName() != USER_PARTITION){
      res = false;
      throw  BadInitialsationWhenM;
    }
    for (i=0; i<_strategyInit->getNbPartition(); i++){
      XEMPartition * partition = _strategyInit->getPartition(i);
      if  (! partition->isComplete()){
        res = false;
        throw  partitionMustBeComplete;
      }
    }
  }


  // test
  //-----
  // MAP => init USER
 // and we suppose that the user has done a good param
  if (_tabAlgo[0]->getAlgoName() == MAP){
    if (_strategyInit->getStrategyInitName() != USER){
      res = false;
      throw  BadInitialsationWhenMAP;
    }
  }
	
  return res;

}





//---------------
// Input strategy
//---------------
void XEMStrategy::input(ifstream & fi, XEMData *& data, int64_t nbNbCluster, int64_t * tabNbCluster, XEMModelType ** tabModelType){
  int64_t j;
  string keyWord = "";
  bool alreadyRead = false;
  string a ="";

  // nbTry
  //------
  fi>>keyWord;
  ConvertBigtoLowString(keyWord);
  if(keyWord.compare("nbtry")==0){
    int64_t nbTry;
    fi>>nbTry;
    setNbTry(nbTry); 
  }
    
  // StrategyInit
  //-----------------
  _strategyInit->input(fi, data, nbNbCluster, tabNbCluster, tabModelType, alreadyRead);
 
  
  // Algos
  //-------
  /* Number of algorithms */
  moveUntilReach(fi,"nbAlgorithm"); 
  if(!fi.eof()){
    for (j=0; j<_nbAlgo; j++){
      delete _tabAlgo[j]; 
    }
    delete[] _tabAlgo;
      
    fi>>_nbAlgo;
    if (_nbAlgo > maxNbAlgo){
      throw nbAlgoTooLarge;
    }
    else if (_nbAlgo <= 0){
      throw nbAlgoTooSmall;
    }

    _tabAlgo = new XEMAlgo * [_nbAlgo];

    for (j=0; j<_nbAlgo; j++){
      fi>>keyWord;
      ConvertBigtoLowString(keyWord);

      if(keyWord.compare("algorithm")==0){
          // tabAlgoType._type
        fi>>a;

        if (a.compare("MAP")==0){
          _tabAlgo[j] = new XEMMAPAlgo();
        }
        else if (a.compare("EM")==0){
          _tabAlgo[j] = new XEMEMAlgo();
        }
        else if (a.compare("CEM")==0){
          _tabAlgo[j] = new XEMCEMAlgo();
        }
        else if (a.compare("SEM")==0){
          _tabAlgo[j] = new XEMSEMAlgo();
        }
        else if (a.compare("M")==0){
          _tabAlgo[j] = new XEMMAlgo();
        }
        else{
          throw wrongAlgoType;
        }

        if ((_tabAlgo[j]->getAlgoName() != MAP) && (_tabAlgo[j]->getAlgoName() != M)){
          fi>>keyWord;
          ConvertBigtoLowString(keyWord);

          if(keyWord.compare("stoprule")==0){
            fi>>a;

            if (a.compare("NBITERATION")==0){
              _tabAlgo[j]->setAlgoStopName(NBITERATION);
            }

            else if (a.compare("EPSILON")==0){
              _tabAlgo[j]->setAlgoStopName(EPSILON);
            }

            else if (a.compare("NBITERATION_EPSILON")==0){
              _tabAlgo[j]->setAlgoStopName(NBITERATION_EPSILON);
            }
            else{
              throw wrongAlgoStopName;
            }
              // nbIteration && epsilon
            fi>>keyWord;
            ConvertBigtoLowString(keyWord);

            if(keyWord.compare("stoprulevalue")==0){
              if (_tabAlgo[j]->getAlgoStopName() == NBITERATION){
                int64_t nbIteration;
                fi>>nbIteration;
                _tabAlgo[j]->setNbIteration(nbIteration);
                  //_tabAlgo[j]->setEpsilon(minEpsilon);
              }
              else if (_tabAlgo[j]->getAlgoStopName() == EPSILON){
                double epsilon;
                fi>>epsilon;
                _tabAlgo[j]->setEpsilon(epsilon);
                 // _tabAlgo[j]->setNbIteration(maxNbIteration);
              }
              else if (_tabAlgo[j]->getAlgoStopName() == NBITERATION_EPSILON){
                int64_t nbIteration;
                double epsilon;
                fi>>nbIteration;
                _tabAlgo[j]->setNbIteration(nbIteration);
                fi>>epsilon;
                _tabAlgo[j]->setEpsilon(epsilon);
              }

            }// end if stopRuleValue
            else{
              throw errorStopRuleValue;
            }

          }// end if StopRule
          else{
            throw errorStopRule;
          }

        }// end if algo !=MAP ou != M

      }// end if algorithm
      else{
        throw errorAlgo;
      }

    }// end for j<nbAlgo*

  }// end if NbAlgo
  else{
    throw errorNbAlgo;
  }
}







//----
//edit
//----
void XEMStrategy::edit(ofstream & oFile){

  oFile<<"\tStrategy : "<<endl;
  oFile<<"\t--------"<<endl;

  //oFile<<"\tnumber of tries : "<<_nbTry<<endl;
  
  oFile<<"\tInitial start parameters method : ";
  string init = XEMStrategyInitNameToString(_strategyInit->getStrategyInitName());
  oFile<<init<<endl;
  oFile<<"\tNumber of algorithms in the strategy : "<<_nbAlgo<<endl;

  for (int64_t i=0; i<_nbAlgo; i++){
    oFile<<"\tAlgorithm "<<i+1<<endl;
    _tabAlgo[i]->edit(oFile);
  }
  //oFile<<"\tNumber of strategy repetitions : "<<_nbStrategyTry<<endl;
}





//-----------
// ostream <<
//-----------
ostream & operator << (ostream & fo, XEMStrategy & strategy){
    // nbTry
  fo<<"nbTry : "<<strategy._nbTry<<endl;
  
  fo<<"init : "<<endl;
  fo<<*(strategy._strategyInit)<<endl;
    
    // nbAlgo
  fo<<"nbAlgo : "<<strategy._nbAlgo<<endl;
  for (int64_t j=0; j<strategy._nbAlgo; j++){
      XEMAlgo * curAlgo = strategy._tabAlgo[j];
      fo<<"Algo n "<<j+1<<" : "<<endl;
      fo<<(*curAlgo);
      fo<<endl;
    }
}




//------------------------------------------------------------------
//others fucntions
//
XEMStrategy * buildDefaultClusteringStrategy(){
  XEMStrategy * strategy = new XEMStrategy();
  
  strategy->setNbTry(nbTryInDefaultClusteringStrategy);
  
  XEMAlgo * algo = createDefaultClusteringAlgo();
  strategy->insertAlgo(algo ,0);
  
  XEMStrategyInit * strategyInit = createDefaultClusteringStrategyInit();
  strategy->setStrategyInit(strategyInit);
  
  return strategy;
}

