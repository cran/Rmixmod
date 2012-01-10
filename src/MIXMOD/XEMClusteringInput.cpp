/***************************************************************************
                             SRC/MIXMOD/XEMClusteringInput.cpp  description
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
#include "XEMClusteringInput.h"



//--------------------
// Default Constructor
//--------------------
XEMClusteringInput::XEMClusteringInput(){
  _strategy = new XEMClusteringStrategy();
}


//-----------------
//  Copy constructor
//-----------------
XEMClusteringInput::XEMClusteringInput(const XEMClusteringInput & cInput ) : XEMInput(cInput){
  _strategy = new XEMClusteringStrategy(*(cInput.getStrategy())); // copy constructor
}



//---------------------------
// Initialisation Constructor
//---------------------------
XEMClusteringInput::XEMClusteringInput(const vector<int64_t> & iNbCluster, const XEMDataDescription & iDataDescription) : XEMInput(iNbCluster, iDataDescription){
  _strategy = new XEMClusteringStrategy();
}




//-----------
// Destructor
//-----------
XEMClusteringInput::~XEMClusteringInput(){
  if (_strategy){
    delete _strategy;
  }
}



// ----------------
// Verif
//-----------------
bool XEMClusteringInput::verif(){
  bool res = XEMInput::verif();
  
  if (res){
    res = _strategy->verify(); 
  }
  return res;
}


