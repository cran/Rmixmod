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
#include "XEMLabel.h"
#include "XEMModel.h"
#include "XEMWeightColumnDescription.h"
#include "XEMQualitativeColumnDescription.h"
#include "XEMQuantitativeColumnDescription.h"
#include "XEMIndividualColumnDescription.h"

#include <algorithm>

//------------
// Constructor by default
//------------
XEMLabelDescription::XEMLabelDescription() : XEMDescription(){
  _label = NULL; _nbCluster = 0;
}



// ---------------------------
//constructor by initilization
// ---------------------------
XEMLabelDescription::XEMLabelDescription( int64_t nbSample
                                        , int64_t nbColumn
                                        , std::vector< XEMColumnDescription* > columnDescription
                                        , FormatNumeric::XEMFormatNumericFile format
                                        , std::string filename
                                        , std::string infoName
                                        )
                                        : XEMDescription(nbSample, nbColumn, columnDescription, format, filename,  infoName)
{
  _label = createLabel();
  // get the number of cluster
  _nbCluster = *max_element(_label->getLabel().begin(), _label->getLabel().end());
}

//---------------------------------
// constructor from a vector of int
//----------------------------------
XEMLabelDescription::XEMLabelDescription( int64_t nbSample
                                        , std::vector<int64_t> vLabel
                                        )
                                        : XEMDescription()
{
  // get the number of cluster
  _nbCluster = *max_element(vLabel.begin(), vLabel.end());
  _infoName = "Label";
  _nbSample = nbSample;
  _nbColumn = 1;
  _fileName = ""; 
  _format = FormatNumeric::txt;
  _columnDescription.resize(1);
  _columnDescription[0] = new XEMQualitativeColumnDescription(0, _nbCluster);
  string name("Label");
  _columnDescription[0]->setName(name);

	_label = new XEMLabel(vLabel.size());
	_label->setLabel(vLabel, vLabel.size());
}
		


//-------------------------------------
// Constructor after an estimation->run
//--------------------------------------
XEMLabelDescription::XEMLabelDescription(XEMModel * estimation) : XEMDescription(){
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
    _nbCluster = estimation->getNbCluster();
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
  _nbCluster = labelDescription._nbCluster;
  _label = new XEMLabel(*(labelDescription.getLabel()));
  return *this;
}

//---------------------
/// Comparison operator
//---------------------
bool XEMLabelDescription::operator ==(const XEMLabelDescription & labelDescription) const{
  if ( _fileName != labelDescription._fileName ) return false;
  if ( _format != labelDescription._format ) return false;
  if ( _infoName != labelDescription._infoName ) return false;
  if ( _nbSample != labelDescription._nbSample ) return false;
  if ( _nbColumn != labelDescription._nbColumn ) return false;
  for (int64_t i = 0; i<_nbColumn; ++i){
    if ( _columnDescription[i]->getName() != labelDescription.getColumnDescription(i)->getName() ) return false;
  }
  if ( _nbCluster != labelDescription._nbCluster ) return false;
  if ( !(_label == labelDescription.getLabel()) ) return false;
  return true;
}

//------------
// Destructor
//------------
XEMLabelDescription::~XEMLabelDescription(){  
  if (_label) delete _label;
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
