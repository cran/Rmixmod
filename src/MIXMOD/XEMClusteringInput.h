/***************************************************************************
                             SRC/MIXMOD/XEMClusteringInput.h  description
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
#ifndef XEMCLUSTERINGINPUT_H
#define XEMCLUSTERINGINPUT_H


#include "XEMInput.h"

// pre-declaration
class XEMClusteringStrategy;

class XEMClusteringInput : public XEMInput{

public:	

  /// Default Constructor
  XEMClusteringInput();

  /// Copy Constructor
  XEMClusteringInput(const XEMClusteringInput & CInput);
  
  /// Initialisation constructor
  XEMClusteringInput(const std::vector<int64_t> & iNbCluster, const XEMDataDescription & iDataDescription);

  /// Destructor
  virtual ~XEMClusteringInput();
  
  // getStrategy
  XEMClusteringStrategy * getStrategy() const;
  
    // setStrategy
  void setStrategy(XEMClusteringStrategy * strat);
  
  
  /// setCriterionName
  virtual void setCriterion(std::vector<XEMCriterionName> const & criterionName);
  
  /// setCriterionName
  virtual void setCriterion(const XEMCriterionName criterionName, unsigned int index);
  
  ///insertCriterionName[i]
  virtual void insertCriterion(const XEMCriterionName criterionName, unsigned int index);
  
  // add a new criterion
  void addCriterion(const XEMCriterionName criterionName);
  
protected : 
  /// verif
  virtual bool verif();
  // Clustering strategy
  XEMClusteringStrategy * _strategy;
};

// getStrategy
inline XEMClusteringStrategy * XEMClusteringInput::getStrategy() const{
  return _strategy;
}

#endif
