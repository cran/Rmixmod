/***************************************************************************
                             SRC/MIXMOD/XEMDataDescription.cpp  description
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

#include "XEMDataDescription.h"
#include "XEMBinaryData.h"
#include "XEMGaussianData.h"
#include "XEMWeightColumnDescription.h"
#include "XEMQuantitativeColumnDescription.h"
#include "XEMQualitativeColumnDescription.h"

//------------
// Constructor by default
//------------
XEMDataDescription::XEMDataDescription() : XEMDescription(){
  _data = NULL;
}


//------------
// Constructor by initialization
//------------
XEMDataDescription::XEMDataDescription(int64_t nbSample, int64_t nbColumn, vector<XEMColumnDescription *> columnDescription, FormatNumeric::XEMFormatNumericFile format, string filename, string infoName) : XEMDescription(nbSample, nbColumn, columnDescription, format, filename,  infoName){
  _data = createData();
}




//------------
// Constructor
//------------
XEMDataDescription::XEMDataDescription(XEMGaussianData * gData){
  _fileName = "";
  _format = FormatNumeric::defaultFormatNumericFile;
  _infoName = "";
  _nbSample = gData->getNbSample();
  _nbColumn = gData->getPbDimension();
  _columnDescription.resize(_nbColumn);
  for (int64_t i = 0; i<_nbColumn; ++i){
    _columnDescription[i] = new XEMQuantitativeColumnDescription(i);
  }
  _data = gData->clone();
  
  if (!_data->hasDefaultWeight()){
    _columnDescription.push_back(new XEMWeightColumnDescription(_nbColumn));
  }
}


//------------
// Constructor
//------------
XEMDataDescription::XEMDataDescription(XEMBinaryData * bData){
  _fileName = "";
  _format = FormatNumeric::defaultFormatNumericFile;
  _infoName = "";
  _nbSample = bData->getNbSample();
  _nbColumn = bData->getPbDimension();
  _columnDescription.resize(_nbColumn);
  int64_t * tabModality = bData->getTabNbModality();
  for (int64_t i = 0; i<_nbColumn; ++i){
    _columnDescription[i] = new XEMQualitativeColumnDescription(i, tabModality[i]);
  }
  _data = bData->clone();
  
  if (!_data->hasDefaultWeight()){
    _columnDescription.push_back(new XEMWeightColumnDescription(_nbColumn));
  }
}





//------------
// Constructor by copy
//------------
XEMDataDescription::XEMDataDescription(XEMDataDescription & dataDescription){
  (*this) = dataDescription; 
}


//------------
// operator =
//------------
XEMDataDescription & XEMDataDescription::operator=(const XEMDataDescription & dataDescription){
  _fileName = dataDescription._fileName;
  _format = dataDescription._format;
  _infoName = dataDescription._infoName;
  _nbSample = dataDescription._nbSample;
  _nbColumn = dataDescription._nbColumn;
  const XEMData * data = dataDescription.getData();
  if (data){
    _data = data->clone();
  }
  else{
    _data = NULL;
  }
  _columnDescription.resize(_nbColumn);
  for (int64_t i = 0; i<_nbColumn; ++i){
    const XEMColumnDescription * cd = dataDescription.getColumnDescription(i);
    _columnDescription[i] = cd->clone();
  }
  return *this ; 
}

//------------
// Destructor
//------------
XEMDataDescription::~XEMDataDescription(){  
  if (_data){
    delete _data;
  }
}



//----------------------------
// createData
//----------------------------
// Create and return XEMData *
XEMData * XEMDataDescription::createData() const{
  // on ne gere pour l'instant que des données toutes qualitatives ou toutes quantitatives
 
  XEMData * data = NULL;

  vector<int64_t> nbModality;
  int64_t nbQualitativeVariable = 0;
  int64_t nbQuantitativeVariable = 0;
  int64_t weightIndex = -1;
  for (int64_t i=0; i<_nbColumn; i++){
    if (typeid(*(_columnDescription[i])) == typeid(XEMQualitativeColumnDescription)){
      nbQualitativeVariable++;
      XEMQualitativeColumnDescription * qualitativeColumnDescription = dynamic_cast<XEMQualitativeColumnDescription *>(_columnDescription[i]);
      nbModality.push_back(qualitativeColumnDescription->getNbFactor());
    }
    if (typeid(*(_columnDescription[i]))==typeid(XEMQuantitativeColumnDescription)){
      nbQuantitativeVariable++;
    }
    if (typeid(*(_columnDescription[i]))==typeid(XEMWeightColumnDescription)){
      if (weightIndex == -1){
        weightIndex = i;
      }
      else{
        throw tooManyWeightColumnDescription;
      }
    }
  } 

  if (nbQualitativeVariable == 0){
    if (nbQuantitativeVariable != 0){
      data = new XEMGaussianData(_nbSample, nbQuantitativeVariable);
    }
    else{
      throw badDataDescription; // nbQuantitativeVariable=0 and nbQualitativeVariable=0
    }
  }
  else{  // nbQualitativeVariable!=0
    if (nbQuantitativeVariable != 0){
      throw badDataDescription; // nbQuantitativeVariable!=0 and nbQualitativeVariable!=0
    }
    else{
      data = new XEMBinaryData(_nbSample, nbQualitativeVariable, nbModality);
    }
  }
  data-> input(*this);

  return data;
}





//-----------------
// is binary Data ?
//-----------------
bool XEMDataDescription::isBinaryData() const{
  bool res = false;
  // on ne gere pour l'instant que des données toutes qualitatives ou toutes quantitatives

  int64_t nbQualitativeVariable = 0;
  int64_t nbQuantitativeVariable = 0;
  for (int64_t i=0; i<_nbColumn; i++){
    if (typeid(*(_columnDescription[i])) == typeid(XEMQualitativeColumnDescription)){
      nbQualitativeVariable++;
    }
    if (typeid(*(_columnDescription[i]))==typeid(XEMQuantitativeColumnDescription)){
      nbQuantitativeVariable++;
    }
  }
  
  if (nbQualitativeVariable != 0){
    if (nbQuantitativeVariable == 0){
      res = true;
    }
    else{
      throw badDataDescription;
    }
  }
  return res;
}


//---------------------
// verify data validity
//---------------------
bool XEMDataDescription::verifyData() const{
  return _data->verify();
}

