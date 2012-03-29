/***************************************************************************
 SRC/MIXMOD/XEMPredictModelOutput.h  description
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
#ifndef XEMPREDICTMODELOUTPUT_H
#define XEMPREDICTMODELOUTPUT_H


#include "XEMModelOutput.h"

// pre-declaration
class XEMModel;

class XEMPredictModelOutput : public XEMModelOutput{
  
public:
  
  /// Default Constructor
  XEMPredictModelOutput();
  
  /// Initialization Constructor 1
  XEMPredictModelOutput(XEMModel * estimation);
  
  /// Initialization Constructor 2
  XEMPredictModelOutput(XEMModelType & modelType, int64_t nbCluster, vector<XEMCriterionOutput> & criterionOutput, double likelihood, XEMParameterDescription & parameterDescription, XEMLabelDescription & labelDescription,  XEMProbaDescription & probaDescription);
  
  /// Initialization Constructor 3
  XEMPredictModelOutput(XEMModelType & modelType, int64_t nbCluster, XEMErrorType error);
  
  /// Copy Constructor
  XEMPredictModelOutput(const XEMPredictModelOutput & cModelOutput);
  
  
  /// Destructor
  virtual ~XEMPredictModelOutput();
  
};


#endif
