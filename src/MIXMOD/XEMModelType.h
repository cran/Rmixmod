/***************************************************************************
                             SRC/MIXMOD/XEMModelType.h  description
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
#ifndef XEMMODELTYPE_H
#define XEMMODELTYPE_H

#include "XEMUtil.h"

/**
  @brief Base class for ModelType(s)
  @author F Langrognet & A Echenim
  */

class XEMModelType{

public:

  /// Default constructor
  XEMModelType();

  // constructor
  XEMModelType(XEMModelName name,int64_t nbSubDimensionFree=0);

  // copy constructor
  XEMModelType(const XEMModelType & iModelType);
  
  /// Destructor
  ~XEMModelType();
  
  
  /// Comparison operator
  bool operator ==(const XEMModelType & modelType) const;
  
  /// Input model type
  void input(ifstream & fi, int64_t nbCluster);

  /// name of the model
  XEMModelName _nameModel;

  XEMModelType* clone();

  //// list of number of subDimensionEqual
  //int64_t _nbSubDimensionEqual;
  //// list of number of subDimensionFree
  //int64_t _nbSubDimensionFree;

  /// list of subDimensionEqual
  int64_t  _subDimensionEqual;

  /// _nbSubDimensionFree : size of array _tabSubDimensionFree
  int64_t _nbSubDimensionFree;

  /// array of subDimensionFree
  int64_t * _tabSubDimensionFree;

  /// getModelName
  const XEMModelName & getModelName() const;

  /// getSubDimensionEqual
  const int64_t & getSubDimensionEqual() const;

  /// getTabSubDimensionFree
  const int64_t * getTabSubDimensionFree() const;

  ///getTabSubDimensionFreeI
  const int64_t & getTabSubDimensionFreeI(int64_t index) const;

  /// setSubDimensionFree
  void setTabSubDimensionFree(int64_t iTabSubDimensionFree, int64_t position) ;

  /// setSubDimensionEqual
  void setSubDimensionEqual(int64_t  iSubDimensionEqual) ;

  /// <<
  friend ostream & operator<<(ostream & fo, XEMModelType & modelType);

  // print out model type
  void print(ostream & flux);
  // print out model type short cut
  void printShortcut(ostream & flux);
  /// editModelType
  void edit(ostream & oFile);
  
};

inline const XEMModelName & XEMModelType::getModelName() const{
  return _nameModel;
}

inline const int64_t & XEMModelType::getSubDimensionEqual() const{
  return _subDimensionEqual;
}

inline const int64_t * XEMModelType::getTabSubDimensionFree() const{
  return _tabSubDimensionFree;
}

inline const int64_t & XEMModelType::getTabSubDimensionFreeI(int64_t index) const{
  return _tabSubDimensionFree[index];
}

inline void XEMModelType::setTabSubDimensionFree(int64_t iTabSubDimensionFree, int64_t position) {
  if (_tabSubDimensionFree==NULL){
    _tabSubDimensionFree = new int64_t[_nbSubDimensionFree];
  }
  _tabSubDimensionFree[position] = iTabSubDimensionFree;
}

inline void XEMModelType::setSubDimensionEqual(int64_t iSubDimensionEqual) {
  _subDimensionEqual = iSubDimensionEqual;
}

#endif
