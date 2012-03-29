/***************************************************************************
 SRC/MIXMOD/XEMLearnInput.h  description
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
#ifndef XEMLEARNINPUT_H
#define XEMLEARNINPUT_H

#include "XEMInput.h"

class XEMLearnInput : public XEMInput{
  
public:	
  
  /// Default Constructor
  XEMLearnInput();
  
  /// Copy Constructor
  XEMLearnInput(const XEMLearnInput & CInput);
  
  /// Initialisation constructor
  XEMLearnInput(XEMDataDescription * learnData, XEMLabelDescription * knownLabelDescription);
  
  /// Destructor
  virtual ~XEMLearnInput();
  
  // Accessors
  const int64_t getNbCVBlock() const;
  
  /// setCriterionName
  virtual void setCriterion(std::vector<XEMCriterionName> const & criterionName);
  
  /// setCriterion
  virtual void setCriterion(const XEMCriterionName criterionName, unsigned int index);
  
  ///insertCriterion
  virtual void insertCriterion(const XEMCriterionName criterionName, unsigned int index);
  
  ///addCriterion
  virtual void addCriterion(const XEMCriterionName criterionName);
  
  
  /// set the number of CV blocks
  void setNbCVBlock(int64_t nbCVBlock);
  
protected : 
  /// verif
  virtual bool verif();
  
// private 
private :
  // number of CV blocks
  int64_t _nbCVBlock;
};


inline const int64_t XEMLearnInput::getNbCVBlock() const{
  return _nbCVBlock;
}

#endif
