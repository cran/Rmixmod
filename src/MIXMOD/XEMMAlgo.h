/***************************************************************************
                             SRC/MIXMOD/XEMMAlgo.h  description
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
#ifndef XEMMALGO_H
#define XEMMALGO_H

#include "XEMAlgo.h"

/**
  @brief Derived class of XEMAlgo for M Algorithm
  @author F Langrognet
  */

class XEMMAlgo : public XEMAlgo{

public:
 
	/// Default constructor
	XEMMAlgo();

  /// copy constructor
  XEMMAlgo(const XEMMAlgo & mAlgo);
        
  /// Constructor
	XEMMAlgo(XEMAlgoStopName algoStopName, double epsilon, int64_t nbIteration);
 
	/// Destructor
	virtual ~XEMMAlgo();

  /// clone
  virtual XEMAlgo * clone();

	/// Run method
	virtual void run(XEMModel *& model);
        
  virtual const XEMAlgoName getAlgoName() const;
        	
  virtual void setEpsilon(double epsilon);
    
  virtual const double getEpsilon() const;
};


inline const XEMAlgoName XEMMAlgo::getAlgoName() const{
  return M;
}

inline const double XEMMAlgo::getEpsilon() const{
  throw internalMixmodError;
}

inline void XEMMAlgo::setEpsilon(double eps){
  throw internalMixmodError;
}

#endif
