/***************************************************************************
                             SRC/MIXMOD/XEMGaussianSample.h  description
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
#ifndef XEMGAUSSIANSample_H
#define XEMGAUSSIANSample_H

#include "XEMSample.h"


/**
  @brief Base class for Sample
  @author F Langrognet & A Echenim
  */

class XEMGaussianSample : public XEMSample{

public:

  /// Constructor
  XEMGaussianSample();

  /// Constructor
  XEMGaussianSample(int64_t pbDimension);

  /// Constructor
  XEMGaussianSample(XEMGaussianSample * iSample);

  /// Constructor
  XEMGaussianSample(int64_t pbDimension, double * tabValue);

  /// Destructor
  virtual ~XEMGaussianSample();
  

  /// Set value vector of sample
  void setDataTabValue(double * tabValue);

  /// Set one value of sample
  void setDataValue(int64_t idxDim, double value);

  /// get value vector of sample
  double * getTabValue() const;

  /// get one value of sample
  double getDataValue(int64_t idxDim) const;


protected :
  
  /// Vector of sample value
  double * _value;


};



//---------------
// inline methods
//---------------

inline double * XEMGaussianSample::getTabValue() const{
  return _value;
}


inline double XEMGaussianSample::getDataValue(int64_t idxDim) const{
  return _value[idxDim];
}

#endif
