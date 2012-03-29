/***************************************************************************
                             SRC/MIXMOD/XEMProbaDescription.h  description
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

#ifndef XEMPROBADESCRIPTION_H
#define XEMPROBADESCRIPTION_H

#include "XEMDescription.h"

// pre-declaration
class XEMModel;
class XEMProba;

class XEMProbaDescription : public XEMDescription
{  
  public:
    /// Default constructor
    XEMProbaDescription();   
        
    ///constructor by initilization
    XEMProbaDescription(int64_t nbSample,  int64_t nbCluster, FormatNumeric::XEMFormatNumericFile format, string filename, string infoName="");
    
    ///constructor after an estimation->run
    XEMProbaDescription(XEMModel * model);
    
    
    ///constructor by copy
    XEMProbaDescription(XEMProbaDescription & probaDescription);
    
    /// Destructor
    virtual ~XEMProbaDescription();
    
  
    /// Comparison operator
    bool operator==( XEMProbaDescription & probaDescription) const;
  
    ///operator=    
    XEMProbaDescription & operator=( XEMProbaDescription & probaDescription);
    
    XEMProba * getProba();
    
    void saveNumericValues(string fileName="");
    
    //void editProba(ostream & f) const;
    
  private :
    
    XEMProba * _proba;
};

inline XEMProba * XEMProbaDescription::getProba(){
  return _proba;
}



  
#endif // XEMPROBADESCRIPTION_H
