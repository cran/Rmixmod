/***************************************************************************
 SRC/MIXMOD/XEMPredictOutput.h  description
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
#ifndef XEMPREDICTOUTPUT_H
#define XEMPREDICTOUTPUT_H


#include "XEMUtil.h"

// pre-declaration
class XEMModel;
class XEMPredictModelOutput;

class XEMPredictOutput{
  
public:	
  
  /// Default Constructor
  XEMPredictOutput();
  
  
  /// Copy Constructor
  XEMPredictOutput(const XEMPredictOutput & lOutput);
  
  /// Initialisation constructor
  XEMPredictOutput(XEMModel * estimation);
  
  /// Destructor
  virtual ~XEMPredictOutput();
  
  /// Comparison operator
  bool operator ==(const XEMPredictOutput & output) const;
  
  /// return the index'th' ClusteringModelOutput
  /// Note : index is between 0 and size(ClusteringModelOutput)-1
  XEMPredictModelOutput * getPredictModelOutput(unsigned int index) const;
  
  std::vector<XEMPredictModelOutput *> const & getPredictModelOutput() const;
  
private :
  // Vector containing output for each model
  std::vector<XEMPredictModelOutput *> _predictModelOutput;
};

inline std::vector<XEMPredictModelOutput *> const & XEMPredictOutput::getPredictModelOutput() const{
  return _predictModelOutput;
}

inline XEMPredictModelOutput * XEMPredictOutput::getPredictModelOutput(unsigned int index) const{
  if ( index<=_predictModelOutput.size() ){
    return _predictModelOutput[index];
  }
  else{
    throw wrongCriterionPositionInGet;
  }
}


#endif
