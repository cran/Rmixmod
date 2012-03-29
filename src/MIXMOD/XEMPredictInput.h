/***************************************************************************
 SRC/MIXMOD/XEMPredictInput.h  description
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
#ifndef XEMPREDICTINPUT_H
#define XEMPREDICTINPUT_H

#include "XEMInput.h"

// pre-declaration
class XEMPredictStrategy;
class XEMParameter;
class XEMParameterDescription;

class XEMPredictInput : public XEMInput{
  
public:	
  
  /// Default Constructor
  XEMPredictInput();
  
  /// Copy Constructor
  XEMPredictInput(const XEMPredictInput & CInput);
  
  /// Initialisation constructor
  XEMPredictInput(XEMDataDescription * predictData, XEMParameterDescription * classificationRule);
  
  /// Destructor
  virtual ~XEMPredictInput();
  
  // accessor
  XEMParameter * getClassificationRule() const;
  
  /// setCriterionName
  virtual void setCriterion(std::vector<XEMCriterionName> const & criterionName);
  
  /// setCriterion
  virtual void setCriterion(const XEMCriterionName criterionName, unsigned int index);
  
  ///insertCriterion
  virtual void insertCriterion(const XEMCriterionName criterionName, unsigned int index);
  
  ///addCriterion
  virtual void addCriterion(const XEMCriterionName criterionName);
  
  /// getCriterion
  virtual XEMCriterionName getCriterionName(unsigned int index) const;
  
  // remove criterion
  virtual void removeCriterion(unsigned int index);
  
  // set model type
  virtual void setModelType(const XEMModelType * modelType, unsigned int index);
  
  // insert model type
  virtual void insertModelType(const XEMModelType * modelType, unsigned int index);
  
  // remove model type
  virtual void removeModelType(unsigned int index);
  
  // add model type
  virtual void addModel(XEMModelName const modelName );

  
protected : 
  /// pointer to a classification rule
  XEMParameter * _classificationRule;
  /// verif
  virtual bool verif();  
};


// accessor
inline XEMParameter * XEMPredictInput::getClassificationRule() const{
  return _classificationRule;
}

#endif
