/***************************************************************************
                             SRC/MIXMOD/XEMCondExe.h  description
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
#ifndef XEMCondExe_H
#define XEMCondExe_H

#include "XEMUtil.h"
#include "XEMOldInput.h"
#include "XEMClusteringInput.h"
#include "XEMEstimation.h"
#include "XEMSelection.h"
class XEMClusteringOutput;

/**
  @brief Base class for XEMCondExe(s)
  @author F Langrognet & A Echenim
  */

class XEMCondExe{

public:

	/// Default constructor
	XEMCondExe();

  /// constructor
  XEMCondExe(XEMOldInput * input, XEMEstimation ** tabEstimation, int64_t nbEstimation);

  /// constructor
  XEMCondExe(XEMClusteringInput * input,  XEMClusteringOutput * clusteringOutput);

	/// Destructor
	virtual ~XEMCondExe();


  /// editTabEstimationError
  void editTabEstimationError(ofstream & oFile);

  /// editTabCriterionError
  void editTabCriterionError(ofstream & oFile, int64_t iCrit);

  ///update
  void update(int64_t nbEstimation, XEMEstimation ** tabEstimation, int64_t nbSelection, XEMSelection ** tabSelection);


    /// data
      XEMData * _data;
	
   /// weight
        double * _weight;
	
  /// Number of (different) samples
        int64_t _nbSample;

  ///Weight total (number of all samples)
	double _weightTotal;

  /// Problem dimension
  int64_t _pbDimension;

  /// Number of numbers of cluster
  int64_t _nbNbCluster;

  /// Table of numbers of cluster
  int64_t * _tabNbCluster;

  /// Number of criteria
  int64_t _nbCriterion;

  /// Table of criterion name
  XEMCriterionName * _tabCriterionName;

  /// Number of models
  int64_t _nbModel;

  /// Table of model type
        XEMModelType ** _tabModel;

  /// Strategy 
  XEMStrategy * _strategy;
  /// Strategy 
  XEMClusteringStrategy * _cStrategy;

	/// knownPartition
	XEMPartition * _knownPartition;
	
  /// Error code for each estimation (nbModel *  nbNbCluster)
  XEMErrorType * _tabEstimationError;

  /// Error code for each selection (size : nbCriterion)
  XEMErrorType * _tabSelectionError;

  /// Error code for each criterion and for each estimation : table in 2 dimensions
  XEMErrorType ** _tabCriterionError;

  /// Global Mixmod Error
  XEMErrorType _errorMixmod;
 	
};
#endif
