/***************************************************************************
                             SRC/MIXMOD/XEMBinaryData.h  description
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
#ifndef XEMBINARYDATA_H
#define XEMBINARYDATA_H

#include "XEMData.h"

// pre-declaration
class XEMPartition;
class XEMBinarySample;

/**
  @brief Base class for Binary Data
  @author F Langrognet & A Echenim
*/

class XEMBinaryData : public XEMData{

public:

	/// Default constructor
	XEMBinaryData();

 	/// Constructor
  XEMBinaryData(const XEMBinaryData & iData);

  /// Constructor
  XEMBinaryData(int64_t nbSample, int64_t pbDimension, const std::string & dataFileName, int64_t * tabNbModality);
  
  /// Constructor
  XEMBinaryData(int64_t nbSample, int64_t pbDimension, std::vector<int64_t>  nbModality);

  /// Constructor
  XEMBinaryData(int64_t nbSample, int64_t pbDimension, std::vector<int64_t>  nbModality, int64_t ** matrix);

  /// Constructor for dataReduce
  XEMBinaryData(int64_t nbSample, int64_t pbDimension, int64_t * tabNbModality, double weightTotal, XEMSample **& matrix, double * weight);
  

  /// Constructor (used in DCV context)
  XEMBinaryData(int64_t nbSample, int64_t pbDimension, XEMData * originalData, XEMCVBlock & block);
  
	/// Desctructor
	virtual ~XEMBinaryData();


  
  virtual void input(const XEMDataDescription & dataDescription);
  
  /** @brief copy
      @return A copy of data
  */
  virtual XEMData * clone() const;

  /**  @brief Copy
       @return A copy data matrix
  */
  virtual XEMSample ** cloneMatrix();

  /** @brief Read data from binary data file
      @param fi Binary Data file to read
  */
  virtual void input(std::ifstream & fi);

  /** @brief Write binary data in output file
      @param fo Output file to write into
  */
  virtual void output(std::ostream & fo);

  
  virtual bool verify()const;
  
  /** @brief Get matrix of data Sample
      @return A vector of XEMSample
  */
   XEMSample ** getDataMatrix() const;

  /** @brief Get matrix of data Sample
      @param idxSample Index of sample to get values
      @return A vector of XEMSample
  */
   int64_t * getDataTabValue(int64_t idxSample) const;

  /** @brief Get tab modality
      @return A vector of number of modality
  */
   int64_t * getTabNbModality() const;
   
   XEMData * reduceData(std::vector<int64_t> & correspondcenceOriginDataToReduceData, XEMPartition * knownPartition, XEMPartition * initPartition, XEMPartition *& oKnownPartition, XEMPartition *& oInitPartition);
  

protected :    

  /// Array of modality for each dimension
  int64_t * _tabNbModality;
	
};



//---------------
// inline methods
//---------------

inline  XEMSample ** XEMBinaryData::getDataMatrix() const{
  return _matrix;
}

inline  int64_t * XEMBinaryData::getTabNbModality() const{
  return _tabNbModality;
}


#endif
