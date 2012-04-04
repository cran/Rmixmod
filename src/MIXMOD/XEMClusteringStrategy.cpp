/***************************************************************************
                             SRC/MIXMOD/XEMClusteringStrategy.cpp  description
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

#include "XEMClusteringStrategy.h"
#include "XEMClusteringStrategyInit.h"
#include "XEMEMAlgo.h"
#include "XEMCEMAlgo.h"
#include "XEMSEMAlgo.h"
#include "XEMGaussianParameter.h"
#include "XEMGaussianSphericalParameter.h"
#include "XEMGaussianDiagParameter.h"
#include "XEMGaussianGeneralParameter.h"
#include "XEMGaussianHDDAParameter.h"
#include "XEMBinaryEkjhParameter.h"
#include "XEMBinaryData.h"
#include "XEMModelType.h"
#include "XEMPartition.h"
#include "XEMModel.h"

//-----------
//Constructor
//-----------
XEMClusteringStrategy::XEMClusteringStrategy(){
  _nbTry = defaultNbTryInStrategy;
  _strategyInit = new XEMClusteringStrategyInit();
  _nbAlgo  = defaultNbAlgo;
  _tabAlgo.reserve(_nbAlgo);
//  _tabAlgo = new XEMAlgo * [_nbAlgo];
  for (int64_t i=0; i<_nbAlgo; i++){
    _tabAlgo.push_back(createDefaultClusteringAlgo());
  }
}


XEMClusteringStrategy::XEMClusteringStrategy(const XEMClusteringStrategy & strategy){
  _nbTry = strategy.getNbTry();
  _strategyInit = new XEMClusteringStrategyInit(*(strategy.getStrategyInit()));
  _nbAlgo = strategy.getNbAlgo();
//  _tabAlgo = new XEMAlgo * [_nbAlgo];
//  XEMAlgo ** tabA = strategy.getTabAlgo();
  std::vector<XEMAlgo*> tabA = strategy.getTabAlgo();
  for (int64_t i=0; i<_nbAlgo; i++){
//    _tabAlgo[i] = tabA[i]->clone();
    _tabAlgo.push_back(tabA[i]->clone());
  }
  
}



//----------
//Destructor
//----------
XEMClusteringStrategy::~XEMClusteringStrategy(){

  for (unsigned int i=0; i<_tabAlgo.size(); i++) {
    delete _tabAlgo[i];
  }
  if (_strategyInit) delete _strategyInit;
}

// setAlgoEpsilon
void XEMClusteringStrategy::setAlgoEpsilon(int64_t position, double epsilonValue){
  _tabAlgo[position]->setEpsilon(epsilonValue);
}

// setAlgoStopRuleTypeValue
void XEMClusteringStrategy::setAlgoStopRule(XEMAlgoStopName stopName, int64_t position){
  _tabAlgo[position]->setAlgoStopName(stopName);
}

void XEMClusteringStrategy::setAlgoIteration( int64_t position, int64_t nbIterationValue){
  _tabAlgo[position]->setNbIteration(nbIterationValue);
}


// setAlgo
void XEMClusteringStrategy::setAlgo(XEMAlgoName algoName, int64_t position){
  if (_tabAlgo[position] != NULL){
    delete _tabAlgo[position];
  }
  switch (algoName) {
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

// setAlgo
void XEMClusteringStrategy::addAlgo(XEMAlgoName algoName){

  switch (algoName) {
    case EM :
      _tabAlgo.push_back(new XEMEMAlgo());
      break;
    case CEM :
      _tabAlgo.push_back(new XEMCEMAlgo());
      break;
    case SEM :
      _tabAlgo.push_back(new XEMSEMAlgo());
      break;
    default :
      throw internalMixmodError;
  }
  _nbAlgo++;
}

// set init parameter
void XEMClusteringStrategy::setInitParam(std::string & paramFileName, int64_t position){
  _strategyInit->setInitParam(paramFileName, position);
}

void XEMClusteringStrategy::setTabInitParameter(XEMParameter ** tabInitParameter, int64_t nbInitParameter){
  _strategyInit->setTabInitParameter(tabInitParameter, nbInitParameter);
}

// set init partition
void XEMClusteringStrategy::setInitPartition(std::string & partitionFileName, int64_t position){
  _strategyInit->setPartition(partitionFileName, position);
}


// set init partition
void XEMClusteringStrategy::setInitPartition(XEMPartition * part, int64_t position){
  _strategyInit->setPartition(part, position);
}

void XEMClusteringStrategy::setTabPartition(XEMPartition ** tabPartition, int64_t nbPartition){
  _strategyInit->setTabPartition(tabPartition, nbPartition);
}

// insert algo
void XEMClusteringStrategy::insertAlgo(XEMAlgoName algoName,int64_t position){
  switch (algoName) {
    case EM :
     _tabAlgo.insert(_tabAlgo.begin()+position,new XEMEMAlgo());
      break;
    case CEM :
      _tabAlgo.insert(_tabAlgo.begin()+position,new XEMCEMAlgo());
      break;
    case SEM :
      _tabAlgo.insert(_tabAlgo.begin()+position,new XEMSEMAlgo());
      break;
    default :
      throw internalMixmodError;
  }
  _nbAlgo++;
}


// removeAlgo
void XEMClusteringStrategy::removeAlgo( unsigned int position){
  if ( position < _tabAlgo.size() ){
    _tabAlgo.erase(_tabAlgo.begin()+position);
    _nbAlgo--;
  }
}

//--------------------
// setStrategyInit
//--------------------
void XEMClusteringStrategy::setStrategyInit(XEMClusteringStrategyInit * iStrategyInit){
  _strategyInit = iStrategyInit; //copy constructor
}


void XEMClusteringStrategy::setStrategyInit(XEMStrategyInitName initName, XEMData *& data, int64_t nbNbCluster, int64_t * tabNbCluster, XEMModelType * modelType){
  int64_t nbSample    = data->_nbSample;
  int64_t pbDimension  = data->_pbDimension;
  std::string fileName = "";
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
        if (isEDDA(modelType->_nameModel)){
          tabInitParameter[k] = new XEMGaussianGeneralParameter(tabNbCluster[k], pbDimension, modelType, fileName);
        }
        else if (isBinary(modelType->_nameModel)){
          int64_t * tabNbModality = dynamic_cast<XEMBinaryData*>(data)->getTabNbModality();
          tabInitParameter[k] = new XEMBinaryEkjhParameter(tabNbCluster[k], pbDimension, modelType, tabNbModality, fileName);
        }
        else if (isHD(modelType->_nameModel)){ 
          tabInitParameter[k] = new XEMGaussianHDDAParameter(tabNbCluster[k], pbDimension, modelType, fileName);
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
void XEMClusteringStrategy::setNbTry(int64_t nbTry){
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
void XEMClusteringStrategy::run(XEMModel * model){ 
  //cout<<"XEMClusteringStrategy Init, nbTry="<<_nbTry<<endl;
  if (_nbTry == 1){
    oneTry(model);
  }
  else{ 
    XEMModel * currentModel = new XEMModel(model);
    oneTry(currentModel);
    XEMModel * bestModel = new XEMModel(currentModel);
    double bestLLorCLL = currentModel->getCompletedLogLikelihoodOrLogLikelihood();
    // others tries
    for (int64_t i=1; i<_nbTry; i++){
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
    //cout<<"fin de XEMClusteringStrategy Init, nb d'essais effectues="<<i<<endl<<endl;
    delete currentModel;
    delete model;
    model = bestModel;
  }
}



//------
//oneTry
//------
void XEMClusteringStrategy::oneTry(XEMModel *& model){
 
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
      _strategyInit->initSMALL_EM(model); 
      break;
    
    case CEM_INIT : 
      _strategyInit->initCEM_INIT(model); 
      break;
    
    case SEM_MAX :
      _strategyInit->initSEM_MAX(model); 
      break;
        
    default :
      throw wrongStrategyInitName;
  }
    
  model->setAlgoName(UNKNOWN_ALGO_NAME);

// model->getParameter()->edit();
 
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




//-------
// verify
//-------
bool XEMClusteringStrategy::verify(){

  bool res = true;

  // Test 
  //-----
  // nbAlogType > 0
  if (_nbAlgo<1 || _tabAlgo.empty()){
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

	if (res){
    res = _strategyInit->verify();
  }
	
  return res;

}




//---------------
// Input strategy
// TODO XEMInput : a enlever
//---------------
void XEMClusteringStrategy::input_FLAT_FORMAT(std::ifstream & fi, XEMData *& data, int64_t nbNbCluster, int64_t * tabNbCluster, XEMModelType * modelType){
  int64_t j;
  std::string keyWord = "";
  bool alreadyRead = false;
  std::string a ="";

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
  _strategyInit->input(fi, data, nbNbCluster, tabNbCluster, modelType, alreadyRead);
 
  
  // Algos
  //-------
  /* Number of algorithms */
  moveUntilReach(fi,"nbAlgorithm"); 
  if(!fi.eof()){
    for (j=0; j<_nbAlgo; j++){
      delete _tabAlgo[j]; 
    }
//    delete[] _tabAlgo;
      
    fi>>_nbAlgo;
    if (_nbAlgo > maxNbAlgo){
      throw nbAlgoTooLarge;
    }
    else if (_nbAlgo <= 0){
      throw nbAlgoTooSmall;
    }

//    _tabAlgo = new XEMAlgo * [_nbAlgo];
    _tabAlgo.resize(_nbAlgo);
    
    for (j=0; j<_nbAlgo; j++){
      fi>>keyWord;
      ConvertBigtoLowString(keyWord);

      if(keyWord.compare("algorithm")==0){
          // tabAlgoType._type
        fi>>a;

        if (a.compare("CEM")==0){
          _tabAlgo[j] = new XEMCEMAlgo();
        }
        else if (a.compare("EM")==0){
          _tabAlgo[j] = new XEMEMAlgo();
        }
        else if (a.compare("SEM")==0){
          _tabAlgo[j] = new XEMSEMAlgo();
        }
        else{
          throw wrongAlgoType;
        }

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
void XEMClusteringStrategy::edit(std::ofstream & oFile){

  oFile<<"\tStrategy : "<<endl;
  oFile<<"\t--------"<<endl;

  //oFile<<"\tnumber of tries : "<<_nbTry<<endl;
  
  oFile<<"\tInitial start parameters method : ";
  std::string init = XEMStrategyInitNameToString(_strategyInit->getStrategyInitName());
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
std::ostream & operator << (std::ostream & fo, XEMClusteringStrategy & strategy){
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
  return fo;
}


const int64_t XEMClusteringStrategy::getNbTryInInit() const{
  return _strategyInit->getNbTry();
}

const int64_t XEMClusteringStrategy::getNbIterationInInit() const{
  return _strategyInit->getNbIteration();
}

const double XEMClusteringStrategy::getEpsilonInInit() const{
  return _strategyInit->getEpsilon();
}

void XEMClusteringStrategy::setNbTryInInit(int64_t nbTry){
  _strategyInit->setNbTry(nbTry);
}

void XEMClusteringStrategy::setStrategyInitName(XEMStrategyInitName initName){
  _strategyInit->setStrategyInitName(initName);
}

void XEMClusteringStrategy::setNbIterationInInit(int64_t nbIteration){
  _strategyInit->setNbIteration(nbIteration);
}

void XEMClusteringStrategy::setEpsilonInInit(double epsilon){
  _strategyInit->setEpsilon(epsilon);
}

const XEMAlgoStopName XEMClusteringStrategy::getStopNameInInit() const{
  return(_strategyInit->getStopName());
}

void XEMClusteringStrategy::setStopNameInInit(XEMAlgoStopName stopName){
  _strategyInit->setStopName(stopName);
}





