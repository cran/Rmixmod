/***************************************************************************
                             SRC/MIXMOD/XEMGaussianData.h  description
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
#ifndef XEMGAUSSIANDATA_H
#define XEMGAUSSIANDATA_H

#include "XEMUtil.h"
#include "XEMData.h"
#include "XEMGaussianSample.h"
#include "XEMOldInput.h"

/**
  @brief Base class for Gaussian Data
  @author F Langrognet & A Echenim
*/

class XEMGaussianData : public XEMData{

public:

  /// Default constructor
  XEMGaussianData();

  /// Constructor
  XEMGaussianData(const XEMGaussianData & iData);
  
  /// Constructor
  XEMGaussianData(int64_t nbSample, int64_t pbDimension, const string & dataFileName);
  
  /// Constructor  (without fill matrix)
  XEMGaussianData(int64_t nbSample, int64_t pbDimension);
  
  /// Constructor  (with matrix)
  XEMGaussianData(int64_t nbSample, int64_t pbDimension, double ** matrix);
  
  /// Constructor
  XEMGaussianData(int64_t nbSample, int64_t pbDimension, double weightTotal, XEMSample **& matrix, double * weight);

  /// Constructor
  // used in DCV context
  XEMGaussianData(int64_t nbSample, int64_t pbDimension, XEMData * originalData, XEMCVBlock & block);
  
  /// Desctructor
  virtual ~XEMGaussianData();


  /** @brief Selector
      @return A copy of data
  */
  XEMData * clone() const;

  /**  @brief Copy
       @return A copy data matrix
  */
  XEMSample ** cloneMatrix();

  
  //TODO a enlever XEMInput
  /** @brief Read data from gaussian data file
      @fi Gaussian Data file to read
  */
  void input(ifstream & fi);
  
  /** @brief Read data from XEMDataDescription
  */
  void input(const XEMDataDescription & dataDescription);

  /** @brief Write gaussian data in output file
      @f0 Output file to write into
  */
  void output(ostream & fo);

  bool verify()const; 
  
  /// pointer to stored values
  double ** _yStore ;
  
  double ** getYStore();
  
  double getInv2PiPow() const;
  
  double getHalfPbDimensionLog2Pi() const;
  
  double getPbDimensionLog2Pi() const ;
  
  double * getTmpTabOfSizePbDimension() const;


  protected :
  
    /// 1/ (2 * pi)^(d/2)
    double _Inv2PiPow ; 
    
    /// 0.5 * p * log(2 * PI)
    double _halfPbDimensionLog2Pi;
    
    double _pbDimensionLog2Pi;
  
    
   // tableau de double longueur _pbDimension 
   // utilisé pour ne pas avoir à faire sans arret allocation / destruction
   // utilisé par XEMnorme, getLogLikeLihoodOne
  double * __tmpTabOfSizePbDimension;
  
  bool _deleteSamples ;

};


inline double ** XEMGaussianData::getYStore(){
  return _yStore;
}

inline double XEMGaussianData::getInv2PiPow() const{
  return _Inv2PiPow;
}

inline double XEMGaussianData::getHalfPbDimensionLog2Pi()const{
  return _halfPbDimensionLog2Pi;
}

inline double XEMGaussianData::getPbDimensionLog2Pi()const{
  return _pbDimensionLog2Pi;
}


inline double* XEMGaussianData::getTmpTabOfSizePbDimension()const{
  return __tmpTabOfSizePbDimension;
}


#endif
