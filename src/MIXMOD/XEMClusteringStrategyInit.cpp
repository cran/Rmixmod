/***************************************************************************
                             SRC/MIXMOD/XEMClusteringStrategyInit.cpp  description
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

#include "XEMClusteringStrategyInit.h"
#include "XEMGaussianParameter.h"
#include "XEMGaussianSphericalParameter.h"
#include "XEMGaussianDiagParameter.h"
#include "XEMGaussianGeneralParameter.h"
#include "XEMGaussianHDDAParameter.h"
#include "XEMBinaryEkjhParameter.h"
#include "XEMBinaryData.h"
//#include <fstream.h>

//------------
// Constructor
//------------
XEMClusteringStrategyInit::XEMClusteringStrategyInit(){
  _strategyInitName = defaultStrategyInitName;
  _nbInitParameter   = 0;
  _tabInitParameter  = NULL;
  _nbPartition           = 0;
  _tabPartition          = NULL; 
  _deleteTabParameter= false;
  
  _nbTry = defaultNbTryInInit;
  _nbIteration = defaultNbIterationInInit;
  _epsilon = defaultEpsilonInInit;
  setStopName(NBITERATION_EPSILON);
}


//------------------
// Copy constructor
//------------------
XEMClusteringStrategyInit::XEMClusteringStrategyInit(const XEMClusteringStrategyInit & strategyInit){
  _strategyInitName = strategyInit.getStrategyInitName();
  _nbInitParameter   = strategyInit.getNbInitParameter();
  
  _nbPartition       = strategyInit.getNbPartition();
  _tabPartition  = NULL; 
  if (_nbPartition !=0){
    _tabPartition = new XEMPartition*[_nbPartition];
    const XEMPartition ** iPartition = strategyInit.getTabPartition();
    for (int64_t i=0; i<_nbPartition; i++){
      _tabPartition[i] = new XEMPartition(*(iPartition[i])); //copy constructor
    }
  }
  
  _nbInitParameter = strategyInit.getNbInitParameter();
  _tabInitParameter      = NULL; 
  if (_nbInitParameter !=0){
    _tabInitParameter = new XEMParameter*[_nbInitParameter];
    const XEMParameter ** iParameter = strategyInit.getTabInitParameter();
    for (int64_t i=0; i<_nbInitParameter; i++){
      _tabInitParameter[i] = (iParameter[i])->clone();
    }
  }
  
  _deleteTabParameter= false; // TODO ?
  
  _nbTry = strategyInit.getNbTry();
  _nbIteration = strategyInit.getNbIteration();
  _epsilon = strategyInit.getEpsilon();
  _stopName = strategyInit.getStopName();

}



  
//-----------
// Destructor
//-----------
XEMClusteringStrategyInit::~XEMClusteringStrategyInit(){
  int64_t i;
  if (_tabInitParameter && _deleteTabParameter){
    for (i=0; i<_nbInitParameter; i++){
      delete _tabInitParameter[i];    
    }
    delete [] _tabInitParameter;
    _tabInitParameter = NULL;
  }

  if (_tabPartition){
    for (i=0; i<_nbPartition; i++){
      delete _tabPartition[i];
      _tabPartition[i] = NULL;
    }
    delete [] _tabPartition;
    _tabPartition = NULL;
  }
}



//---------------------
// setStrategyInitName 
//---------------------
void XEMClusteringStrategyInit::setStrategyInitName(XEMStrategyInitName initName){
  
  // delete ?
  int64_t i;
  if (_tabInitParameter && _deleteTabParameter){
    for (i=0; i<_nbInitParameter; i++){
      delete _tabInitParameter[i];    
    }
    delete [] _tabInitParameter;
    _tabInitParameter = NULL;
  }

  if (_tabPartition){
    for (i=0; i<_nbPartition; i++){
      delete _tabPartition[i];
      _tabPartition[i] = NULL;
    }
    delete [] _tabPartition;
    _tabPartition = NULL;
  }
  
  //
  _strategyInitName = initName;
  _nbInitParameter   = 0;
  _tabInitParameter  = NULL;
  _nbPartition           = 0;
  _tabPartition          = NULL; 
  _deleteTabParameter= false;
  
  _nbTry = defaultNbTryInInit;
  _nbIteration = defaultNbIterationInInit;
  if (_strategyInitName == SEM_MAX){
    _nbIteration = defaultNbIterationInInitForSemMax;
    setStopName(NBITERATION);
  }
  if (_strategyInitName == USER || _strategyInitName == USER_PARTITION){
    _nbTry = 1;
  }
  _epsilon = defaultEpsilonInInit;

}




//-------------------
// set init parameter
//-------------------
void XEMClusteringStrategyInit::setInitParam(string & paramFileName, int64_t position){
  ifstream paramFile(paramFileName.c_str(), ios::in);
  if (! paramFile.is_open()){
    throw wrongParamFileName;
  }
  if (_tabInitParameter != NULL){
    _tabInitParameter[position]->input(paramFile);
    paramFile.close();
  }
  else{
    throw internalMixmodError;
  }
}
  
  
  
//----------------
// setTabInitParam
//----------------
void XEMClusteringStrategyInit::setTabInitParameter(XEMParameter ** tabInitParameter, int64_t nbInitParameter){
  int64_t i;
  if (_tabInitParameter && _deleteTabParameter){
    for (i=0; i<_nbInitParameter; i++){
      delete _tabInitParameter[i];    
    }
    delete [] _tabInitParameter;
    _tabInitParameter = NULL;
  }
  
  _tabInitParameter = tabInitParameter;
  _nbInitParameter = nbInitParameter;
}
  
  
  
  
//------------------  
//set Init Partition
//------------------
void XEMClusteringStrategyInit::setPartition(XEMPartition * part, int64_t position){
  if (part != NULL && _tabPartition!=NULL){
    _tabPartition[position] = part;
  }
  else{
    throw internalMixmodError;
  }
}
  
  
//------------------
//set Init Partition
//----------------
void XEMClusteringStrategyInit::setPartition(string & partitionFileName, int64_t position){
  ifstream partitionFile(partitionFileName.c_str(), ios::in);
  if (! partitionFile.is_open()){
    throw wrongPartitionFileName;
  }
  try{
    partitionFile>>(*_tabPartition[position]);
    partitionFile.close();
  }catch(XEMErrorType errorType){
    throw badInitPart;
  }
}
  
  
 
//----------------
// setTabPartition
//----------------
void XEMClusteringStrategyInit::setTabPartition(XEMPartition ** tabPartition, int64_t nbPartition){
  int64_t i;
  if (_tabPartition){
    for (i=0; i<_nbPartition; i++){
      delete _tabPartition[i];
      _tabPartition[i] = NULL;
    }
    delete [] _tabPartition;
    _tabPartition = NULL;
  }
  
  _tabPartition = tabPartition;
  _nbPartition = nbPartition;
}
  
  
//------------  
// setStopName
//-------------
void XEMClusteringStrategyInit::setStopName(XEMAlgoStopName stopName){
  if (_strategyInitName == SMALL_EM){
    _stopName = stopName;
  }else if (_strategyInitName == SEM_MAX && stopName==NBITERATION){
    _stopName = NBITERATION;
  }else{
    throw badSetStopNameInInit;
  }
}
  
  
  
//---------
// setNbTry
//---------
void XEMClusteringStrategyInit::setNbTry(int64_t nbTry){
  if (_strategyInitName==SMALL_EM || _strategyInitName==CEM_INIT || _strategyInitName==RANDOM){
    if (nbTry > maxNbTryInInit){
      throw nbTryInInitTooLarge;
    }
    else if (nbTry < minNbTryInInit){
      throw nbTryInInitTooSmall;
    }
    else{
      _nbTry = nbTry;
    }
  }
  else{
    throw badSetNbTryInInit;
  }
}
  
  
  
//----------------
// set NbIteration
//----------------
void XEMClusteringStrategyInit::setNbIteration(int64_t nbIteration){
  if (_strategyInitName==SMALL_EM || _strategyInitName==SEM_MAX){
    if (nbIteration > maxNbIterationInInit){
      throw nbIterationInInitTooLarge;
    }
    else if (nbIteration < minNbIterationInInit){
      throw nbIterationInInitTooSmall;
    }
    else{
      _nbIteration = nbIteration;
    }
  }
  else{
    throw badSetNbIterationInInit;
  }
}
  
  
//-----------
// setEpsilon
//-----------
void XEMClusteringStrategyInit::setEpsilon(double epsilon){
  if (_strategyInitName==SMALL_EM){
    if (epsilon > maxEpsilonInInit){
      throw epsilonInInitTooLarge;
    }
    else if (epsilon < minEpsilonInInit){
      throw epsilonInInitTooSmall;
    }
    else{
      _epsilon = epsilon;
    }
  }
  else{
    throw badSetEpsilonInInit;
  }
}
  
  
  
//-------
// verify
//-------
bool XEMClusteringStrategyInit::verify() const{
  bool res = true;
  //if init is USER or PARTITION, nbTry must be 1
  if ((_strategyInitName == USER_PARTITION || _strategyInitName == USER_PARTITION) && _nbTry != 1){
    res = false;
    throw wrongNbStrategyTryValue;
   }
  
  return res;
}




// input
void XEMClusteringStrategyInit::input(ifstream & fi, XEMData *& data, int64_t nbNbCluster, int64_t * tabNbCluster, XEMModelType * modelType, bool & alreadyRead){
  string keyWord = "";
  string a ="";
  int64_t k;
  int64_t pbDimension  = data->_pbDimension;
  int64_t nbSample    = data->_nbSample;
  
  moveUntilReach(fi,"inittype"); 
  if(!fi.eof()){

    fi >> a;
    if (a.compare("RANDOM")==0){
      setStrategyInitName(RANDOM);
      // nbTryInInit
      fi>>keyWord;
      ConvertBigtoLowString(keyWord);
      if(keyWord.compare("nbtryininit")==0){
        int64_t nbTry;
        fi>>nbTry;
        setNbTry(nbTry); 
      }
      else{
        alreadyRead = true;
      }
    }

    // USER init type //
    else if (a.compare("USER")==0){
      setStrategyInitName(USER);

      // tabInitFileName
      fi >> keyWord;
      ConvertBigtoLowString(keyWord);

      if(keyWord.compare("initfile")==0){
       // _strategyInit->_nbInitParameter  = nbNbCluster;
        XEMParameter ** tabInitParameter = new XEMParameter * [nbNbCluster];
        string * tabFileName = new string[nbNbCluster];
        for (k=0; k<nbNbCluster; k++){
          tabFileName[k] = "";
        }
        readTabFileName(fi, nbNbCluster, tabFileName, keyWord);
        alreadyRead = true;


        for (k=0; k<nbNbCluster; k++){
          if (isEDDA(modelType->_nameModel)){
            tabInitParameter[k] = new XEMGaussianGeneralParameter(tabNbCluster[k], pbDimension, modelType, tabFileName[k]);
          }
          else if (isBinary(modelType->_nameModel)){
            int64_t * tabNbModality = ((XEMBinaryData*)data)->getTabNbModality();
            tabInitParameter[k] = new XEMBinaryEkjhParameter(tabNbCluster[k], pbDimension, modelType, tabNbModality, tabFileName[k]);
          }
          else if (isHD(modelType->_nameModel)){
            tabInitParameter[k] = new XEMGaussianHDDAParameter(tabNbCluster[k], pbDimension, modelType, tabFileName[k]);
          }
          else throw internalMixmodError;
        }
        setTabInitParameter(tabInitParameter, nbNbCluster);
        delete[] tabFileName;
      }
      else{
        throw errorInitFile;
      }
    }

    // USER_PARTITION init type //

    else if (a.compare("USER_PARTITION")==0){
      setStrategyInitName(USER_PARTITION);	

      //label
      fi>>keyWord;
      ConvertBigtoLowString(keyWord);
      if(keyWord.compare("initfile")==0){
        //_strategyInit->_nbPartition  = nbNbCluster;
        XEMPartition ** tabPartition = new XEMPartition * [nbNbCluster];
        string * tabFileName = new string[nbNbCluster];
        for (k=0; k<nbNbCluster; k++){
          tabFileName[k] = "";
        }
        readTabFileName(fi, nbNbCluster, tabFileName, keyWord);
        alreadyRead = true;

        for (k=0; k<nbNbCluster; k++){
        XEMNumericPartitionFile partitionFile;
        partitionFile._fileName = tabFileName[k];
        partitionFile._format = FormatNumeric::defaultFormatNumericFile;
        partitionFile._type = TypePartition::defaultTypePartition;
          tabPartition[k] = new XEMPartition(nbSample, tabNbCluster[k], partitionFile);
        }
        setTabPartition(tabPartition, nbNbCluster);
        delete[] tabFileName;
      }
      else{
        throw errorInitFile;
      }
    }


    else if (a.compare("SMALL_EM")==0){
      setStrategyInitName(SMALL_EM);
      // nbTry
      fi>>keyWord;
      ConvertBigtoLowString(keyWord);
      if(keyWord.compare("nbtryininit")==0){
        int64_t nbTry;
        fi>>nbTry;
        setNbTry(nbTry); 
      }
      else{
        alreadyRead = true;
      }
      
      bool nbIteration = false;
      bool epsilon = false;
      //nbIterationInInit
      if (!alreadyRead){
        fi>>keyWord;
      }
      ConvertBigtoLowString(keyWord);
      if(keyWord.compare("nbiterationininit")==0){
        nbIteration = true;
        int64_t nbIteration;
        fi>>nbIteration;
        setNbIteration(nbIteration); 
        alreadyRead = false;
      }
      else{
        alreadyRead = true;
      }
      
      //epsilonInInit
      if (!alreadyRead){
        fi>>keyWord;
        alreadyRead = false;
      }
      ConvertBigtoLowString(keyWord);
      if(keyWord.compare("epsilonininit")==0){
        epsilon = true;
        double epsilon;
        fi>>epsilon;
        setEpsilon(epsilon); 
        alreadyRead = false;
      }
      else{
        alreadyRead = true;
      }
      if (nbIteration && epsilon){
        setStopName(NBITERATION_EPSILON);
      }
      if (nbIteration && !epsilon){
        setStopName(NBITERATION);
      }
      if (!nbIteration && epsilon){
        setStopName(EPSILON);
      }
      if (!nbIteration && !epsilon){
        setStopName(NBITERATION_EPSILON);
      }
    }

    else if (a.compare("CEM_INIT")==0){
      setStrategyInitName(CEM_INIT);
      fi>>keyWord;
      ConvertBigtoLowString(keyWord);
      if(keyWord.compare("nbtryininit")==0){
        int64_t nbTry;
        fi>>nbTry;
        setNbTry(nbTry); 
      }
      else{
        alreadyRead = true;
      }
    }

    else if (a.compare("SEM_MAX")==0){
      setStrategyInitName(SEM_MAX);
      fi>>keyWord;
      ConvertBigtoLowString(keyWord);
      if(keyWord.compare("nbiterationininit")==0){
        int64_t nbIteration;
        fi>>nbIteration;
        setNbIteration(nbIteration); 
      }
      else{
        alreadyRead = true;
      }
    }

    else{
      throw wrongStrategyInitName;
    }
  }
}





    
// ostream <<
//-----------
ostream & operator << (ostream & fo, XEMClusteringStrategyInit & strategyInit){
    //strategyInitType
  string init = XEMStrategyInitNameToString(strategyInit._strategyInitName);
    fo<<"\t strategyInitName : "<<init<<endl;
    
    // nbTryInInit
    int64_t nbTryInInit = strategyInit._nbTry;
    fo<<"\t nbTryInInit : "<<nbTryInInit<<endl;
    
    // stopNameInInit
    string stopNameInInit = XEMAlgoStopNameToString(strategyInit._stopName);
    fo<<"\t stopNameInInit : "<<stopNameInInit<<endl;
    
    // nbIterationInInit
    int64_t nbIterationInInit = strategyInit._nbIteration;
    fo<<"\t nbIterationInInit : "<<nbIterationInInit<<endl;
    
    
    // epsilonInInit
    double epsilonInInit = strategyInit._epsilon;
    fo<<"\t epsilonInInit : "<<epsilonInInit<<endl;
    
    int64_t nbInitParameter = strategyInit._nbInitParameter;
    fo<<"\t nbInitParameter : "<<nbInitParameter<<endl;
    for (int64_t p=0; p<nbInitParameter; p++){
      //
    }
    fo<<endl;
    int64_t nbPartition = strategyInit._nbPartition;
    fo<<"\t nbLabel : "<<nbPartition<<endl;
    for (int64_t p=0; p<nbPartition; p++){
      const XEMPartition * part = strategyInit._tabPartition[p];
      fo<<*(part)<<endl;
    }
    return fo;
}


