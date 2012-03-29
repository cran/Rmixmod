/***************************************************************************
                             SRC/MIXMOD/XEMProbaDescription.cpp  description
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

#include "XEMProbaDescription.h"
#include "XEMProba.h"
#include "XEMModel.h"
#include "XEMQuantitativeColumnDescription.h"
#include <sstream>

//------------
// Constructor by default
//------------
XEMProbaDescription::XEMProbaDescription() : XEMDescription(){
  _proba = NULL;
}


// ---------------------------
//constructor by initilization
// ---------------------------
XEMProbaDescription::XEMProbaDescription(int64_t nbSample,  int64_t nbCluster, FormatNumeric::XEMFormatNumericFile format, string filename, string infoName){
  _infoName = "infoName";
  _nbSample = nbSample;
  _nbColumn = nbCluster;
  _fileName = filename;
  _format = format;
  _columnDescription.resize(nbCluster);
  for (int64_t i=0; i<nbCluster; i++){
    _columnDescription[i] = new XEMQuantitativeColumnDescription(i);
    string name("Proba cluster=");
    std::ostringstream sNum;
    sNum << (i+1);
    name.append(sNum.str());
    _columnDescription[i]->setName(name);
  }
  _proba = new XEMProba(_nbSample, nbCluster);
  ifstream fi(filename.c_str(), ios::in);
  if (! fi.is_open()){
    throw wrongLabelFileName;
  }
  _proba->input(fi);
}


//------------
// Constructor after an estimation->run
//------------
XEMProbaDescription::XEMProbaDescription(XEMModel * model) : XEMDescription(){
  if (model){
    _infoName = "Probability";
    _nbSample = model->getNbSample();
    _nbColumn = model->getNbCluster();
    _fileName = "";
    _format = FormatNumeric::txt;
    _columnDescription.resize(_nbColumn);
    for (int64_t iCol=0; iCol<_nbColumn; iCol++){
      _columnDescription[iCol] = new XEMQuantitativeColumnDescription(iCol);
      string name("Probability for cluster ");
      std::ostringstream sNum;
      sNum << (iCol+1);
      name.append(sNum.str());
      _columnDescription[iCol]->setName(name);
    }
    _proba = new XEMProba(model);
  }
  else{
    throw nullPointerError;
  }
}








//------------
// Constructor by copy
//------------
XEMProbaDescription::XEMProbaDescription(XEMProbaDescription & probaDescription){
  (*this) = probaDescription; 
}

//------------
// operator ==
//------------
bool XEMProbaDescription::operator==( XEMProbaDescription & probaDescription) const{
  if ( _fileName != probaDescription._fileName ) return false;
  if ( _format != probaDescription._format ) return false;
  if ( _infoName != probaDescription._infoName ) return false;
  if ( _nbSample != probaDescription._nbSample ) return false;
  if ( _nbColumn != probaDescription._nbColumn ) return false;
  for (int64_t i = 0; i<_nbColumn; ++i){
    if ( _columnDescription[i]->getName() != probaDescription.getColumnDescription(i)->getName() ) return false;
  }
  if ( !(_proba == probaDescription.getProba()) ) return false;
  return true; 
}


//------------
// operator =
//------------
XEMProbaDescription & XEMProbaDescription::operator=( XEMProbaDescription & probaDescription){
  _fileName = probaDescription._fileName;
  _format = probaDescription._format;
  _infoName = probaDescription._infoName;
  _nbSample = probaDescription._nbSample;
  _nbColumn = probaDescription._nbColumn;
  _columnDescription.resize(_nbColumn);
  for (int64_t i = 0; i<_nbColumn; ++i){
    const XEMColumnDescription * cd = probaDescription.getColumnDescription(i);
    _columnDescription[i] = cd->clone();
  }
  _proba = new XEMProba(*(probaDescription.getProba()));
  return *this ; 
}


//------------
// Destructor
//------------
XEMProbaDescription::~XEMProbaDescription(){  
  if (_proba){
    delete _proba;
  }
}



//--------
// ostream
//--------
void XEMProbaDescription::saveNumericValues(string fileName){
  //if (_fileName==""){
    ofstream fo(fileName.c_str(), ios::out);
    _proba->edit(fo);
    _fileName = fileName;
  //}
  /* else : if _fileName!="", probaDescription has been created by a XML file.
  In this case, the numeric file already exists. 
  */
}





