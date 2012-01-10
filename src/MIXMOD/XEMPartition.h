/***************************************************************************
                             SRC/MIXMOD/XEMPartition.h  description
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
#ifndef XEMPARTITION_H
#define XEMPARTITION_H

#include "XEMUtil.h"


/** @brief Base class for Partition(s)
@author F Langrognet & A Echenim
*/

class XEMPartition{
  
  public:
    
    /// Default constructor
    XEMPartition();
    
    /// Constructor
    XEMPartition(XEMPartition * iPartition);
    
    /// Constructor 
    XEMPartition(int64_t nbSample, int64_t nbCluster, const XEMNumericPartitionFile & partitionFile);
    
    /// Constructor
    // used in DCV context
    XEMPartition(XEMPartition * originalPartition,  XEMCVBlock & block);
    
    
    /// Destructor
    virtual ~XEMPartition();
    
    
    /** @brief Set the dimension of the Partition
    @param nbSample The number of samples
    @param nbCluster The number of clusters
    */
    void setDimension(int64_t nbSample, int64_t nbCluster);
    
    int64_t getGroupNumber(int64_t idxSample);
    
    
    /** verify if partition is complete 
    - each line has one (and only one) '1'
    - each cluster appears at least one time
    */
    bool isComplete();
    int64_t** getTabValue() const;
    int64_t* getTabValueI(int64_t index) const;
    
    
    ///get Format
    const XEMNumericPartitionFile & getPartitionFile() const;
    
    /// get Number of samples
    int64_t getNbSample()const;
    
    /// get Number of clusters
    int64_t getNbCluster() const;
    
    bool operator==(XEMPartition & otherPartition);
    
    
    /** @brief Friend method
    @return Operator >> overloaded to read Partition from input files
    */	
    friend ifstream & operator >> (ifstream & fi, XEMPartition & partition);
    
    /** @brief Friend method
    @return Operator << overloaded to write Partition 
    */
    friend ostream & operator << (ostream & fo, const XEMPartition & partition);	
    
    /// Number of samples
    int64_t _nbSample;
    
    /// Number of clusters
    int64_t _nbCluster;
    
    int64_t ** _tabValue;     // aggregate
    
    ///name of partition
    XEMNumericPartitionFile _partitionFile;
    
    bool   _deleteValues;
    
};

inline int64_t ** XEMPartition::getTabValue() const{
  return _tabValue;
}

inline int64_t * XEMPartition::getTabValueI(int64_t index) const{
  return _tabValue[index];
}

inline const XEMNumericPartitionFile & XEMPartition::getPartitionFile() const{
  return _partitionFile;
}

inline int64_t XEMPartition::getNbSample()const{
  return _nbSample;
}

inline int64_t XEMPartition::getNbCluster() const{
  return _nbCluster;
}
#endif
