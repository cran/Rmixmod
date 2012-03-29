/***************************************************************************
                             SRC/MIXMOD/XEMQualitativeColumnDescription.cpp  description
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

#include "XEMQualitativeColumnDescription.h"

XEMQualitativeColumnDescription::XEMQualitativeColumnDescription():XEMColumnDescription(){
}

XEMQualitativeColumnDescription::XEMQualitativeColumnDescription(int64_t index, int64_t nbFactor):XEMColumnDescription(index){
 _nbFactor = nbFactor;
 _variableDescription.resize(nbFactor);
 for (int64_t i = 0; i<nbFactor ; ++i){
    _variableDescription[i].name = "";
    _variableDescription[i].num = i+1;    
  }
}


XEMQualitativeColumnDescription::~XEMQualitativeColumnDescription(){
  
}

string XEMQualitativeColumnDescription::editType(){
  return "Qualitative";
}

XEMColumnDescription * XEMQualitativeColumnDescription::clone()const{
  XEMQualitativeColumnDescription * QCD = new XEMQualitativeColumnDescription();
  
  QCD->_index = _index;
  QCD->_name = _name;
  QCD->_nbFactor = _nbFactor;
  
  //filling of structure which describes variables
  QCD->_variableDescription.resize(_variableDescription.size());
  for (unsigned int i = 0; i<_variableDescription.size() ; ++i){
    VariableDescription varDescription;
    varDescription.name = _variableDescription[i].name;
    varDescription.num = _variableDescription[i].num;
    QCD->_variableDescription[i] = varDescription;
  }
  return QCD;  
}

void XEMQualitativeColumnDescription::setVariableDescription(VariableDescription & variableDescription, unsigned int index){
  if (index<_variableDescription.size()){
    _variableDescription[index].name  = variableDescription.name;
    _variableDescription[index].num  = variableDescription.num;
  }
}
