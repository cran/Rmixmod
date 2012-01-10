/***************************************************************************
                             SRC/MIXMOD/XEMGaussianParameter.h  description
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
#ifndef XEMGaussianParameter_H
#define XEMGaussianParameter_H

#include "XEMUtil.h"
#include "XEMParameter.h"
#include "XEMDiagMatrix.h"

class XEMMatrix;
/**
  @brief Base class for XEMGaussianParameter(s)
  @authors A Echenim & F. Langrognet & Y. Vernaz
*/
class XEMGaussianParameter : public XEMParameter{

public:

  //----------------------------
  // constructors / desctructors
  // ---------------------------

  /// Default constructor
  XEMGaussianParameter();

  /// Constructor
  // called by XEMModel
  XEMGaussianParameter(XEMModel * iModel, XEMModelType * iModelType);

  /// Constructor
  // called by XEMGaussianEDDAParameter if initialization is USER
  XEMGaussianParameter(int64_t  iNbCluster, int64_t  iPbDimension, XEMModelType * iModelType);

  
  /// Constructor
  XEMGaussianParameter(const XEMGaussianParameter * iParameter);

  /// Destructor
  virtual ~XEMGaussianParameter();

  virtual XEMParameter* clone()const =0  ;

	/// reset to default values
	virtual void reset();
  //----------
  // selectors
  //----------

  /// get TabMean
  double ** getTabMean() const ;

  //----------------
  // compute methods
  //----------------

	///computeDiagGlobalDataVariance
	void computeGlobalDiagDataVariance(XEMDiagMatrix * matrixDiagDataVar);

  /// Compute table of cluster scattering matrices Wk and W
  virtual void computeTabWkW();


  /// compute label of idxSample
  int64_t  computeClassAssigment(int64_t  idxSample);

  /// Compute table of means of the samples for each cluster
  void computeTabMean();

  /// Compute table of means of the samples for each cluster in initUSER_PARTITION case
	/// outputs :
	/// -  nbInitializedCluster
	/// - tabNotInitializedCluster (array of size _nbCluster)
  void computeTabMeanInitUSER_PARTITION(int64_t  & nbInitializedCluster, bool * tabNotInitializedCluster, XEMPartition * initPartition);
  
  /// Compute table of sigmas of the samples for each cluster
  // NB : compute also lambda, shape, orientation, wk, w
  //virtual void computeTabSigma();


  //Compute normal probability density function
  //       for iSample the sample and kCluster th cluster
  virtual double getPdf(int64_t  iSample, int64_t  kCluster) const = 0;


  // compute normal probability density function
  // for all i=1,..,n and k=1,..,K
  virtual void getAllPdf(double ** tabFik,double * tabProportion) const = 0;

  // compute normal probability density function
  // for the line x within the kCluster cluster
  virtual double getPdf(XEMSample * x, int64_t  kCluster) const = 0;


  //-----------
  // Algorithms
  //-----------

  /// Maximum a posteriori step method
  void MAPStep();

  /// Maximization step method
  virtual void MStep();

    
    //     SELECTORS for square matrices
  
  /// get TabSigma
  //XEMMatrix ** getTabSigma();

  /** @brief Selector
      @return Table of cluster scattering matrices Wk
  */
  XEMMatrix ** getTabWk() const;

  /** @brief Selector
      @return Scattering matrix W
  */
  XEMMatrix * getW() const;


  // edit (for debug)
  virtual void edit() = 0;

  /// Edit
  virtual void edit(ofstream & oFile, bool text=false) = 0;
  
  virtual void input(ifstream & fi) = 0;
  
  virtual double getLogLikelihoodOne() const = 0;

  /// recopie sans faire construction / destruction
  // utilisé par SMALL_EM, CEM_INIT
  virtual void recopy(XEMParameter * otherParameter) = 0;
  
  
  virtual void updateForCV(XEMModel * originalModel, XEMCVBlock & CVBlock) ;
	
	
	
	//init
	//----
	void updateForInitRANDOMorUSER_PARTITION(XEMSample ** tabSampleForInit, bool * tabClusterToInitialize);
  
	
	/// init user
	virtual void initUSER(XEMParameter * iParam) = 0;
  
protected :

  // Square matrices
    // Table of covariance Matrix of each cluster
  //XEMMatrix ** _tabSigma;

  // Table of inverse of covariance matrix of each cluster
  //XEMMatrix ** _tabInvSigma; 

  /// Table of cluster scattering matrix Wk
  XEMMatrix ** _tabWk;

  /// Scattering matrix W
  XEMMatrix * _W;
  
  // 1/det(Sigma)
  //double * _tabInvSqrtDetSigma;
  

  /// Table of means vector of each cluster
  double ** _tabMean;
    
    
  // called by constructor
  // update _freeProportion
  void initFreeProportion(XEMModelType * iModelType);
      
  
  /// compute Mean when only one cluster
  /// called by initRANDOM, getLogLikelihoodOne
  void computeMeanOne(double * Mean, double * weight, double** y_Store, int64_t  nbSample, double totalWeight)const;
    
  
  void putIdentityInDiagonalMatrix(double * mat_store);
  
  void putIdentityInMatrix(double * mat_store);
  
  void initDiagonalMatrixToZero(double * A_store);

  double determinantDiag(double * mat_store, XEMErrorType errorType);

};


//---------------
// inline methods
//---------------

inline double ** XEMGaussianParameter::getTabMean() const {
  return this->_tabMean;
}

/*inline XEMMatrix ** XEMGaussianParameter::getTabSigma(){
  return _tabSigma;
}*/


inline XEMMatrix ** XEMGaussianParameter::getTabWk() const{
  return _tabWk;
}

inline XEMMatrix * XEMGaussianParameter::getW()  const{
  return _W;
}

#endif
