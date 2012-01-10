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
#include "XEMClusteringMain.h"
#include "XEMEstimation.h"
#include "XEMClusteringModelOutput.h"

using namespace std;

class XEMClusteringOutput{

public:	

  /// Default Constructor
  XEMClusteringOutput();

  
  /// Copy Constructor
  XEMClusteringOutput(const XEMClusteringOutput & cOutput);
  
  /// Initialisation constructor
  XEMClusteringOutput(vector<XEMEstimation*> & estimations);

  /// Destructor
  virtual ~XEMClusteringOutput();

  
  bool atLeastOneEstimationNoError() const;
  
  /// sort vector of XEMClusteringModelOutput (with the ith criterion value)
  void sort(XEMCriterionName criterionName);
  
  void editFile() const;
  
  /// return the index'th' ClusteringModelOutput
  /// Note : index is between 0 and size(ClusteringModelOutput)-1
  XEMClusteringModelOutput * getClusteringModelOutput(int64_t index) const;
  
  int64_t getNbClusteringModelOutput() const;
  
  
  XEMErrorType getError() const;
  
  void setError(XEMErrorType & error);
  
  void setClusteringModelOutput(vector<XEMClusteringModelOutput *> & clusteringModelOutput);
  
  
  private :
    vector<XEMClusteringModelOutput *> _clusteringModelOutput;
    
    XEMErrorType _error;
    
};



inline  XEMErrorType XEMClusteringOutput::getError() const{
  return _error;
}

inline  void XEMClusteringOutput::setError(XEMErrorType & error){
  _error = error;
}

inline  XEMClusteringModelOutput *  XEMClusteringOutput::getClusteringModelOutput(int64_t index) const{
  if (index>=0 && index<_clusteringModelOutput.size()) {
    return _clusteringModelOutput[index];
  }
  else{
    throw wrongIndexInGetMethod;
  }
}

//
inline int64_t XEMClusteringOutput::getNbClusteringModelOutput() const{
  return _clusteringModelOutput.size();
}


#endif
