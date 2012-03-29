/***************************************************************************
                             SRC/MIXMOD/XEMClusteringOutput.h  description
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
#ifndef XEMCLUSTERINGOUTPUT_H
#define XEMCLUSTERINGOUTPUT_H


#include "XEMUtil.h"

// pre-declaration
class XEMModel;
class XEMCriterion;
class XEMClusteringModelOutput;

class XEMClusteringOutput{

public:	

  /// Default Constructor
  XEMClusteringOutput(std::vector<XEMCriterionName> const & criterionName);

  
  /// Copy Constructor
  XEMClusteringOutput(const XEMClusteringOutput & cOutput);
  
  /// Initialisation constructor
  XEMClusteringOutput(std::vector<XEMModel*> const & estimations, std::vector<XEMCriterionName> const & criterionName);

  /// Destructor
  virtual ~XEMClusteringOutput();

  /// Comparison operator
  bool operator ==(const XEMClusteringOutput & output) const;
  
  bool atLeastOneEstimationNoError() const;
  
  const int getNbEstimationWithNoError() const;
  
  /// sort vector of XEMClusteringModelOutput (with the ith criterion value)
  void sort(XEMCriterionName criterionName);
  
  void editFile() const;
  
  /// return the index'th' ClusteringModelOutput
  /// Note : index is between 0 and size(ClusteringModelOutput)-1
  XEMClusteringModelOutput * getClusteringModelOutput(const int64_t index) const;
  
  int64_t getNbClusteringModelOutput() const;
  
  std::vector<XEMClusteringModelOutput*> const &  getClusteringModelOutput() const;
  
  void setClusteringModelOutput(vector<XEMClusteringModelOutput *> & clusteringModelOutput);
  
  const int getCriterionSize() const;
  const XEMCriterionName & getCriterionName(const int index) const;
  const std::vector<XEMCriterionName> & getCriterionName() const;
  
private :
  // Vector containing output for each model
  std::vector<XEMClusteringModelOutput*> _clusteringModelOutput;
  // vector containing criterion name
  // that will be useful to deal with output in mixmodGUI
  std::vector<XEMCriterionName> const & _criterionName;
};


inline  std::vector<XEMClusteringModelOutput*> const & XEMClusteringOutput::getClusteringModelOutput() const{
  return _clusteringModelOutput;
}

inline  XEMClusteringModelOutput * XEMClusteringOutput::getClusteringModelOutput(const int64_t index) const{
  return _clusteringModelOutput[index];
}

//
inline int64_t XEMClusteringOutput::getNbClusteringModelOutput() const{
  return _clusteringModelOutput.size();
}

inline const int XEMClusteringOutput::getCriterionSize() const{
  return _criterionName.size();
}

inline const XEMCriterionName & XEMClusteringOutput::getCriterionName(const int index) const{
  return _criterionName[index];
}

inline const std::vector<XEMCriterionName> & XEMClusteringOutput::getCriterionName() const{
  return _criterionName;
}

#endif
