/***************************************************************************
                             SRC/MIXMOD/XEMICLCriterion.cpp  description
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
#include "XEMICLCriterion.h"
#include "XEMCriterionOutput.h"
#include "XEMModel.h"



//------------
// Constructor
//------------
XEMICLCriterion::XEMICLCriterion(XEMModel * model) : XEMCriterion(model)
{}




//-----------
// Destructor
//-----------
XEMICLCriterion::~XEMICLCriterion(){
}


//---
//run
//---
void XEMICLCriterion::run(XEMCriterionOutput & output)
{
 /* Compute ICL (Integrated Completed Likelihood) Criterion */
  
  // initialize value
  double value = 0.0;
  // initialize error
  XEMErrorType error = noError;
    
  try{
    const double completedLoglikelihood = _model->getCompletedLogLikelihood();
    const int64_t freeParameter = _model->getFreeParameter();
    const double logN  = _model->getLogN();

    //_value =  -2.0*loglikelihood  -(2.0*completedLoglikelihood) + (freeParameter*logN);
    value =   -2.0*completedLoglikelihood + freeParameter*logN;
  }
  catch(XEMErrorType & e){
    error = e;
  }
  // add name to criterion output
  output.setCriterionName(ICL);
  // add value to criterion output
  output.setValue(value);
  // add error to criterion output
  output.setError(error);
}
