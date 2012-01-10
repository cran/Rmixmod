/***************************************************************************
                             SRC/MIXMOD/XEMGaussianSample.cpp  description
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
#include "XEMGaussianSample.h"



//------------
// Constructor
//------------
XEMGaussianSample::XEMGaussianSample():XEMSample(){
  _value = NULL;
}



//------------
// Constructor
//------------
XEMGaussianSample::XEMGaussianSample(int64_t pbDimension):XEMSample(pbDimension){
  _value = new double[pbDimension];
  initToZero(_value,pbDimension);
}



//------------
// Constructor
//------------
XEMGaussianSample::XEMGaussianSample(XEMGaussianSample * iSample):XEMSample(iSample){
  _value  = copyTab(iSample->_value,_pbDimension);
}



//------------
// Constructor
//------------
XEMGaussianSample::XEMGaussianSample(int64_t pbDimension, double * tabValue):XEMSample(pbDimension){ 
  _value = copyTab( tabValue, pbDimension);
}



//-----------
// Destructor
//-----------
XEMGaussianSample::~XEMGaussianSample(){
  if(_value)
    delete[] _value;
  _value = NULL;
}



//--------------
// set tab value
//--------------
void XEMGaussianSample::setDataTabValue(double * tabValue){  
   recopyTab(tabValue,_value,_pbDimension);
}



//----------
// set value
//----------
void XEMGaussianSample::setDataValue(int64_t idxDim, double value){
  _value[idxDim] = value;
}


