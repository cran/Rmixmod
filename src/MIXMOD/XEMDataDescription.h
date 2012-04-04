/***************************************************************************
                             SRC/MIXMOD/XEMDataDescription.h  description
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

#ifndef XEMDATADESCRIPTION_H
#define XEMDATADESCRIPTION_H

#include "XEMDescription.h"

// pre-declaration
class XEMData;
class XEMGaussianData;
class XEMBinaryData;

class XEMDataDescription : public XEMDescription
{  
  public:
    /// Default constructor
    XEMDataDescription();   
        
    ///constructor by initilization
    XEMDataDescription(int64_t nbSample, int64_t nbColumn, std::vector<XEMColumnDescription *> columnDescription, FormatNumeric::XEMFormatNumericFile format, std::string filename, std::string infoName="");
    
    /// constructor with a gaussianData
    XEMDataDescription(XEMGaussianData * gData);
    
    /// constructor with a binaryData
    XEMDataDescription(XEMBinaryData * bData);
    
    ///constructor by copy
    XEMDataDescription(XEMDataDescription & dataDescription);
    
    /// Destructor
    virtual ~XEMDataDescription();
    
  
    ///operator=    
    XEMDataDescription & operator=(const XEMDataDescription & dataDescription);
    
    XEMData * getData() const;
    
    /// is binary Data ?
    bool isBinaryData() const;
    
    /// verify data validity
    bool verifyData() const;
    
    void saveNumericValues(std::string fileName="");
    
  private :
    
    XEMData * _data;
    
    /// Create and return XEMData *
    XEMData * createData() const;
};

inline XEMData * XEMDataDescription::getData() const{
  return _data;
}


inline void XEMDataDescription::saveNumericValues(std::string fileName){
  throw internalMixmodError;
}


  
#endif // XEMDATADESCRIPTION_H
