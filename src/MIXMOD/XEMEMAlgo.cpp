/***************************************************************************
                             SRC/MIXMOD/XEMEMAlgo.cpp  description
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
#include "XEMEMAlgo.h"
#include "XEMUtil.h" 
#include <string>
#include <sstream>

//-----------
//Constructor
//-----------
XEMEMAlgo::XEMEMAlgo(){
}

XEMEMAlgo::XEMEMAlgo(XEMAlgoStopName algoStopName, double epsilon, int64_t nbIteration)
:XEMAlgo( algoStopName, epsilon, nbIteration){
}


/// Copy constructor
XEMEMAlgo::XEMEMAlgo(const XEMEMAlgo & emAlgo):XEMAlgo(emAlgo){
  }
  
  
//----------
//Destructor
//----------
XEMEMAlgo::~XEMEMAlgo(){
}



// clone
//------
XEMAlgo * XEMEMAlgo::clone(){
  return (new XEMEMAlgo(*this));
}

//---
//run
//---
void XEMEMAlgo::run(XEMModel *& model){

/*std::ostringstream numero;
std::string nomfic;
    
std::string basestring ="parameter";
std::string suffix = ".txt" ;

ofstream likeli("likelihoods.txt", ios::out);
*/

  _indexIteration = 1;
  model->setAlgoName(EM);

#if DEBUG > 0
  cout<<"Debut Algo EM :"<<endl;
  model->editDebugInformation();
#endif

  model->Estep();    // E Step

  
  model->Mstep();    // M Step

#if DEBUG > 0
  cout<<"Apres la 1ere iteration de EM :"<<endl;
  model->editDebugInformation();
#endif
  _indexIteration++;
	
  while (continueAgain()){
    model->Estep();    // E Step
    model->Mstep();    // M Step
#if DEBUG > 0
  cout<<"Apres la "<<_indexIteration<<" eme iteration de EM :"<<endl;
  model->editDebugInformation();
#endif
    _indexIteration++;
    _xml_old = _xml;
    _xml = model->getLogLikelihood(true);  // true : to compute fik

  }

    model->Estep();    // E step to update Tik an fik

}

