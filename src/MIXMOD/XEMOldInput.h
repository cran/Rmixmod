/***************************************************************************
                             SRC/MIXMOD/XEMOldInput.h  description
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
#ifndef XEMOLDINPUT_H
#define XEMOLDINPUT_H

#include "XEMUtil.h"
#include "XEMData.h"
#include "XEMPartition.h"
//#include "XEMStrategy.h"
#include "XEMModelType.h"
class XEMStrategy;

/** @brief Input Class for MIXMOD API 
@author F Langrognet & A Echenim

Mixmod inputs are stored in a XEMOldInput object.<br>
Required inputs :<br>
-----------------------
\li nbSample (int64_t) : number of samples
\li pbDimension (int64_t) : problem dimension (number of variables)
\li nbCluster  : nbCluster values 
	<ul>
		<li> nbNbCluster (int64_t) : size of the following array</li>
		<li> tabNbCluster (int64_t *) : Array of nbCluster values</li>
	</ul>
\li data : a XEMGaussianData or XEMBinaryData object


Optional inputs (inputs with default value) :<br>
-----------------------
\li model : model type values
	<ul>
		<li> nbModelTye (int64_t) : size of the following array</li>
		<li>tabModelType : array of XEMModelType value \see XEMModelType.h</li>
	</ul>
\li criterion : criterion types values
	<ul>
		<li> nbCriterionName (int64_t) : size of the following array</li>
		<li> tabCriterionName : array of XEMCriterionName value \see XEMUtil.h </li>
	</ul>
\li knownPartition: fixed Partition (in Discriminant Analysis context)
	<ul>
		<li>  nbTabKnownPartition (int64_t) :  size of the following array = nbNbCluster</li>
		<li> tabknownPartition (also called tabKnownPartition) : array of XEMPartition (a XEMPartition is a Partition !)</li>
	</ul>
\li strategy : strategies to apply (initialization method + algortihms)
	<ul>
		<li> nbStrategy (int64_t) : number of strategies (and size of the following array)</li>
		<li> tabStrategy : array of XEMStrategy</li>
	</ul>
\li subDimensionFree : subDimension value if a High Dimension Model is choosen with free subdimension (....Dk....) : an array (size=nbNbCluster) of int64_t* value. subDimension[u] is NULL if subDimensions are unknown, or subDimension[u][k] = subDimension of the (k+1)th class if nbCluster=u+1
\li subDimensionEqual : subDimension value if a High Dimension Model is choosen with equal subdimension (....D....) : an array (size=nbNbCluster) of int64_t * value. subDimension[u] is NULL if this subDimension is unknown or *(subDimension[u])=subDimension of all classes.


*/

class XEMOldInput{

public:	

  /// Default Constructor
	XEMOldInput();

  /** @brief Constructor
	@param iNbSample number of samples
	@param iPbDimension problem dimension (or number of variables)
	@param iNbNbCluster size of tabNbCluster array
	@param iTabNbCluster array of number of clusters
	@param iData data object (see XEMGaussianData and XEMBinary classes)
	\see XEMGaussianData.h
	\see XEMBinaryData.h
	*/
  XEMOldInput(int64_t iNbSample, int64_t iPbDimension, int64_t iNbNbCluster, int64_t * iTabNbCluster, XEMData * iData, bool binaryDataType=false);

	
  // Constructor used in DCV context
  XEMOldInput(XEMOldInput * originalInput, XEMCVBlock & learningBlock);
	
	
	
  /// Destructor
  virtual ~XEMOldInput();

	
  /** @brief Criterion Selector : to set Criterion optional input
	@param iNbCriterionName Number of criteria types
	@param iTabCriterionName Array of criteria names (see XEMCriterionName)
	\see XEMUtil.h
   */
  void setCriterionName(int64_t iNbCriterionName, XEMCriterionName * iTabCriterionName);

	
  /** @brief Model Selector : to set Model optional input
	@param iNbModelType Number of model types
	@param iTabModelType Array of model types 
	\see XEMModelType.h
  */
  void setModelType(int64_t iNbModelType, XEMModelType ** iTabModelType);

	
  /** @brief Strategy Selector : to set Strategy optional input
	@param iNbStrategy Number of strategies
	@param iTabStrategy Array of strategies (see XEMStrategy class)
	\see XEMStrategy.h
  */
  void setStrategy(int64_t iNbStrategy, XEMStrategy ** iTabStrategy);


  /** @brief knownLabel Selector : to set Known partition (known partition) optional input
	@param iNbKnownPartition Number of knownPartition
	@param iTabKnownPartition Array of known partitions (see XEMPartition class)
	\see XEMPartition.h
	 */    
  void setKnownPartition(int64_t iNbKnownPartition, XEMPartition ** iTabKnownPartition);

  
  /** finalize */
  void finalize();

/// getNbSample 
  const int64_t & getNbSample() const;   

  /// getPbDimension
  const int64_t & getPbDimension() const;

  /// getXEMData
  const XEMData*  getXEMData() const;

  ///getXEMPartition
  const XEMPartition**  getTabPartition() const;

  ///getXEMPartition[i]
  const XEMPartition*  getTabPartitionI(int64_t index) const;

  ///getNbNbCluster
  const int64_t & getNbNbCluster() const;

  ///getTabNbCluster;
  const int64_t*  getTabNbCluster() const;

  /// getTabNbCluster[I]
  const int64_t getTabNbClusterI(int64_t index) const;
  ///getNbCriterionName
  const int64_t & getNbCriterionName() const;

  ///getTabCriterionName
  const XEMCriterionName *  getTabCriterionName() const;

  ///getTabCriterionName[i]
  const XEMCriterionName & getCriterionName(int64_t index) const;

  /// getNbModelType
  const int64_t & getNbModelType() const;

  /// getTabModelType
  const XEMModelType**  getTabModelType() const;

  /// getTabModelType[i]
  const XEMModelType*  getTabModelTypeI(int64_t index) const;

  /// getNbStrategy
  const int64_t & getNbStrategy() const;

  /// getTabStrategy
  const XEMStrategy**  getTabStrategy() const;

  /// getStrategy[i]
  const XEMStrategy*  getStrategy(int64_t index) const;

  ///getNbKnownPartition();
  int64_t getNbKnownPartition() const;

  ///setWeight();
  void setWeight(string & fileNameWeight);


  /// setCriterionName
  void setCriterionName(XEMCriterionName criterionName, int64_t position);

  ///insertCriterionName
  void insertCriterionName(XEMCriterionName criterionName, int64_t position);

  /// removeCriterionName
  void removeCriterionName(int64_t position);

  /// setModelType
  void setModelType(XEMModelName nameModel, int64_t position);

  /// insertModelType
  void insertModelType(XEMModelName nameModel, int64_t position);

  /// removeModelType
  void removeModelType(int64_t position);

  /// setKnownPartition
  void setKnownPartition(string & fileName, int64_t position);


  /// removeKnownPartition
  void removeKnownPartition(int64_t position, bool decrease);

  /// insertKnownPartition
  //void insertKnownPartition(string & fileName, int64_t position);

  /// setNbTryInStrategy
  void setNbTryInStrategy(int64_t strategyPosition, int64_t nbTry);
  
  /// getNbTryInStrategy
  int64_t getNbTryInStrategy(int64_t strategyPosition);
  
  /// setNbTryInInit
  void setNbTryInInit(int64_t strategyPosition, int64_t nbTry);
  
  /// getNbTryInInit
  int64_t getNbTryInInit(int64_t strategyPosition);
  
  /// setNbIterationInInit
  void setNbIterationInInit(int64_t strategyPosition, int64_t nbIteration);
  
  /// getNbIterationInInit
  int64_t getNbIterationInInit(int64_t strategyPosition);
  
  /// setEpsilonInInit
  void setEpsilonInInit(int64_t strategyPosition, double epsilon);
  
  /// getEpsilonInInit
  double getEpsilonInInit(int64_t strategyPosition);

  /// setStrategyInitName
  void setStrategyInitName(XEMStrategyInitName initName, int64_t position);

  /// getStopNameInInit
  const XEMAlgoStopName getStopNameInInit(int64_t strategyPosition) const;
  
  /// setStopNameInInit
  void setStopNameInInit(int64_t strategyPosition, XEMAlgoStopName stopName);
  
  /// setInitParam
  void setInitParam(int64_t strategyPosition, string & paramFileName, int64_t position);

  /// setInitPartition
  void setInitPartition(int64_t strategyPosition, string & partitionFileName, int64_t position);


  /// setAlgoName
  void setAlgoName(int64_t strategyPosition, XEMAlgoName algoName, int64_t position);

  /// removeAlgo
  void removeAlgo(int64_t strategyPosition,int64_t position);

  // insertAlgoName
//  void insertAlgoName(int64_t strategyPosition,XEMAlgoName algoName, int64_t position);

/// insert default algo
  void insertAlgo(int64_t strategyPosition, int64_t position);

  /// set algo stop rule
  void setAlgoStopRule(int64_t strategyPosition, int64_t position, XEMAlgoStopName stopName);

  ///set nb iteration
  void setAlgoIteration(int64_t strategyPosition, int64_t position,int64_t nbIterationValue);

  ///set nb epsilon
  void setAlgoEpsilon(int64_t strategyPosition,int64_t position, double epsilonValue);



  /// setSubDimensionEqual
  void setSubDimensionEqual(int64_t position, int64_t subDimensionValue);

  /// setSubDimensionFreel
  void setTabSubDimensionFree(int64_t positionModel, int64_t subDimensionValue, int64_t position);

  //-------
  private :
  //-------
  
  // Number of samples (no reduced data)
    int64_t  _nbSample;

  // Problem dimension
  int64_t _pbDimension;

  // Current data
  XEMData * _data; // aggregate
  

  // _data must be deleted in the desctructor ?
  bool _deleteData;
  
  // Knwon label
  XEMPartition ** _tabKnownPartition; // aggregate
  
  // Number of numbers of cluster
  int64_t _nbNbCluster;

  // Table of numbers of cluster
  int64_t * _tabNbCluster; // aggregate

  // _tabNbCluster must be deleted in the destructor ?
  bool _deleteTabNbCluster;

  // Number of criteria
  int64_t _nbCriterionName;

  // Table of criterion type
  XEMCriterionName * _tabCriterionName; // aggregate

  // _tabCriterionName must be deleted in teh desctructor ?
  bool _deleteTabCriterionName;

  // Number of models
  int64_t _nbModelType;

  // Table of model type
  XEMModelType ** _tabModelType; // aggregate
  // _tabModelType must be deleted in teh desctructor ?
  bool _deleteTabModelType;

  // Number of strategy
  int64_t _nbStrategy;

  // Table of strategies
  XEMStrategy ** _tabStrategy; // aggregate

  bool _binaryDataType;
  
  // number of Cv blocks	
  int64_t _numberOfCVBlocks ; // default 10 (see XEMUtil.h)
  
	// Initialisation method of CV blocks
  XEMCVinitBlocks _CVinitBlocks;
  
  // Initialisation method of DCV blocks
  XEMDCVinitBlocks _DCVinitBlocks; // how initialize the blocks (see XEMUtil.h)
  // number of DCVBlock
  int64_t _numberOfDCVBlocks; // number of DCV Blocks (default 10)

  int64_t _nbKnownPartition;
  /** a input object must be finalized (verif, ...)
  */
  bool _finalized;

  //. verification of inputs validity  
  bool verif();
	
 











  

	/********************/
	/*		FRIENDS       */
	/********************/
	
	friend class XEMCVCriterion;
	friend class XEMDCVCriterion;
	friend class XEMData;
	friend class XEMMain;
	friend class XEMCondExe;
	friend class XEMOutput;
	friend class XEMSelection;
	
	
  /** @brief Friend method
	@return Operator >> overloaded to read Input dat from input files
	 */
	friend ifstream & operator >> (ifstream & fi, XEMOldInput & input);

  /** @brief Friend method
	@return Operator << overloaded to write Input data  in output files
	 */
	friend ostream & operator << (ostream & fo, XEMOldInput & input);

	
};

inline const int64_t & XEMOldInput::getNbSample() const{
  return _nbSample;
}

inline const int64_t & XEMOldInput::getPbDimension() const{
  return _pbDimension;
}

  inline const XEMData*  XEMOldInput::getXEMData() const{
    return _data;
  }

  inline void  XEMOldInput::setWeight(string & fileNameWeight){
    	if (fileNameWeight.compare("") == 0){	
		return _data->setWeightDefault();
	}else{
		return _data->setWeight(fileNameWeight);
	}
  }

  inline const XEMPartition**  XEMOldInput::getTabPartition() const{
    return  const_cast<const XEMPartition**>(_tabKnownPartition);
  }

  inline const XEMPartition*  XEMOldInput::getTabPartitionI(int64_t index) const{
    return _tabKnownPartition[index];
  }

  inline const int64_t & XEMOldInput::getNbNbCluster() const{
    return _nbNbCluster;
  }

  inline  const int64_t*  XEMOldInput::getTabNbCluster() const{
    return _tabNbCluster;
  }

  inline const int64_t XEMOldInput::getTabNbClusterI(int64_t index) const{
    return _tabNbCluster[index];
  }
  inline const int64_t & XEMOldInput::getNbCriterionName() const{
    return _nbCriterionName;
  }

  inline const XEMCriterionName *  XEMOldInput::getTabCriterionName() const{
    return _tabCriterionName;
  }

  inline const XEMCriterionName & XEMOldInput::getCriterionName(int64_t index) const{
    return _tabCriterionName[index];
  }

  inline const int64_t & XEMOldInput::getNbModelType() const{
    return _nbModelType;
  }

  inline const XEMModelType**  XEMOldInput::getTabModelType() const{
    return const_cast<const XEMModelType**>(_tabModelType);
  }


  inline const XEMModelType*  XEMOldInput::getTabModelTypeI(int64_t index) const{
    return _tabModelType[index];
  }

  inline const int64_t & XEMOldInput::getNbStrategy() const{
    return _nbStrategy;
  }

  inline const XEMStrategy**  XEMOldInput::getTabStrategy() const{
    return  const_cast<const XEMStrategy**>(_tabStrategy);
  }

  inline const XEMStrategy*  XEMOldInput::getStrategy(int64_t index) const{
    return _tabStrategy[index];
  }

  inline int64_t  XEMOldInput::getNbKnownPartition() const{
    int64_t nbPartition;
    if (_tabKnownPartition){
      nbPartition = _nbNbCluster;
    }
    else{
      nbPartition = 0;
    }
    return nbPartition;
  }

#endif
