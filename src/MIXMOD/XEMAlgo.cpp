/***************************************************************************
                             SRC/MIXMOD/XEMAlgo.cpp  description
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
#include "XEMAlgo.h"
#include "XEMEMAlgo.h"


//-----------
//Constructor
//-----------
XEMAlgo::XEMAlgo(){
  _indexIteration = 0;
  _algoStopName   = defaultAlgoStopName;
  _epsilon        = defaultEpsilon;
  _nbIteration    = defaultNbIteration;
  _xml_old = 0;
  _xml = 0;
}


// copy constructor
XEMAlgo::XEMAlgo(const XEMAlgo & algo){
  _indexIteration = algo._indexIteration;
  _algoStopName   = algo._algoStopName;
  _epsilon        = algo._epsilon;
  _nbIteration    = algo._nbIteration;
  _xml_old = algo._xml_old;
  _xml = algo._xml;
}



XEMAlgo::XEMAlgo(XEMAlgoStopName algoStopName, double espsilon, int64_t nbIteration){
  _indexIteration = 1;
  _algoStopName   = algoStopName;
  setEpsilon(espsilon);
  setNbIteration(nbIteration);
  _xml_old = 0;
  _xml = 0;
}



//----------
//Destructor
//----------
XEMAlgo::~XEMAlgo(){
}


//--------------
// continueAgain
//--------------
/* Stopping rule for algorithm : continueAgain */
bool XEMAlgo::continueAgain(){
  //cout<<"XEMAlgo::continueAgain"<<endl;
  bool result, res1,  res2;
  double diff;
  
  if (_indexIteration==1){
    result = true;
    return result;
  }
  else{

    if (_indexIteration>maxNbIteration){
      result = false;
      return result;
    }
    else{
      switch (_algoStopName){
        case NBITERATION :
          result = (_indexIteration<=_nbIteration); break;
      
        case EPSILON :
          if (_indexIteration<=3){
            result = true;
          }
          else {
            diff = fabs(_xml - _xml_old);
            result = (diff >= _epsilon);
          }
          break;
      
        case NBITERATION_EPSILON :
          res1 = (_indexIteration<=_nbIteration);
          if (_indexIteration<=3){
            res2 =true;
          }
          else {
            diff   = fabs(_xml - _xml_old);
            res2   = (diff >= _epsilon);
          };
          result = (res1 && res2);
          break;

          default : result = (_indexIteration<=_nbIteration);
      }
      return result;
    }
  }
}



//-----------
// ostream <<
//-----------
std::ostream & operator << (std::ostream & fo, XEMAlgo & algo){
  XEMAlgoName algoName = algo.getAlgoName();
  fo<<"\t  Type : "<<XEMAlgoNameToString(algoName);

  fo<<"\t  Stopping rule : ";
  
  switch (algo._algoStopName){
  
    case NBITERATION :
      fo<<"NBITERATION"<<endl;
      fo<<"\t  Number of iterations : "<<algo._nbIteration<<endl;
      break;
    case NBITERATION_EPSILON :
      fo<<"NBITERATION_EPSILON"<<endl;
      fo<<"\t  Number of iterations : "<<algo._nbIteration<<endl;
      fo<<"\t  Set tolerance (xml criterion) : "<<algo._epsilon<<endl;
      break;
    case EPSILON :
      fo<<"EPSILON"<<endl;
      fo<<"\t  Set tolerance (xml criterion) : "<<algo._epsilon<<endl;
      break;
    case NO_STOP_NAME :
      break;
    default :
      break;
  }
  return fo;
}


void XEMAlgo::edit(std::ofstream & oFile){
  XEMAlgoName algoName = getAlgoName();
  oFile<<"\t  Type : "<<XEMAlgoNameToString(algoName)<<endl;

  oFile<<"\t  Stopping rule : ";
  
  switch (_algoStopName){
  
    case NBITERATION :
      oFile<<"NBITERATION"<<endl;
      oFile<<"\t  Number of iterations : "<<_nbIteration<<endl;
      break;
    case NBITERATION_EPSILON :
      oFile<<"NBITERATION_EPSILON"<<endl;
      oFile<<"\t  Number of iterations : "<<_nbIteration<<endl;
      oFile<<"\t  Set tolerance (xml criterion) : "<<_epsilon<<endl;
      break;
    case EPSILON :
      oFile<<"EPSILON"<<endl;
      oFile<<"\t  Set tolerance (xml criterion) : "<<_epsilon<<endl;
      break;
    case NO_STOP_NAME :
      break;
    default :
      break;
  }
}
  

//----------------
// others fucntions
//
XEMAlgo * createDefaultClusteringAlgo(){
  if (defaultClusteringAlgoName != EM){
    throw internalMixmodError;
  }
  XEMAlgo * algo = new XEMEMAlgo();
  return algo;
}

