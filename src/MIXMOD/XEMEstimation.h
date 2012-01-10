/***************************************************************************
                             SRC/MIXMOD/XEMEstimation.h  description
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
#ifndef XEMESTIMATION_H
#define XEMESTIMATION_H


#include "XEMUtil.h"
#include "XEMModel.h"
#include "XEMCriterionOutput.h"
#include "XEMClusteringStrategy.h"

class XEMCriterion;

/**
  @brief Base class for Estimation(s)
  @author F Langrognet & A Echenim
  */

class XEMEstimation{

public:

  /// Default constructor
 XEMEstimation();

 /// copy constructor
 XEMEstimation(const XEMEstimation & estimation);
 
 
  /// Constructor
 XEMEstimation(XEMClusteringStrategy *& strategy, XEMModelType * modelType, int64_t nbCluster, XEMData *& data, vector<XEMCriterionName> & criterionName, XEMPartition *& knownPartition, vector<int64_t> corresp);
  
 
  /// Constructor
 XEMEstimation(XEMStrategy *& strategy, XEMModelType * modelType, int64_t nbCluster, XEMData *& data, XEMPartition *& knownPartition, vector<int64_t> corresp);
  
  /// Destructor
  virtual ~XEMEstimation();

	
  /** @brief Selector
     @return The best Model of the current strategy
  */
  XEMModel * getModel();
  
  
  /// get Parameter
  XEMParameter * getParameter() const;

  /// get model type
  XEMModelType * getModelType();

  /// get nbCluster
  int64_t getNbCluster();

  /** @brief Set the error type estimation
      @param errorType Type of error to set
  */
  void setErrorType(XEMErrorType errorType);

  /** @brief Selector
      @return The type of the error
  */
  XEMErrorType getErrorType();
	
  /// get the strategy 
  XEMClusteringStrategy * getClusteringStrategy();
  /// get the strategy 
  XEMStrategy * getStrategy();
  
  //getcorrespondenceOriginDataToReduceData
  const vector<int64_t> & getcorrespondenceOriginDataToReduceData()const;

  /// Run method
  void run();
  
  /// 
  int64_t getCriterionSize() const;
  
  //XEMCriterionName getCriterionName(int64_t index) const;
  
  /// getCriterionOutput
  vector<XEMCriterionOutput> getCriterionOutput() ;
  
  /// getCriterionOutput(index)
  XEMCriterionOutput & getCriterionOutput(int64_t index) ;
  
  // getData
  XEMData * getData();
  
  
  /// getNbSample
  int64_t getNbSample() const;
	
/** @brief Friend method
      @return Operator << overloaded to write Estimation in output files
*/
  friend ostream & operator << (ostream & fo, XEMEstimation & estimation);

private :

  /// Current strategy
  XEMClusteringStrategy * _cStrategy;
  
  /// Current strategy
  XEMStrategy * _strategy;

  /// Number of cluster
  int64_t _nbCluster;

  /// Current model type
  XEMModelType * _modelType;

  /// Current model error
  XEMErrorType _errorType;
  
  /// model
  XEMModel * _model;
  
  vector<int64_t> _correspondenceOriginDataToReduceData;

  vector<XEMCriterion*> _criterion;
  vector<XEMCriterionOutput> _criterionOutput;

};



//---------------
// inline methods
//---------------

inline XEMModel * XEMEstimation::getModel(){
  return (_model);
}

inline XEMData * XEMEstimation::getData(){
  return (_model->getData());
}

inline XEMModelType * XEMEstimation::getModelType(){
  return _modelType;
}

inline int64_t XEMEstimation::getNbCluster(){
  return _nbCluster;
}


inline  int64_t XEMEstimation::getNbSample() const{
  return (_model->getNbSample());
}

inline XEMClusteringStrategy * XEMEstimation::getClusteringStrategy(){
  return _cStrategy;
}
inline XEMStrategy * XEMEstimation::getStrategy(){
  return _strategy;
}

inline XEMErrorType XEMEstimation::getErrorType(){
  return (_errorType);
}

inline const vector<int64_t> & XEMEstimation::getcorrespondenceOriginDataToReduceData()const{
  return _correspondenceOriginDataToReduceData;
}

inline XEMParameter * XEMEstimation::getParameter() const{
  if (_model){
    return _model->getParameter();
  }
  else{
    throw nullPointerError;
  }
}
  
inline  int64_t XEMEstimation::getCriterionSize() const{
  return _criterion.size();
}

inline vector<XEMCriterionOutput> XEMEstimation::getCriterionOutput(){
  return _criterionOutput;
}

inline XEMCriterionOutput & XEMEstimation::getCriterionOutput(int64_t index){
  if (index>=0 && index<_criterionOutput.size()){
    return _criterionOutput[index];
  }
  else{
    internalMixmodError;
  }
}


#endif
