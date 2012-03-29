/***************************************************************************
                             SRC/MIXMOD/XEMData.cpp  description
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

#include "XEMData.h"


//------------
// Constructor
//------------
XEMData::XEMData(){
  throw internalMixmodError;
}



//------------
// Constructor
//------------
XEMData::XEMData(const XEMData & iData){
  _nbSample    = iData._nbSample;	
  _fileNameWeight    = iData._fileNameWeight;
  _fileNameData = iData._fileNameData;
  _weightTotal = iData._weightTotal;
  _pbDimension = iData._pbDimension;
  _defaultWeight = iData.hasDefaultWeight();
  _weight = copyTab(iData._weight, _nbSample);
}



//------------
// Constructor
//------------
XEMData::XEMData(int64_t nbSample, int64_t pbDimension){
 _nbSample    = nbSample;
 _weightTotal = _nbSample;  // true if there is no weight else it will be changed
 _pbDimension = pbDimension;
 _weight      = new double[_nbSample];
 _defaultWeight = true;
 setWeightDefault();
 _fileNameWeight="";
 _fileNameData = "";
}



//------------
// Constructor for dataReduce
//------------
XEMData::XEMData(int64_t nbSample, int64_t pbDimension, double weightTotal, double * weight){
  _nbSample    = nbSample;
  _pbDimension = pbDimension;
  _weightTotal = weightTotal;
  _weight = new double[_nbSample];
  _defaultWeight = false; // TODO
  _weight = copyTab(weight, _nbSample);
  _fileNameWeight="";
  _fileNameData="";
}



//----------
//Destructor
//----------
XEMData::~XEMData(){
 if (_weight){
    delete[] _weight;
    _weight = NULL;
 }
}



//---------
// selector
//---------
void XEMData::setWeightTotal(double weightTotal){
   _weightTotal = weightTotal;
}



//----------
// setWeight
//----------
void XEMData::setWeight(string weightFileName){
  _defaultWeight = true;
  
  if (weightFileName.compare("") == 0){	
    setWeightDefault();
  }else{
  
    _weightTotal = 0.0;

    ifstream weightFile(weightFileName.c_str(), ios::in);
    if (! weightFile.is_open()){
      _fileNameWeight="";
      throw wrongWeightFileName;
    }
    int64_t i=0;
    while(i<_nbSample && !weightFile.eof()){
      weightFile>>_weight[i];
      if (_weight[i] != 1){
        _defaultWeight = false;
      }
      _weightTotal += _weight[i];
      i++;
    }
    weightFile.close();
    if (i!=_nbSample){
      _fileNameWeight="";
      throw wrongWeightFileName;
    }
    _fileNameWeight=weightFileName;
  }
}


//----------
// setWeight
//----------
void XEMData::setWeight(double* weight){
  _defaultWeight = true;
  _weightTotal = 0.0;
    
  int64_t i=0;
  while(i<_nbSample){
    _weight[i] = weight[i];
      if (_weight[i] != 1){
        _defaultWeight = false;
      }
    _weightTotal += _weight[i];
    i++;
  }
}


//----------
// setWeightDefault
//----------
void XEMData::setWeightDefault(){
  _defaultWeight = true;
	_fileNameWeight="";
  for (int64_t i=0; i<_nbSample; ++i){
			_weight[i]=1;
	}
}


bool XEMData::verify() const{
  bool res = true;
  
  // _weightTotal must be an integer
  int64_t iWeightTotal = (int64_t)_weightTotal;
  if (_weightTotal - iWeightTotal != 0){
    res = false;
    throw weightTotalIsNotAnInteger;
  }
  
  // others verify ?
  
  return res;
  
}

