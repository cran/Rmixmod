/***************************************************************************
                             SRC/MIXMOD/XEMLabelDescription.h  description
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

#ifndef XEMLABELDESCRIPTION_H
#define XEMLABELDESCRIPTION_H

#include "XEMDescription.h"

// pre-declaration
class XEMModel;
class XEMLabel;

class XEMLabelDescription : public XEMDescription
{  
  public:
    /// Default constructor
    XEMLabelDescription();   
    
    ///constructor by initilization
    XEMLabelDescription(int64_t nbSample, int64_t nbColumn, std::vector< XEMColumnDescription* > columnDescription, FormatNumeric::XEMFormatNumericFile format, std::string filename, std::string infoName = "");
    
		/// constructor from a vector of int
		XEMLabelDescription(int64_t nbSample, std::vector<int64_t> vLabel); 
		
    ///constructor after an estimation->run
    XEMLabelDescription(XEMModel * estimation);
    
    
    ///constructor by copy
    XEMLabelDescription(XEMLabelDescription & labelDescription);
    
    /// Destructor
    virtual ~XEMLabelDescription();
  
  
    /// Comparison operator
    bool operator ==(const XEMLabelDescription & labelDescription) const;
  
    ///operator=    
    XEMLabelDescription & operator=( XEMLabelDescription & labelDescription);
    
    const XEMLabel * getLabel() const;
    XEMLabel * getLabel();
  
    const int64_t getNbCluster() const;
  
    void saveNumericValues(std::string fileName="");
    
  private :
    
    XEMLabel * _label;
    XEMLabel * createLabel();
    int64_t _nbCluster;
};

inline const XEMLabel * XEMLabelDescription::getLabel() const{
  return _label;
}

inline XEMLabel * XEMLabelDescription::getLabel(){
  return _label;
}

inline const int64_t XEMLabelDescription::getNbCluster() const{
  return _nbCluster;
}
  
#endif // XEMLABELDESCRIPTION_H
