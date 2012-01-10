/***************************************************************************
                             SRC/MIXMOD/XEMNECCriterion.cpp  description
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
#include "XEMNECCriterion.h"

//------------
// Constructor
//------------
XEMNECCriterion::XEMNECCriterion(){
}




//-----------
// Destructor
//-----------
XEMNECCriterion::~XEMNECCriterion(){}




//---
//run
//---
void XEMNECCriterion::run(XEMModel * model, double & value, XEMErrorType & error){

  error = noError;
  
  try{
    /* Compute NEC (An Entropy Criterion) */
    value = 0;
    if (model->getNbCluster() == 1){
      value = 1;
    }
    else{
      double entropy          = model->getEntropy();
      double loglikelihood    = model->getLogLikelihood(false);  // false : to not compute fik because already done
      double loglikelihoodOne = model->getLogLikelihoodOne();
      if (fabs(loglikelihood-loglikelihoodOne) < minValueForLLandLLOne){
        throw pbNEC;
      }  
      value = entropy / (loglikelihood-loglikelihoodOne);
    }
  }
  catch(XEMErrorType & e){
    error = e;
  }
}

