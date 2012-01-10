/***************************************************************************
                             SRC/MIXMOD/XEMParameter.h  description
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
#ifndef XEMParameter_H
#define XEMParameter_H

#include "XEMUtil.h"
#include "XEMModelType.h"
#include "XEMData.h"
#include "XEMMatrix.h"
#include "XEMPartition.h"

class XEMModel;

/**
  @brief Base class for XEMParameter(s)
  @author F Langrognet & A Echenim
  */

class XEMParameter{

public:


  /// Default constructor
  XEMParameter();

  /// Constructor
  // called by XEMGaussianParameter or XEMBinaryParameter (called by XENStrategyType)
	// if USER initialisation
  XEMParameter(int64_t  iNbCluster, int64_t  iPbDimension, XEMModelType * iModelType);

  /// Constructor
  // called by XEMParameter (called by XEMModel)
  XEMParameter(XEMModel * iModel, XEMModelType * iModelType);

  /// Constructor ccopy)
  XEMParameter(const XEMParameter * iParameter);

  /// Destructor
  virtual ~XEMParameter();
	
	
	/// reset to default values
	virtual void reset()=0;
	
	//void initRandomForMeanOrCenter();

	
  /** @brief Selector
      @return A copy of the model
  */
  virtual XEMParameter * clone()const =0 ;



  
  
  //--------
  // compute
  //--------

  /** Compute normal probability density function
       for iSample the sample and kCluster th cluster
       */
  virtual double getPdf(int64_t  iSample, int64_t  kCluster)  const = 0;


  // compute normal probability density function
  // for all i=1,..,n and k=1,..,K
  virtual void getAllPdf(double ** tabFik, double * tabProportion) const = 0;


  /** Compute normal probability density function
       for x vector and kCluster th cluster
       */
  virtual double getPdf(XEMSample * x, int64_t  kCluster)  const= 0;


  // computeTabProportion
  void computeTabProportion();


  /** @brief Selector
      @return The number of free parameters
  */
  virtual int64_t  getFreeParameter() const = 0;

  /// get loglikelihood with one cluster
  virtual double getLogLikelihoodOne() const = 0;
  
  /// compute Tik for xi (i=0 -> _nbSample-1) when underflow
  virtual void computeTikUnderflow(int64_t  i, double ** tabTik);


  //---------------
  // initialization
  //---------------

  /// init user
  virtual void initUSER(XEMParameter * iParam) = 0;


  int64_t  generateRandomIndex(bool * tabIndividualCanBeUsedForInitRandom, double * weight, double totalWeight);
		
	/// initialize attributes before an InitRandom  
	virtual void  initForInitRANDOM() = 0;

	virtual void updateForInitRANDOMorUSER_PARTITION(XEMSample ** tabSampleForInit, bool * tabClusterToInitialze) = 0;
	
	
	
	/// initialize attributes for init USER_PARTITION
	/// outputs :
	/// -  nbInitializedCluster
	/// - tabNotInitializedCluster (array of size _nbCluster)
	
  virtual void initForInitUSER_PARTITION(int64_t  & nbInitializedCluster, bool * tabNotInitializedCluster, XEMPartition * initPartition) = 0;
	


  //-----------
  // Algorithms
  //-----------

  /// Maximum a posteriori step method
  //virtual void MAPStep() = 0;

  /// Maximization step method
  virtual void MStep()= 0;



  //-------
  // in/out
  //-------

  // edit (for debug)
  virtual void edit()=0;

  /// edit
  virtual void edit(ofstream & oFile, bool text=false)=0;

  /// input
  virtual void input(ifstream & fi)=0;


  //-------
  // select
  //-------

  /// get TabProportion
  double * getTabProportion() const ;
  
  /// get nbCluster
  int64_t  getNbCluster() const ;

  /// get pbDimension
  int64_t  getPbDimension() const ;

  /// getFreeProportion
  bool getFreeProportion() const ;

  /// getModel
  XEMModel * getModel() const ;

  /// getModelType
  XEMModelType * getModelType() const ;
  
  /// setModel
  void setModel(XEMModel * iModel);
  
  ///getFilename
  const string & getFilename() const;
  
  ///setFilename
  void setFilename(const string & filename);
  
  /// recopie sans faire construction / destruction
  // utilisé par SMALL_EM, CEM_INIT
  virtual void recopy(XEMParameter * otherParameter) = 0;
  
  
  virtual void updateForCV(XEMModel * originalModel, XEMCVBlock & CVBlock) = 0;
  
  ///get Format
  FormatNumeric::XEMFormatNumericFile getFormat()const;
	
  ///set FormatNumeric
  void setFormat(const FormatNumeric::XEMFormatNumericFile format);
  
protected :

  /// Number of classes
  int64_t  _nbCluster;

  /// problem dimension
  int64_t  _pbDimension;
  
  /// Table of proportion of each cluster
  double * _tabProportion;

  /// free proportion ?
  bool _freeProportion;

  /// model
  XEMModel * _model; // not agregated

  ///modelType
  XEMModelType * _modelType;
	
private :
  
  ///filename
  string _filename;

  FormatNumeric::XEMFormatNumericFile _format;
};

 //---------------
 // inline methods
 //---------------

inline int64_t  XEMParameter::getNbCluster() const {
  return _nbCluster;
}

inline int64_t  XEMParameter::getPbDimension() const {
  return _pbDimension;
}


inline double * XEMParameter::getTabProportion() const {
  return _tabProportion;
}

inline bool XEMParameter::getFreeProportion() const {
  return _freeProportion;
}

inline XEMModel * XEMParameter::getModel() const {
  return _model;
}

inline  void XEMParameter::setModel(XEMModel * iModel) {
  _model = iModel;
}

inline XEMModelType * XEMParameter::getModelType() const {
  return _modelType;
}

inline const string & XEMParameter::getFilename() const{
  return _filename;
}

inline void XEMParameter::setFilename(const string & filename){
  _filename = filename;
}

inline FormatNumeric::XEMFormatNumericFile XEMParameter::getFormat() const{
  return _format;
}

inline void XEMParameter::setFormat(const FormatNumeric::XEMFormatNumericFile format){
  _format = format;
}


#endif
