/***************************************************************************
                             SRC/MIXMOD/XEMSEMAlgo.h  description
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
#ifndef XEMSEMALGO_H
#define XEMSEMALGO_H

#include "XEMAlgo.h"

/**
  @brief Derived class of XEMAlgo for SEM Algorithm(s)
  @author F Langrognet & A Echenim
  */

class XEMSEMAlgo : public XEMAlgo{

public:
 
	/// Default constructor
  XEMSEMAlgo();

  /// Copy constructor
  XEMSEMAlgo(const XEMSEMAlgo & semAlgo);

  /// Constructor
	XEMSEMAlgo(XEMAlgoStopName algoStopName, int64_t nbIteration);

	/// Destructor
	virtual ~XEMSEMAlgo();

    /// clone
        XEMAlgo * clone();
        
	/// Run method
	void run(XEMModel *& model);

        const XEMAlgoName getAlgoName() const;
        
        
        void setAlgoStopName(XEMAlgoStopName * algoStopName);
        
        void setNbIteration(int64_t nbIteration);
        
        void setEpsilon(double epsilon);
    
        const double getEpsilon() const;
};

inline const XEMAlgoName XEMSEMAlgo::getAlgoName() const{
  return SEM;
}

inline const double XEMSEMAlgo::getEpsilon() const{
  throw internalMixmodError;
}

inline void XEMSEMAlgo::setEpsilon(double eps){
  throw internalMixmodError;
}

#endif
