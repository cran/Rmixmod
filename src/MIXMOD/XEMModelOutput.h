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
#include "XEMModelType.h"
#include "XEMCriterionOutput.h"
#include "XEMParameter.h"
#include "XEMParameterDescription.h"

// pre-declaration
class XEMProbaDescription;
class XEMLabelDescription;

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
  XEMModelOutput(XEMModel * estimation);
  
  /// Initialization Constructor 2
  XEMModelOutput(XEMModelType & modelType, int64_t nbCluster, vector<XEMCriterionOutput> & criterionOutput, double likelihood, XEMParameterDescription & parameterDescription, XEMLabelDescription & labelDescription,  XEMProbaDescription & probaDescription);
  
  ///Initialization Constructor 3
  XEMModelOutput(XEMModelType & modelType, int64_t nbCluster, XEMErrorType error);
  
  /// Destructor
  virtual ~XEMModelOutput();
  
  /// Comparison operator
  bool operator ==(const XEMModelOutput & modelOutput) const;
  
  // --- get --- ///
  //-------------//
  XEMModelType getModelType() const; 
  
  int64_t getNbCluster() const;
    
  XEMParameterDescription  * getParameterDescription() const ;
  
  XEMLabelDescription * getLabelDescription() const ;
  
  XEMProbaDescription * getProbaDescription() const ;
  
  XEMErrorType getStrategyRunError() const;
  
  XEMModel * getModel() const;
  
  double getLikelihood() const;
  
  XEMCriterionOutput const & getCriterionOutput(XEMCriterionName criterionName) const;
  XEMCriterionOutput const & getCriterionOutput(const int index) const;
  XEMCriterionOutput & getCriterionOutput(XEMCriterionName criterionName);
  
  // set criterion output
  void setCriterionOutput( XEMCriterionOutput const & criterionOutput );
  
  //-------
  protected :
  
  // criterion output
  XEMCriterionOutput _criterionOutput[maxNbCriterion];
  
  // type of the model
  XEMModelType _modelType; 
  
  // the number of cluster
  int64_t _nbCluster;
  
  // parameter description for that model
  XEMParameterDescription  * _parameterDescription;
  
  // labels for the model
  XEMLabelDescription * _labelDescription;
  
  // the probabilities of the model
  XEMProbaDescription * _probaDescription;
  
  // the model likelyhood
  double _likelihood;
    
  // the error
  XEMErrorType _strategyRunError;
};


inline XEMModelType XEMModelOutput::getModelType() const{
  return _modelType;
}

inline int64_t XEMModelOutput::getNbCluster() const{
  return _nbCluster;
}

inline  XEMParameterDescription  * XEMModelOutput::getParameterDescription() const {
  return _parameterDescription;
}
  
inline  XEMLabelDescription * XEMModelOutput::getLabelDescription() const {
  return _labelDescription;
}
  
inline  XEMProbaDescription * XEMModelOutput::getProbaDescription() const {
  return _probaDescription;
}

inline  XEMErrorType XEMModelOutput::getStrategyRunError() const {
  return _strategyRunError;
}

inline double XEMModelOutput::getLikelihood() const {
  return _likelihood;
}

inline XEMCriterionOutput const & XEMModelOutput::getCriterionOutput(XEMCriterionName criterionName) const{
  return _criterionOutput[criterionName];
}

inline XEMCriterionOutput const & XEMModelOutput::getCriterionOutput(const int index) const{
  return _criterionOutput[index];
}

inline XEMCriterionOutput & XEMModelOutput::getCriterionOutput(XEMCriterionName criterionName){
  return _criterionOutput[criterionName];
}


// Define structure to sort by criterion name
struct SortByCriterion 
{ 
  // Constructor
  SortByCriterion(XEMCriterionName criterionName) : _criterionName(criterionName) {}
  
  // Destructor
  ~SortByCriterion(){}
  
  // operator()
  inline bool operator ()( const XEMModelOutput * m1, const XEMModelOutput * m2 ) const 
  {
    const XEMErrorType error1 = m1->getCriterionOutput(_criterionName).getError();
    const XEMErrorType error2 = m2->getCriterionOutput(_criterionName).getError();
    if ( error1 & error2) return false;
    if ( error1 ) return false;
    if ( error2 ) return true;
    const double value1 = m1->getCriterionOutput(_criterionName).getValue();
    const double value2 = m2->getCriterionOutput(_criterionName).getValue();
    if ( value1 == value2 ) { return m1->getParameterDescription()->getParameter()->getFreeParameter() < m2->getParameterDescription()->getParameter()->getFreeParameter(); }
    else{ return value1 < value2; }
  } 
  
private:
  // criterion name
  XEMCriterionName _criterionName;
}; 


#endif
