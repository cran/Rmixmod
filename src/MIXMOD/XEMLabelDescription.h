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
#include "XEMEstimation.h"
#include "XEMLabel.h"


class XEMLabelDescription : public XEMDescription
{  
  public:
    /// Default constructor
    XEMLabelDescription();   
    
    ///constructor by initilization
    XEMLabelDescription(int64_t nbSample, int64_t nbColumn, std::vector< XEMColumnDescription* > columnDescription, FormatNumeric::XEMFormatNumericFile format, string filename, string infoName = "");
        
    ///constructor after an estimation->run
    XEMLabelDescription(XEMEstimation * estimation);
    
    
    ///constructor by copy
    XEMLabelDescription(XEMLabelDescription & labelDescription);
    
    /// Destructor
    virtual ~XEMLabelDescription();
    
  
    ///operator=    
    XEMLabelDescription & operator=( XEMLabelDescription & labelDescription);
    
    const XEMLabel * getLabel() const;
    
    void saveNumericValues(string fileName="");
    
  private :
    
    XEMLabel * _label;
    XEMLabel* createLabel();
};

inline const XEMLabel * XEMLabelDescription::getLabel() const{
  return _label;
}



  
#endif // XEMLABELDESCRIPTION_H
