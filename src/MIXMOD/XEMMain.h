/***************************************************************************
                             SRC/MIXMOD/XEMMain.h  description
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
#ifndef XEMMMAIN_H
#define XEMMMAIN_H

/*#include <iostream.h>*/

#include "XEMOldInput.h"
#include "XEMOutput.h"
#include "XEMEstimation.h"
#include "XEMSelection.h"
#include "XEMError.h"
#include "XEMDCVCriterion.h"

/**
	@brief Main class for Mixmod
	@author F Langrognet & A Echenim
  */

class XEMMain{

public:

	/// Default constructor
	XEMMain();

  /// Constructor
	XEMMain(XEMOldInput * input);
	
	/// Destructor
	virtual ~XEMMain();

	
	/** @brief Selector
      @return The number of Selections
  */
  int64_t getNbSelection();

  /** @brief Selector
      @return The table of Selections
  */
	XEMSelection **& getTabSelection();

  /** @brief Selector
      @return The number of Estimations
  */
  int64_t getNbEstimation();

  /** @brief Selector
      @return The table of estimations
  */
	XEMEstimation **& getTabEstimation();

  /** @brief Selector
      @return The number of numbers of Clusters
  */	
  int64_t getNbNbCluster();

  /** @brief Selector
      @return The number of Models
  */
  int64_t getNbModelType();

	/// Run method
	void run(XEMOutput * output = NULL);
	
        /// getDCVCriterion
        XEMDCVCriterion * getDCVCriterion();
   
private :

  /// Number of criteria
  int64_t _nbCriterion;

  /// Number of numbers of clusters
  int64_t _nbNbCluster;

  /// Number of models
  int64_t _nbModelType;

  /// Number of estimations
  int64_t _nbEstimation;

  /// Table of estimations
	XEMEstimation ** _tabEstimation; // aggregate

  /// Number of selections
  int64_t _nbSelection;

  /// Table of selections
	XEMSelection ** _tabSelection; // aggregate


};




inline int64_t XEMMain::getNbSelection(){
	return _nbSelection;
}

inline int64_t XEMMain::getNbEstimation(){
	return _nbEstimation;
}

inline int64_t XEMMain::getNbNbCluster(){
	return _nbNbCluster;
}

inline int64_t XEMMain::getNbModelType(){
	return _nbModelType;
}

inline XEMSelection **& XEMMain::getTabSelection(){
	return _tabSelection;
}

inline XEMEstimation **& XEMMain::getTabEstimation(){
	return _tabEstimation;
}

#endif
