/***************************************************************************
                             SRC/MIXMOD/XEMModel.h  description
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
#ifndef XEMMODEL_H
#define XEMMODEL_H

#include "XEMUtil.h"
#include "XEMError.h"

/**
	  @brief Base class for Model(s)
	  @author F Langrognet & A Echenim
*/

// pre-declaration
class XEMParameter;
class XEMData;
class XEMClusteringStrategyInit;
class XEMPartition;
class XEMSample;
class XEMModelType;
class XEMLabelDescription;

class XEMModel{

public:

  /// Default constructor
  XEMModel();

  /// Constructor
  XEMModel(XEMModel * iModel);

  /// Constructor
  XEMModel(XEMModelType * modelType, int64_t  nbCluster, XEMData *& data, XEMPartition * knownPartition);
  
  /// Destructor
  virtual ~XEMModel();


  void updateForCV(XEMModel * originalModel, XEMCVBlock & CVBlock) ;
  //-------
  // select
  //-------

  /** @brief Selector
      @return The parameters
  */
  XEMParameter *  getParameter();

  /** @brief Selector
      @return The current number for cluster
  */
  int64_t  getNbCluster();

  /** @brief Selector
      @return The current data
  */
  XEMData * getData();

  /** @brief Selector
      @return The type of Error
  */
  XEMErrorType const getErrorType() const;
  
  /** @brief Selector
   @return The type of the model
   */
  XEMModelType * const & getModelType() const;
  
  /** @brief Selector
      @return The number of samples
  */
  int64_t  getNbSample();
  
  /** @brief Selector
      @return Table of Fik of each cluster : probabilitites: _fik = pk * f(xi,muk,Sk)
  */
  double ** getTabFik();

  /// return _tabSumF
  double * getTabSumF();

  /** @brief Selector
      @return Table of Tik of each cluster :
        conditionnal probabilities that xi arises from the k-th mixture component, 0 <= tik[i]k0] <= 1
  */
  double ** getTabTik();

  /** @brief Selector
      @return Table of Zik zik[i][k0] = 1 if xi arises from the k0-th mixture component, 0 else
  */
  int64_t  ** getTabZikKnown();


  double ** getTabCik();

  /// getTabZikKnown
  bool * getTabZiKnown();

  /** @brief Selector
      @return Table of number of elements in each cluster
  */
  double * getTabNk();

  bool getDeleteData();
  
  //---------
  // compute
  //--------

  /// comute _fik
  void computeFik();

  /// Compute the number of points in each class
  void computeNk();

  /** @brief Compute the log-likelihood
      @return The log-likelihood
  */
  double getLogLikelihood(bool fikMustBeComputed);


  /** @brief Compute the log-likelihood with one cluster
      @return The log-likelihood
  */
  double getLogLikelihoodOne();

  /** @brief Compute the entropy
      @return The entropy
  */

  double getEntropy();

  /** @brief Compute the completed log-likelihood
      @return The completed log-likelihood
  */
  double getCompletedLogLikelihood();

  /** get completed LL (if CEM) or LL (elseif)*/
  double getCompletedLogLikelihoodOrLogLikelihood() ;
  
  /// return the number of free parameters
  int64_t  getFreeParameter();


  /** @brief Selector
      @return Log of the weight total
  */
  double getLogN();
	
  /// getLabel and partition
  /// label=1...nbSample
  void getLabelAndPartitionByMAPOrKnownPartition(int64_t  * label, int64_t  ** partition);

  /// get label of the ith individual (i=0 .... nbSample-1) by MAP (or known label)
  /// return value in [0 nbCluster-1]
  int64_t  getLabelByMAPOrKnownPartition(int64_t  i);
  
	
  /// get knownLabel of the ith individual (i=0 .... nbSample-1)
  /// return value in [0 nbCluster-1]
	/// throw an error if the label is unkonwn
  int64_t  getKnownLabel(int64_t  i);

  /// getPostProba
  double ** getPostProba();

	
	
	//--------
  // compute
  //--------



  /** @brief Compute the label of the i0-th point of the sample
      @return The label of i0 (i0=0 -> _nBSample -1)
  */
  int64_t  computeLabel(int64_t  i0);


  /** @brief Compute the label of new point x
      @return The label of x
  */
  int64_t  computeLabel(XEMSample * x);



  //------
  // algo
  //------

  /// Maximum a posteriori step method
  void MAPstep();

  /// Expectation step method
  void Estep();

  /// Maximization step method
  void Mstep();

  /// Stochastique classification step method
  void Sstep();

  /// Classification step method
  void Cstep();	


  //-----
  // init
  //-----
  
  /// Random center initialization of the parameters of the model
  void initRANDOM(int64_t  nbTry);


/// random step for init RANDOM or USER_PARTITION
	void randomForInitRANDOMorUSER_PARTITION(bool * tabIndividualCanBeUsedForInitRandom, bool * tabClusterToInitialize);
		
  /// User initialization of the parameters of the model
  void initUSER(XEMParameter * initParameter);

  /// User partition initialisation of the parameters of the model
  void initUSER_PARTITION(XEMPartition * initPartition, int64_t nbTryInInit=defaultNbTryInInit);
  
  // set name of the algorithm
  void setParameter(XEMParameter * parameter);
  
  // set name of the algorithm
  void setAlgoName(XEMAlgoName algoName);
  
  // set an error for the model
  void setError(XEMErrorType errorType);

  /// Fix label Known
  void FixKnownPartition(XEMPartition *& y);


  // edit debug information
  void editDebugInformation();
  void editFik();
  void editCik();
	void editTik();
	void editNk();

protected :

  /// type of the model
  XEMModelType * _modelType;
  
  /// Number of clusters
  int64_t  _nbCluster;

  /// Number of samples
  int64_t  _nbSample;

  /// Current data
  XEMData * _data;
  bool _deleteData;

  /// parameter of model
  XEMParameter * _parameter;
  
  /// Probabilitites: _fik = pk * f(xi,muk,Sk)
  /// dim : _nbSample * _nbCluster
  double ** _tabFik;

  /// table of sum of _tabFik for all k (dim : _nbSample)
  double * _tabSumF;

  /// Conditionnal probabilities that x(i) arises from the k-th mixture component, 0 <= tik[i][k0] <= 1
  /// dim : _nbSample * _nbCluster
  double ** _tabTik;

  /// zikKnown : _tabZikKonwn[i][k0] = 1 if xi arises from the k0-th mixture component, 0 else*
  /// dim : _nbSample * _nbCluster
  int64_t  ** _tabZikKnown;

  /** classification array for individual i and class k
	// if zikKnown 
	//		cik = zikKnown
	//	else :
	//		cik = tik if EM
	//		cik = zik by MAP rule if CEM or MAP
	//		cik = 'random' if SEM
	*/
  double ** _tabCik;

	
  /// is the label zik known (fixed)
  bool * _tabZiKnown;


  /// Number of points in each class
  double * _tabNk;

  // name of the algorithm
  XEMAlgoName _algoName;
  
  // Error handler
  XEMError _error;
  
};


//--------------
//inline methods
//--------------
inline bool * XEMModel::getTabZiKnown(){
  return _tabZiKnown;
}

inline int64_t  ** XEMModel::getTabZikKnown(){
  return _tabZikKnown;
}


inline double ** XEMModel::getTabCik(){
  return _tabCik;
}

inline double ** XEMModel::getTabTik(){
  return _tabTik;
}

inline double ** XEMModel::getTabFik(){
  return _tabFik;
}

inline double * XEMModel::getTabSumF(){
  return _tabSumF;
}

inline double * XEMModel::getTabNk(){
  return _tabNk;
}

inline int64_t  XEMModel::getNbCluster(){
  return _nbCluster;	
}

inline XEMData * XEMModel::getData(){
  return _data;
}


inline XEMParameter * XEMModel::getParameter(){
  return _parameter;
}

inline  int64_t  XEMModel::getNbSample(){

  return  _nbSample;
}

inline double ** XEMModel::getPostProba(){
  return _tabTik;
}

inline XEMModelType * const & XEMModel::getModelType() const{
  return _modelType;
}

inline XEMErrorType const XEMModel::getErrorType() const{
  return _error.getError();
}

#endif
