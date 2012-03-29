/***************************************************************************
                             SRC/MIXMOD/XEMParameterDescription.cpp  description
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

#include "XEMParameterDescription.h"
#include "XEMGaussianGeneralParameter.h"
#include "XEMBinaryEkjhParameter.h"
#include "XEMModel.h"
#include "XEMModelType.h"
#include "XEMModelOutput.h"
#include "XEMParameter.h"
#include "XEMData.h"

//------------
// Constructor by default
//------------
XEMParameterDescription::XEMParameterDescription(){
  _parameter = NULL;
}



//-------------------------------------
// Constructor after an estimation->run
//--------------------------------------
XEMParameterDescription::XEMParameterDescription(XEMModel* iEstimation){
 
  if (iEstimation){
     _infoName = "Parameter";
     //_nbSample = iEstimation->getNbSample();
     _nbCluster = iEstimation->getNbCluster();
     _nbVariable = iEstimation->getData()->_pbDimension;
     _format = FormatNumeric::defaultFormatNumericFile;
     _filename = "";     
     _modelType = new XEMModelType(*iEstimation->getModelType());
     _parameter = iEstimation->getParameter()->clone();
     if (isBinary(_modelType->_nameModel)){
       XEMBinaryParameter * bParameter = dynamic_cast<XEMBinaryParameter*> (iEstimation->getParameter());
       recopyTabToVector(bParameter->getTabNbModality(), _nbFactor, _nbCluster);
     }
     saveNumericValues(_filename);
  }
  else{
    throw nullPointerError;
  }
  
}


//-------------------------------------
// Constructor after an estimation->run
//--------------------------------------
XEMParameterDescription::XEMParameterDescription(XEMModelOutput* iEstimation){
  
  if (iEstimation){
    _infoName = "Parameter";
    //_nbSample = iEstimation->getNbSample();
    _nbCluster = iEstimation->getNbCluster();
    _nbVariable = iEstimation->getParameterDescription()->getNbVariable();
    _format = FormatNumeric::defaultFormatNumericFile;
    _filename = "";     
    _modelType = new XEMModelType(*iEstimation->getParameterDescription()->getModelType());
    _parameter = iEstimation->getParameterDescription()->getParameter()->clone();
    if (isBinary(_modelType->_nameModel)){
      XEMBinaryParameter * bParameter = dynamic_cast<XEMBinaryParameter*> (iEstimation->getParameterDescription()->getParameter());
      recopyTabToVector(bParameter->getTabNbModality(), _nbFactor, _nbCluster);
    }
  }
  else{
    throw nullPointerError;
  }
  
}


// ---------------------------
//constructor by initilization for Binary
// ---------------------------
XEMParameterDescription::XEMParameterDescription( int64_t nbCluster
                                                , int64_t nbVariable
                                                , vector< int64_t > nbFactor
                                                , FormatNumeric::XEMFormatNumericFile format
                                                , string filename
                                                , string infoName
                                                , XEMModelName& modelName
                                                )
{
  _infoName = "Parameter";
  _nbVariable = nbVariable;
  _filename = filename;
  _nbCluster = nbCluster;
  _format = format;
  _nbFactor = nbFactor;
  _modelType = new XEMModelType(modelName);
  ifstream fi(filename.c_str(), ios::in);
  if (! fi.is_open()){
    throw wrongLabelFileName;
  }
  int64_t * tabNbFactor = new int64_t[_nbVariable];
  recopyVectorToTab(nbFactor,  tabNbFactor);
  // create _parameter : always a XEMBinaryEkjhParameter is created
  _parameter = new XEMBinaryEkjhParameter(nbCluster, _nbVariable , _modelType, tabNbFactor, filename);
}


// ---------------------------
//constructor by initilization for Binary
// ---------------------------
XEMParameterDescription::XEMParameterDescription( int64_t nbCluster
                                                , int64_t nbVariable
                                                , vector< int64_t > nbFactor
                                                , XEMModelName& modelName
                                                , double * proportions
                                                , double **  centers
                                                , double *** scatters
                                                )
{
  _infoName = "Parameter";
  _nbVariable = nbVariable;
  _filename = "";
  _nbCluster = nbCluster;
  _format = FormatNumeric::defaultFormatNumericFile;
  _nbFactor = nbFactor;
  _modelType = new XEMModelType(modelName);

  int64_t * tabNbFactor = new int64_t[_nbVariable];
  recopyVectorToTab(nbFactor,  tabNbFactor);
  // create _parameter : always a XEMBinaryEkjhParameter is created
  _parameter = new XEMBinaryEkjhParameter(nbCluster, _nbVariable , _modelType, tabNbFactor, proportions, centers, scatters);
}


// -----------------------------------------
//constructor by initilization for Gaussian
// ----------------------------------------
XEMParameterDescription::XEMParameterDescription( int64_t nbCluster
                                                , int64_t nbVariable
                                                , FormatNumeric::XEMFormatNumericFile format
                                                , string filename
                                                , string infoName
                                                , XEMModelName& modelName
                                                )
{
  _infoName = "Parameter";
  _nbVariable = nbVariable;
  _filename = filename;
  _nbCluster = nbCluster;
  _format = format;
  //_nbFactor  empty
  _modelType = new XEMModelType(modelName);
  ifstream fi(filename.c_str(), ios::in);
  if (! fi.is_open()){
    throw wrongLabelFileName;
  }
  // create _parameter : always a XEMGaussianGeneralParameter is created
  _parameter = new XEMGaussianGeneralParameter(nbCluster, _nbVariable , _modelType, filename);
}


// ---------------------------
//constructor by initilization for Gaussian
// ---------------------------
XEMParameterDescription::XEMParameterDescription( int64_t nbCluster
                                                , int64_t nbVariable
                                                , XEMModelName& modelName
                                                , double * proportions
                                                , double **  means
                                                , double *** variances
                                                )
{
  _infoName = "Parameter";
  _nbVariable = nbVariable;
  _filename = "";
  _nbCluster = nbCluster;
  _format = FormatNumeric::defaultFormatNumericFile;
  //_nbFactor  empty
  _modelType = new XEMModelType(modelName);
  
  // create _parameter : always a XEMBinaryEkjhParameter is created
  _parameter = new XEMGaussianGeneralParameter(nbCluster, _nbVariable , _modelType, proportions, means, variances);
}

//------------
// Desconstructor 
//------------
XEMParameterDescription::~XEMParameterDescription()
{
  if ( _modelType ) delete _modelType;
  if ( _parameter ) delete _parameter;
}


/// Comparison operator
bool XEMParameterDescription::operator==( XEMParameterDescription & paramDescription) const{
  if ( _infoName != paramDescription.getInfoName() ) return false;
  if ( _nbVariable != paramDescription.getNbVariable() ) return false;
  if ( _filename != paramDescription.getFilename() ) return false;
  if ( _nbCluster != paramDescription.getNbCluster() ) return false;
  if ( _format != paramDescription.getFormat() ) return false;
  if ( !(_modelType == paramDescription.getModelType()) ) return false;
  for (unsigned int i = 0; i<_nbFactor.size(); ++i){
    if ( _nbFactor[i] != paramDescription.getTabNbFactor()[i] ) return false;
  }
  if ( !(_parameter == paramDescription.getParameter()) ) return false;
  return true; 
}


//--------
// ostream
//--------
void XEMParameterDescription::saveNumericValues(string fileName){
  //if (_filename==""){
    ofstream fo(fileName.c_str(), ios::out);
    _parameter->edit(fo);
    _filename = fileName;
  //}
  /* else : if _fileName!="", paprameterDescription has been created by a XML file.
  In this case, the numeric file already exists. 
  */
}



