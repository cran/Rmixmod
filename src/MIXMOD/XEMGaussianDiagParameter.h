/***************************************************************************
                             SRC/MIXMOD/XEMGaussianDiagParameter.h  description
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
#ifndef XEMGaussianDiagParameter_H
#define XEMGaussianDiagParameter_H

#include "XEMGaussianEDDAParameter.h"

// pre-declaration
class XEMDiagMatrix;

/**
  @brief Derived class of XEMGaussianParameter for Spherical Gaussian Model(s)
  @author F Langrognet
  */

class XEMGaussianDiagParameter : public XEMGaussianEDDAParameter{

public:

  /// Default constructor
  XEMGaussianDiagParameter();

  /// Constructor
  // called by XEMModel
  XEMGaussianDiagParameter(XEMModel * iModel, XEMModelType * iModelType);

  /// Constructor (copy)
  XEMGaussianDiagParameter(const XEMGaussianDiagParameter * iParameter);

  /// Destructor
  virtual ~XEMGaussianDiagParameter();

	/// reset to default values
	virtual void reset();

  /** @brief Selector
      @return A copy of the model
  */
  XEMParameter * clone() const;
 
	/// initialisation USER
  void initUSER(XEMParameter * iParam);
	
  /// Compute table of sigmas of the samples of each cluster
  // NB : compute also lambda, shpae, orientation, wk, w
  void computeTabSigma();
  
  
  //     SELECTORS
  // ------ / -------- //
  
  double * getTabLambda() const;
  
  //double ** getTabShape();
  XEMDiagMatrix** getTabShape() const;

  double getLogLikelihoodOne() const;

  int64_t getFreeParameter() const;

protected :
  /// Table of volume of each cluster
  double * _tabLambda;               /* Volume      */

  //double ** _tabShape;  
   XEMDiagMatrix** _tabShape;


};

inline double * XEMGaussianDiagParameter::getTabLambda() const{
  return _tabLambda;
}

inline XEMDiagMatrix** XEMGaussianDiagParameter::getTabShape() const{
  return _tabShape;
}
#endif
