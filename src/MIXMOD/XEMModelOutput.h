/***************************************************************************
                             SRC/MIXMOD/XEMModelOutput.h  description
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
#ifndef XEMMODELOUTPUT_H
#define XEMMODELOUTPUT_H


#include "XEMUtil.h"
#include "XEMEstimation.h"
#include "XEMModel.h"
#include "XEMParameterDescription.h"
#include "XEMProbaDescription.h"
#include "XEMLabelDescription.h"


using namespace std;

/* Note :
A XEMModelOutput object could be created :
- with an XEMEstimation (after calculation)
- or without XEMEstimation. In this case, XEMModelOutput is created from XML mixmod file which contains input and output information. 
*/


class XEMModelOutput{

public:

  /// Default Constructor
  XEMModelOutput();
  
  /// Copy Constructor
  XEMModelOutput(const XEMModelOutput & modelOutput);
  
  /// Initialization Constructor 1
  XEMModelOutput(XEMEstimation * estimation);
  
  /// Initialization Constructor 2
  XEMModelOutput(XEMModelType & modelType, int64_t nbCluster, vector<XEMCriterionOutput> & criterionOutput, double likelihood, XEMParameterDescription & parameterDescription, XEMLabelDescription & labelDescription,  XEMProbaDescription & probaDescription);
  
  ///Initialization Constructor 3
  XEMModelOutput(XEMModelType & modelType, int64_t nbCluster, XEMErrorType error);
  
  /// Destructor
  virtual ~XEMModelOutput();

  
  // --- get --- ///
  //-------------//
  XEMModelType getModelType() const; 
  
  int64_t getNbCluster() const;
  
  int64_t getCriterionSize() const;
  
  vector<XEMCriterionOutput> getCriterionOutput();
  
  XEMParameterDescription  * getParameterDescription() ;
  
  XEMLabelDescription * getLabelDescription() ;
  
  XEMProbaDescription * getProbaDescription() ;
  
  XEMErrorType getStrategyRunError() const;
  
  XEMEstimation * getEstimation();
  
  double getLikelihood();
  
  // TODO a enlever
  friend class XEMOutput;
  
  friend class XEMClusteringOutput;
  
  
  
  
  //-------
  protected :
    
  XEMEstimation * _estimation;
  
  
  XEMModelType _modelType; 
  
  int64_t _nbCluster;
  
  vector<XEMCriterionOutput > _criterionOutput;
  
  XEMParameterDescription  * _parameterDescription;
  
  XEMLabelDescription * _labelDescription;
  
  XEMProbaDescription * _probaDescription;
  
  double _likelihood;
    
  XEMErrorType _strategyRunError;
};


inline XEMModelType XEMModelOutput::getModelType() const{
  return _modelType;
}

inline int64_t XEMModelOutput::getNbCluster() const{
 return _nbCluster;
}



inline  int64_t XEMModelOutput::getCriterionSize() const{
  return _criterionOutput.size();
}


inline vector<XEMCriterionOutput> XEMModelOutput::getCriterionOutput(){
    return _criterionOutput;
}


inline  XEMParameterDescription  * XEMModelOutput::getParameterDescription() {
  return _parameterDescription;
}
  
inline  XEMLabelDescription * XEMModelOutput::getLabelDescription() {
  return _labelDescription;
}
  
inline  XEMProbaDescription * XEMModelOutput::getProbaDescription() {
  return _probaDescription;
}

inline  XEMErrorType XEMModelOutput::getStrategyRunError() const{
  return _strategyRunError;
}

inline  XEMEstimation * XEMModelOutput::getEstimation(){
  if (_estimation){
    return _estimation;
  }
  else{
    throw nullPointerError;
  }
}

inline double XEMModelOutput::getLikelihood()
{
  return _likelihood;
}


/*

inline const XEMProbaOutput * const XEMModelOutput::getProbaOutput() const{
  if (_model){
    return _model->getProbaOutput();
  }
  else{
    throw nullPointerEroor;
  }
}*/

#endif
