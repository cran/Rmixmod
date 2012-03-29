/***************************************************************************
                             SRC/MIXMOD/XEMCEMAlgo.cpp  description
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
#include "XEMCEMAlgo.h"
#include "XEMModel.h"

//-----------
//Constructor
//-----------
XEMCEMAlgo::XEMCEMAlgo(){
}


/// Copy constructor
XEMCEMAlgo::XEMCEMAlgo(const XEMCEMAlgo & cemAlgo):XEMAlgo(cemAlgo){
}

XEMCEMAlgo::XEMCEMAlgo(XEMAlgoStopName algoStopName, double epsilon, int64_t nbIteration)
:XEMAlgo(algoStopName, epsilon, nbIteration){
}



//----------
//Destructor
//----------
XEMCEMAlgo::~XEMCEMAlgo(){}



// clone
//------
XEMAlgo * XEMCEMAlgo::clone(){
  return (new XEMCEMAlgo(*this));
}

//---
//run
//---
void XEMCEMAlgo::run(XEMModel *& model){
  _indexIteration = 1;
//  model = model;
  model->setAlgoName(CEM);

#if DEBUG > 0
  cout<<"Debut de l'ago CEM :"<<endl;
  model->editDebugInformation();
#endif
  
  while (continueAgain()){
    model->Estep();        /* E Step */
    model->Cstep();        /* C Step */
    model->Mstep();        /* M Step */
#if DEBUG > 0
  cout<<"Apres la "<<_indexIteration<<" eme iteration de CEM :"<<endl;
  model->editDebugInformation();
#endif
    _indexIteration++;
   _xml_old = _xml;
   _xml = model->getCompletedLogLikelihood();
  }
	
	//to update tik, fik
	model->Estep();
	//to compute Cik
	model->Cstep();
        
}

