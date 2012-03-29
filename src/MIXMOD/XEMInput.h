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
#include "XEMDataDescription.h"

// pre-declaration
class XEMLabelDescription;
class XEMPartition;

class XEMInput{

public:	

  /// Default Constructor
  XEMInput();

  /// Copy Constructor
  XEMInput(const XEMInput & input);
  
  /// Initialisation constructor
  XEMInput(const std::vector<int64_t> & iNbCluster, const XEMDataDescription & iDataDescription);

  /// clone initialisation
  void cloneInitialisation(const std::vector<int64_t> & iNbCluster, const XEMDataDescription & iDataDescription);
  
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
  std::vector<int64_t> getNbCluster() const;
  
  /// get ith NbCluster
  int64_t getNbCluster(int64_t index) const;
  
  /// get NbCluster size
  int64_t getNbClusterSize() const;
  
  
  //------ Criterion  ----//
  /// get All Criterion Name
  std::vector <XEMCriterionName> const & getCriterionName() const;
  
  ///getNbCriterionName
  int64_t getNbCriterion() const;

  ///getCriterionName[i]
  XEMCriterionName getCriterionName(unsigned int index) const;
  
  /// setCriterionName
  virtual void setCriterion(std::vector<XEMCriterionName> const & criterionName) =0;
  
  /// setCriterionName
  virtual void setCriterion(const XEMCriterionName criterion, unsigned int index) =0;

  ///insertCriterionName[i]
  virtual void insertCriterion(const XEMCriterionName criterion, unsigned int index) =0;
  
  // add new criterion
  virtual void addCriterion( const XEMCriterionName criterion ) =0;
  
  /// removeCriterionName
  void removeCriterion(unsigned int index);
	
  
  // ----- ModelType ----//
  /// getNbModelType
  int64_t getNbModelType() const;

  /// getModelType[i]
  XEMModelType *  getModelType(unsigned int index) const;

  
  
  /// setModelType
  void setModelType(const XEMModelType * modelType, unsigned int index);

  /// insertModelType
  void insertModelType(const XEMModelType * modelType, unsigned int index);

  // add new model type
  void addModel( XEMModelName const modelName );
  
  /// setModel
  void setModel(std::vector<XEMModelName> const & modelName);
  
  /// removeModelType
  void removeModelType(unsigned int index);

  /// setSubDimensionEqual
 // void setSubDimensionEqual(int64_t modelTypePosition, int64_t subDimensionValue);

  /// setSubDimensionFreel
 // void setSubDimensionFree(int64_t modelTypePosition, int64_t subDimensionValue, int64_t dimensionPosition);

  ///setWeight();
  void setWeight(std::string weightFileName);
  
  ///setWeight();
  void setWeight(double* weight);
  
  ///removeWeight();
  void removeWeight();
  
  ///insertWeight();
  void insertWeight(std::string weightFileName);
  
  
  ///removeWeight();
  //void removeWeight();
  
  // ----- KnownPartition ----//
  /// getKnownPartition
  XEMPartition * getKnownPartition() const;
  
  /// setKnownPartition
  void setKnownPartition(std::string iFileName);
  
  /// insertKnownPartition
  void insertKnownPartition(std::string iFileName);
  
  /// removeKnownPartition
  void removeKnownPartition();
  
	
	
  //------- KnownLabel -----------//
  ///getKnownLabelDescription
  const XEMLabelDescription * getKnownLabelDescription() const;
  
  /// setKnownLabelDescription
  void setKnownLabelDescription(XEMLabelDescription & labeldescription);
  
  /// removeLabel
  void removeKnownLabelDescription();
  
  //
  /// isBinaryData
  const bool isBinaryData() const;
  
  // finalize 
  void finalize();
  
  /// get Data Description
  const XEMDataDescription & getDataDescription() const;
  
  /// get Data 
  XEMData *  getData() const;
  
  bool isFinalized() const;
  
  // print out input
  void edit(std::ostream & out ) const;
  
  protected : 
  //---------
  ///. verification of inputs validity  
  virtual bool verif();
  
  // Criterion
  std::vector <XEMCriterionName> _criterionName;
  
  // ModelType
  std::vector <XEMModelType*> _modelType;
  
  /** a input object must be finalized (verif, ...)
   */
  bool _finalized;
 
  //-------
  private :
  //-------
    
  /// Data Description
  XEMDataDescription _dataDescription;
  
  // Known Partition
  XEMPartition * _knownPartition; 
  XEMLabelDescription * _knownLabelDescription;
    
  // Number of samples (no reduced data)
  int64_t  _nbSample;
  
  // Problem dimension
  int64_t _pbDimension;
  
  // nbCluster
  std::vector<int64_t> _nbCluster;

};

inline int64_t XEMInput::getNbSample() const{
  return _nbSample;
}

inline int64_t XEMInput::getPbDimension() const{
  return _pbDimension;
}


inline std::vector<int64_t> XEMInput::getNbCluster() const{
  return _nbCluster;
}

inline int64_t XEMInput::getNbCluster(int64_t index) const{
  return _nbCluster[index];
}

inline int64_t XEMInput::getNbClusterSize() const{
  return _nbCluster.size();
}

inline int64_t XEMInput::getNbCriterion() const{
  return _criterionName.size();
}

inline int64_t XEMInput::getNbModelType() const{
  return _modelType.size();
}

inline XEMPartition * XEMInput::getKnownPartition() const{
  return _knownPartition;
}

inline const XEMLabelDescription * XEMInput::getKnownLabelDescription() const{
 return _knownLabelDescription; 
}

inline const bool XEMInput::isBinaryData() const{
 return _dataDescription.isBinaryData(); 
}

inline bool XEMInput::isFinalized() const{
  return _finalized;
}

inline const XEMDataDescription & XEMInput::getDataDescription() const{
  return _dataDescription;
}

inline XEMData * XEMInput::getData() const{
  return _dataDescription.getData();
}

inline vector<XEMCriterionName> const & XEMInput::getCriterionName() const{
  return _criterionName;
}


#endif
