/***************************************************************************
                             SRC/MIXMOD/XEMClusteringStrategy.h  description
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
#ifndef XEMClusteringStrategy_H
#define XEMClusteringStrategy_H

#include "XEMUtil.h"

// pre-declaration
class XEMClusteringStrategyInit;
class XEMModelType;
class XEMParameter;
class XEMData;
class XEMModel;
class XEMPartition;

/**
  @brief Base class for Strategy(s)
  @author F Langrognet 
 */

class  XEMClusteringStrategy
{
  public:

	/// Default constructor
  XEMClusteringStrategy();

  /// Constructor
  XEMClusteringStrategy(const XEMClusteringStrategy & strategy);

    
	/// Destructor
  ~XEMClusteringStrategy();
  
  ///--------------------------------
  /// Strategy initialisation methods
  ///--------------------------------
  
  ///getStrategyInit
  const XEMClusteringStrategyInit * getStrategyInit() const;

  /// setStrategyInit
  void setStrategyInit(XEMClusteringStrategyInit * iStrategyInit);

  /// setStrategyInit
  void setStrategyInit(XEMStrategyInitName  initName, XEMData *& data, int64_t nbNbCluster, int64_t * tabNbCluster, XEMModelType * modelType);

  /// setStrategyInitName
  void setStrategyInitName(XEMStrategyInitName initName);
  
  /// setInitParam
  void setInitParam(std::string & paramFileName, int64_t position);
  
  /// setInitParam
  void setTabInitParameter(XEMParameter ** tabInitParameter, int64_t nbInitParameter);
    
  /// setInitPartition
  void setInitPartition(std::string & partitionFileName, int64_t position);
    
  /// setInitPartition
  void setInitPartition(XEMPartition * part, int64_t position);

  /// setTabPartition
  void setTabPartition(XEMPartition ** tabPartition, int64_t nbPartition);
    
  /// getNbTryInInit
  const int64_t getNbTryInInit() const;
  
  /// setNbTryInInit
  void setNbTryInInit(int64_t nbTry);
  
  /// getNbIterationInInit
  const int64_t getNbIterationInInit() const;
  
  /// set NbIterationInInit
  void setNbIterationInInit(int64_t nbIteration);
  
  /// getEpsilonInInit
  const double getEpsilonInInit() const;
  
  /// setEpsilonInInit
  void setEpsilonInInit(double epsilon);
  
  /// getStopNameInInit
  const XEMAlgoStopName getStopNameInInit() const;
  
  /// setStopNameInInit
  void setStopNameInInit(XEMAlgoStopName stopName);
  
  
  ///--------------------------------
  /// Algo methods
  ///--------------------------------
  
  ///getAlgo[i]
  const XEMAlgo * getAlgo(int64_t index) const;
  
  /// setAlgo
  void setAlgo(XEMAlgoName algoName, int64_t position);
  
  /// addAlgo
  void addAlgo(XEMAlgoName algoName);

  /// removeAlgo
  void removeAlgo(unsigned int  position);

  /// getTabAlgo
  std::vector<XEMAlgo*> const & getTabAlgo() const;
  std::vector<XEMAlgo*> & getTabAlgo();

  /// insertAlgo
  void insertAlgo(XEMAlgoName algoName, int64_t position);

  /// setAlgoStopRuleTypeValue
  void setAlgoStopRule(XEMAlgoStopName stopName, int64_t position);
  void setAlgoIteration(  int64_t position, int64_t nbIterationValue);
  void setAlgoEpsilon( int64_t position, double epsilonValue);
  
  ///nbTry
  const int64_t getNbTry()const;
  void setNbTry(int64_t nbTry);

  const int64_t getNbAlgo() const; 
    
  /// Input strategy (FLAT FORMAT)
  // TODO XEMInput : a enlever
  void input_FLAT_FORMAT(std::ifstream & fi, XEMData *& data, int64_t nbNbCluster, int64_t * tabNbCluster, XEMModelType * modelType);
  
	/// Run method
  void run(XEMModel * model);
        
  bool verify();
    
  void edit(std::ofstream & oFile);
        
  friend std::ostream & operator << (std::ostream & fo, XEMClusteringStrategy & strategy);
    
    
private :

  /// Number of try in the strategy
  int64_t _nbTry;

  /// strategyInit
  XEMClusteringStrategyInit * _strategyInit;

  /// Number of algorithm in the strategy
  int64_t _nbAlgo;

  /// Table of algorithm
  std::vector<XEMAlgo*> _tabAlgo;    // aggregate
    
  void oneTry(XEMModel *& model);

};


inline const XEMClusteringStrategyInit * XEMClusteringStrategy::getStrategyInit() const{
  return _strategyInit;
}

inline const XEMAlgo * XEMClusteringStrategy::getAlgo(int64_t index) const{
  return _tabAlgo[index];
}

inline std::vector<XEMAlgo*> const & XEMClusteringStrategy::getTabAlgo() const{
  return _tabAlgo;
}

inline std::vector<XEMAlgo*> & XEMClusteringStrategy::getTabAlgo(){
  return _tabAlgo;
}

inline const int64_t XEMClusteringStrategy::getNbAlgo() const{
  return _nbAlgo;
} 
inline const int64_t XEMClusteringStrategy::getNbTry()const{
return _nbTry;
}
    
#endif
