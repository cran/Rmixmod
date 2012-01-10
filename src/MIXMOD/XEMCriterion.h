/***************************************************************************
                             SRC/MIXMOD/XEMCriterion.h  description
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
#ifndef XEMCriterion_H
#define XEMCriterion_H

#include "XEMModel.h"
#include <stdio.h>


/**
	@brief Base class for Criterion
	@author F Langrognet & A Echenim
  */

//-----------------------------------
// best value are always the samllest
//-----------------------------------

class XEMCriterion{

public:

	/// Default constructor
	XEMCriterion();

	/// Destructor
	virtual ~XEMCriterion();


	/**@brief Selector
		 @return The value of the criterion
	*/
	double getValue();


  /** @brief Set the error type criterion
      @param errorType Type of error to set
  */
  void setErrorType(XEMErrorType errorType);

  /** @brief Selector
      @return The type of the error
  */
  XEMErrorType getErrorType();

  virtual XEMCriterionName getCriterionName()const=0 ;
  
  /// Run method
  virtual void run(XEMModel * model, double & value, XEMErrorType & error)=0;


};



#endif
