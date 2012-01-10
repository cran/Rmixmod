/***************************************************************************
                             SRC/MIXMOD/XEMWeightColumnDescription.cpp  description
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

#include "XEMWeightColumnDescription.h"

XEMWeightColumnDescription::XEMWeightColumnDescription():XEMColumnDescription(){
}

XEMWeightColumnDescription::XEMWeightColumnDescription(int64_t index):XEMColumnDescription(index){
 _index = index; 
}

XEMWeightColumnDescription::~XEMWeightColumnDescription(){
  
}

string XEMWeightColumnDescription::editType(){
  return "Weight";
}

XEMColumnDescription * XEMWeightColumnDescription::clone()const{
  XEMWeightColumnDescription * WCD = new XEMWeightColumnDescription();
  WCD->_index = _index;
  WCD->_name = _name;
  return WCD;  
}
