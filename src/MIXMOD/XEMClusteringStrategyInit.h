/***************************************************************************
                             SRC/MIXMOD/XEMClusteringStrategyInit.h  description
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
#ifndef XEMClusteringStrategyInit_H
#define XEMClusteringStrategyInit_H

#include "XEMUtil.h"
#include "XEMParameter.h"
#include "XEMPartition.h"

/**
  @brief Base class for StrategyInitType(s)
  @author F Langrognet 
  */

class XEMClusteringStrategyInit{

public:

  /// Default constructor
  XEMClusteringStrategyInit();
  
  /// Copy constructor
  XEMClusteringStrategyInit(const XEMClusteringStrategyInit & strategyInit);

  /// Destructor
  virtual ~XEMClusteringStrategyInit();

  
  /// getStrategyInitName
  const XEMStrategyInitName & getStrategyInitName() const;
  
  /// setStrategyInitName
  void setStrategyInitName(XEMStrategyInitName initName);
  
  
  
  /// getNbTry
  const int64_t getNbTry() const;
  
  /// setNbTry
  void setNbTry(int64_t nbTry);
  
  
  
  /// getNbIteration
  const int64_t getNbIteration() const;
  
  /// set NbIteration
  void setNbIteration(int64_t nbIteration);
  
  
  
  /// getEpsilon
  const double getEpsilon() const;
  
  /// setEpsilon
  void setEpsilon(double epsilon);
  
  
  /// getStopName
  const XEMAlgoStopName getStopName() const;
  
  /// setStopName
  void setStopName(XEMAlgoStopName stopName);
  
  /* Parameter */
  //-----------//
  /// getNbInitParameter
  const int64_t & getNbInitParameter() const;

  /// getTabInitParameter
  const XEMParameter ** getTabInitParameter() const;

  /// getTabInitParameter
  XEMParameter * getInitParameter(int64_t index) const;
  
  /// setInitParam
  void setInitParam(string & paramFileName, int64_t position);
  
  /// setTabInitParam
  void setTabInitParameter(XEMParameter ** tabInitParameter, int64_t nbInitParameter);
  
  
  /* Partition */
  //-----------//
  ///  getNbPartition
  const int64_t & getNbPartition() const;

  /// getTabPartition
  const XEMPartition ** getTabPartition() const;

  /// getTabPartition
  XEMPartition * getPartition(int64_t index) const;
  
  ///set Init Partition
  void setPartition(XEMPartition * part, int64_t position);
  
  ///set Init Partition
  void setPartition(string & paramFileName, int64_t position);
  
  /// setTabPartition
  void setTabPartition(XEMPartition ** tabPartition, int64_t nbPartition);
  
  
  // input
  void input(ifstream & fi,XEMData *& data, int64_t nbNbCluster, int64_t * tabNbCluster, XEMModelType * modelType, bool & alreadyRead);

  bool  verify() const;  
  
  
  friend ostream & operator << (ostream & fo, XEMClusteringStrategyInit & strategyInit);
  
  //friend class XEMStrategy;
  
  
  private :  
  
  /// Initialization strategy type
  XEMStrategyInitName _strategyInitName;

  /// nbTry
  int64_t _nbTry;
  
  /// stopName (for smallEm & sem_max)
  XEMAlgoStopName _stopName;
  
  /// nbIteration
  int64_t _nbIteration;
  
  /// epsilon
  double _epsilon;
  
  
  /* USER */
  /// number of InitParameter
  int64_t _nbInitParameter;

  /// Init Parameters to initialize strategy
  XEMParameter ** _tabInitParameter;

  
  /* USER_PARTITION */
  /// number of Partition
  int64_t _nbPartition;

  /// Labels to initialize strategy
  XEMPartition ** _tabPartition;
  

  bool _deleteTabParameter ;
  
};

inline  const XEMStrategyInitName & XEMClusteringStrategyInit::getStrategyInitName() const{
  return _strategyInitName;
}

inline const XEMAlgoStopName XEMClusteringStrategyInit::getStopName() const{
  return _stopName;
}

inline const int64_t & XEMClusteringStrategyInit::getNbInitParameter() const{
  return _nbInitParameter;
}

inline const int64_t & XEMClusteringStrategyInit::getNbPartition() const{
  return _nbPartition;
}

inline const XEMParameter ** XEMClusteringStrategyInit::getTabInitParameter() const{
  return const_cast<const XEMParameter**>(_tabInitParameter);
}

inline XEMParameter * XEMClusteringStrategyInit::getInitParameter(int64_t index) const{
  return _tabInitParameter[index];
}

  inline const XEMPartition** XEMClusteringStrategyInit::getTabPartition() const{
    return  const_cast<const XEMPartition**>(_tabPartition);
  }

  inline XEMPartition* XEMClusteringStrategyInit::getPartition(int64_t index) const{
    return _tabPartition[index];
  }
  
 
  inline const int64_t XEMClusteringStrategyInit::getNbTry() const{
    return _nbTry;
  }
  
  inline const int64_t XEMClusteringStrategyInit::getNbIteration() const{
    return _nbIteration;
  }
  
  inline const double XEMClusteringStrategyInit::getEpsilon() const{
    return _epsilon;
  }
  
  
#endif
