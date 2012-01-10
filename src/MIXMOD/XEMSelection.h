/***************************************************************************
                             SRC/MIXMOD/XEMSelection.h  description
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
#ifndef XEMSELECTION_H
#define XEMSELECTION_H


#include "XEMUtil.h"
#include "XEMCriterion.h"
#include "XEMEstimation.h"
#include "XEMClusteringInput.h"

/**
  @brief Base class for Selection(s)
  @author F Langrognet & A Echenim
  */

class XEMSelection{

public: 
	
	/// Default Constructor
	XEMSelection();

  /// Constructor
  XEMSelection(XEMCriterionName criterionName, XEMEstimation **& tabEstimation, int64_t nbEstimation, XEMClusteringInput * input);
  
  /// Constructor
  XEMSelection(XEMCriterionName criterionName, XEMEstimation **& tabEstimation, int64_t nbEstimation, XEMOldInput * input);
  
	/// Destructor
	virtual ~XEMSelection();

	
	/** @brief Selector
      @return The type of the current criterion
  */
	XEMCriterionName getCriterionName();
		
  /** @brief Set the error type criterion
      @param errorType Type of error to set
  */
  void setErrorType(XEMErrorType errorType);

  /** @brief Selector
      @return The type of the error
  */
  XEMErrorType getErrorType();

  /// getBestIndexEstimation
  int64_t getBestIndexEstimation();

  /// getCriterionErrorType for estimation
  XEMErrorType getCriterionErrorType(XEMEstimation *& estimation);
  
  /// getCriterionErrorType for jth estimation
  XEMErrorType getCriterionErrorType(int64_t j);
  
  /// getCriterionValue for estimation
  double getCriterionValue(XEMEstimation *& estimation);

  /// getBestCriterionValue
  double getBestCriterionValue();

  /// getCriterion
  XEMCriterion * getCriterion();


  /// getCVLabel of the best Estimation
  int64_t * getCVLabelOfBestEstimation();
 
  XEMErrorType * getTabCriterionErrorForEachEstimation();
  
  double * getTabCriterionValueForEachEstimation();
   
  /// Run method
  void run();

  
     
private :

  /// Select the best estimation
  void selectBestEstimation();

  
  
  ///  Best index in _tabEstimation for best estimation
  int64_t _bestIndexEstimation;

  /// Name of the current criterion
  XEMCriterionName _criterionName;

  /// Number of estimation
  int64_t _nbEstimation;

  /// Table of estimation
  XEMEstimation ** _tabEstimation;
  
  
  
  /// XEMCriterion 
  XEMCriterion * _criterion;  // aggregate

  
  ///
  double * _tabCriterionValueForEachEstimation;
  ///
  XEMErrorType * _tabCriterionErrorForEachEstimation;
  
  
  /// Current error : if all criterion have error
  XEMErrorType _errorType;
  
  
  /// int64_t ** CV label of each estimation (only in cv or dcv configuration)
  int64_t ** _CVLabelOfEachEstimation;
  
  /// int64_t * CV label of best estimation (only in cv or dcv configuration)
  int64_t * _CVLabelOfBestEstimation;
  
};



inline XEMCriterionName XEMSelection::getCriterionName(){
  return _criterionName;
}

inline XEMErrorType XEMSelection::getErrorType(){
  return(_errorType);
}

inline int64_t XEMSelection::getBestIndexEstimation(){
   return _bestIndexEstimation;
}

inline XEMCriterion * XEMSelection::getCriterion(){
  return _criterion;
}

inline XEMErrorType * XEMSelection::getTabCriterionErrorForEachEstimation(){
	return _tabCriterionErrorForEachEstimation;
}

inline double * XEMSelection::getTabCriterionValueForEachEstimation(){
	return _tabCriterionValueForEachEstimation;
}

#endif
