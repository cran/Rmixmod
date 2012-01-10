/***************************************************************************
                             SRC/MIXMOD/XEMDCVCriterion.h  description
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
#ifndef XEMDCVCriterion_H
#define XEMDCVCriterion_H

#include "XEMOldInput.h"
#include "XEMRandom.h"
#include "XEMCriterion.h"
#include "XEMEstimation.h"

/**
  @brief Derived class of XEMCriterion for DCV Criterion
  @author F Langrognet & A Echenim
 */
 
class XEMDCVCriterion : public XEMCriterion{


public :

  /// Constructor
  XEMDCVCriterion();
  
  /// Constructor
  XEMDCVCriterion(XEMEstimation **& tabEstimation, int64_t nbEstimation, XEMOldInput * originalInput);
  
  /// Destructor
  virtual ~XEMDCVCriterion();
  
  /**@brief setModel
     @param iModel Base model
  */
  void setModel(XEMModel * iModel);
  
  XEMCriterionName getCriterionName() const;
  
  /// Run method
  void run(XEMModel * model, double & value, XEMErrorType & error);

  ///
  double * getTabCriterionValueForEachEstimation();
  
  ///
  XEMErrorType * getTabCriterionErrorForEachEstimation();

  
  XEMErrorType * getTabDCVErrorForEachEstimation();
  ///
  int64_t getBestIndexEstimation();
  
  /// edit
  void edit(ofstream & fi);
  
  /// editNumeric
  void editNumeric(ofstream & fi);
  
  
private :

  /// Table of XEMCVBlock representing 'learning blocks' (S-Si) 
  XEMCVBlock * _tabLearningBlock;
  
  // number of DCV Blocks (= number of learning blocks or test blocks)  
  int64_t  _nbDCVBlock;
  
  /// Table of XEMCVBlock representing 'teste blocks' (Si) 
  XEMCVBlock * _tabTestBlock;
  
  /// table of index of best estimation (size : _nbDCVBlock)
  int64_t * _tabIndexOfBestEstimation;  
  
  /// Table of CV value (on tests samples) for the all best model (size : _nbDCVBlock)
  double * _tabCVValueOnBestModel;
  
  ///origninal input
  XEMOldInput * _originalInput;
  
  // initialisation method of cv blocks
  XEMDCVinitBlocks _DCVinitBlocks;
  
  // type of best model for the vth block
  XEMModelName * _tabBestModelVType; // aggregate

  //
  double * _tabCriterionValueForEachEstimation;
  
  //
  XEMErrorType * _tabCriterionErrorForEachEstimation;

  XEMErrorType * _tabDCVErrorForEachEstimation;
  //
  int64_t _bestIndexEstimation;
  
  // nbEstimation
  int64_t _nbEstimation;
  
  XEMEstimation ** _tabEstimation;
  
  void createDCVBlocks();
  
  
  XEMData * _data;
  
  double * _weight;
  
  int64_t  _weightTotal;
  
  double _value;
};

//---------------
// inline methods
//---------------

inline XEMErrorType * XEMDCVCriterion::getTabCriterionErrorForEachEstimation(){
	return _tabCriterionErrorForEachEstimation;
}

inline XEMErrorType * XEMDCVCriterion::getTabDCVErrorForEachEstimation(){
        return _tabDCVErrorForEachEstimation;
}


inline double * XEMDCVCriterion::getTabCriterionValueForEachEstimation(){
	return _tabCriterionValueForEachEstimation;
}

inline int64_t XEMDCVCriterion::getBestIndexEstimation(){
	return _bestIndexEstimation;
}

inline XEMCriterionName XEMDCVCriterion::getCriterionName() const{
  return DCV;
}


#endif
