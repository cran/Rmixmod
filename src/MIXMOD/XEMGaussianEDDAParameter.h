/***************************************************************************
                             SRC/MIXMOD/XEMGaussianEDDAParameter.h  description
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
#ifndef XEMGaussianEDDAParameter_H
#define XEMGaussianEDDAParameter_H

#include "XEMGaussianParameter.h"

// pre-declaration
class XEMMatrix;

class XEMGaussianEDDAParameter : public XEMGaussianParameter{

public:

/// Default constructor
  XEMGaussianEDDAParameter();

  /// Constructor
  // called by XEMModel
  XEMGaussianEDDAParameter(XEMModel * iModel, XEMModelType *  iModelType);

  
 /// Constructor
	// called if USER initialisation
  XEMGaussianEDDAParameter(int64_t  iNbCluster, int64_t  iPbDimension, XEMModelType * iModelType);


  /// Constructor
  XEMGaussianEDDAParameter(const XEMGaussianEDDAParameter * iParameter);

  /// Destructor
  virtual ~XEMGaussianEDDAParameter();
  
  /// Comparison operator
  virtual bool operator ==(const XEMGaussianEDDAParameter & param) const;
  
	/// reset to default values
	virtual void reset();


 /** @brief Selector
      @return Table of inverse of sqrt of determinant of covariance matrix for each cluster
  */
  double * getTabInvSqrtDetSigma() const;

  /** @brief Selector
      @return Table of inverse of covariance matrix for each cluster
  */
  XEMMatrix ** getTabInvSigma() const;


 /** @brief Selector
      @return Table of covariance matrix for each cluster
  */

  XEMMatrix ** getTabSigma() const;
 
  /// Compute normal probability density function
  ///       for iSample the sample and kCluster th cluster
  double getPdf(int64_t  iSample, int64_t  kCluster) const;


  /// compute normal probability density function
  /// for all i=1,..,n and k=1,..,K
  void getAllPdf(double ** tabFik,double * tabProportion) const;


  /// compute normal probability density function
  /// for the line x within the kCluster cluster
  double getPdf(XEMSample * x, int64_t  kCluster) const;

  void updateTabInvSigmaAndDet() ;

  void computeTikUnderflow(int64_t  i, double ** tabTik);

  void edit();

  void edit(std::ofstream & oFile, bool text=false);

  void recopy(XEMParameter * otherParameter);

  virtual int64_t  getFreeParameter()  const= 0;
  virtual void computeTabSigma() = 0;

  void updateForCV(XEMModel * originalModel, XEMCVBlock & CVBlock) ;

		
  virtual XEMParameter* clone() const = 0 ;
	void MStep();
	void MAPStep();
	virtual void input(std::ifstream & fi);
	virtual void input( double * proportions
                    , double **  means
                    , double *** variances
                    );
	
	//init
	//----
	
  /// User initialisation of the parameters of the model
	virtual void initUSER(XEMParameter * iParam);
	
	
/// initialize attributes before an initRANDOM
	void initForInitRANDOM();
	
	
	/// initialize attributes for init USER_PARTITION
	/// outputs :
	/// -  nbInitializedCluster
	/// - tabNotInitializedCluster (array of size _nbCluster)
  void initForInitUSER_PARTITION(int64_t  & nbInitializedCluster, bool * tabNotInitializedCluster, XEMPartition * initPartition);

protected :
  /// Table of inverse of covariance matrix of each cluster
  XEMMatrix ** _tabInvSigma; 

  /// Table of covariance Matrix of each cluster
  XEMMatrix ** _tabSigma;

  /// 1/det(Sigma)
  double * _tabInvSqrtDetSigma;

	
};


inline double * XEMGaussianEDDAParameter::getTabInvSqrtDetSigma() const{
  return _tabInvSqrtDetSigma;
}

inline XEMMatrix ** XEMGaussianEDDAParameter::getTabInvSigma() const{
  return _tabInvSigma;
}

inline XEMMatrix ** XEMGaussianEDDAParameter::getTabSigma() const{
  return _tabSigma;
}

#endif
