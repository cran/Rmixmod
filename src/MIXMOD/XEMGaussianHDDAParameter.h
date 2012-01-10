/***************************************************************************
                             SRC/MIXMOD/XEMGaussianHDDAParameter.h  description
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
#ifndef XEMGAUSSIANHDDAPARAMETER_H
#define XEMGAUSSIANHDDAPARAMETER_H



#include "XEMGaussianParameter.h"
#include "XEMMatrix.h"
#include "XEMGeneralMatrix.h"
#include "XEMOldInput.h"

class XEMGaussianHDDAParameter : public XEMGaussianParameter{

public:

  /// Default constructor
  XEMGaussianHDDAParameter();

  /// Constructor
  // called by XEMModel
  XEMGaussianHDDAParameter(XEMModel * iModel, XEMModelType *  iModelType);

  /// Constructor
  // called by XEMStrategyType
  XEMGaussianHDDAParameter(int64_t  iNbCluster, int64_t  iPbDimension, XEMModelType * iModelType, string & iFileName);

  /// Constructor
  XEMGaussianHDDAParameter(const XEMGaussianHDDAParameter * iParameter);

  
  /// Destructor
  virtual ~XEMGaussianHDDAParameter();

	/// reset to default values
	virtual void reset();


  /** @brief Selector
      @return A copy of the model
  */
  XEMParameter * clone() const;


 /** @brief Selector
      @return Table of shape matrix for each cluster
  */
  XEMDiagMatrix ** getTabShape() const;

  /** @brief Selector
      @return Table of orientation matrix for each cluster
  */
  XEMGeneralMatrix ** getTabQ() const;

  /** @brief Selector
      @return Control the shape of the density in the subspace Ei
  */
  double** getTabA() const;

  /** @brief Selector
      @return Control the shape of the density in the subspace orthogonal to Ei
  */
  double* getTabB() const;

  /** @brief Selector
      @return Dimension of each subspace
  */
  int64_t  * getTabD() const; 

  XEMSymmetricMatrix ** getTabGammak() const;

  double ** getGamma() const;

  void MStep();

	// init
	//-----
	
/// initialize attributes before an InitRandom  
	virtual void  initForInitRANDOM() ;

	
	
	
	
	/// initialize attributes for init USER_PARTITION
	/// outputs :
	/// -  nbInitializedCluster
	/// - tabNotInitializedCluster (array of size _nbCluster)
  void initForInitUSER_PARTITION(int64_t  & nbInitializedCluster, bool * tabNotInitializedCluster, XEMPartition * initPartition);
	
	

/// User initialisation of the parameters of the model
  virtual void initUSER(XEMParameter* iParam);


  double getLogLikelihoodOne() const;

  void edit();

  void edit(ofstream & oFile, bool text=false);

  void recopy(XEMParameter * otherParameter);

  double getPdf(int64_t  iSample, int64_t  kCluster) const;

  void getAllPdf(double ** tabFik,double * tabProportion) const;

  double getPdf(XEMSample * x, int64_t  kCluster)const;

  double* computeLoglikelihoodK(double** K);


  void input(ifstream & fi);

protected :
  /// Table of shape matrix of each cluster
  XEMDiagMatrix** _tabShape;

  /// Table of orientation matrix of each cluster
  XEMGeneralMatrix ** _tabQk;

  int64_t  __storeDim; 
///Table of parameters Akj of each cluster
  double ** _tabAkj;
///Table of parameters Bk of each cluster
  double * _tabBk;
///Table of sub dimension of each cluster
  int64_t  * _tabDk; 

/// _tabGammak = _Gammak * _Gammak' replaces matrix Wk when tabNk smaller than _pbDimension
  XEMSymmetricMatrix ** _tabGammak; // matrice nk * p
/// Array of individuals * weight - mean[k] in class k
  double ** _Gammak;

  int64_t  getFreeParameter()const;

  void computeTabWkW();

///compute function of cost for each tabQk_k
  double ** computeCost(XEMGeneralMatrix ** tabQ)const;

///compute parameters for the model AkjBkQk
  void computeAkjBkQk();
///compute parameters for the model AkjBQk
  void computeAkjBQk();
///compute parameters for the model AjBkQk
  void computeAjBkQk();
///compute parameters for the model AjBQk
  void computeAjBQk();

///compute parameters for the model AkBkQk
  void computeAkBkQk();
///compute parameters for the model AkBQk
  void computeAkBQk();


/// compute the intrinsic dimension when non given
  void computeTabDk();
};

inline XEMDiagMatrix** XEMGaussianHDDAParameter::getTabShape()const{
  return _tabShape;
}

inline XEMGeneralMatrix ** XEMGaussianHDDAParameter::getTabQ() const{
  return _tabQk;
}

inline double** XEMGaussianHDDAParameter::getTabA() const{
  return _tabAkj;
}

inline double* XEMGaussianHDDAParameter::getTabB() const{
  return _tabBk;
}

inline int64_t  * XEMGaussianHDDAParameter::getTabD() const{
  return _tabDk;
}



inline XEMSymmetricMatrix** XEMGaussianHDDAParameter::getTabGammak() const {
  return _tabGammak;
}

inline double ** XEMGaussianHDDAParameter::getGamma() const{
  return _Gammak;
}

#endif

