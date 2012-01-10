/***************************************************************************
                             SRC/MIXMOD/XEMIndividualColumnDescription.h  description
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
#ifndef XEMINDIVIDUALCOLUMNDESCRIPTION_H
#define XEMINDIVIDUALCOLUMNDESCRIPTION_H

#include "XEMColumnDescription.h"
  
//Individual Description
struct IndividualDescription{
  int64_t num;
  string name;
};

class XEMIndividualColumnDescription : public XEMColumnDescription
{
  public :
    /// Default constructor
    XEMIndividualColumnDescription();   
    
    /// initialization constructor
    XEMIndividualColumnDescription(int64_t index);
    
    /// Destructor
    virtual ~XEMIndividualColumnDescription();
    
    string editType();
    
    XEMColumnDescription * clone()const;
    
    const vector<IndividualDescription> & getIndividualDescription()const;
    
    void setIndividualDescription(IndividualDescription & individualDescription, int64_t index);
    
    void insertIndividualDescription(IndividualDescription individualDescription, int64_t index);
    
  private :
    vector<IndividualDescription> _individualDescription;
    
};

inline const vector<IndividualDescription> & XEMIndividualColumnDescription::getIndividualDescription()const{
 return _individualDescription;
}

#endif // XEMINDIVIDUALCOLUMNDESCRIPTION_H
