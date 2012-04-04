/***************************************************************************
                             SRC/MIXMOD/XEMParameterDescription.h  description
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

#ifndef XEMPARAMETERDESCRIPTION_H
#define XEMPARAMETERDESCRIPTION_H

#include "XEMUtil.h"

// pre-declaration
class XEMModel;
class XEMModelOutput;
class XEMParameter;

class XEMParameterDescription
{  
  public:
    /// Default constructor
    XEMParameterDescription();
    
    /// Constructor
    XEMParameterDescription(XEMModel* iEstimation);
  
    /// Constructor
    XEMParameterDescription(XEMModelOutput* iEstimation);
  
    // constructor for Binary
    /// Constructor
    XEMParameterDescription( int64_t nbCluster
                           , int64_t nbVariable
                           , std::vector< int64_t > nbFactor
                           , FormatNumeric::XEMFormatNumericFile format
                           , std::string filename
                           , std::string infoName
                           , XEMModelName & modelName
                           );
    // ---------------------------
    XEMParameterDescription( int64_t nbCluster
                           , int64_t nbVariable
                           , std::vector< int64_t > nbFactor
                           , XEMModelName& modelName
                           , double * proportions
                           , double **  centers
                           , double *** scatters
                           );
  
    // constructor for Gaussian
    /// Constructor
    XEMParameterDescription( int64_t nbCluster
                           , int64_t nbVariable
                           , FormatNumeric::XEMFormatNumericFile format
                           , std::string filename
                           , std::string infoName
                           , XEMModelName & modelName
                           );
    // ---------------------------
    XEMParameterDescription( int64_t nbCluster
                           , int64_t nbVariable
                           , XEMModelName& modelName
                           , double * proportions
                           , double **  means
                           , double *** variances
                           );

    /// Desconstructor
    ~XEMParameterDescription();
  
    /// Comparison operator
    bool operator==( XEMParameterDescription & paramDescription) const;
  
    /// getParameter
    XEMParameter * getParameter(); 
    
    ///getInfoName
    std::string getInfoName();
    
    ///getPbDimension
    int64_t getNbVariable();
    
    ///getNbCluster
    int64_t getNbCluster();
    
    ///getFormat
    FormatNumeric::XEMFormatNumericFile getFormat();
    
    ///getFilename
    std::string getFilename();
    
    ///getModelType
    XEMModelType * getModelType();
    
    ///getTabNbModality
    std::vector<int64_t> & getTabNbFactor();
    
    void saveNumericValues(std::string fileName="");
    
    
  private :
    
   std::string _infoName;
    
   int64_t _nbVariable;
   
   int64_t _nbCluster;
   
   FormatNumeric::XEMFormatNumericFile _format;//format of  numeric file
   
   std::string _filename;
   
   std::vector<int64_t> _nbFactor;
   
   XEMModelType * _modelType;
    
   XEMParameter * _parameter;
    
};

inline XEMParameter * XEMParameterDescription::getParameter(){
  if (_parameter){
    return _parameter;
  }
  else{
    throw nullPointerError;
  }
}
inline int64_t XEMParameterDescription::getNbCluster()
{
  return _nbCluster;
}


inline std::string XEMParameterDescription::getInfoName(){
  return _infoName;
}

inline int64_t XEMParameterDescription::getNbVariable()
{
  return _nbVariable;
}

inline FormatNumeric::XEMFormatNumericFile XEMParameterDescription::getFormat(){
   return _format; 
}

inline std::string XEMParameterDescription::getFilename()
{
  return _filename;
}

inline XEMModelType * XEMParameterDescription::getModelType()
{
 return _modelType;
}

inline std::vector<int64_t> & XEMParameterDescription::getTabNbFactor()
{
  return _nbFactor;
}


#endif // XEMDATADESCRIPTION_H
