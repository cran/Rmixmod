/***************************************************************************
 SRC/MIXMOD/XEMPredictStrategy.h  description
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
#ifndef XEMPredictStrategy_H
#define XEMPredictStrategy_H


/**
 @brief Base class for Strategy(s)
 @author Rémi Lebret
 */


// class pre-declaration
class XEMModel;
class XEMParameter;

class  XEMPredictStrategy
{
public:
  
	/// Default constructor
  XEMPredictStrategy(XEMParameter * classificationRule);
  
  /// Constructor
  XEMPredictStrategy(const XEMPredictStrategy & strategy);
  
	/// Destructor
  ~XEMPredictStrategy();
  
	/// Run method
  void run(XEMModel * model);
  
  // verify method
  bool verify();
  
  // get pointer to the algorithm
  XEMParameter * getClassificationRule() const;
  
private:
  // classification rule
  XEMParameter * _classificationRule;
  
};
// get pointer to the algorithm
inline XEMParameter * XEMPredictStrategy::getClassificationRule() const{
  return _classificationRule;
}

#endif
