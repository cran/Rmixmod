/***************************************************************************
                             SRC/MIXMOD/XEMOldModelOutput.h  description
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
#ifndef XEMOldModelOutput_H
#define XEMOldModelOutput_H

#include "XEMSelection.h"
#include "XEMEstimation.h"
#include "XEMCriterionOutput.h"
#include "XEMUtil.h"
#include "XEMLikelihoodOutput.h"
#include "XEMProbaOutput.h"

/**
  @brief Base class for XEMOldModelOutput(s)
  @author F Langrognet & A Echenim
  */

class XEMOldModelOutput{

public:

	/// Default constructor
	XEMOldModelOutput();

	/// Destructor
	virtual ~XEMOldModelOutput();


  /// setErrorType
  void setErrorType(XEMErrorType error);

  /// udpate
  void update(XEMSelection * selection, XEMEstimation ** tabEstimation, int64_t nbModelType);

  /// update //TODO
   void update1(XEMEstimation * bestEstimation, int64_t indexCriterion);
 
  /// update
  void update(XEMEstimation * estimation, int64_t  nbSelection, XEMSelection ** tabSelection);
  
  /// update //TODO
  void update2(XEMEstimation * estimation);

  /// Model Type
  XEMModelType * _modelType;

	/// rank of the best model type
  int64_t  _rankOfBestModelType;

  /// nb Cluster
  int64_t  _nbCluster;

  /// strategy
  XEMStrategy * _strategy;
  /// strategy
  XEMClusteringStrategy * _cStrategy;

	
  /// number of criterion  (size of _tabCriterionOutput)
  int64_t  _nbCriterionOutput;

  /// Table of CriterionOutput
  XEMCriterionOutput ** _tabCriterionOutput;

  /// parameter
  XEMParameter * _param;

  /// probability
  XEMProbaOutput * _probaOutput;

  /// Likelihood
  XEMLikelihoodOutput * _likelihoodOutput;

  /// error code for this model, this number of class and this strategy
  // (for table of all or best model)
  XEMErrorType _errorType;


  XEMModelType * getModelType() const;
  int64_t  getNbCluster() const;
  XEMParameter* getParameter() const;
  XEMProbaOutput * getProbaOutput() const;
  XEMLikelihoodOutput * getLikelihoodOutput() const;
  XEMCriterionOutput** getTabCriterionOutput() const;
  XEMCriterionOutput* getTabCriterionOutput(int64_t  index) const;
  int64_t  getNbCriterionOutput() const;

};

  inline XEMModelType * XEMOldModelOutput::getModelType() const{
    return _modelType;
  }

  inline int64_t  XEMOldModelOutput::getNbCluster() const{
    return _nbCluster;
  }

  inline XEMParameter* XEMOldModelOutput:: getParameter() const{
    return _param;
  }

  inline XEMProbaOutput * XEMOldModelOutput::getProbaOutput() const{
    return _probaOutput;
  }

  inline  XEMLikelihoodOutput * XEMOldModelOutput::getLikelihoodOutput() const{
    return _likelihoodOutput;
  }

  inline int64_t  XEMOldModelOutput::getNbCriterionOutput() const{
    return _nbCriterionOutput;
  }


  inline XEMCriterionOutput** XEMOldModelOutput::getTabCriterionOutput() const{
    return _tabCriterionOutput;
  }

  inline XEMCriterionOutput* XEMOldModelOutput::getTabCriterionOutput(int64_t  index) const{
    return _tabCriterionOutput[index];
  }







#endif
