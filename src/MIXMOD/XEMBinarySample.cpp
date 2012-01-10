/***************************************************************************
                             SRC/MIXMOD/XEMBinarySample.cpp  description
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
#include "XEMBinarySample.h"



//------------
// Constructor
//------------
XEMBinarySample::XEMBinarySample():XEMSample(){
  _value = NULL;
}



//------------
// Constructor
//------------
XEMBinarySample::XEMBinarySample(int64_t pbDimension):XEMSample(pbDimension){
  _value = new int64_t[_pbDimension];
}



//------------
// Constructor
//------------
XEMBinarySample::XEMBinarySample(XEMBinarySample * iSample):XEMSample(iSample){           
  _value = copyTab(iSample->_value, _pbDimension);  
}



//------------
// Constructor
//------------
XEMBinarySample::XEMBinarySample(int64_t pbDimension, int64_t * tabValue):XEMSample(pbDimension){
  _value = copyTab(tabValue, _pbDimension);
}



//-----------
// Destructor
//-----------
XEMBinarySample::~XEMBinarySample(){
  if(_value){
    delete[] _value;
    _value = NULL;
  }
}



//-------------
// set tab value
//--------------
void XEMBinarySample::setDataTabValue(int64_t * tabValue){
  int64_t j;
  for(j=0; j<_pbDimension; j++)
    _value[j] = tabValue[j];
}



//----------
// set value
//----------
void XEMBinarySample::setDataValue(int64_t idxDim, int64_t iValue){
  _value[idxDim] = iValue;
}



