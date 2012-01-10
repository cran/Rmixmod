/***************************************************************************
                             SRC/MIXMOD/XEMLabelDescription.cpp  description
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

#include "XEMLabelDescription.h"

//------------
// Constructor by default
//------------
XEMLabelDescription::XEMLabelDescription() : XEMDescription(){
  _label = NULL;
}



// ---------------------------
//constructor by initilization
// ---------------------------
XEMLabelDescription::XEMLabelDescription(int64_t nbSample, int64_t nbColumn, std::vector< XEMColumnDescription* > columnDescription, FormatNumeric::XEMFormatNumericFile format, string filename, string infoName):XEMDescription(nbSample, nbColumn, columnDescription, format, filename,  infoName){
  _label = createLabel();
//   _infoName = "infoName";
//   _nbSample = nbSample;
//   _nbColumn = 1;
//   _fileName = filename;
//   _format = format;
//   _columnDescription.resize(1);
//   _columnDescription[0] = new XEMQualitativeColumnDescription(0, nbCluster);
//   string name("Label");
//   _columnDescription[0]->setName(name);
//   _label = new XEMLabel(_nbSample);
//   ifstream fi(filename.c_str(), ios::in);
//   if (! fi.is_open()){
//     throw wrongLabelFileName;
//   }
//   _label->input(fi, nbCluster);
}


//-------------------------------------
// Constructor after an estimation->run
//--------------------------------------
XEMLabelDescription::XEMLabelDescription(XEMEstimation * estimation) : XEMDescription(){
   if (estimation){
    _infoName = "Label";
    _nbSample = estimation->getNbSample();
    _nbColumn = 1;
    _fileName = ""; 
    _format = FormatNumeric::txt;
    _columnDescription.resize(1);
    _columnDescription[0] = new XEMQualitativeColumnDescription(0, estimation->getNbCluster());
    string name("Label");
    _columnDescription[0]->setName(name);
    _label = new XEMLabel(estimation);
  }
  else{
    throw nullPointerError;
  }
}








//------------
// Constructor by copy
//------------
XEMLabelDescription::XEMLabelDescription(XEMLabelDescription & labelDescription){
  (*this) = labelDescription; 
}


//------------
// operator =
//------------
XEMLabelDescription & XEMLabelDescription::operator=( XEMLabelDescription & labelDescription){
  _fileName = labelDescription._fileName;
  _format = labelDescription._format;
  _infoName = labelDescription._infoName;
  _nbSample = labelDescription._nbSample;
  _nbColumn = labelDescription._nbColumn;
  _columnDescription.resize(_nbColumn);
  _label = new XEMLabel(*(labelDescription.getLabel()));
}

//------------
// Destructor
//------------
XEMLabelDescription::~XEMLabelDescription(){  
}


//--------
// ostream
//--------
void XEMLabelDescription::saveNumericValues(string fileName){
  //if (_fileName==""){
    ofstream fo(fileName.c_str(), ios::out);
    _label->edit(fo);
    _fileName = fileName;
  //}
  /* else : if _fileName!="", labelDescription has been created by a XML file.
  In this case, the numeric file already exists. 
  */
}


XEMLabel* XEMLabelDescription::createLabel()
{
  XEMLabel * label = new XEMLabel();
  
  int64_t nbQualitativeVariable = 0;
  int64_t nbIndividualVariable = 0;

  for (int64_t i=0; i<_nbColumn; i++){
    if (typeid(*(_columnDescription[i])) == typeid(XEMQualitativeColumnDescription)){
      nbQualitativeVariable++;
    }
    if (typeid(*(_columnDescription[i]))==typeid(XEMQuantitativeColumnDescription)){
      throw badLabelDescription;
    }
    if (typeid(*(_columnDescription[i]))==typeid(XEMWeightColumnDescription)){
        throw tooManyWeightColumnDescription;
    }
    if (typeid(*(_columnDescription[i]))==typeid(XEMIndividualColumnDescription)){
      nbIndividualVariable++;
    }
  } 

  if (nbQualitativeVariable != 1 || nbIndividualVariable>1){
    throw badLabelDescription;
  }
  label-> input(*this);
  return label;
}
