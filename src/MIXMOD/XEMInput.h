/***************************************************************************
                             SRC/MIXMOD/XEMInput.h  description
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
#ifndef XEMINPUT_H
#define XEMINPUT_H


#include "XEMUtil.h"
#include "XEMData.h"
#include "XEMPartition.h"
#include "XEMClusteringStrategy.h"


class XEMLabelDescription;

using namespace std;

class XEMInput{

public:	

  /// Default Constructor
  XEMInput();

  /// Copy Constructor
  XEMInput(const XEMInput & input);
  
  /// Initialisation constructor
  XEMInput(const vector<int64_t> & iNbCluster, const XEMDataDescription & iDataDescription);

  /// clone initialisation
  void cloneInitialisation(const vector<int64_t> & iNbCluster, const XEMDataDescription & iDataDescription);
  
  // Constructor used in DCV context
  XEMInput(XEMInput * originalInput, XEMCVBlock & learningBlock);
			
  /// Destructor
  virtual ~XEMInput();
  
  
  //----------------------------
  // Modifiers
  //----------------------------
  
  //------ Sample  ----//
  /// getNbSample 
  int64_t getNbSample() const;   

  
  //------ Dimension  ----//
  /// getPbDimension
  int64_t getPbDimension() const;

  
  //------ nbCluster  ----//
  /// get all NbCluster
  vector<int64_t> getNbCluster() const;
  
  /// get ith NbCluster
  int64_t getNbCluster(int64_t index) const;
  
  /// get NbCluster size
  int64_t getNbClusterSize() const;
  
  
  //------ Criterion  ----//
  /// get All Criterion Name
  vector <XEMCriterionName> getCriterionName() const;
  
  ///getNbCriterionName
  int64_t getNbCriterionName() const;

  ///getCriterionName[i]
  XEMCriterionName getCriterionName(int64_t index) const;

  /// setCriterionName
  void setCriterionName(XEMCriterionName criterionName, int64_t index);

  ///insertCriterionName[i]
  void insertCriterionName(XEMCriterionName criterionName, int64_t index);

  /// removeCriterionName
  void removeCriterionName(int64_t index);
	
  
  // ----- ModelType ----//
  /// getNbModelType
  int64_t getNbModelType() const;

  /// getModelType[i]
  XEMModelType *  getModelType(int64_t index) const;

  /// setModelType
  void setModelType(const XEMModelType * modelType, int64_t index);

  /// insertModelType
  void insertModelType(const XEMModelType * modelType, int64_t index);

  /// removeModelType
  void removeModelType(int64_t index);

  /// setSubDimensionEqual
 // void setSubDimensionEqual(int64_t modelTypePosition, int64_t subDimensionValue);

  /// setSubDimensionFreel
 // void setSubDimensionFree(int64_t modelTypePosition, int64_t subDimensionValue, int64_t dimensionPosition);

  ///setWeight();
  void setWeight(string weightFileName);
  
  ///setWeight();
  void setWeight(double* weight);
  
  ///removeWeight();
  void removeWeight();
  
  ///insertWeight();
  void insertWeight(string weightFileName);
  
  
  ///removeWeight();
  //void removeWeight();
  
  
  // ----- KnownPartition ----//
  /// getKnownPartition
  XEMPartition * getKnownPartition() const;
  
  /// setKnownPartition
  void setKnownPartition(string iFileName);
  
  /// insertKnownPartition
  void insertKnownPartition(string iFileName);
  
  /// removeKnownPartition
  void removeKnownPartition();
  
  //------- Label -----------//
  ///getLabel
  const XEMLabelDescription * getLabel() const;
  
  /// setLabel
  void setLabel(XEMLabelDescription & labeldescription);
  
  /// removeLabel
  void removeLabel();
  
  // -----------------------------------------------------------------------------------------------
  // Other virtual Selector methods throw error (otherwise this method is write in inherited classes
  // -----------------------------------------------------------------------------------------------
  virtual XEMClusteringStrategy * getStrategy() const;
  
    // setStrategy
  virtual void setStrategy(XEMClusteringStrategy * strat);
  
  
  
  //
  /// isBinaryData
  const bool isBinaryData() const;
  
  
  void finalize();
  
  /// get Data Description
  const XEMDataDescription & getDataDescription() const;
  
  /// get Data 
  XEMData *  getData() const;
  
  bool isFinalized() const;
  
  protected : 
  //---------
  ///. verification of inputs validity  
    virtual bool verif();
 

  //-------
  private :
  //-------
    
  /// Data Description
  XEMDataDescription _dataDescription;
    
  // Number of samples (no reduced data)
  int64_t  _nbSample;
  
  // Problem dimension
  int64_t _pbDimension;
  
  // nbCluster
  vector<int64_t> _nbCluster;

  // Known Partition
  XEMPartition * _knownPartition; 
  XEMLabelDescription * _labelDescription;
  
  // CriterionName
  vector <XEMCriterionName> _criterionName;
  
  // ModelType
  vector <XEMModelType *> _modelType;
  
  /** a input object must be finalized (verif, ...)
  */
  bool _finalized;

};

inline int64_t XEMInput::getNbSample() const{
  return _nbSample;
}

inline int64_t XEMInput::getPbDimension() const{
  return _pbDimension;
}


inline vector<int64_t> XEMInput::getNbCluster() const{
  return _nbCluster;
}

inline int64_t XEMInput::getNbCluster(int64_t index) const{
  return _nbCluster[index];
}

inline int64_t XEMInput::getNbClusterSize() const{
  return _nbCluster.size();
}

inline int64_t XEMInput::getNbCriterionName() const{
  return _criterionName.size();
}

inline int64_t XEMInput::getNbModelType() const{
  return _modelType.size();
}

inline XEMPartition * XEMInput::getKnownPartition() const{
  return _knownPartition;
}

inline const XEMLabelDescription * XEMInput::getLabel() const{
 return _labelDescription; 
}

inline const bool XEMInput::isBinaryData() const{
 return _dataDescription.isBinaryData(); 
}

inline bool XEMInput::isFinalized() const{
  return _finalized;
}



inline XEMClusteringStrategy * XEMInput::getStrategy()const{
  throw errorInXEMInputSelector;
}


inline void XEMInput::setStrategy(XEMClusteringStrategy * strat){
  throw errorInXEMInputSelector;
}


inline const XEMDataDescription & XEMInput::getDataDescription() const{
  return _dataDescription;
}

inline XEMData * XEMInput::getData() const{
  return _dataDescription.getData();
}

inline vector <XEMCriterionName> XEMInput::getCriterionName() const{
  return _criterionName;
}


/*


inline const int64_t & XEMOldInput::getNbSample() const{
  return _nbSample;
}

inline const int64_t & XEMOldInput::getPbDimension() const{
  return _pbDimension;
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

  inline const XEMCriterionName *  XEMOldInput::getTabCriterionName() const{
    return _tabCriterionName;
  }

  inline const XEMCriterionName & XEMOldInput::getCriterionName(int64_t index) const{
    return _tabCriterionName[index];
  }


  inline const XEMModelType**  XEMOldInput::getTabModelType() const{
    return const_cast<const XEMModelType**>(_tabModelType);
  }


  inline const XEMModelType*  XEMOldInput::getTabModelTypeI(int64_t index) const{
    return _tabModelType[index];
  }
*/
/*
  inline int64_t  XEMOldInput::getNbKnownPartition() const{
    int64_t nbPartition;
    if (_tabKnownPartition){
      nbPartition = _nbNbCluster;
    }
    else{
      nbPartition = 0;
    }
    return nbPartition;
  }*/

#endif
