/***************************************************************************
                             SRC/MIXMOD/XEMData.h  description
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
#ifndef XEMDATA_H
#define XEMDATA_H

#include "XEMUtil.h"
#include "XEMSample.h"
#include "XEMDataDescription.h"


/**
  @brief Base class for Data
  @author F Langrognet & A Echenim
  */

class XEMData{

public:

	/// Default constructor
	XEMData();

 	/// Constructor
  XEMData(const XEMData & iData);

  /// Constructor
  XEMData(int64_t nbSample, int64_t pbDimension);

 	/// Constructor (for dataReduce)
  XEMData(int64_t nbSample, int64_t pbDimension, double weightTotal, double * weight);

	/// Desctructor
	virtual ~XEMData();


  /** @brief Selector
		  @param weightTotal Value to set total weight of samples
	*/
  void setWeightTotal(double weightTotal);

  /// setWeight
  void setWeight(string weightFileName);
  /// setWeight
  void setWeight(double* weight);

  /// setWeightDefault
  void setWeightDefault();

  /** @brief Read data from data file
      @fi Data file to read
  */
  virtual void input(ifstream & fi)=0;

  
  /** @brief Read data from XEMDataDescription
  */
  virtual void input(const XEMDataDescription & dataDescription)=0;
  
  
  /** @brief Write data in output file
      @f0 Output file to write into
  */
  virtual void output(ostream & fo)=0;

  /** @brief Selector
      @return A copy of data
  */
  virtual XEMData * clone() const =0;

  virtual XEMSample ** cloneMatrix()=0;

  virtual bool verify() const;
  
  const string & getFileName()const;

  /// Problem dimension
  int64_t _pbDimension;

  /// Number of samples
  int64_t _nbSample;

  /// Weight total of samples
  double _weightTotal;

  /// Array of samples values (size=nbSample)
  XEMSample ** _matrix;

  /// Weight column vector
  double * _weight;

  /// getMatrix
  const XEMSample** getData() const ;

  /// getMatrix[i]
  const XEMSample* getDataI(int64_t index) const ;

  ///getWeight
  const double* getWeight() const;

  ///getWeight[i]
  const double & getWeightI(int64_t index) const;

  ///get FilenameWeight
  const string & getFileNameWeight()const;
  
    /// get dimension
  int64_t getPbDimension()const;
  
    /// get Number of samples
  int64_t getNbSample()const;
  
  /// hasDefaultWeight
  bool hasDefaultWeight() const;
  
protected :
  
  //TODO XEMInput : a enlever
  ///filename of weight
  string _fileNameWeight;
  
  /// defaultWeight
  bool _defaultWeight;
  
  ///filename of data
  string _fileNameData;
  
};

inline  const XEMSample ** XEMData::getData() const{
  return const_cast<const XEMSample**>(_matrix);
}

inline  const string & XEMData::getFileNameWeight() const{
  return _fileNameWeight;
}

inline  const string & XEMData::getFileName() const{
  return _fileNameData;
}

inline  const XEMSample * XEMData::getDataI(int64_t index) const{
  return _matrix[index];
}

inline const double * XEMData::getWeight() const{
  return _weight;
}

inline const double & XEMData::getWeightI(int64_t index) const{
  return _weight[index];
}
    /// get dimension
inline int64_t XEMData::getPbDimension()const{
 return  _pbDimension;
}
  
/// get Number of samples
inline int64_t XEMData::getNbSample()const{
 return _nbSample; 
}
/// hasDefaultWeight
inline bool XEMData::hasDefaultWeight() const{
  return _defaultWeight;
}


#endif
