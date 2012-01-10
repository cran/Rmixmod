/***************************************************************************
                             SRC/MIXMOD/XEMAlgo.h  description
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
#ifndef XEMALGO_H
#define XEMALGO_H

#include "XEMModel.h"
#include "XEMUtil.h"
#include "XEMData.h"
#include "XEMPartition.h"


/**
        @brief Base class for Algorithm(s)
        @author F Langrognet & A Echenim
 */

class XEMAlgo
{

  public:

                /// Default constructor
    XEMAlgo();
    
    /// copy constructor
    XEMAlgo(const XEMAlgo & algo);

                /// Constructor
    XEMAlgo ( XEMAlgoStopName algoStopName, double epsilon, int64_t nbIteration);

                /// Destructor
    virtual ~XEMAlgo();

    /// clone
   virtual XEMAlgo * clone() = 0;
    
                /// Run method
    virtual void run(XEMModel *& model) = 0;

    
    void edit(ofstream & oFile);
    
    virtual const XEMAlgoStopName getAlgoStopName() const;
    
    virtual const XEMAlgoName getAlgoName() const = 0;
    
    virtual void setAlgoStopName(XEMAlgoStopName algoStopName);
    
    virtual void setNbIteration(int64_t nbIteration);
    
    virtual const int64_t getNbIteration() const;
    
    virtual void setEpsilon(double epsilon);
    
    virtual const double getEpsilon() const;
    
    friend ostream & operator << (ostream & fo, XEMAlgo & algo);
    
  protected :
    
    
    /// Type of stopping rule of the algorithm
    XEMAlgoStopName _algoStopName;

    /// Number of iterations
    int64_t _nbIteration;

    /// Current iteration number
    int64_t _indexIteration;

    /// Value of Epsilon (default 1.e-4)
    double _epsilon;

    
    
                /** @brief Selector
    @return     1 if algorithm not reached else 0
                 */
    bool continueAgain();

    double _xml_old;

    double _xml ;

#if SAVE_ALL_MODELS
    XEMModel ** _tabModel;         // aggregate
#endif

};

inline void XEMAlgo::setAlgoStopName(XEMAlgoStopName algoStopName){
  _algoStopName = algoStopName;
}

inline void XEMAlgo::setNbIteration(int64_t nbIteration){
  if (nbIteration<minNbIteration){
    throw nbIterationTooSmall;
  }
  else if (nbIteration>maxNbIteration){
    throw nbIterationTooLarge;
  }
  else{
    _nbIteration = nbIteration;
  }
}


inline const int64_t XEMAlgo::getNbIteration() const{
  return _nbIteration;
}

inline void XEMAlgo::setEpsilon(double epsilon){
  if (epsilon<minEpsilon){
    throw epsilonTooSmall;
  }
  else if (epsilon>maxEpsilon){
    throw epsilonTooLarge;
  }
  else{
    _epsilon = epsilon;
  }
}


inline const double XEMAlgo::getEpsilon() const{
  return _epsilon;
}

inline const XEMAlgoStopName XEMAlgo::getAlgoStopName() const{
  return _algoStopName;
}


// others fucntions
XEMAlgo * createDefaultClusteringAlgo();

#endif
