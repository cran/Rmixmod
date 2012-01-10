/***************************************************************************
                             SRC/MIXMOD/XEMColumnDescription.h  description
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
#ifndef XEMCOLUMNDESCRIPTION_H
#define XEMCOLUMNDESCRIPTION_H

#include "XEMUtil.h"

class XEMColumnDescription
{
  public :
    /// Default constructor
    XEMColumnDescription();
    
    /// Initialization constructor
    XEMColumnDescription(int64_t index);
    
    /// Destructor
    virtual ~XEMColumnDescription();
    
    //
    virtual string editType()=0;
    
    
    virtual XEMColumnDescription * clone()const = 0;
    
    ///selector
    
    ///get index of column
    const int64_t & getIndex()const ;
    
    ///get name of column 
    const string & getName()const;
    
    ///set name of column
    void setName(string & strName);
    
  protected : 
    
    ///index of column (0 to XEMDataDescription::_nbColumn-1)
    int64_t _index;
    
    ///name of column (optional)
    string _name;
};

inline const int64_t & XEMColumnDescription::getIndex()const{
  return _index;
}

inline const string & XEMColumnDescription::getName()const{
  return _name; 
}

inline void XEMColumnDescription::setName(string & strName){
   _name = strName;
}


#endif // XEMCOLUMNDESCRIPTION_H
