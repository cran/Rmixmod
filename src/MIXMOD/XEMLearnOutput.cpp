/***************************************************************************
 SRC/MIXMOD/XEMLearnOutput.cpp  description
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

#include "XEMLearnOutput.h"
#include "XEMLearnModelOutput.h"
#include "XEMBICCriterion.h"
#include "XEMCVCriterion.h"
#include "XEMError.h"

#include <algorithm>


//--------------------
// Default Constructor
//--------------------
XEMLearnOutput::XEMLearnOutput()
{}


//-----------------
//  Copy constructor
//-----------------
XEMLearnOutput::XEMLearnOutput( const XEMLearnOutput & lOutput )
                              : _learnModelOutput(lOutput.getLearnModelOutput())
{}


//---------------------------
// Initialisation Constructor
//---------------------------
XEMLearnOutput::XEMLearnOutput( std::vector<XEMModel*> const & estimations )
                              : _learnModelOutput(estimations.size())
{
  int64_t sizeEstimation = estimations.size();
  // loop over the estimations
  for (unsigned int i=0; i<sizeEstimation; i++){
    _learnModelOutput[i] = new XEMLearnModelOutput(estimations[i]);
  }  
}



//-----------
// Destructor
//-----------
XEMLearnOutput::~XEMLearnOutput(){
  for (unsigned int i=0; i<_learnModelOutput.size(); i++){
    delete _learnModelOutput[i];
  }
}


//---------------------
/// Comparison operator
//---------------------
bool XEMLearnOutput::operator ==(const XEMLearnOutput & output) const{
  
  for (unsigned int i=0; i<_learnModelOutput.size(); i++){
    if ( !(_learnModelOutput[i] == output.getLearnModelOutput(i)) )
      return false;
  }
  return true;
}

//--------------------
// XEMLearnOutput
//--------------------
bool XEMLearnOutput::atLeastOneEstimationNoError() const{
  bool res = false;
  unsigned int i=0;
  while (res==false && i<_learnModelOutput.size()){
    if (_learnModelOutput[i]->getStrategyRunError() == noError){
      res = true;
    }
    i++;
  }
  return res;
}


const int XEMLearnOutput::getNbEstimationWithNoError() const{
  int n=getNbLearnModelOutput();
  for( unsigned int i=0; i<_learnModelOutput.size(); i++ ){
    if (_learnModelOutput[i]->getStrategyRunError() ){ --n; }
  }
  return n;
}

//-----
// sort
//-----
void XEMLearnOutput::sort(XEMCriterionName criterionName){
  
  // sort
  std::sort(_learnModelOutput.begin(), _learnModelOutput.end(), SortByCriterion(criterionName));
}

void XEMLearnOutput::editFile() const{
  //TODO 
}


void XEMLearnOutput::setLearnModelOutput(vector<XEMLearnModelOutput *> & learnModelOutput){
  for (unsigned int i=0; i<_learnModelOutput.size(); i++){
    delete _learnModelOutput[i];
  }
  _learnModelOutput = learnModelOutput;
}

