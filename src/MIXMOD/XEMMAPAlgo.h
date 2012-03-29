/***************************************************************************
                             SRC/MIXMOD/XEMMAPAlgo.h  description
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
#ifndef XEMMAPALGO_H
#define XEMMAPALGO_H

#include "XEMAlgo.h"

/**
  @brief Derived class of XEMAlgo for MAP Algorithm
  @author F Langrognet
  */

class XEMMAPAlgo : public XEMAlgo{

public:
 
	/// Default constructor
	XEMMAPAlgo();
        
  // copy constructor
  XEMMAPAlgo(const XEMMAPAlgo & mapAlgo);
  
  /// Constructor
	XEMMAPAlgo(XEMAlgoStopName algoStopName, double epsilon, int64_t nbIteration);
 
	/// Destructor
	virtual ~XEMMAPAlgo();

        
  /// clone
  virtual XEMAlgo * clone();

	/// Run method
	virtual void run(XEMModel *& model);

  virtual const XEMAlgoName getAlgoName() const;
        
  virtual void setEpsilon(double epsilon);
    
  virtual const double getEpsilon() const;
        
protected :

  /// Number of clusters
  int64_t _nbCluster;
		
};

inline const XEMAlgoName XEMMAPAlgo::getAlgoName() const{
  return MAP;
}

inline const double XEMMAPAlgo::getEpsilon() const{
  throw internalMixmodError;
}

inline void XEMMAPAlgo::setEpsilon(double eps){
  throw internalMixmodError;
}

#endif
