/***************************************************************************
                             SRC/MIXMOD/XEMMAlgo.cpp  description
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
#include "XEMMAlgo.h"
#include "XEMModel.h"


//------------
// Constructor
//------------
XEMMAlgo::XEMMAlgo(){
  _algoStopName = NBITERATION;
  _nbIteration =1 ;
}




/// Copy constructor
XEMMAlgo::XEMMAlgo(const XEMMAlgo & mAlgo):XEMAlgo(mAlgo){
}


//------------
// Constructor
//------------
XEMMAlgo::XEMMAlgo(XEMAlgoStopName algoStopName, double epsilon, int64_t nbIteration)
	:XEMAlgo(algoStopName, epsilon, nbIteration){
}



//-----------
// Destructor
//-----------
XEMMAlgo::~XEMMAlgo(){}



// clone
//------
XEMAlgo * XEMMAlgo::clone(){
  return (new XEMMAlgo(*this));
}


//---
//run
//---
void XEMMAlgo::run(XEMModel *& model){
  _indexIteration = 0;
//  model = model;
  model->setAlgoName(M);
  model->Mstep();   // M Step
  model->Estep(); // E step to update Tik
  //cout << "\nMaximization algorithm \n";
}

