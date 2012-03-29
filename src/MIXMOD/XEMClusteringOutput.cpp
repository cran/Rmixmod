/***************************************************************************
                             SRC/MIXMOD/XEMClusteringOutput.cpp  description
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
#include "XEMClusteringOutput.h"
#include "XEMClusteringModelOutput.h"
#include "XEMModel.h"
#include "XEMBICCriterion.h"
#include "XEMICLCriterion.h"
#include "XEMNECCriterion.h"

#include <algorithm>

//--------------------
// Default Constructor
//--------------------
XEMClusteringOutput::XEMClusteringOutput(std::vector<XEMCriterionName> const & criterionName) : _criterionName(criterionName)
{}

  
//-----------------
//  Copy constructor
//-----------------
XEMClusteringOutput::XEMClusteringOutput( const XEMClusteringOutput & cOutput )
                                        : _clusteringModelOutput(cOutput.getClusteringModelOutput())
                                        , _criterionName(cOutput.getCriterionName())
{}
  

//---------------------------
// Initialisation Constructor
//---------------------------
XEMClusteringOutput::XEMClusteringOutput( std::vector<XEMModel*> const & estimations
                                        , std::vector<XEMCriterionName> const & criterionName
                                        )
                                        : _clusteringModelOutput(estimations.size())
                                        , _criterionName(criterionName)
{
  // get a constant of the number of estimations
  const int64_t sizeEstimation = estimations.size();
  
  // loop over the estimations
  for (unsigned int i=0; i<sizeEstimation; i++){
    _clusteringModelOutput[i] = new XEMClusteringModelOutput(estimations[i]);
  }
  
}



//-----------
// Destructor
//-----------
XEMClusteringOutput::~XEMClusteringOutput()
{
  for (unsigned int i=0; i<_clusteringModelOutput.size(); i++){
    delete _clusteringModelOutput[i];
  }
}


//---------------------
/// Comparison operator
//---------------------
bool XEMClusteringOutput::operator ==(const XEMClusteringOutput & output) const{
  
  for (unsigned int i=0; i<_clusteringModelOutput.size(); i++){
    if ( !(_clusteringModelOutput[i] == output.getClusteringModelOutput(i)) )
      return false;
  }
  return true;
}

//--------------------
// XEMClusteringOutput
//--------------------
bool XEMClusteringOutput::atLeastOneEstimationNoError() const{
  bool res = false;
  unsigned int i=0;
  while (res==false && i<_clusteringModelOutput.size()){
    if ( _clusteringModelOutput[i]->getStrategyRunError() == noError ){
      res = true;
    }
    i++;
  }
  return res;
}

const int XEMClusteringOutput::getNbEstimationWithNoError() const{
  int n=getNbClusteringModelOutput();
  for( unsigned int i=0; i<_clusteringModelOutput.size(); i++ ){
    if (_clusteringModelOutput[i]->getStrategyRunError() ){ --n; }
  }
  return n;
}

//-----
// sort
//-----
void XEMClusteringOutput::sort(XEMCriterionName criterionName){
  
  // sort
  std::sort(_clusteringModelOutput.begin(), _clusteringModelOutput.end(), SortByCriterion(criterionName));
/*
  int64_t i,j;
 i=0;
 
 //find indexCriterion
 int64_t indexCriterion = -1;
 XEMEstimation * estimation0 = _clusteringModelOutput[0]->_estimation;
 while (i<estimation0->getCriterionSize()){
    if (estimation0->getCriterionOutput(i).getCriterionName() == criterionName){
      indexCriterion = i;
      break;
    }
    else{
      i++;
    }
  }
  if (i==estimation0->getCriterionSize()){
    throw internalMixmodError;
  }
 
 
  // treatment criterion Error
  //--------------------------
  int64_t nbCriterionError = 0;
  for (i=0; i<_clusteringModelOutput.size()-nbCriterionError; i++){
    vector<XEMCriterionOutput> criterionOutput = _clusteringModelOutput[i]->getCriterionOutput();
    if (criterionOutput[indexCriterion].getError() != noError){
      XEMClusteringModelOutput * tmp = _clusteringModelOutput[i];
      _clusteringModelOutput.erase(_clusteringModelOutput.begin()+i);
      _clusteringModelOutput.push_back(tmp);
      nbCriterionError++;
    }
  }
 
  for (i=0; i<_clusteringModelOutput.size()-nbCriterionError; i++){
    //find bestCriterionValue
    //cout<<"i : "<<i<<endl;
    int64_t bestIndex = i;
    vector<XEMCriterionOutput> criterionOutput = _clusteringModelOutput[bestIndex]->getCriterionOutput();
    double bestCriterionValue = criterionOutput[indexCriterion].getValue();
    for (j=i+1; j<_clusteringModelOutput.size(); j++){ 
      vector<XEMCriterionOutput> criterionOutputJ = _clusteringModelOutput[j]->getCriterionOutput();
      if (criterionOutputJ[indexCriterion].getError() == noError){
        double criterionValueJ = criterionOutputJ[indexCriterion].getValue();
        if (bestCriterionValue > criterionValueJ){
          //  cout<<"if ... bestCriterionValue : "<<bestCriterionValue<<endl;
          bestIndex = j;
          bestCriterionValue = criterionValueJ;
        } 
        else{
          if (bestCriterionValue == criterionValueJ){
            // the model with less parameter is choosen
            XEMEstimation * estimationJ = NULL;
            XEMEstimation * bestEstimation = NULL;
            try{
              estimationJ = _clusteringModelOutput[j]->getEstimation();
              bestEstimation = _clusteringModelOutput[bestIndex]->getEstimation();
            }
            catch (XEMError & e){
              throw wrongSortCallInXEMModelOutput;
            }
            int64_t freeParameterJ = estimationJ->getModel()->getFreeParameter();
            int64_t bestFreeParameter = bestEstimation->getModel()->getFreeParameter();
            if (freeParameterJ < bestFreeParameter){
              // if this Estimation is better 
              //  cout<<"if ... bestCriterionValue : "<<bestCriterionValue<<endl;
              bestIndex = j;
              bestCriterionValue = criterionValueJ;
            }
          }
        }
      }// end if // noError
    }
    // swap
    if (bestIndex != i){
      XEMClusteringModelOutput * tmp = _clusteringModelOutput[i];
      _clusteringModelOutput[i] = _clusteringModelOutput[bestIndex];
      _clusteringModelOutput[bestIndex] = tmp;
    }
  }
  */
//    cout<<"Apres le tri - size : "<<_clusteringModelOutput.size()<<endl;
//      cout<<((_clusteringModelOutput[0]->_estimation->getCriterionOutput())[indexCriterion])->_criterionName<<endl;
//    for (i=0; i<_clusteringModelOutput.size(); i++){
//      cout<<((_clusteringModelOutput[i]->_estimation->getCriterionOutput())[indexCriterion])->_value<<endl;
//    }
  
  
  /*
  !! Old Version !!
 vector<XEMClusteringModelOutput*> clusteringModelOutputTmp;
 clusteringModelOutputTmp.resize(_clusteringModelOutput.size());
 for (i=0; i<_clusteringModelOutput.size(); i++){
   clusteringModelOutputTmp[i] = _clusteringModelOutput[i];
 }
 
  for (i=0; i<_clusteringModelOutput.size(); i++){
//     cout<<"i = "<<i<<endl;
    //find best ClusteringModelOutput
    int64_t bestIndex = 0;
    vector<XEMCriterionOutput*> criterionOutput = clusteringModelOutputTmp[bestIndex]->getCriterionOutput();
    double bestCriterionValue = criterionOutput[indexCriterion]->_value;
//     cout<<"bestCriterionValue : "<<bestCriterionValue<<endl;
    for (j=1; j<clusteringModelOutputTmp.size(); j++){ 
    
      vector<XEMCriterionOutput*> criterionOutputJ = clusteringModelOutputTmp[j]->getCriterionOutput();
      double criterionValueJ = criterionOutputJ[indexCriterion]->_value;
//       cout<<"j : "<<j<<"criterionValueJ : "<<criterionValueJ<<endl;
      if (bestCriterionValue > criterionValueJ){
//         cout<<"if ... bestCriterionValue : "<<bestCriterionValue<<endl;
        bestIndex = j;
        bestCriterionValue = criterionValueJ;
      }
    }
    _clusteringModelOutput[i] = clusteringModelOutputTmp[bestIndex];
    clusteringModelOutputTmp.erase(clusteringModelOutputTmp.begin()+bestIndex);
//     cout<<"best Value : "<<bestCriterionValue<<endl;
//     cout<<"size de clusteringModelOutputTmp : "<<clusteringModelOutputTmp.size()<<endl;
  }
  
//   cout<<"size : "<<_clusteringModelOutput.size()<<endl;
//     cout<<((_clusteringModelOutput[0]->_estimation->getCriterionOutput())[indexCriterion])->_criterionName<<endl;
//   for (i=0; i<_clusteringModelOutput.size(); i++){
//     cout<<((_clusteringModelOutput[i]->_estimation->getCriterionOutput())[indexCriterion])->_value<<endl;
//   }
//   int64_t tmp=1;
 */
}


void XEMClusteringOutput::editFile() const{
  //TODO 
}


void XEMClusteringOutput::setClusteringModelOutput(vector<XEMClusteringModelOutput *> & clusteringModelOutput){
  for (unsigned int i=0; i<_clusteringModelOutput.size(); i++){
    delete _clusteringModelOutput[i];
  }
  _clusteringModelOutput = clusteringModelOutput;
}

