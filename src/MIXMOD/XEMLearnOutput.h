/***************************************************************************
 SRC/MIXMOD/XEMLearnOutput.h  description
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
#ifndef XEMLEARNOUTPUT_H
#define XEMLEARNOUTPUT_H


#include "XEMUtil.h"

// pre-declaration
class XEMCriterion;
class XEMModel;
class XEMLearnMain;
class XEMLearnModelOutput;

using namespace std;

class XEMLearnOutput{
  
public:	
  
  /// Default Constructor
  XEMLearnOutput();
  
  
  /// Copy Constructor
  XEMLearnOutput(const XEMLearnOutput & lOutput);
  
  /// Initialisation constructor
  XEMLearnOutput( std::vector<XEMModel*> const & estimations );
  
  /// Destructor
  virtual ~XEMLearnOutput();
  
  /// Comparison operator
  bool operator ==(const XEMLearnOutput & output) const;
  
  bool atLeastOneEstimationNoError() const;
  const int getNbEstimationWithNoError() const;
  
  /// sort the model output
  void sort(XEMCriterionName criterionName);
  
  void editFile() const;
  
  /// return the index'th' ClusteringModelOutput
  /// Note : index is between 0 and size(ClusteringModelOutput)-1
  XEMLearnModelOutput * getLearnModelOutput(unsigned int index) const;
  
  std::vector<XEMLearnModelOutput*> const & getLearnModelOutput() const;
  
  int64_t getNbLearnModelOutput() const;
  
  void setLearnModelOutput(vector<XEMLearnModelOutput *> & learnModelOutput);
  
private :
  // Vector containing output for each model
  std::vector<XEMLearnModelOutput *> _learnModelOutput;
};


inline  std::vector<XEMLearnModelOutput*> const & XEMLearnOutput::getLearnModelOutput() const{
  return _learnModelOutput;
}

inline  XEMLearnModelOutput *  XEMLearnOutput::getLearnModelOutput(unsigned int index) const{
  if (index<_learnModelOutput.size()) {
    return _learnModelOutput[index];
  }
  else{
    throw wrongIndexInGetMethod;
  }
}

//
inline int64_t XEMLearnOutput::getNbLearnModelOutput() const{
  return _learnModelOutput.size();
}



#endif
