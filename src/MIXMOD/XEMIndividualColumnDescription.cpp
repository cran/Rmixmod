/***************************************************************************
                             SRC/MIXMOD/XEMIndividualColumnDescription.cpp  description
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

#include "XEMIndividualColumnDescription.h"


XEMIndividualColumnDescription::XEMIndividualColumnDescription():XEMColumnDescription(){  
}

XEMIndividualColumnDescription::XEMIndividualColumnDescription(int64_t index):XEMColumnDescription(index){
  _index = index; 
  _individualDescription.resize(0);
}

XEMIndividualColumnDescription::~XEMIndividualColumnDescription(){  
}

string XEMIndividualColumnDescription::editType(){
  return "Individual";
}

XEMColumnDescription * XEMIndividualColumnDescription::clone()const{
  XEMIndividualColumnDescription * ICD = new XEMIndividualColumnDescription();
  ICD->_index = _index;
  ICD->_name = _name;
  
  //filling of structure which describes individuals
  ICD->_individualDescription.resize(_individualDescription.size());
  for (int64_t i = 0; i<_individualDescription.size() ; ++i){
    IndividualDescription indDescription;
    indDescription.name = _individualDescription[i].name;
    indDescription.num = _individualDescription[i].num;
    ICD->_individualDescription[i] = indDescription;
  }
  return ICD;  
}

void XEMIndividualColumnDescription::setIndividualDescription(IndividualDescription & individualDescription, int64_t index){
  if (index>=0 && index<_individualDescription.size()){
    _individualDescription[index].name  = individualDescription.name;
    _individualDescription[index].num  = individualDescription.num;
  }
}

void XEMIndividualColumnDescription::insertIndividualDescription(IndividualDescription individualDescription, int64_t index){
  if (index>=0 && index<=_individualDescription.size()){
    _individualDescription.insert(_individualDescription.begin()+index, individualDescription);
  }else{
    throw;
  }
}
