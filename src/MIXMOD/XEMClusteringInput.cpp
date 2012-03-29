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
#include "XEMClusteringStrategy.h"

//--------------------
// Default Constructor
//--------------------
XEMClusteringInput::XEMClusteringInput()
{
  _strategy = new XEMClusteringStrategy();
}


//-----------------
//  Copy constructor
//-----------------
XEMClusteringInput::XEMClusteringInput( const XEMClusteringInput & cInput ) 
                                      : XEMInput(cInput)
{
  _strategy = new XEMClusteringStrategy(*cInput.getStrategy());
}



//---------------------------
// Initialisation Constructor
//---------------------------
XEMClusteringInput::XEMClusteringInput( const vector<int64_t> & iNbCluster
                                      , const XEMDataDescription & iDataDescription
                                      ) 
                                      : XEMInput(iNbCluster, iDataDescription)
{
  _strategy = new XEMClusteringStrategy();
}


//-----------
// Destructor
//-----------
XEMClusteringInput::~XEMClusteringInput()
{
  if ( _strategy ) delete _strategy;
}


//-----------
// setStrategy
//-----------
void XEMClusteringInput::setStrategy(XEMClusteringStrategy * strat)
{
  _strategy = new XEMClusteringStrategy(*strat);
}

//setCriterion
//----------------
void XEMClusteringInput::setCriterion(const XEMCriterionName criterionName, unsigned int index){
  if (index<_criterionName.size()){
    switch (criterionName) {
      case BIC : _criterionName[index] = BIC; break;
      case CV :  throw XEMDAInput;
      case ICL : _criterionName[index] = ICL; break;
      case NEC : _criterionName[index] = NEC; break;
      case UNKNOWN_CRITERION_NAME : throw internalMixmodError;break;
      default : throw internalMixmodError;
    }
  }
  else{
    throw wrongCriterionPositionInSet;
  }
  _finalized = false;
}

//setCriterion
//----------------
void XEMClusteringInput::setCriterion(std::vector<XEMCriterionName> const & criterionName){
   
  // copy vector contents
  _criterionName = criterionName;
  
  // check vector contents
  for ( unsigned int iCriterion=0; iCriterion<_criterionName.size(); iCriterion++ ){
    switch (_criterionName[iCriterion]) {
      case BIC : break;
      case CV :  throw XEMDAInput;
      case ICL : break;
      case NEC : break;
      case UNKNOWN_CRITERION_NAME : throw internalMixmodError;break;
      default : throw internalMixmodError;
    }
  }
  _finalized = false;
}

// insertCriterion
//-----------------
void XEMClusteringInput::insertCriterion(const XEMCriterionName criterionName, unsigned int index){
  if (index<=_criterionName.size()){
    switch (criterionName) {
      case BIC : _criterionName.insert (_criterionName.begin()+index , BIC); break;
      case CV :  throw XEMDAInput;
      case ICL : _criterionName.insert (_criterionName.begin()+index , ICL); break;
      case NEC : _criterionName.insert (_criterionName.begin()+index , NEC); break;
      case UNKNOWN_CRITERION_NAME : throw internalMixmodError;break;
      default : throw internalMixmodError;
    }
  }
  else{
    throw wrongCriterionPositionInInsert;
  }
  _finalized = false;
}


// add Criterion
//-----------------
void XEMClusteringInput::addCriterion(const XEMCriterionName criterionName){
  
  bool found=false;
  for (unsigned int iCriterion=0; iCriterion<_criterionName.size(); iCriterion++ ){
    if ( _criterionName[iCriterion] == criterionName ) found = true;
  }
  if (!found) {
    switch (criterionName) {
      case BIC : _criterionName.push_back(BIC); break;
      case CV :  throw XEMDAInput;
      case ICL : _criterionName.push_back(ICL); break;
      case NEC : _criterionName.push_back(NEC); break;
      case UNKNOWN_CRITERION_NAME : throw internalMixmodError;break;
      default : throw internalMixmodError;
    }
  }
  _finalized = false;
}


// ----------------
// Verif
//-----------------
bool XEMClusteringInput::verif(){
  bool res = XEMInput::verif();
  return res;
}

