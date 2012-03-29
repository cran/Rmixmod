/***************************************************************************
                             SRC/MIXMOD/XEMLikelihoodOutput.cpp  description
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
#include "XEMLikelihoodOutput.h"
#include "XEMModel.h"

//------------
// Constructor
//------------
XEMLikelihoodOutput::XEMLikelihoodOutput(){
}

XEMLikelihoodOutput::XEMLikelihoodOutput(double logLikelihood, double completeLogLikelihood, double entropy, int64_t nbFreeParam){
  _logLikelihood = logLikelihood;
  _completeLogLikelihood = completeLogLikelihood;
  _entropy = entropy;
  _nbFreeParam = nbFreeParam;
}



//------------
// Constructor
//------------
XEMLikelihoodOutput::XEMLikelihoodOutput(XEMModel * model){
	_logLikelihood = model->getLogLikelihood(false);  // false : to not compute fik because already done
  _completeLogLikelihood = model->getCompletedLogLikelihood();
  _entropy = model->getEntropy();
  _nbFreeParam = model->getFreeParameter();
}


		
//----------
//Destructor
//----------
XEMLikelihoodOutput::~XEMLikelihoodOutput(){
}



//----
//edit
//----
void XEMLikelihoodOutput::edit(std::ofstream & oFile, bool text){
  if(text){
    oFile<<"\t\t\tNumber of Free Parameters : "<<_nbFreeParam<<endl;
    oFile<<"\t\t\tLog-Likelihood : "<<_logLikelihood<<endl;
    oFile<<"\t\t\tComplete Log-Likelihood : "<<_completeLogLikelihood<<endl;
    oFile<<"\t\t\tEntropy : "<<_entropy<<endl;
  }
  else{
    oFile<<_nbFreeParam<<endl;
    oFile<<_logLikelihood<<endl;
    oFile<<_completeLogLikelihood<<endl;
    oFile<<_entropy<<endl;
  }
}
