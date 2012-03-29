/***************************************************************************
                             SRC/MIXMOD/XEMDescription.h  description
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

#ifndef XEMDESCRIPTION_H
#define XEMDESCRIPTION_H

#include "XEMUtil.h"

// pre-declaration
class XEMColumnDescription;

class XEMDescription
{  
  public:
    /// Default constructor
    XEMDescription();   
        
    ///constructor by initilization
    XEMDescription(int64_t nbSample, int64_t nbColumn, vector<XEMColumnDescription *> columnDescription, FormatNumeric::XEMFormatNumericFile format, string filename, string InfoName="");
    
    ///constructor by copy
    XEMDescription(XEMDescription & description);
    
    /// Destructor
    virtual ~XEMDescription();
    
    ///initialization ColumnDescription by default
    void initializationColumnDescription();
  
    ///operator=    
    XEMDescription & operator=(const XEMDescription & description);
    
    /// get
    //get Name
    string getInfoName()const;
    
    //get NbSample
    int64_t getNbSample() const;
    
    //get FileName
    string getFileName()const ;
    
    //get NbColumn
    int64_t getNbColumn()const ;
    
    //get Format
    FormatNumeric::XEMFormatNumericFile getFormat() const;
    
    //get ColumnDescription
    const XEMColumnDescription * getColumnDescription(int64_t index)const;
    
    const vector<XEMColumnDescription*> getAllColumnDescription()const;
    
    int64_t getPbDimension() const;
    
    ///set
    //set InfoName
    void setInfoName(string & iInfoName);
    
    virtual void saveNumericValues(string fileName="") = 0;
    
  protected :
    
    string _infoName;
    int64_t _nbSample;
    int64_t _nbColumn;
    string _fileName; //numeric file name
    FormatNumeric::XEMFormatNumericFile _format;//format of  numeric file
    vector<XEMColumnDescription*> _columnDescription;//each variable has a description
    
    
};

inline string XEMDescription::getInfoName()const {
   return _infoName; 
}

inline int64_t XEMDescription::getNbSample()const {
   return _nbSample; 
}
  
inline int64_t XEMDescription::getNbColumn()const{
   return _nbColumn; 
}

inline string XEMDescription::getFileName()const{
   return _fileName; 
}

inline const XEMColumnDescription * XEMDescription::getColumnDescription(int64_t index)const{
  if (index>=0 && index <=_nbColumn)
    return _columnDescription[index];
  else throw wrongIndexInGetMethod;
}

inline const vector<XEMColumnDescription*> XEMDescription::getAllColumnDescription()const{
 return _columnDescription; 
}

inline FormatNumeric::XEMFormatNumericFile XEMDescription::getFormat()const{
   return _format; 
}
 
inline void XEMDescription::setInfoName(string & iInfoName){
  _infoName = iInfoName; 
}

  
#endif // XEMDESCRIPTION_H
